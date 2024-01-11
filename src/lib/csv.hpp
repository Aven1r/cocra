#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

/**
 * @brief Создать CSV файл с данными о задачах
 * 
 * @param filePath Путь к CSV файлу
 */
void createCSV(const std::string& filePath) {
    std::ofstream csvFile(filePath, std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to create CSV file: " << filePath << std::endl;
        return;
    }

    // Проверка, пустой ли файл
    csvFile.seekp(0, std::ios::end);
    if (csvFile.tellp() == 0) {
        csvFile << "name,rating,tags,folderpath\n";
    }

    csvFile.close();
}

/**
 * @brief Добавить задачу в CSV файл
 * 
 * @param filePath Относительный путь к CSV файлу
 * @param name Название задачи
 * @param rating Рейтинг задачи
 * @param tags Теги задачи
 * @param folderPath Путь к папке с задачей
 */
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