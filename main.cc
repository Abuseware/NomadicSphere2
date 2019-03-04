#include <iostream>
#include <iomanip>
#include <algorithm>

#include <dirent.h>

#include "Image.h"
#include "Database.h"

static const std::string dataDir = "data";

Database db;

bool readDB() {
    DIR *dir;
    struct dirent *ent;

    std::vector<std::string> files;

    if ((dir = opendir(dataDir.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (ent->d_name[0] != '.') {
                files.push_back(dataDir + PATH_SEPARATOR + ent->d_name);
            }
        }
        closedir(dir);

        std::sort(files.begin(), files.end());
        for (auto &file : files) {
            if (!db.parseFile(file)) {
                std::cout << "Nie można odczytać pliku: " << file << std::endl;
            }
        }
    } else {
        std::cout << "Nie można odczytać katalogu bazy danych!" << std::endl;
        return false;
    }

    std::cout << "Wczytano " << db.size() << " rekordów." << std::endl;

    return true;
}

int main() {

    std::string breakLine = std::string(79, '-');

    breakLine[0] = '+';
    breakLine[20] = '+';
    breakLine[33] = '+';
    breakLine[78] = '+';

    if (!readDB()) {
        return 1;
    }

    while (true) {
        std::string query;
        std::cout << ">> Podaj SWM: ";
        std::getline(std::cin, query);
        if (query == "!q") {
            break;
        }

        std::cout << breakLine << std::endl;
        std::cout << "| " << std::setw(17) << "SWM";
        std::cout << " | " << std::setw(13) << "Źródło";
        std::cout << " | " << std::setw(42) << "Informacje";
        std::cout << " |" << std::endl;
        //std::cout << breakLine << std::endl;

        auto results = db.findImage(query);
        std::stable_sort(results.begin(), results.end(), Image::compare);

        std::string lastSWM;
        for (auto &result : results) {
            if (lastSWM != result.getSwm()) {
                lastSWM = result.getSwm();
                std::cout << breakLine << std::endl;
            }

            std::cout << "| " << std::setw(17) << result.getSwm();
            std::cout << " | " << std::setw(10) << result.getLocation();
            std::cout << std::left << " | " << std::setw(42) << (result.getNotes().empty() ? "" : result.getNotes())
                      << std::right;
            std::cout << " |" << std::endl;
        }
        std::cout << breakLine << std::endl;

        std::cout << "Znaleziono: " << results.size() << std::endl;
    }


    return 0;
}