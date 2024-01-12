cmake_minimum_required(VERSION 3.20)
project(cocra_parser CXX)

add_library(${PROJECT_NAME} parser.cpp parser.hpp generator.cpp generator.hpp csv.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl)
target_link_libraries(${PROJECT_NAME} PUBLIC cpr::cpr)
target_link_libraries(${PROJECT_NAME} PUBLIC LibXml2::LibXml2)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC htmlparser)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${htmlparser_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC html2md)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${html2md_SOURCE_DIR})

