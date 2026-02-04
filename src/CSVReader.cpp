#include "CSVReader.h"
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> 
CSVReader::readCSV(const std::string& filename) {

    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);

    std::string line;

    while (getline(file, line)) {

        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}
