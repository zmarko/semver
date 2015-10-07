set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS unit_test_framework)
#find_package(Threads)

include_directories(../include)

include_directories(
	${Boost_INCLUDE_DIRS}
)

add_executable(semver200_parser_tests semver200_parser_tests.cpp)
target_link_libraries(semver200_parser_tests
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	semver
)

add_executable(semver200_comparator_tests semver200_comparator_tests.cpp)
target_link_libraries(semver200_comparator_tests
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	semver
)
