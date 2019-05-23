cmake_minimum_required(VERSION 2.8)
project(semver)
set(VERSION_MAJOR   "1")
set(VERSION_MINOR   "1")
set(VERSION_PATCH   "1")
set(VERSION_RELEASE "SNAPSHOT")
set(VERSION_BUILD   "")

option(SEMVER_ENABLE_TESTING "Adds tests subdirectory and enables testing" OFF)
option(SEMVER_USE_STATIC_CRT "Use static (DLL) run-time lib." OFF)

# Build full version string, including optional components
string(COMPARE NOTEQUAL VERSION_RELEASE "" HAVE_RELEASE)
string(COMPARE NOTEQUAL VERSION_BUILD "" HAVE_BUILD)
set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
if(HAVE_RELEASE)
	set(VERSION_FULL "${VERSION_FULL}-${VERSION_RELEASE}")
endif()
if(HAVE_BUILD)
	set(VERSION_FULL "${VERSION_FULL}+${VERSION_BUILD}")
endif()

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
	add_definitions(-std=c++14)
endif()

if(WIN32)
	add_definitions(-DWINVER=0x0601)
	add_definitions(-D_WIN32_WINNT=0x0601)
endif()

if(MSVC)
  foreach (flag_var
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (SEMVER_USE_STATIC_CRT)
      string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
    endif()
  endforeach()

	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

add_subdirectory(src)

if (SEMVER_ENABLE_TESTING)
  add_subdirectory(test)

  enable_testing()
  add_test(NAME semver200_parser_tests COMMAND semver200_parser_tests)
  add_test(NAME semver200_comparator_tests COMMAND semver200_comparator_tests)
  add_test(NAME semver200_version_tests COMMAND semver200_version_tests)
  add_test(NAME semver200_modifier_tests COMMAND semver200_modifier_tests)
endif()
