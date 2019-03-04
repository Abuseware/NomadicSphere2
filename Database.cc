//
// Created by Artur Kaleta on 2019-03-02.
//

#include <algorithm>

#include "Database.h"

Database::Database(const std::string fileName) {
    parseFile(fileName);
}

Image Database::parseLine(const std::string location, const std::string line) {
    std::string swm, notes;

    long split = line.find('_');
    if (split != std::string::npos) {
        swm = line.substr(0, split);
        notes = line.substr(split + 1);
        std::replace(notes.begin(), notes.end(), '-', ' ');
        std::replace(notes.begin(), notes.end(), '_', ';');
    } else {
        swm = line;
        notes = "";
    }

    //std::cout << "Dodano: " << swm << " - " << notes << " @" << location << std::endl;


    return Image(swm, notes, location);
}

bool Database::parseFile(const std::string fileName) {
    auto file = std::ifstream(fileName);

    if (!file) {
        return false;
    }

    unsigned long splitPath = fileName.rfind(PATH_SEPARATOR);
    unsigned long splitExt = fileName.rfind('.');

    std::string location = fileName.substr(splitPath + 1, splitExt - splitPath - 1);


    std::string line;

    while (std::getline(file, line)) {
        line = line.substr(0, line.rfind('\r'));
        imageList.push_back(parseLine(location, line));
    }

    return true;
}

std::vector<Image> Database::findImage(std::string swm) {
    std::vector<Image> results;

    for (auto &img : imageList) {
        if (img.getSwm().find(swm) != std::string::npos) {
            results.push_back(img);
        }
    }

    return results;
}

unsigned long Database::size() {
    return imageList.size();
}
