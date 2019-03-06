//
// Created by Artur Kaleta on 2019-03-02.
//

#include <string>
#include <vector>
#include <fstream>
#include "Image.h"

#ifndef IMAGESEARCH2_DATABASE_H
#define IMAGESEARCH2_DATABASE_H


class Database {
private:
    std::vector<Image> imageList;

public:
    Database() = default;

    explicit Database(std::string fileName);

    Image parseLine(std::wstring location, std::wstring line);

    bool parseFile(std::string fileName);

    std::vector<Image> findImage(std::wstring swm);

    void clear();

    unsigned long size();


};


#endif //IMAGESEARCH2_DATABASE_H
