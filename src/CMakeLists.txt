include_directories(
	../include
)

add_library(semver
	Semver200_parser.cpp Semver200_comparator.cpp
)
