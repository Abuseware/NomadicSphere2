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


#include "Image.h"

Image::Image() {
    this->swm = nullptr;
    this->notes = nullptr;
    this->location = nullptr;
}

Image::Image(const std::wstring swm, const std::wstring notes, const std::wstring location) {
    this->swm = swm;
    this->notes = notes;
    this->location = location;
}

const std::wstring &Image::getSwm() const {
    return swm;
}

void Image::setSwm(const std::wstring &swm) {
    Image::swm = swm;
}

const std::wstring &Image::getNotes() const {
    return notes;
}

void Image::setNotes(const std::wstring &notes) {
    Image::notes = notes;
}

const std::wstring &Image::getLocation() const {
    return location;
}

void Image::setLocation(const std::wstring &location) {
    Image::location = location;
}

bool Image::compare(Image a, Image b) {
    return (a.getSwm() < b.getSwm());
}
