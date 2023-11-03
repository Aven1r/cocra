# CMakeList.txt: проект CMake для Parser; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (parser parser.cpp)
set_property(TARGET parser PROPERTY CXX_STANDARD 20)
target_link_libraries(parser PRIVATE CURL::libcurl)
target_link_libraries(parser PRIVATE cpr::cpr)
target_link_libraries(parser PRIVATE LibXml2::LibXml2)