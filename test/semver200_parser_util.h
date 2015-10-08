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

#include <boost/test/unit_test.hpp>
#include <ostream>
#include "version.h"

version::Semver200_parser p;
const version::Prerelease_identifiers no_rel_ids;
const version::Build_identifiers no_build_ids;

#define N Identifier_type::num
#define A Identifier_type::alnum

namespace boost {
	namespace test_tools {
		namespace tt_detail {
			template<>
			void print_log_value<version::Build_identifiers>::operator()(std::ostream& os,
				const version::Build_identifiers& ids) {
				for (const auto& id : ids) {
					os << id << ",";
				}
			}

			template<>
			void print_log_value<version::Prerelease_identifiers>::operator()(std::ostream& os,
				const version::Prerelease_identifiers& ids) {
				for (const auto& id : ids) {
					os << id.first << "|" << static_cast<int>(id.second) << "|" << ",";
				}
			}
		}
	}
}

#define CHECK_NORMALS(VER, MAJOR, MINOR, PATCH) { \
	auto v = p.parse(VER); \
	BOOST_CHECK_EQUAL(v.major, MAJOR); \
	BOOST_CHECK_EQUAL(v.minor, MINOR); \
	BOOST_CHECK_EQUAL(v.patch, PATCH); \
	BOOST_CHECK_EQUAL(v.prerelease_ids, no_rel_ids); \
	BOOST_CHECK_EQUAL(v.build_ids, no_build_ids); \
}

#define CHECK_PARSE_ERROR(VER) { \
	BOOST_CHECK_THROW(p.parse(VER), version::Parse_error); \
}

#define CHECK_PREREL(VER, MAJOR, MINOR, PATCH, IDS) { \
	auto v = p.parse(VER); \
	BOOST_CHECK_EQUAL(v.major, MAJOR); \
	BOOST_CHECK_EQUAL(v.minor, MINOR); \
	BOOST_CHECK_EQUAL(v.patch, PATCH); \
	BOOST_CHECK_EQUAL(v.prerelease_ids, IDS); \
	BOOST_CHECK_EQUAL(v.build_ids, no_build_ids); \
}

#define CHECK_BUILD(VER, MAJOR, MINOR, PATCH, IDS) { \
	auto v = p.parse(VER); \
	BOOST_CHECK_EQUAL(v.major, MAJOR); \
	BOOST_CHECK_EQUAL(v.minor, MINOR); \
	BOOST_CHECK_EQUAL(v.patch, PATCH); \
	BOOST_CHECK_EQUAL(v.prerelease_ids, no_rel_ids); \
	BOOST_CHECK_EQUAL(v.build_ids, IDS); \
}

#define CHECK_PREREL_BUILD(VER, MAJOR, MINOR, PATCH, RIDS, BIDS) { \
	auto v = p.parse(VER); \
	BOOST_CHECK_EQUAL(v.major, MAJOR); \
	BOOST_CHECK_EQUAL(v.minor, MINOR); \
	BOOST_CHECK_EQUAL(v.patch, PATCH); \
	BOOST_CHECK_EQUAL(v.prerelease_ids, RIDS); \
	BOOST_CHECK_EQUAL(v.build_ids, BIDS); \
}
