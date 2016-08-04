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
#define QUARTER_POSITION 4.f

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

class Caracteristics
{
public :
    Caracteristics();
    ~Caracteristics();

    std::vector<define_caract_t> all_caract_type;

    int get_id(FILE* file);
    int get_rects(FILE* file, caract_t &caract);
    void set_rects(FILE* file, caract_t caract);
    bool compare_caracts(caract_t caract1, caract_t caract2, int ID_1, int ID_2);
    unsigned int get_nb_caracteristics(FILE* file);
    void define_all_caract_type();
};

#endif // CARACT_H_INCLUDED
