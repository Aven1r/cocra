#include "parser.hpp"


std::string ExtractTextFromNode(xmlNode* node) {
    std::string html;

    xmlBufferPtr buffer = xmlBufferCreate();
    if (buffer == nullptr) {
        std::cerr << "Failed to create buffer" << std::endl;
        return html;
    }
    xmlNodeDump(buffer, node->doc, node, 0, 0);

    html = reinterpret_cast<char*>(buffer->content);


    size_t found = html.find("$$$");
    while (found != std::string::npos) {
        html.replace(found, 3, "$");
        found = html.find("$$$", found + 1);
    }

    xmlBufferFree(buffer);


    return html;
}

void processText(std::string& text) {
    bool inLatex = false;
    int flag = 0;
    size_t pos = 0;
    while ((pos = text.find('[', pos)) != std::string::npos) {
        if (pos > 0 && text[pos - 1] != ' ' && text[pos-1]!='!' && text[pos-1]!='$') {
            text.insert(pos, " ");
            pos++;
        }
        pos++;
    }
   
    
   
    
    for (size_t i = 0; i < text.length();++i) {
        if (text[i] == '$') {
            inLatex = !inLatex;
            continue;
        }

        if (inLatex) {
            
            if (text[i] == '\n') {
                text.erase(i, 1);
                --i;
            }
            else if (text[i] == '\\' && text[i + 1] == '\\') {
                text.erase(i + 1, 1);
            }
        }
        if (text.substr(i, 3) == "```") {
            
            flag++;
            if (flag % 2 !=0) {
                text.insert(i+3, "\n");
            }
            else{
                text.insert(i, "\n");
            }
        }
    }
    text.insert(0, "# ");
    

        
 }





xmlNode* FindNodeWithClass(xmlNode* node, const char* className) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar*)"div") == 0) {
        xmlChar* nodeClassName = xmlGetProp(node, (const xmlChar*)"class");
        if (nodeClassName && xmlStrcmp(nodeClassName, (const xmlChar*)className) == 0) {
            return node;
        }
    }

    for (xmlNode* child = node->children; child; child = child->next) {
        xmlNode* foundNode = FindNodeWithClass(child, className);
        if (foundNode != nullptr) {
            return foundNode;
        }
    }

    return nullptr;
}

void template_html(std::string& htmlString) {
    std::size_t pos = 0;

    while ((pos = htmlString.find("<div class=\"section-title\">", pos)) != std::string::npos) {
        pos += 25;
        pos = htmlString.find(">", pos);
        if (pos != std::string::npos) {
            htmlString.insert(pos + 1, "# ");
        }
    }

    pos = 0;
    while ((pos = htmlString.find("<div class=\"property-title\">", pos)) != std::string::npos) {
        pos += 27;
        pos = htmlString.find("<", pos);
        if (pos != std::string::npos) {
            htmlString.insert(pos, ":");
        }
    }
}

json GetJson() {
    cpr::Response response = cpr::Get(cpr::Url{ "https://codeforces.com/api/problemset.problems" });
    if (response.status_code != 200) {
        std::cerr << "Failed to retrieve problemset" << std::endl;
        return 1;
    }

    json json_response = json::parse(response.text);
    return json_response;
}
void parsing(const std::string& path_to_problems, const std::string& lang, json json_response, int numProblems) {
    std::string folderPath = path_to_problems + "/" + "problems/";
    int count = 0;
    for (const auto& problem : json_response["result"]["problems"]) {
        if (count >= numProblems and numProblems != 0) {
            break;
        }
        std::string contestId = problem["contestId"].dump();
        std::string problemId = problem["index"].dump();
        problemId.erase(remove(problemId.begin(), problemId.end(), '\"'), problemId.end());
        std::string folderPathproblem = folderPath + contestId + problemId;
        std::string filePath = folderPathproblem + "/" + contestId + problemId + ".md";
        if (fs::exists(filePath)) {
            std::cout << "Folder for problem " << contestId << problemId << " already exists. Skipping." << std::endl;
            count--;
            continue;
        }
        
        std::string url = "https://codeforces.com/problemset/problem/" + contestId + "/" + problemId + "?locale=" + lang;

        cpr::Response problemResponse = cpr::Get(cpr::Url{ url });

        if (problemResponse.status_code != 200) {
            std::cerr << "Failed to retrieve webpage for problem " << contestId << problemId << std::endl;
            continue;
        }

        htmlDocPtr doc = htmlReadMemory(problemResponse.text.c_str(), problemResponse.text.size(), nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        if (doc == nullptr) {
            std::cerr << "Failed to parse HTML for problem " << contestId << problemId << std::endl;
            continue;
        }

        xmlNode* rootNode = xmlDocGetRootElement(doc);
        xmlNode* statementNode = nullptr;

        statementNode = FindNodeWithClass(rootNode, "problem-statement");

        std::string statementText;
        if (statementNode != nullptr) {
            statementText = ExtractTextFromNode(statementNode);
        }
        else {
            std::cerr << "Failed to find statement node for problem " << contestId << problemId << std::endl;
            continue;
        }
        html::parser p;
        html::node_ptr node = p.parse(statementText);

        statementText = node->to_html('  ');
        template_html(statementText);
      
        auto* options = new html2md::Options();
        options->splitLines = false;
        options->formatTable = false;
        html2md::Converter c(statementText,options);
        auto md = c.convert();
        //processText(statementText);
        processText(md);
        
        fs::create_directories(folderPathproblem);

        std::ofstream outputFile(filePath);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output file for problem " << contestId << problemId << std::endl;
            continue;
        }

        outputFile << md;

        outputFile.close();

        std::cout << "Problem statement for " << contestId << problemId << " has been saved to " << filePath << std::endl;

        xmlFreeDoc(doc);
        xmlCleanupParser();

        count++;
    }
}