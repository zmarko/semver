/*
The MIT License (MIT)

Copyright (c) 2015 Marko Zivanovic

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <functional>
#include <vector>
#include "version.h"

using namespace std;

namespace {
	enum class Parser_phase {
		major, minor, patch, release, build
	};

	using Phase_transition_hook = function<void(string&)>;
	using Transition = tuple<const char, Parser_phase, Phase_transition_hook>;
	using Validator = function<void(const string&, const char)>;

	inline Transition mkx(const char c, Parser_phase p, Phase_transition_hook pth) {
		return make_tuple(c, p, pth);
	}

	inline void process_char(const char c, Parser_phase& phase, Parser_phase& prev_phase,
		const vector<Transition>& transitions, string& target, Validator validate) {
		for (const auto& t : transitions) {
			if (c == get<0>(t)) {
				prev_phase = phase;
				phase = get<1>(t);
				if (get<2>(t)) get<2>(t)(target);
				return;
			}
		}
		validate(target, c);
		target.push_back(c);
	}

	inline void normal_version_validator(const string& tgt, const char c) {
		if (c < '0' || c > '9') throw version::Parse_error("invalid character encountered: " + string(1, c));
		if (tgt.compare(0, 1, "0") == 0) throw version::Parse_error("leading 0 not allowed");
	}

	inline void release_version_validator(const string& tgt, const char c) {
		if ((c < '0' || c > '9') && (c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && c != '.' && c != '-')
			throw version::Parse_error("invalid character encountered: " + string(1, c));
	}

	bool is_identifier_numeric(const string& id) {
		return id.find_first_not_of("0123456789") == string::npos;
	}

	bool check_identifier_for_leading_0(const string& id) {
		return id.length() > 1 && id[0] == '0';
	}

}

namespace version {

	Version_data Semver200_parser::parse(const string& s) {
		string major;
		string minor;
		string patch;
		string release_id;
		string build_id;
		Prerelease_identifiers release;
		Build_identifiers build;
		Parser_phase prev_phase;

		auto release_hook = [&](string& id) {
			if (id.empty()) throw version::Parse_error("version identifier cannot be empty");
			Identifier_type t = Identifier_type::alnum;
			if (is_identifier_numeric(id)) {
				t = Identifier_type::num;
				if (check_identifier_for_leading_0(id)) {
					throw Parse_error("numeric identifiers cannot have leading 0");
				}
			}
			release.push_back(Prerelease_identifier(id, t));
			id.clear();
		};

		auto build_hook = [&](string& id) {
			// process last token left from parsing release data
			if (prev_phase == Parser_phase::release) release_hook(release_id);
			if (id.empty()) throw version::Parse_error("version identifier cannot be empty");
			build.push_back(id);
			id.clear();
		};

		auto major_xs = {
			mkx('.', Parser_phase::minor, {})
		};
		auto minor_xs = {
			mkx('.', Parser_phase::patch, {})
		};
		auto patch_xs = {
			mkx('-', Parser_phase::release, {}),
			mkx('+', Parser_phase::build, {})
		};
		auto release_xs = {
			mkx('.', Parser_phase::release, release_hook),
			mkx('+', Parser_phase::build, {})
		};
		auto build_xs = {
			mkx('.', Parser_phase::build, build_hook)
		};
		Parser_phase phase = Parser_phase::major;
		for (const auto& c : s) {
			switch (phase) {
			case Parser_phase::major:
				process_char(c, phase, prev_phase, major_xs, major, normal_version_validator);
				break;
			case Parser_phase::minor:
				process_char(c, phase, prev_phase, minor_xs, minor, normal_version_validator);
				break;
			case Parser_phase::patch:
				process_char(c, phase, prev_phase, patch_xs, patch, normal_version_validator);
				break;
			case Parser_phase::release:
				process_char(c, phase, prev_phase, release_xs, release_id, release_version_validator);
				break;
			case Parser_phase::build:
				process_char(c, phase, prev_phase, build_xs, build_id, release_version_validator);
				break;
			}
		}

		// Trigger appropriate hooks in order to process last token, because no phase transition was
		// triggered for it.
		if (phase == Parser_phase::release) {
			release_hook(release_id);
		}
		if (phase == Parser_phase::build) {
			build_hook(build_id);
		}

		try {
			return Version_data{ stoi(major), stoi(minor), stoi(patch), release, build };
		} catch (invalid_argument& ex) {
			throw Parse_error(ex.what());
		}
	}
}
