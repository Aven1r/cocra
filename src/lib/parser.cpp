#include "parser.hpp"


std::string ExtractTextFromNode(xmlNode* node) {
    std::string html;

    xmlBufferPtr buffer = xmlBufferCreate();
    if (buffer == nullptr) {
        std::cerr << "Failed to create buffer" << std::endl;
        return html;
    }
    xmlNodeDump(buffer, node->doc, node, 0, 1);


    html = (char*)(buffer->content);

    // Format latex formula
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
        if (pos > 0 && text[pos - 1] != ' ' && text[pos - 1] != '!' && text[pos - 1] != '$') {
            text.insert(pos, " ");
            pos++;
        }
        pos++;
    }
    bool foundExclamation = false;
    bool removePipe = false;
    size_t index = 0;

    while (index < text.length()) {
        if (text.substr(index, 3) == "![]") {
            foundExclamation = true;
        }
        else if (foundExclamation && text[index] == ')') {
            if (index + 1 < text.length() && text[index + 1] != '\n') {
                text.insert(index + 1, "\n");
                foundExclamation = false;
            }
        }
        if (text[index] == '|' && ((text[index + 2] == '!' || text[index - 2] == ')') && text[index + 1] == '\n')) {
            text.erase(index, 1);
        }

        ++index;
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


void editString(std::string& aString)
{
    if (aString.size() == 0)
        return;

    std::regex emptyLineRegex("\\n\\s*\\n");

    aString = std::regex_replace(aString, emptyLineRegex, "\n");


    size_t capIndex = aString.find("\n");
    aString.insert(capIndex, "\n");


    std::stringstream ss(aString);
    std::string line;
    size_t index = 0;
    bool cap = false;

    while (std::getline(ss, line))
    {
        if (!line.empty())
        {
            size_t lastCharIndex = line.size() - 1;
            if ((std::isupper(char(line.front())) && (line.back() == '.' || line.back() == ':' || line.back() == '!' || line.back() == '?')) ||
                (line.front() == '$') && (line.back() == '.' || line.back() == ':' || line.back() == '!' || line.back() == '?'))
            {
                if (!cap)
                {
                    aString.insert(index, "\n");
                    index++;
                    cap = true;
                }

                if ((char)ss.peek() != '#' && (char)ss.peek() != '!' && (char)ss.peek())
                {
                    aString.insert(index + lastCharIndex + 1, "\n");
                    index++;
                }
            }

            if ((std::isdigit(char(line.front()))) && (line.back() == '.' || line.back() == ':' || line.back() == '!' || line.back() == '?') &&
                (!std::isdigit(char((char)ss.peek()))))
            {
                aString.insert(index + lastCharIndex + 1, "\n");
                index++;
            }

            if ((line.front() == '-' && (line.back() == '.' || line.back() == ':' || line.back() == '!' || line.back() == '?') && ((char)ss.peek() != '-')))
            {
                aString.insert(index + lastCharIndex + 1, "\n");
                index++;
            }

        }
        index += line.size() + 1;
    }

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
            htmlString.insert(pos, ": ");
        }
    }
}

void logMessage(const std::string& logFilePath, const std::string& message) {
    std::ofstream logFile(logFilePath, std::ios_base::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logFile << std::put_time(std::localtime(&now), "%Y-%m-%d %X") << ": " << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Failed to opáen log file." << std::endl;
    }
}

void createCSV(const std::string& filePath) {
    std::ofstream csvFile(filePath, std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to create CSV file: " << filePath << std::endl;
        return;
    }

    // Check if the file is empty
    csvFile.seekp(0, std::ios::end);
    if (csvFile.tellp() == 0) {
        csvFile << "name,rating,tags,folderpath\n";
    }

    csvFile.close();
}

void addProblemToCSV(const std::string& filePath, const std::string& name, int rating, const std::vector<std::string>& tags, const std::string& folderPath) {
    std::ofstream csvFile(filePath, std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << filePath << std::endl;
        return;
    }
    for (const auto& tag : tags) {
        csvFile << name << "," << rating << "," << tag << "," << folderPath << "\n";
    }
    csvFile.close();
}



