#include <iostream>
#include <string>
#include <fstream>

inline void saveStringToFile(const std::string& text, const std::string& filename) {
    std::ofstream outputFile(filename);

    if (outputFile.is_open()) {
        outputFile << text << std::endl;
        outputFile.close();
        std::cout << "Data has been written to '" << filename << "'" << std::endl;
    } else {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
    }
}
