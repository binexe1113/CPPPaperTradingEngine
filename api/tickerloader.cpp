#include <fstream>
#include <string>
#include <vector>

//ibov_yahoo = magic name from the source file
std::vector<std::string> loadTickers(const std::string& filename) {

    std::ifstream file(filename);
    std::vector<std::string> tickers;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty())
            tickers.push_back(line);
    }
    return tickers;
}

