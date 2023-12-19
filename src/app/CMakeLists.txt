cmake_minimum_required(VERSION 3.20)

add_executable(ProblemParser main.cpp)
add_executable(SetGenerator generateset.cpp)

target_link_libraries(ProblemParser PUBLIC htmlparser)
target_link_libraries(ProblemParser PUBLIC cocra_parser)
target_link_libraries(SetGenerator PUBLIC htmlparser)
target_link_libraries(SetGenerator PUBLIC cocra_parser)

target_include_directories(ProblemParser PUBLIC ${htmlparser_SOURCE_DIR})
target_include_directories(ProblemParser PUBLIC ../lib)
target_include_directories(SetGenerator PUBLIC ${htmlparser_SOURCE_DIR})
target_include_directories(SetGenerator PUBLIC ../lib)