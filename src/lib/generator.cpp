#include "generator.hpp"

std::string getCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y%m%d_%H%M%S");

    return ss.str();
}

void ProblemSetGenerator::addProblemtoMD(const std::string& currentTimestamp, const std::string& tag, const std::string& pathToFolder, const std::string& problemFolderPath) {
    std::string problemSetFilePath = pathToFolder + "/problems/problemsets";
    std::filesystem::create_directory(problemSetFilePath);

    problemSetFilePath += "/" + tag + currentTimestamp + ".md";
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

void ProblemSetGenerator::generateProblemSet(){
    std::string pathtocsv = this->pathToFolder_ + "/problems/" + "ParsedProblems.csv";
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

        if ((rating >= std::stoi(this->lowest_rating) && rating <= std::stoi(this->highest_rating)) || std::stoi(this->lowest_rating) == 0) {
            if (tag == this->tag_ || this->tag_ == "all") {
                addProblemtoMD(currentTimestamp, this->tag_, this->pathToFolder_, problemPath);
            }
        }
    }
    std::string setFilePath = this->tag_ + currentTimestamp + ".md";
    std::cout << "Problem set " << setFilePath << " has been saved" << std::endl;

    csvFile.close();
}