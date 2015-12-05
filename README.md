# About
This project is MIT-licensed, C++14 implementation of [semantic versioning](http://semver.org) parser and comparator. Semantic versioning 2.0.0 specification is supported out-of-the-box and the code should be flexible-enough to support future revisions or other similar versioning schemes.

# Usage
Parsing and comparing two version strings:
```c++
#include "semver200.h"

void main(int, char**) {
    version::Semver200_version v1("1.0.0");
    version::Semver200_version v2("2.0.0");

    if (v2 > v1) {
        std::cout << v2 << " is indeed greater than " << v1 << std::endl;
    } else {
        std::cout << "This thing is broken, what a waste of time!" << std::endl;
    }
}
```

Accessing individual version components:

```c++
#include "semver200.h"

void main(int, char**) {
    version::Semver200_version v("1.2.3-alpha.1+build.no.123");
    std::cout << "Major: " << v.major() << std::endl;
    std::cout << "Minor: " << v.minor() << std::endl;
    std::cout << "Patch: " << v.patch() << std::endl;
    std::cout << "Pre-release: " << v.prerelease() << std::endl;
    std::cout << "Build: " << v.build() << std::endl;
}
```

should generate following output:
```
Major: 1
Minor: 2
Patch: 3
Pre-release: alpha.1
Build: build.no.123
```

# Build
The code is written in C++14, so, fairly recent compiler is required to build it. Following compiler were tested:
- Microsoft Visual Studio 2015
- GCC 5.1.1
- Clang 3.7.0

Library itself does not have any external dependencies. Unit tests that verify the library work as expected, on the other hand, depend on the [Boost.Test](http://www.boost.org/doc/libs/1_59_0/libs/test/doc/html/index.html) library. If you do not have and do not want to install Boost.Test, then comment out appropriate section of CMakeLists.txt file.

The code comes with CMake project files. In order to build it you should:

- create, if it doesn’t already exist, directory `build` in the project directory;
- invoke `cmake ..` in build directory;
- once CMake is done, use your toolset (Visual Studio, nmake, make, …) to build the library;
- remember to link in the library you have built and to include `./include` directory to your build.
