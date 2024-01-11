#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>


class ProblemSetGenerator {
    private:
    std::string pathToFolder_; 
    std::string lowest_rating;
    std::string highest_rating;
    std::string tag_;

    public:
    /**
     * @brief Конструктор класса ProblemSetGenerator
     * 
     * @param pathToFolder 
     * @param ratings 
     * @param tag 
     */
    ProblemSetGenerator(std::string pathToFolder, std::string ratings, std::string tag);

    /**
     * @brief Метод для добавления задачи в MD файл
     * 
     * @param currentTimestamp 
     * @param tag 
     * @param pathToFolder 
     * @param problemFolderPath 
     */
    void addProblemtoMD(const std::string& currentTimestamp, const std::string& tag, const std::string& pathToFolder, const std::string& problemFolderPath);

    /**
     * @brief Метод для генерации набора задач
     * 
     */
    void generateProblemSet();
};

/**
 * @brief Получить текущее время в виде строки
 * 
 * @return std::string 
 */
std::string getCurrentTimeAsString();