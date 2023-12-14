cmake_minimum_required(VERSION 3.20)

add_executable(example main.cpp)

target_link_libraries(example PUBLIC htmlparser)
target_link_libraries(example PUBLIC cocra_parser)

target_include_directories(example PUBLIC ${htmlparser_SOURCE_DIR})
target_include_directories(example PUBLIC ../lib)