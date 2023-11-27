#ifndef PARSE
#define PARSE

#include <iostream>
#include <fstream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <libxml/HTMLparser.h>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::string ExtractTextFromNode(xmlNode* node, int indentLevel = 0);

xmlNode* FindNodeWithClass(xmlNode* node, const char* className);

#endif
