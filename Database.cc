/*
 * Copyright (c) 2019, Artur "Licho" Kaleta
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>

#include "Database.h"

Database::Database(const std::string fileName) {
    parseFile(fileName);
}

Image Database::parseLine(const std::string location, const std::string line) {
    std::string swm = line;
    std::string notes = "";

    unsigned long split = line.find('_');
    if (split != std::string::npos) {
        swm = line.substr(0, split);
        notes = line.substr(split + 1);
        std::replace(notes.begin(), notes.end(), '-', ' ');
        std::replace(notes.begin(), notes.end(), '_', ';');
    }

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
        if (!line.empty())
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

void Database::clear() {
    imageList.clear();
}
