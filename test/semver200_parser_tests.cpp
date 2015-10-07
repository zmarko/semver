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

#define BOOST_TEST_MODULE semver200_parser_tests

#include <boost/test/unit_test.hpp>
#include "version.h"

using namespace version;

Version_data parse(const std::string& s) {
	Semver200_parser p;
	return p.parse(s);
}

BOOST_AUTO_TEST_CASE(parse_normal) {
	auto r = parse("1.0.0");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "");
	BOOST_CHECK_EQUAL(r.build, "");
}

BOOST_AUTO_TEST_CASE(parse_normal_bad_format_1) {
	BOOST_CHECK_THROW(parse("bad.version"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_bad_format_2) {
	BOOST_CHECK_THROW(parse("1.2.3.4"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_single) {
	auto r = parse("1.0.0-prerelease");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerelease");
	BOOST_CHECK_EQUAL(r.build, "");
}

BOOST_AUTO_TEST_CASE(parse_prerel_ok_multiple_1) {
	auto r = parse("1.0.0-prerelease.rel-1");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerelease.rel-1");
	BOOST_CHECK_EQUAL(r.build, "");
}

BOOST_AUTO_TEST_CASE(parse_prerel_ok_multiple_2) {
	auto r = parse("1.0.0-prerelease.rel-1.b.c.f");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerelease.rel-1.b.c.f");
	BOOST_CHECK_EQUAL(r.build, "");
}

BOOST_AUTO_TEST_CASE(parse_normal_build_single) {
	auto r = parse("1.0.0+test");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "");
	BOOST_CHECK_EQUAL(r.build, "test");
}

BOOST_AUTO_TEST_CASE(parse_normal_build_multiple) {
	auto r = parse("1.0.0+test.123.4.5");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "");
	BOOST_CHECK_EQUAL(r.build, "test.123.4.5");
}

BOOST_AUTO_TEST_CASE(parse_normal_prerel_single_build_single) {
	auto r = parse("1.0.0-prerel+test");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerel");
	BOOST_CHECK_EQUAL(r.build, "test");
}

BOOST_AUTO_TEST_CASE(parse_normal_prerel_single_build_multiple) {
	auto r = parse("1.0.0-prerel+test.1.2.3");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerel");
	BOOST_CHECK_EQUAL(r.build, "test.1.2.3");
}

BOOST_AUTO_TEST_CASE(parse_normal_prerel_multiple_build_single) {
	auto r = parse("1.0.0-prerel.1.2.3+test");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerel.1.2.3");
	BOOST_CHECK_EQUAL(r.build, "test");
}

BOOST_AUTO_TEST_CASE(parse_normal_prerel_multiple_build_multiple) {
	auto r = parse("1.0.0-prerel.1.2.3+test.1.2.3");
	BOOST_CHECK_EQUAL(r.major, 1);
	BOOST_CHECK_EQUAL(r.minor, 0);
	BOOST_CHECK_EQUAL(r.patch, 0);
	BOOST_CHECK_EQUAL(r.release, "prerel.1.2.3");
	BOOST_CHECK_EQUAL(r.build, "test.1.2.3");
}

BOOST_AUTO_TEST_CASE(parse_normal_all_empty) {
	BOOST_CHECK_THROW(parse(".."), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_major_empty) {
	BOOST_CHECK_THROW(parse(".0.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_minor_empty) {
	BOOST_CHECK_THROW(parse("1..0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_patch_empty) {
	BOOST_CHECK_THROW(parse("1.1."), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_major_with_letters) {
	BOOST_CHECK_THROW(parse("1a.0.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_minor_with_letters) {
	BOOST_CHECK_THROW(parse("1.0a.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_patch_with_letters) {
	BOOST_CHECK_THROW(parse("1.0.0a"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_major_leading_0) {
	BOOST_CHECK_THROW(parse("01.0.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_minor_leading_0) {
	BOOST_CHECK_THROW(parse("1.00.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_patch_leading_0) {
	BOOST_CHECK_THROW(parse("1.0.00"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_major_negative) {
	BOOST_CHECK_THROW(parse("-1.0.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_minor_negative) {
	BOOST_CHECK_THROW(parse("1.-1.0"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_normal_patch_negative) {
	BOOST_CHECK_THROW(parse("1.0.-1"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_multiple) {
	BOOST_CHECK_NO_THROW(parse("1.0.0-pre-rel.123.test.deadbeef.31415"));
}

BOOST_AUTO_TEST_CASE(parse_prerel_invalid_chars) {
	BOOST_CHECK_THROW(parse("1.0.0-prerelease#1"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_empty_id) {
	BOOST_CHECK_THROW(parse("1.0.0-prerel..test"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_num_with_leading_0) {
	BOOST_CHECK_THROW(parse("1.0.0-prerel.01"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_build_single_id) {
	BOOST_CHECK_NO_THROW(parse("1.0.0+build"));
}

BOOST_AUTO_TEST_CASE(parse_build_multiple_id) {
	BOOST_CHECK_NO_THROW(parse("1.0.0+build.123.test.deadbeef.31415"));
}

BOOST_AUTO_TEST_CASE(parse_build_invalid_chars) {
	BOOST_CHECK_THROW(parse("1.0.0+build#1"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_build_empty_id) {
	BOOST_CHECK_THROW(parse("1.0.0+build..test"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_single_build_single) {
	BOOST_CHECK_NO_THROW(parse("1.0.0-prerelease+build"));
}

BOOST_AUTO_TEST_CASE(parse_prerel_multiple_build_single) {
	BOOST_CHECK_NO_THROW(parse("1.0.0-pre-rel.123.test.deadbeef.31415+build"));
}

BOOST_AUTO_TEST_CASE(parse_prerel_invalid_chars_build_single) {
	BOOST_CHECK_THROW(parse("1.0.0-prerelease#1+build"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_empty_id_build_single) {
	BOOST_CHECK_THROW(parse("1.0.0-prerel..test+build"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_num_with_leading_0_build_single) {
	BOOST_CHECK_THROW(parse("1.0.0-prerel.01+build"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_single_build_multiple) {
	BOOST_CHECK_NO_THROW(parse("1.0.0-prerelease+build.1.2.3"));
}

BOOST_AUTO_TEST_CASE(parse_prerel_multiple_build_multiple) {
	BOOST_CHECK_NO_THROW(parse("1.0.0-pre-rel.123.test.deadbeef.31415+build.1.2.3"));
}

BOOST_AUTO_TEST_CASE(parse_prerel_invalid_chars_build_multiple) {
	BOOST_CHECK_THROW(parse("1.0.0-prerelease#1+build.1.2.3"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_empty_id_build_multiple) {
	BOOST_CHECK_THROW(parse("1.0.0-prerel..test+build.1.2.3"), Parse_error);
}

BOOST_AUTO_TEST_CASE(parse_prerel_num_with_leading_0_build_multiple) {
	BOOST_CHECK_THROW(parse("1.0.0-prerel.01+build.1.2.3"), Parse_error);
}
