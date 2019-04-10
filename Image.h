//
// Created by Artur Kaleta on 2019-03-02.
//

#include <string>

#ifndef IMAGESEARCH2_IMAGE_H
#define IMAGESEARCH2_IMAGE_H


class Image {
private:
    std::string swm;
    std::string notes;
    std::string location;

public:
    const std::string &getSwm() const;

    void setSwm(const std::string &swm);

    const std::string &getNotes() const;

    void setNotes(const std::string &notes);

    const std::string &getLocation() const;

    void setLocation(const std::string &location);

    Image();

    Image(std::string swm, std::string notes, std::string location);

    static bool compare(Image a, Image b);


};


#endif //IMAGESEARCH2_IMAGE_H
