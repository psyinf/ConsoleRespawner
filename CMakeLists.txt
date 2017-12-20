cmake_minimum_required(VERSION 3.9)
project (ConsoleRespawnerApp)

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/CMakeModules/"
	"$ENV{ES_CMAKE_MODULES}/"
)
SET(COMPILER_STRING vc140)
SET(ARCH_STRING "64")

set(BOOST_ROOT "$ENV{ES_3RDPARTY_DIR}/boost")
set(BOOST_LIBRARYDIR "$ENV{ES_3RDPARTY_DIR}/boost/${COMPILER_STRING}/lib${ARCH_STRING}")
set(Boost_USE_STATIC_LIBS ON)


find_package(Boost REQUIRED	program_options filesystem)

SET(CMAKE_DEBUG_POSTFIX "_d")

file(GLOB HEADER_FILES "src/*.h")
file(GLOB CPP_FILES "src/*.cpp")

SET(APP_NAME ConsoleRespawner)

add_executable(	${APP_NAME}
	${HEADER_FILES}
	${CPP_FILES}
)

target_link_libraries( ${APP_NAME} PRIVATE
	Boost::boost
	Boost::program_options
	Boost::filesystem
)

set_target_properties(${APP_NAME} PROPERTIES DEBUG_POSTFIX _d)

install(TARGETS ${APP_NAME}
	RUNTIME DESTINATION bin/
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)