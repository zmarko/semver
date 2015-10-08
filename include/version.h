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

#include <string>
#include <vector>

namespace version {

	enum class Identifier_type { alnum, num };

	using Prerelease_identifier = std::pair<std::string, Identifier_type>;
	using Prerelease_identifiers = std::vector<Prerelease_identifier>;
	using Build_identifier = std::string;
	using Build_identifiers = std::vector<Build_identifier>;

	struct Version_data {
		int major;
		int minor;
		int patch;
		Prerelease_identifiers prerelease_ids;
		Build_identifiers build_ids;
	};

	template<typename Parser, typename Comparator>
	class Basic_version {
	public:
		Basic_version(const std::string&, Parser, Comparator);

		bool operator>(const Basic_version&);
		bool operator>=(const Basic_version&);
		bool operator<(const Basic_version&);
		bool operator<=(const Basic_version&);
		bool operator==(const Basic_version&);
		bool operator!=(const Basic_version&);

	private:
		Parser parser_;
		Comparator comparator_;
		const Version_data ver_;
	};

	struct Semver200_parser {
		Version_data parse(const std::string&);
	};

	struct Semver200_comparator {
		int compare(const Version_data&, const Version_data&);
	};

	class Semver200 : public Basic_version<Semver200_parser, Semver200_comparator> {};

	class Parse_error : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

}
