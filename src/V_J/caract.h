#ifndef CARACT_H_INCLUDED
#define CARACT_H_INCLUDED

#include <vector>
#include <iostream>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include "../config.h"

#define FULL_SIZE 1.f
#define HALF_SIZE 2.f
#define THIRD_SIZE 3.f

#define SAME_POSITION 0.f
#define HALF_POSITION 2.f
#define QUARTER_POSITION 4.f

#define FIRST_RECT_WIEGHT -1

enum caracteristic_type_t
{
    true_caract,
    false_caract
};

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

#endif // CARACT_H_INCLUDED
