#ifndef CARACT_H_INCLUDED
#define CARACT_H_INCLUDED

#include <vector>
#include <iostream>
#include <string.h>

#define FULL_SIZE 1.f
#define HALF_SIZE 2.f
#define THIRD_SIZE 3.f

#define SAME_POSITION 0.f
#define HALF_POSITION 2.f

#define FIRST_RECT_WIEGHT -1

struct rect_t
{
    int x;
    int y;
    int length;
    int height;
    int wieght;
};

struct caract_t
{
    unsigned int nb_rect;
    std::vector<rect_t> caract;
};

struct define_rect_caract_t
{
    float offset_x;
    float offset_y;
    float length_factor;
    float height_factor;
    int wieght;
};

struct define_caract_t
{
    unsigned int nb_rect;
    std::vector<define_rect_caract_t> caract;
};

std::vector<define_caract_t> all_caract_type;

void get_rects(FILE* file, caract_t &caract)
{
    char tag_rect[] = "<rect>";
    char tag_rect_end[] = "<\\rect>";
    char temp[100];
    caract.nb_rect = 0;
    caract.caract.clear();

    rect_t rect;

    do{fscanf(file,"%s",temp);}while(strcmp(temp,tag_rect));
    do
    {
        caract.nb_rect ++;
        caract.caract.resize(caract.nb_rect);
        fscanf(file, "%d %d %d %d %d", &rect.x, &rect.y, &rect.length, &rect.height, &rect.wieght);
        caract.caract[caract.nb_rect - 1] = rect;
        do{fscanf(file,"%s",temp);}while(strcmp(temp,tag_rect_end));
        fscanf(file,"%s",temp);
    }
    while(!strcmp(temp,tag_rect));
}

void define_all_caract_type()
{
    all_caract_type.clear();
    all_caract_type.resize(2);

    define_rect_caract_t rect2;
    define_rect_caract_t rect3;

    define_caract_t define_caract;

    /* carct 1 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = FULL_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type[0] = define_caract;

     /* carct 2 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = FULL_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type[1] = define_caract;

    std::cout << "size : " << all_caract_type.size() << std::endl;
}

#endif // CARACT_H_INCLUDED
