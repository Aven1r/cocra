cmake_minimum_required(VERSION 3.20)
project(example)
include(FetchContent)

FetchContent_Declare(
    htmlparser
    GIT_REPOSITORY https://github.com/mylogin/htmlparser.git
    GIT_TAG main
)

FetchContent_MakeAvailable(htmlparser)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

include(FetchContent)
FetchContent_Declare(
  htmlparser
  GIT_REPOSITORY https://github.com/mylogin/htmlparser.git
  GIT_TAG        5c1f476c96da77f1e3a2a6a7f9c6ddf8b5e50d6d
)
FetchContent_MakeAvailable(htmlparser)

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(lib)
add_subdirectory(app)
