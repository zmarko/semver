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

#define BOOST_TEST_MODULE semver200_comparator_tests

#include <boost/test/unit_test.hpp>
#include "version.h"

using namespace version;

int compare(const std::string& l, const std::string& r) {
	Semver200_comparator c;
	Semver200_parser p;
	auto lv = p.parse(l);
	auto rv = p.parse(r);
	return c.compare(lv, rv);
}

BOOST_AUTO_TEST_CASE(compare_normal_eq) {
	BOOST_CHECK(compare("0.0.0", "0.0.0") == 0);
}

BOOST_AUTO_TEST_CASE(compare_normal_major_gt_1) {
	BOOST_CHECK(compare("1.0.0", "0.0.0") == 1);
}

BOOST_AUTO_TEST_CASE(compare_normal_major_gt_2) {
	BOOST_CHECK(compare("1.0.0", "0.0.5") == 1);
}

BOOST_AUTO_TEST_CASE(compare_normal_major_gt_3) {
	BOOST_CHECK(compare("1.0.0", "0.17.0") == 1);
}

BOOST_AUTO_TEST_CASE(compare_normal_minor_gt_1) {
	BOOST_CHECK(compare("1.1.0", "1.0.0") == 1);
}

BOOST_AUTO_TEST_CASE(compare_normal_minor_gt_2) {
	BOOST_CHECK(compare("1.1.0", "1.0.32") == 1);
}

BOOST_AUTO_TEST_CASE(compare_normal_patch_gt) {
	BOOST_CHECK(compare("1.1.3", "1.1.0") == 1);
}

BOOST_AUTO_TEST_CASE(compare_normal_major_lt_1) {
	BOOST_CHECK(compare("1.0.0", "2.0.0") == -1);
}

BOOST_AUTO_TEST_CASE(compare_normal_major_lt_2) {
	BOOST_CHECK(compare("1.0.1", "2.0.0") == -1);
}

BOOST_AUTO_TEST_CASE(compare_normal_major_lt_3) {
	BOOST_CHECK(compare("1.3.0", "2.0.0") == -1);
}

BOOST_AUTO_TEST_CASE(compare_normal_minor_lt_1) {
	BOOST_CHECK(compare("1.1.0", "1.2.0") == -1);
}

BOOST_AUTO_TEST_CASE(compare_normal_minor_lt_2) {
	BOOST_CHECK(compare("1.1.3", "1.2.0") == -1);
}

BOOST_AUTO_TEST_CASE(compare_normal_patch_lt) {
	BOOST_CHECK(compare("1.1.3", "1.1.5") == -1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_single_eq) {
	BOOST_CHECK(compare("1.1.3-prerel", "1.1.3-prerel") == 1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_multiple_eq) {
	BOOST_CHECK(compare("1.1.3-prerel.1.2.3", "1.1.3-prerel.1.2.3") == 1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_multiple_num_eq) {
	BOOST_CHECK(compare("1.1.3-prerel.123", "1.1.3-prerel.123") == 1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_multiple_num_gt) {
	BOOST_CHECK(compare("1.1.3-prerel.123", "1.1.3-prerel.120") == 1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_multiple_num_lt) {
	BOOST_CHECK(compare("1.1.3-prerel.123", "1.1.3-prerel.125") == -1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_single_alpha_gt) {
	BOOST_CHECK(compare("1.1.3-alpha", "1.1.3-beta") == 1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_single_alpha_lt) {
	BOOST_CHECK(compare("1.1.3-beta", "1.1.3-gamma") == -1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_single_alpha_gt_2) {
	BOOST_CHECK(compare("1.1.3-alpha5", "1.1.3-alpha4") == 1);
}

BOOST_AUTO_TEST_CASE(compare_prerel_single_alpha_lt_2) {
	BOOST_CHECK(compare("1.1.3-beta3", "1.1.3-beta5") == -1);
}

