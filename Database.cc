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
#include <thread>
#include <mutex>

#include "Database.h"


Database::Database(const std::string &fileName) {
    parseFile(fileName);
}

Image Database::parseLine(const std::string &location, const std::string &line) {
    std::string swm = line;
    std::string notes;

    unsigned long split = line.find('_');
    if (split != std::string::npos){
        swm = line.substr(0UL, split);
        notes = line.substr(split + 1UL);
        std::replace(notes.begin(), notes.end(), '-', ' ');
        std::replace(notes.begin(), notes.end(), '_', ';');
    }

    return Image(swm, notes, location);
}

bool Database::parseFile(const std::string &fileName) {
    auto file = std::ifstream(fileName);

    if (!file){
        return false;
    }

    unsigned long splitPath = fileName.rfind('/');
    unsigned long splitExt = fileName.rfind('.');

    std::string location = fileName.substr(splitPath + 1, splitExt - splitPath - 1);


    std::string line;
    while (std::getline(file, line)) {
        line = line.substr(0, line.rfind('\r'));
        if (!line.empty())
            imageList.emplace_back(parseLine(location, line));
    }

    return true;
}

void Database::findImageThread(const std::vector<Image> &source, std::vector<Image> &results, std::mutex &resultsLock,
                               const unsigned long &begin, const unsigned long &end, const std::string &swm) {

    for (unsigned long i = begin; i != end; i++) {
        if (source[i].getSwm().find(swm) != std::string::npos){
            resultsLock.lock();
            results.emplace_back(source[i]);
            resultsLock.unlock();
        }
        //std::this_thread::yield();
    }
}

std::vector<Image> Database::findImage(const std::string &swm) {
    auto nproc = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::vector<Image> results;
    std::mutex resultsLock;

    if (nproc > 0){
        for (unsigned int i = 0; i < nproc; i++) {
            auto size = imageList.size();
            unsigned long begin = (size / nproc) * i;
            unsigned long end;
            if (i == nproc - 1){
                end = size;
            } else{
                end = (size / nproc) * (i + 1);
            }
            threads.emplace_back(Database::findImageThread, std::ref(imageList), std::ref(results),
                                 std::ref(resultsLock), begin, end, swm);
        }

        for (auto &t : threads) {
            t.join();
        }
    } else{
        for (auto &img : imageList) {
            if (img.getSwm().find(swm) != std::string::npos){
                results.emplace_back(img);
            }
        }
    }

    std::stable_sort(results.begin(), results.end(), Image::compare);

    return results;
}

unsigned long Database::size() {
    return imageList.size();
}

void Database::clear() {
    imageList.clear();
}
