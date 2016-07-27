#include "caract.h"
#include <cstdio>
#include <cstdlib>

Caracteristics::Caracteristics()
{

}

Caracteristics::~Caracteristics()
{

}

int Caracteristics::get_rects(FILE* file, caract_t &caract)
{
    char tag_rect[] = "<rect>";
    char tag_rect_end[] = "<\\rect>";
    char temp[100];
    caract.nb_rect = 0;
    caract.caract.clear();

    rect_t rect;
    int test;

    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_rect)) && (test == 1));
    if(test != 1)
        return -1;
    do
    {
        caract.nb_rect ++;
        caract.caract.resize(caract.nb_rect);
        test = fscanf(file, "%d %d %d %d %d", &rect.x, &rect.y, &rect.length, &rect.height, &rect.wieght);
            caract.caract[caract.nb_rect - 1] = rect;
            do{fscanf(file,"%s",temp);}while(strcmp(temp,tag_rect_end));
            fscanf(file,"%s",temp);
    }
    while(!strcmp(temp,tag_rect));
    return 0;
}

void Caracteristics::set_rects(FILE* file, caract_t caract)
{
    for(unsigned int i = 0; i < caract.nb_rect; i++)
    {
        fprintf(file, "<rect> %d %d %d %d %d <\\rect> ", caract.caract[i].x, caract.caract[i].y, caract.caract[i].length,caract.caract[i].height, caract.caract[i].wieght);
    }
}

void Caracteristics::define_all_caract_type()
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
