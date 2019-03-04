//
// Created by Artur Kaleta on 2019-03-02.
//

#include "Image.h"

Image::Image() {
    this->swm = nullptr;
    this->notes = nullptr;
    this->location = nullptr;
}

Image::Image(const std::string swm, const std::string notes, const std::string location) {
    this->swm = swm;
    this->notes = notes;
    this->location = location;
}

const std::string &Image::getSwm() const {
    return swm;
}

void Image::setSwm(const std::string &swm) {
    Image::swm = swm;
}

const std::string &Image::getNotes() const {
    return notes;
}

void Image::setNotes(const std::string &notes) {
    Image::notes = notes;
}

const std::string &Image::getLocation() const {
    return location;
}

void Image::setLocation(const std::string &location) {
    Image::location = location;
}

bool Image::compare(Image a, Image b) {
    return (a.getSwm() < b.getSwm());
}
