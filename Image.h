//
// Created by Artur Kaleta on 2019-03-02.
//

#include <string>

#ifndef IMAGESEARCH2_IMAGE_H
#define IMAGESEARCH2_IMAGE_H


class Image {
private:
    std::wstring swm;
    std::wstring notes;
    std::wstring location;

public:
    const std::wstring &getSwm() const;

    void setSwm(const std::wstring &swm);

    const std::wstring &getNotes() const;

    void setNotes(const std::wstring &notes);

    const std::wstring &getLocation() const;

    void setLocation(const std::wstring &location);

    Image();

    Image(std::wstring swm, std::wstring notes, std::wstring location);

    static bool compare(Image a, Image b);


};


#endif //IMAGESEARCH2_IMAGE_H
