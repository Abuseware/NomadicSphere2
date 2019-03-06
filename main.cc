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

#include <codecvt>

#include <dirent.h>

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#endif

#include "Image.h"
#include "Database.h"

#define COLOR_NONE L"\e[0m"
#define COLOR_ERROR L"\e[91m"
#define COLOR_INFO L"\e[93m"
#define COLOR_QUERY L"\e[32m"

#define COLOR_LINE L"\e[37m"

#define COLOR_SWM L"\e[31m"
#define COLOR_MATCH L"\e[91m"
#define COLOR_LOC L"\e[32m"
#define COLOR_NOTE L"\e[93m"

#define DATA_DIR "data"

Database db;

bool readDB() {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

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
                std::wcout << COLOR_ERROR << L"Error while opening file " << converter.from_bytes(file) << COLOR_NONE
                           << std::endl;
            }
        }
    } else {
        std::wcout << COLOR_ERROR << L"No data available!" << COLOR_NONE << std::endl;
        return false;
    }

    std::wcout << COLOR_INFO << L"Loaded " << db.size() << L" records." << COLOR_NONE << std::endl;

    return true;
}

int main() {
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(65001);
#endif

    std::locale loc = std::locale("");
    std::locale::global(loc);
    std::setlocale(LC_ALL, loc.name().c_str());
    std::wcout.imbue(loc);
    std::wcin.imbue(loc);

    //std::wcout << L"Mąkę żytnią mielił ze zbóż" << std::endl;

    std::wstring headLine = std::wstring(79, L'\u2501');
    std::wstring breakLine = headLine;
    std::wstring endLine = headLine;

    headLine[0] = L'\u250F';  // ┏
    headLine[20] = L'\u2533'; // ┳
    headLine[33] = L'\u2533'; // ┳
    headLine[78] = L'\u2513'; // ┓


    breakLine[0] = L'\u2523';  // ┣
    breakLine[20] = L'\u254B'; // ╋
    breakLine[33] = L'\u254B'; // ╋
    breakLine[78] = L'\u252B'; // ┫

    endLine[0] = L'\u2517';  // ┗
    endLine[20] = L'\u253B'; // ┻
    endLine[33] = L'\u253B'; // ┻
    endLine[78] = L'\u251B'; // ┛


    if (!readDB()) {
        return 1;
    }

    while (true) {
        std::wstring query;
        std::wcout << COLOR_QUERY L">> Search for: " << COLOR_NONE;
        std::getline(std::wcin, query);
        if (query == L"!q") {
            break;
        } else if (query == L"!r") {
            db.clear();
            readDB();
            continue;
        }

        auto results = db.findImage(query);

        std::wcout << COLOR_INFO << L"Found " << results.size() << L" records." << COLOR_NONE << std::endl;

        if (!results.empty()) {
            std::wcout << COLOR_LINE << headLine << COLOR_NONE << std::endl;
            std::wcout << COLOR_LINE << L"\u2503 " << COLOR_NONE << std::setw(17) << L"SWM";
            std::wcout << COLOR_LINE << L" \u2503 " << COLOR_NONE << std::setw(10) << L"Source";
            std::wcout << COLOR_LINE << L" \u2503 " << COLOR_NONE << std::left << std::setw(42) << L"Additional info"
                       << std::right;
            std::wcout << COLOR_LINE << L" \u2503" << COLOR_NONE << std::endl;

            std::stable_sort(results.begin(), results.end(), Image::compare);

            std::wstring lastSWM;
            for (auto &result : results) {
                if (lastSWM != result.getSwm()) {
                    lastSWM = result.getSwm();
                    std::wcout << COLOR_LINE << breakLine << COLOR_NONE << std::endl;
                }

                std::wstring colorized = lastSWM.substr(0, lastSWM.find(query)) +
                                         COLOR_MATCH + lastSWM.substr(lastSWM.find(query), query.size()) +
                                         COLOR_SWM + lastSWM.substr(lastSWM.find(query) + query.size());


                std::wcout << COLOR_LINE << L"\u2503 " << COLOR_SWM
                           << std::setw(17 + (wcslen(COLOR_MATCH) + wcslen(COLOR_SWM))) << colorized << COLOR_NONE;
                std::wcout << COLOR_LINE << L" \u2503 " << COLOR_LOC << std::setw(10) << result.getLocation()
                           << COLOR_NONE;
                std::wcout << COLOR_LINE << L" \u2503 " << COLOR_NOTE << std::left << std::setw(42)
                           << (result.getNotes().empty() ? L"" : result.getNotes()) << std::right << COLOR_NONE;
                std::wcout << COLOR_LINE << L" \u2503" << COLOR_NONE << std::endl;
            }
            std::wcout << COLOR_LINE << endLine << COLOR_NONE << std::endl;
        }
    }


    return 0;
}