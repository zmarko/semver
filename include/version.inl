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

#pragma once

#include "version.h"

namespace version {

	namespace {

		/// Utility function to splice all vector elements to output stream, using designated separator 
		/// between elements and function object for getting values from vector elements.
		template<typename T, typename F>
		std::ostream& splice(std::ostream& os, const std::vector<T>& v, const std::string& sep, F read) {
			for (auto it = v.cbegin(); it < v.cend() - 1; ++it) {
				os << read(*it) << sep;
			}
			os << read(*v.crbegin());
			return os;
		}
	}

	template<typename P, typename C>
	const int Basic_version<P, C>::major() const {
		return ver_.major;
	}

	template<typename P, typename C>
	const int Basic_version<P, C>::minor() const {
		return ver_.minor;
	}

	template<typename P, typename C>
	const int Basic_version<P, C>::patch() const {
		return ver_.patch;
	}

	template<typename P, typename C>
	const std::string Basic_version<P, C>::prerelease() const {
		std::stringstream ss;
		splice(ss, ver_.prerelease_ids, ".", [](const auto& id) { return id.first;});
		return ss.str();
	}

	template<typename P, typename C>
	const std::string Basic_version<P, C>::build() const {
		std::stringstream ss;
		splice(ss, ver_.build_ids, ".", [](const auto& id) { return id;});
		return ss.str();
	}


	template<typename P, typename C>
	bool operator<(const Basic_version<P, C>& l, const Basic_version<P, C>& r) {
		return l.comparator_.compare(l.ver_, r.ver_) == -1;
	}

	template<typename P, typename C>
	bool operator==(const version::Basic_version<P, C>& l, const version::Basic_version<P, C>& r) {
		return l.comparator_.compare(l.ver_, r.ver_) == 0;
	}

	template<typename P, typename C>
	std::ostream& operator<<(std::ostream& os, const version::Basic_version<P, C>& v) {
		os << v.ver_.major << "." << v.ver_.minor << "." << v.ver_.patch;
		std::string prl = v.prerelease();
		if (!prl.empty()) {
			os << "-" << prl;
		}
		std::string bld = v.build();
		if (!bld.empty()) {
			os << "+" << bld;
		}
		return os;
	}

	/// Compare if two version objects are different.
	template<typename P, typename C>
	inline bool operator!=(const version::Basic_version<P, C>& l, const version::Basic_version<P, C>& r) {
		return !(l == r);
	}

	/// Compare if left version object is greater than the right.
	template<typename P, typename C>
	inline bool operator>(const version::Basic_version<P, C>& l, const version::Basic_version<P, C>& r) {
		return r < l;
	}

	/// Compare if left version object is greater than or equal the right.
	template<typename P, typename C>
	inline bool operator>=(const version::Basic_version<P, C>& l, const version::Basic_version<P, C>& r) {
		return !(l < r);
	}

	/// Compare if left version object is less than or equal the right.
	template<typename P, typename C>
	inline bool operator<=(const version::Basic_version<P, C>& l, const version::Basic_version<P, C>& r) {
		return !(l > r);
	}
}