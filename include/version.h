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

#include <ostream>
#include <string>
#include <vector>

namespace version {

	/// Parse_error is thrown on all parsing and validation errors.
	class Parse_error : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	/// Type of prerelease identifier: alphanumeric or numeric.
	/**
	Type of identifier affects comparison: alphanumeric identifiers are compared as ASCII strings, while
	numeric identifiers are compared as numbers.
	*/
	enum class Identifier_type {
		alnum, ///< Identifier is alphanumerical
		num ///< Identifier is numeric
	};

	/// Container for prerelease identifier value and it's type.
	/**
	Prerelease version string consist of an optional series of dot-separated identifiers.
	These identifiers can be either numerical or alphanumerical.
	This structure describes one such identifier.
	*/
	using Prerelease_identifier = std::pair<std::string, Identifier_type>;

	/// Container for all prerelease identifiers for a given version string.
	using Prerelease_identifiers = std::vector<Prerelease_identifier>;

	/// Build identifier is just a string and has no meaning for the purpose of version precedence.
	using Build_identifier = std::string;

	/// Container for all build identifiers for a given version string.
	using Build_identifiers = std::vector<Build_identifier>;

	/// Description of version broken into parts, as per semantic versioning specification.
	struct Version_data {
		int major; ///< Major version, change only on incompatible API modifications
		int minor; ///< Minor version, change on compatible API modifications
		int patch; ///< Patch version, change only on bugfixes

		/// Optional series of prerelease identifiers.
		Prerelease_identifiers prerelease_ids;

		/// Optional series of build identifiers.
		Build_identifiers build_ids;
	};

	/// Forward declaration required for operators' template declarations.
	template<typename P, typename C>
	class Basic_version;

	/// Compare if left version object is less than the right.
	template<typename P, typename C>
	bool operator<(const Basic_version<P, C>&, const Basic_version<P, C>&);

	/// Compare if two version objects are equal.
	template<typename P, typename C>
	bool operator==(const Basic_version<P, C>& l, const Basic_version<P, C>& r);

	/// Output version object to stream using standard semver format (X.Y.Z-PR+B).
	template<typename P, typename C>
	std::ostream& operator<<(std::ostream& os, const Basic_version<P, C>& v);


	/// Generic version description and comparison class.
	/**
	Basic_version class describes general version object without prescribing parsing,
	validation and comparison rules. These rules are implemented by supplied P and
	C parameters.
	*/
	template<typename Parser, typename Comparator>
	class Basic_version {
	public:
		/// Construct Basic_version object using P to parse version string and C for comparison.
		Basic_version(const std::string& v, Parser p, Comparator c)
			: parser_(p), comparator_(c), ver_(parser_.parse(v)) {};

		const int major() const;
		const int minor() const;
		const int patch() const;
		const std::string prerelease() const;
		const std::string build() const;

		friend bool operator< <>(const Basic_version& l, const Basic_version& r);
		friend bool operator== <>(const Basic_version& l, const Basic_version& r);
		friend std::ostream& operator<< <>(std::ostream& os, const Basic_version& v);

	private:
		Parser parser_;
		Comparator comparator_;
		const Version_data ver_;
	};

	/// Compare if two version objects are different.
	template<typename Parser, typename Comparator>
	bool operator!=(const version::Basic_version<Parser, Comparator>& l, const version::Basic_version<Parser, Comparator>& r);

	/// Compare if left version object is greater than the right.
	template<typename Parser, typename Comparator>
	bool operator>(const version::Basic_version<Parser, Comparator>& l, const version::Basic_version<Parser, Comparator>& r);

	/// Compare if left version object is greater than or equal the right.
	template<typename Parser, typename Comparator>
	bool operator>=(const version::Basic_version<Parser, Comparator>& l, const version::Basic_version<Parser, Comparator>& r);

	/// Compare if left version object is less than or equal the right.
	template<typename Parser, typename Comparator>
	bool operator<=(const version::Basic_version<Parser, Comparator>& l, const version::Basic_version<Parser, Comparator>& r);

}

#include "version.inl"
