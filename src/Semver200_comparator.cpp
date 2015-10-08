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

#include <algorithm>
#include "version.h"

using namespace std;

namespace {

	using namespace version;

	int compare_normal(const Version_data& l, const Version_data& r) {
		if (l.major > r.major) return 1;
		if (l.major < r.major) return -1;
		if (l.minor > r.minor) return 1;
		if (l.minor < r.minor) return -1;
		if (l.patch > r.patch) return 1;
		if (l.patch < r.patch) return -1;
		return 0;
	}

	int compare_prerel_identifiers(const Prerelease_identifier& l, const Prerelease_identifier& r) {
		if (l.second == Identifier_type::alnum && r.second == Identifier_type::alnum) {
			return l.first.compare(r.first);
		} else if (l.second == Identifier_type::alnum && r.second == Identifier_type::num) {
			return 1;
		} else if (l.second == Identifier_type::num && r.second == Identifier_type::alnum) {
			return -1;
		} else if (l.second == Identifier_type::num && r.second == Identifier_type::num) {
			int li = stoi(l.first);
			int ri = stoi(r.first);
			if (li == ri) return 0;
			return li > ri ? 1 : -1;
		}
		throw logic_error("unexpected identifier types: " + to_string(static_cast<int>(l.second)) + ", " +
			to_string(static_cast<int>(r.second)));
	}
}

namespace version {

	int Semver200_comparator::compare(const Version_data& l, const Version_data& r) const {
		int cmp = compare_normal(l, r);
		if (cmp != 0) return cmp;

		// prerelease version is always higher than prerelease
		if (l.prerelease_ids.empty() && !r.prerelease_ids.empty()) return 1;
		if (r.prerelease_ids.empty() && !l.prerelease_ids.empty()) return -1;

		// compare prerelease by looking at each identifier: numeric ones are compared as numbers,
		// alphanum as ASCII strings
		auto shorter = min(l.prerelease_ids.size(), r.prerelease_ids.size());
		for (size_t i = 0; i < shorter; i++) {
			cmp = compare_prerel_identifiers(l.prerelease_ids[i], r.prerelease_ids[i]);
			if (cmp != 0) return cmp;
		}

		// prerels are the same, to the length of the shorter one;
		// if they are the same length, then versions are equal, otherwise, longer wins
		if (l.prerelease_ids.size() == r.prerelease_ids.size()) return 0;
		return l.prerelease_ids.size() > r.prerelease_ids.size() ? 1 : -1;
	}

}