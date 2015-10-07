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

	using Phase_transition_hook = function<void(const string&)>;
	using Transition = tuple<const char, Parser_phase, Phase_transition_hook>;
	using Validator = function<void(const string&, const char)>;

	inline Transition mkx(const char c, Parser_phase p, Phase_transition_hook px) {
		return make_tuple(c, p, px);
	}

	inline void process_char(const char c, Parser_phase& phase, const vector<Transition> transitions,
		string& target, Validator validate) {
		for (const auto& t : transitions) {
			if (c == get<0>(t)) {
				phase = get<1>(t);
				if (get<2>(t)) get<2>(t)(target);
				return;
			}
		}
		validate(target, c);
		target.push_back(c);
	}

	auto normal_version_validator = [](const string& tgt, const char c) {
		if (c < '0' || c > '9') throw version::Parse_error("invalid character encountered: " + string(1, c));
		if (tgt.compare(0, 1, "0") == 0) throw version::Parse_error("leading 0 not allowed");
	};

	auto release_version_validator = [](const string& tgt, const char c) {
		if ((c < '0' || c > '9') && (c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && c != '.' && c != '-')
			throw version::Parse_error("invalid character encountered: " + string(1, c));
	};

	void validate_release_identifier(const string& id) {
		if (id.find_first_not_of("0123456789") == string::npos
			&& id.compare(0, 1, "0") == 0) {
			throw version::Parse_error("leading 0 not allowed");
		}
	}

	void validate_release_identifiers(const string& release) {
		string id;
		for (const auto& c : release) {
			if (c == '.') {
				validate_release_identifier(id);
				id.clear();
			} else {
				id.push_back(c);
			}
		}
		validate_release_identifier(id);
	}
}

namespace version {

	Version_data Semver200_parser::parse(const string& s) {
		string major;
		string minor;
		string patch;
		string release;
		string build;

		auto process_release_component = [&](const string& t) {
			if (t.empty()) throw Parse_error("version identifier cannot be empty");
			if (t.compare(t.length() - 1, 1, ".") == 0) throw Parse_error("version identifier cannot be empty");
			release.push_back('.');};

		auto process_build_component = [&](const string& t) {
			if (t.empty()) throw Parse_error("version identifier cannot be empty");
			if (t.compare(t.length() - 1, 1, ".") == 0) throw Parse_error("version identifier cannot be empty");
			build.push_back('.');};

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
			mkx('.', Parser_phase::release, process_release_component),
			mkx('+', Parser_phase::build, {})
		};
		auto build_xs = {
			mkx('.', Parser_phase::build, process_build_component)
		};
		Parser_phase phase = Parser_phase::major;
		for (const auto& c : s) {
			switch (phase) {
			case Parser_phase::major:
				process_char(c, phase, major_xs, major, normal_version_validator);
				break;
			case Parser_phase::minor:
				process_char(c, phase, minor_xs, minor, normal_version_validator);
				break;
			case Parser_phase::patch:
				process_char(c, phase, patch_xs, patch, normal_version_validator);
				break;
			case Parser_phase::release:
				process_char(c, phase, release_xs, release, release_version_validator);
				break;
			case Parser_phase::build:
				process_char(c, phase, build_xs, build, release_version_validator);
				break;
			}
		}

		validate_release_identifiers(release);

		try {
			return Version_data{ stoi(major), stoi(minor), stoi(patch), release, build };
		} catch (invalid_argument& ex) {
			throw Parse_error(ex.what());
		}
	}

	int Semver200_comparator::compare(const Version_data& l, const Version_data& r) {
		throw 1;
	}

}
