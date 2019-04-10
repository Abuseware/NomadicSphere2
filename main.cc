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


#include <iostream>
#include <iomanip>

#include <algorithm>

#include <cstring>

#include <dirent.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

#include "Image.h"
#include "Database.h"
#include "locale_en.h"

#define COLOR_NONE "\e[0m"
#define COLOR_ERROR "\e[91m"
#define COLOR_INFO "\e[93m"
#define COLOR_QUERY "\e[32m"

#define COLOR_LINE "\e[37m"

#define COLOR_SWM "\e[31m"
#define COLOR_MATCH "\e[91m"
#define COLOR_LOC "\e[32m"
#define COLOR_NOTE "\e[93m"

#define DATA_DIR "data"

Database db;

bool readDB() {
    DIR *dir;
    struct dirent *ent;

    std::vector<std::string> files;

    if ((dir = opendir(DATA_DIR)) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (ent->d_name[0] != '.') {
                files.push_back(std::string(DATA_DIR) + PATH_SEPARATOR + ent->d_name);
            }
        }
        closedir(dir);

        std::sort(files.begin(), files.end());
        for (auto &file : files) {
            if (!db.parseFile(file)) {
                std::cout << COLOR_ERROR << STR_ERROR_FILE << file << COLOR_NONE << std::endl;
            }
        }
    } else {
        std::cout << COLOR_ERROR << STR_ERROR_DIR << COLOR_NONE << std::endl;
        return false;
    }

    std::cout << COLOR_INFO << STR_INFO_RECORDS << db.size() << COLOR_NONE << std::endl;

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
        std::cout << COLOR_QUERY << STR_QUERY << COLOR_NONE;
        std::getline(std::cin, query);
        if (query.empty()){
            continue;
        } else if (query == "!q") {
            break;
        } else if (query == "!r") {
            db.clear();
            readDB();
            continue;
        }

        auto results = db.findImage(query);

        std::cout << COLOR_INFO << STR_INFO_RESULTS << results.size() << COLOR_NONE << std::endl;

        if (!results.empty()) {
            std::cout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;
            std::cout << COLOR_LINE << "| " << COLOR_NONE << std::setw(17) << STR_HEADER_SWM;
            std::cout << COLOR_LINE << " | " << COLOR_NONE << std::setw(10) << STR_HEADER_LOCATION;
            std::cout << COLOR_LINE << " | " << COLOR_NONE << std::left << std::setw(42) << STR_HEADER_INFO
                       << std::right;
            std::cout << COLOR_LINE << " |" << COLOR_NONE << std::endl;

            std::stable_sort(results.begin(), results.end(), Image::compare);

            std::string lastSWM;
            for (auto &result : results) {
                if (lastSWM != result.getSwm()) {
                    lastSWM = result.getSwm();
                    std::cout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;
                }

                std::string colorized = lastSWM.substr(0, lastSWM.find(query)) +
                                         COLOR_MATCH + lastSWM.substr(lastSWM.find(query), query.size()) +
                                         COLOR_SWM + lastSWM.substr(lastSWM.find(query) + query.size());


                std::cout << COLOR_LINE << "| " << COLOR_SWM
                           << std::setw(17 + (strlen(COLOR_MATCH) + strlen(COLOR_SWM))) << colorized << COLOR_NONE;
                std::cout << COLOR_LINE << " | " << COLOR_LOC << std::setw(10) << result.getLocation()
                           << COLOR_NONE;
                std::cout << COLOR_LINE << " | " << COLOR_NOTE << std::left << std::setw(42)
                           << (result.getNotes().empty() ? "" : result.getNotes().substr(0, 41)) << std::right << COLOR_NONE;
                std::cout << COLOR_LINE << " |" << COLOR_NONE << std::endl;
            }
            std::cout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;
        }
    }

    std::cout << COLOR_INFO << STR_INFO_BYE << COLOR_NONE << std::endl;


    return 0;
}