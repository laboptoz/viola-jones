#include "caract.h"
#include <cstdio>
#include <cstdlib>

Caracteristics::Caracteristics()
{

}

Caracteristics::~Caracteristics()
{

}

int Caracteristics::get_id(FILE* file)
{
    char tag_id[] = "<ID>";
    char temp[50];
    int ID, test;

    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_id)) && (test == 1));
    if(test != 1)
        return -1;

    test = fscanf(file,"%d",&ID);
    if(test != 1)
        return -1;
    else
        return ID;
}

int Caracteristics::get_rects(FILE* file, caract_t &caract)
{
        char tag_rect[] = "<rect>";
        char tag_rect_end[] = "<\\rect>";
        char temp[50];
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

void Caracteristics::set_rects(FILE* file, caract_t caract)
{
        for(unsigned int i = 0; i < caract.nb_rect; i++)
        {
            fprintf(file, "<rect> %d %d %d %d %d <\\rect> ", caract.caract[i].x, caract.caract[i].y, caract.caract[i].length,caract.caract[i].height, caract.caract[i].wieght);
        }
}

bool Caracteristics::compare_caracts(caract_t caract1, caract_t caract2, int ID_1, int ID_2)
{
    bool result = true;

    if(ID_1 == ID_2)
    {
        if(caract1.nb_rect == caract2.nb_rect)
        {
            for(unsigned int i = 0; i < caract1.nb_rect; i++)
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
            return -1;
    }
    else
        return -1;
}

unsigned int Caracteristics::get_nb_caracteristics(FILE* file)
{
    fseek(file, 0, 0);
    char tag_nb_caract[] = "<CAR>";
    char temp[30];
    unsigned int test;
    do{test = fscanf(file,"%s",temp);}while((strcmp(temp,tag_nb_caract)) && (test == 1));
    fscanf(file,"%u",&test);

    return test;
}

void Caracteristics::define_all_caract_type()
{
    all_caract_type.clear();
    all_caract_type.reserve(6);

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
    all_caract_type.push_back(define_caract);

     /* carct 2 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = FULL_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

     /* carct 3 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 2;

    rect3.offset_x = HALF_POSITION;
    rect3.offset_y = HALF_POSITION;
    rect3.length_factor = HALF_SIZE;
    rect3.height_factor = HALF_SIZE;
    rect3.wieght = 2;

    define_caract.nb_rect = 3;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    define_caract.caract[1] = rect3;
    all_caract_type.push_back(define_caract);

    /* carct 4 */
    rect2.offset_x = QUARTER_POSITION;
    rect2.offset_y = QUARTER_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 4;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

    /* carct 5 */
    rect2.offset_x = QUARTER_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = FULL_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

    /* carct 6 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = QUARTER_POSITION;
    rect2.length_factor = FULL_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);


    std::cout << "nb caracteristic patern : " << all_caract_type.size() << std::endl;
}
