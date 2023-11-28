#ifndef PARSE
#define PARSE

#include <iostream>
#include <fstream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <libxml/HTMLparser.h>
#include <filesystem>
#include "html.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

std::string ExtractTextFromNode(xmlNode* node);

xmlNode* FindNodeWithClass(xmlNode* node, const char* className);

json GetJson();

void parsing(const std::string& path_to_problems,const std::string& lang, json json_response);
#endif
