#ifndef PARSE
#define PARSE

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#include "html.hpp"
#include <html2md.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <libxml/HTMLparser.h>


using json = nlohmann::json;
namespace fs = std::filesystem;

std::string ExtractTextFromNode(xmlNode* node);

xmlNode* FindNodeWithClass(xmlNode* node, const char* className);


void parsing(const std::string& path_to_problems,const std::string& lang, int numProblems, int timeDelay=1);

void processText(std::string& text);

void template_html(std::string& htmlString);

void editString(std::string& aString);

void logMessage(const std::string& logFilePath, const std::string& message);


#endif