void parsing(const std::string& path_to_problems, const std::string& lang, int numProblems, int timeDelay) {
    std::string folderPath = path_to_problems + "/" + "problems/";
    std::string csvFilePath = folderPath + "ParsedProblems.csv";
    std::string logFilePath = folderPath + "log.txt";
    if (std::ifstream(logFilePath)) {
        std::remove(logFilePath.c_str());
    }
    fs::create_directories(folderPath);
    std::ofstream logFile(logFilePath);
    cpr::Response response = cpr::Get(cpr::Url{ "https://codeforces.com/api/problemset.problems" });
    if (response.status_code != 200) {
        logMessage(logFilePath, "Failed to retrieve problemset");
        return;
    }
    else {
        logMessage(logFilePath, "Parsing json success");
    }
    json json_response = json::parse(response.text);
    setlocale(LC_ALL, "Russian");

    int count = 0;
    createCSV(csvFilePath);
    
    for (const auto& problem : json_response["result"]["problems"]) {
        if (count >= numProblems and numProblems != 0) {
            break;
        }
        std::string contestId = problem["contestId"].dump();
        std::string problemId = problem["index"].dump();
        std::string name = problem["name"].dump();
        std::string rating;
        if (problem.find("rating") != problem.end()) {
            rating = problem["rating"].dump();
        } else {
            rating = "0";
        }
        std::vector<std::string> tags = problem["tags"];

        problemId.erase(remove(problemId.begin(), problemId.end(), '\"'), problemId.end());
        std::string folderPathproblem = folderPath + contestId + problemId;
        std::string filePath = folderPathproblem + "/" + contestId + problemId + ".md";
        if (fs::exists(filePath)) {
            logMessage(logFilePath, "Folder for problem " + contestId + problemId + " already exists. Skipping.");
            continue;
        }
        
        std::string url = "https://codeforces.com/problemset/problem/" + contestId + "/" + problemId + "?locale=" + lang;
        std::chrono::milliseconds delay(timeDelay);
        std::this_thread::sleep_for(delay);
        cpr::Response problemResponse = cpr::Get(cpr::Url{ url });

        if (problemResponse.status_code != 200) {
            logMessage(logFilePath, "Failed to retrieve webpage for problem " + contestId + problemId);
            continue;
        }

        htmlDocPtr doc = htmlReadMemory(problemResponse.text.c_str(), problemResponse.text.size(), nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        if (doc == nullptr) {
            logMessage(logFilePath, "Failed to parse HTML for problem " + contestId + problemId);
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
            logMessage(logFilePath, "Failed to find statement node for problem " + contestId + problemId);
            continue;
        }
        html::parser p;
        html::node_ptr node = p.parse(statementText);

        statementText = node->to_raw_html();
        template_html(statementText);
      
        auto* options = new html2md::Options();
        options->splitLines = false;
        options->formatTable = false;
        html2md::Converter c(statementText,options);
        auto md = c.convert();
        processText(md);

        editString(md);
        
        fs::create_directories(folderPathproblem);

        std::ofstream outputFile(filePath);
        if (!outputFile.is_open()) {
            logMessage(logFilePath, "Failed to open output file for problem " + contestId + problemId);
            continue;
        }

        outputFile << md;

        outputFile.close();
        logMessage(logFilePath, "Problem statement for " + contestId + problemId + " has been saved to " + filePath);
        addProblemToCSV(csvFilePath, name, std::stoi(rating), tags, filePath);

        xmlFreeDoc(doc);
        xmlCleanupParser();

        count++;
    }
}

std::string getCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y%m%d_%H%M%S");

    return ss.str();
}

ProblemSetGenerator::ProblemSetGenerator(std::string path_to_csv, std::string rating, std::string tag) {
    this->pathToFolder_ = path_to_csv;

    if (rating.find('-') != std::string::npos) {
        // User provided a range of ratings
        size_t dashPos = rating.find('-');

        std::string lowestRatingStr = rating.substr(0, dashPos);
        std::string highestRatingStr = rating.substr(dashPos + 1);

        this->lowest_rating = lowestRatingStr;
        this->highest_rating = highestRatingStr;
    } else {
        this->lowest_rating = rating;
        this->highest_rating = this->lowest_rating;
    }

    this->tag_ = tag;
}


void addProblemtoMD(const std::string& currentTimestamp, const std::string& tag, const std::string& pathToFolder, const std::string& problemFolderPath) {
    std::string problemSetFilePath = pathToFolder + "/" + tag + currentTimestamp + ".md";
    std::ofstream outputFile(problemSetFilePath, std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for problem set" << std::endl;
        return;
    }

    std::ifstream problemFile(problemFolderPath);
    if (problemFile.is_open()) {
        std::string line;
        while (std::getline(problemFile, line)) {
            outputFile << line << std::endl;
        }
        problemFile.close();
    } else {
        std::cerr << "Failed to open problem file: " << problemFolderPath << std::endl;
    }

    outputFile << "<div style=\"page-break-after: always;\"></div>\n\n"; // Add page break
    outputFile.close();
}
 
void ProblemSetGenerator::generateProblemSet(){
    std::string pathtocsv = this->pathToFolder_ + "/" + "ParsedProblems.csv";
    std::ifstream csvFile(pathtocsv);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << this->pathToFolder_ << std::endl;
        return;
    }

    std::string currentTimestamp = getCurrentTimeAsString();

    std::string line;
    std::getline(csvFile, line); // Skip the first line

    while (std::getline(csvFile, line)) {
        std::stringstream lineStream(line);
        std::string cell;

        std::getline(lineStream, cell, ','); // Name
        std::getline(lineStream, cell, ','); // Rating
        int rating = std::stoi(cell);
        std::getline(lineStream, cell, ','); // Tags
        std::string tag = cell;
        std::getline(lineStream, cell, ','); // Problem path
        std::string problemPath = cell;

        if (rating >= std::stoi(this->lowest_rating) && rating <= std::stoi(this->highest_rating)) {
            if (tag == this->tag_) {
                addProblemtoMD(currentTimestamp, tag, this->pathToFolder_, problemPath);
            }
        }
    }
    std::string setFilePath = this->tag_ + currentTimestamp + ".md";
    std::cout << "Problem set " << setFilePath << " has been saved" << std::endl;

    csvFile.close();
}
