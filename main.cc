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
#include <chrono>

#include <algorithm>

#include <cstring>

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#pragma execution_character_set("utf-8")

#else

#include <dirent.h>

#endif

#include "Image.h"
#include "Database.h"
#include "locale_en.h"

#define COLOR_NONE "\x1b[0m"
#define COLOR_ERROR "\x1b[91m"
#define COLOR_INFO "\x1b[93m"
#define COLOR_QUERY "\x1b[32m"

#define COLOR_LINE "\x1b[37m"

#define COLOR_SWM "\x1b[31m"
#define COLOR_MATCH "\x1b[91m"
#define COLOR_LOC "\x1b[32m"
#define COLOR_NOTE "\x1b[93m"

#define DATA_DIR "data"

Database db;

bool readDB() {
    std::vector<std::string> files;

#if defined(_WIN32) || defined(_WIN64)
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFile(DATA_DIR "\\*", &ffd);
    if (hFind == INVALID_HANDLE_VALUE){
        std::cout << COLOR_ERROR << STR_ERROR_DIR << COLOR_NONE << std::endl;
        return false;
    }

    do {
        if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)){
            files.emplace_back(std::string(DATA_DIR) + '/' + ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
#else
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(DATA_DIR)) != nullptr){
        while ((ent = readdir(dir)) != nullptr) {
            if (ent->d_name[0] != '.'){
                files.emplace_back(std::string(DATA_DIR) + '/' + ent->d_name);
            }
        }
        closedir(dir);
    } else{
        std::cout << COLOR_ERROR << STR_ERROR_DIR << COLOR_NONE << std::endl;
        return false;
    }
#endif

    std::sort(files.begin(), files.end());
    for (auto &file : files) {
        if (!db.parseFile(file)){
            std::cout << COLOR_ERROR << STR_ERROR_FILE << file << COLOR_NONE << std::endl;
        }
    }

    std::cout << COLOR_INFO << STR_INFO_RECORDS << db.size() << COLOR_NONE << std::endl;

    return true;
}

int main() {
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

#if defined(_MSC_VER)
        auto output = GetStdHandle(STD_OUTPUT_HANDLE);
        if(output == INVALID_HANDLE_VALUE) return 1;

        DWORD oldmode, newmode;
        if(!GetConsoleMode(output, &oldmode)) return 1;

        newmode = ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        if(!SetConsoleMode(output, newmode)) return 1;
#endif
#endif

    std::string breakLine = std::string(79, '-');

    breakLine[0] = '+';
    breakLine[20] = '+';
    breakLine[33] = '+';
    breakLine[78] = '+';

    if (!readDB()){
        return 1;
    }

    while (true) {
        std::string query;
        std::cout << COLOR_QUERY << STR_QUERY << COLOR_NONE;
        std::getline(std::cin, query);
        if (std::cin.eof()) break;
        if (query.empty()){
            continue;
        } else if (query == "!q"){
            break;
        } else if (query == "!r"){
            db.clear();
            readDB();
            continue;
        }

        std::cout << std::endl;

        auto t1 = std::chrono::high_resolution_clock::now();
        auto results = db.findImage(query);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

        if (!results.empty()){
            std::cout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;
            std::cout << COLOR_LINE << "| " << COLOR_NONE << std::setw(17) << STR_HEADER_SWM;
            std::cout << COLOR_LINE << " | " << COLOR_NONE << std::setw(10) << STR_HEADER_LOCATION;
            std::cout << COLOR_LINE << " | " << COLOR_NONE << std::left << std::setw(42) << STR_HEADER_INFO
                      << std::right;
            std::cout << COLOR_LINE << " |" << COLOR_NONE << std::endl;

            std::string lastSWM;
            for (auto &result : results) {
                if (lastSWM != result.getSwm()){
                    lastSWM = result.getSwm();
                    std::cout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;
                }

                std::string colorized = lastSWM.substr(0, lastSWM.find(query)) +
                                        COLOR_MATCH + lastSWM.substr(lastSWM.find(query), query.size()) +
                                        COLOR_SWM + lastSWM.substr(lastSWM.find(query) + query.size());


                std::cout << COLOR_LINE << "| " << COLOR_SWM
                          << std::setw(17 + (int)(strlen(COLOR_MATCH) + strlen(COLOR_SWM))) << colorized << COLOR_NONE;
                std::cout << COLOR_LINE << " | " << COLOR_LOC << std::setw(10) << result.getLocation()
                          << COLOR_NONE;
                std::cout << COLOR_LINE << " | " << COLOR_NOTE << std::left << std::setw(42)
                          << (result.getNotes().empty() ? std::string() : result.getNotes().substr(0, 41)) << std::right
                          << COLOR_NONE;
                std::cout << COLOR_LINE << " |" << COLOR_NONE << std::endl;
            }
            std::cout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;

            std::cout << COLOR_INFO << STR_INFO_RESULTS << results.size() << " (" << time.count() << "s)" << COLOR_NONE
                      << std::endl << std::endl;
        }
    }

    std::cout << std::endl << COLOR_INFO << STR_INFO_BYE << COLOR_NONE << std::endl;


    return 0;
}