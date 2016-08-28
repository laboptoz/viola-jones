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

inline int go_to_data(FILE* file)
{
    char tag_data[] = "<D>";
    char temp[50];
    int test;

    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_data)) && (test == 1));

    if(test != 1)
        return ERROR;
    else
        return 0;
}

inline int get_id(FILE* file)
{
    char tag_id[] = "<ID>";
    char temp[50];
    int ID, test;

    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_id)) && (test == 1));
    if(test != 1)
        return ERROR;

    test = fscanf(file,"%d",&ID);
    if(test != 1)
        return ERROR;
    else
        return ID;
}

inline int get_rects(FILE* file, caract_t &caract)
{
        char tag_rect[] = "<R>";
        char tag_rect_end[] = "<\\R>";
        char temp[50];
        caract.nb_rect = 0;
        caract.caract.clear();

        rect_t rect;
        int test;

        do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_rect)) && (test == 1));
        if(test != 1)
            return ERROR;

        do
        {
            caract.nb_rect ++;
            caract.caract.resize(caract.nb_rect);
            test = fscanf(file, "%d %d %d %d %d", &rect.x, &rect.y, &rect.length, &rect.height, &rect.wieght);
            if(test == 5)
            {
                caract.caract[caract.nb_rect - 1] = rect;
                do{fscanf(file,"%s",temp);}while(strcmp(temp,tag_rect_end));
                fscanf(file,"%s",temp);
            }
            else
                printf("error !!\n\n");
        }
        while((!strcmp(temp,tag_rect)) && (test == 5));
    return 0;
}

inline void set_rects(FILE* file, caract_t &caract)
{
        register unsigned int i;
        for(i = 0; i < caract.nb_rect; i++)
        {
            fprintf(file, "<R> %d %d %d %d %d <\\R> ", caract.caract[i].x, caract.caract[i].y, caract.caract[i].length,caract.caract[i].height, caract.caract[i].wieght);
        }
}

inline void set_nb_caract(FILE* file, int nb_caract)
{
    char tag_car[] = "<CAR>";
    char temp[50];
    int test;

    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_car)) && (test == 1));

    fprintf(file, " %d", nb_caract);
}

inline bool compare_caracts(caract_t caract1, caract_t caract2, int ID_1, int ID_2)
{
    bool result = true;

    if(ID_1 == ID_2)
    {
        if(caract1.nb_rect == caract2.nb_rect)
        {
            register unsigned int i;
            for(i = 0; i < caract1.nb_rect; i++)
            {
                result = result &&
                        (caract1.caract[i].x == caract2.caract[i].x) &&
                        (caract1.caract[i].y == caract2.caract[i].y) &&
                        (caract1.caract[i].length == caract2.caract[i].length) &&
                        (caract1.caract[i].height == caract2.caract[i].height) &&
                        (caract1.caract[i].wieght == caract2.caract[i].wieght);
            }
            return result;
        }
        else
            return ERROR;
    }
    else
        return ERROR;
}

inline unsigned int get_nb_caracteristics(FILE* file)
{
    fseek(file, 0, 0);
    char tag_nb_caract[] = "<CAR>";
    char temp[30];
    unsigned int test;
    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_nb_caract)) && (test == 1));
    fscanf(file,"%u",&test);

    return test;
}

#endif // CARACT_H_INCLUDED
