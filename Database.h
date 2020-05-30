//
// Created by Artur Kaleta on 2019-03-02.
//

#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include "Image.h"

#ifndef IMAGESEARCH2_DATABASE_H
#define IMAGESEARCH2_DATABASE_H


class Database {
private:
    std::vector<Image> imageList;

    static void findImageThread(const std::vector<Image> &source, std::vector<Image> &results, std::mutex &resultsLock,
                                const unsigned long &begin, const unsigned long &end, const std::string &swm);

public:
    Database() = default;

    explicit Database(const std::string &fileName);

    Image parseLine(const std::string &location, const std::string &line);

    bool parseFile(const std::string &fileName);

    std::vector<Image> findImage(const std::string &swm);

    void clear();

    size_t size();


};


#endif //IMAGESEARCH2_DATABASE_H
