#include "File_caract.h"
#include <cmath>
#include <string.h>
#include <string>
#include <iostream>


File_caract::File_caract()
{
    strcpy(tag_rect, "<R>");
    strcpy(tag_rect_end, "<\\R>");
    strcpy(tag_data, "<D>");
    strcpy(tag_id, "<ID>");
    strcpy(tag_car, "<CAR>");
}

File_caract::~File_caract()
{

}

void File_caract::compute_variances()
{
    if(file_open(read_update_mode) != ERROR)
    {
        int eof, count_image;
        float sum, scare_sum, variance, mean, mini, maxi;
        fpos_t position1, position2;

        rewind(get_file_id());
        do
        {
            eof = go_to_data();
            if(eof != ERROR)
            {
                fgetpos (get_file_id(), &position1);
                fscanf(get_file_id(), "%d %f %f %f %f %f <\\D>", &count_image, &mini, &maxi, &sum, &scare_sum, &variance);
                fgetpos (get_file_id(), &position2);

                mean = sum / (float)count_image;
                variance = scare_sum - mean*mean;
                variance = variance >= 0 ? sqrtf(variance) : VARIANCE_ERROR;

                fsetpos (get_file_id(), &position1);
                fprintf(get_file_id(), " %03d %07.1f %07.1f %07.1f %07.1f %05.1f", count_image, mini, maxi, sum, scare_sum, variance);
                fsetpos (get_file_id(), &position2);
            }
        }
        while(eof != ERROR);

        file_close();
    }
}

void File_caract::generate_caracteristics_from_file(File_caract base_file)
{
    if(file_open(write_mode) != ERROR)
    {
        go_to_origin();
        if(base_file.file_open(read_mode) != ERROR)
        {
            int ID, i =0;
            caract_t caracteristics;
            unsigned int nb_caracts = 0;

            base_file.go_to_origin();

            do
            {
                ID = base_file.get_id();
                i = base_file.get_rects(caracteristics);

                if(i == 0)
                {
                    fprintf(get_file_id(), "<ID> %d <\\ID> ", ID);
                    set_rects(caracteristics);
                    fprintf(get_file_id(), "<D> %03d %07.1f %07.1f %07.1f %07.1f %05.3f <\\D> \n", 0, 0.f, 0.f, 0.f, 0.f, 0.f);
                    nb_caracts++;
                }
            }
            while(i == 0);

            set_nb_caract(nb_caracts);

            base_file.file_close();
        }
        file_close();
    }
}

int File_caract::go_to_data()
{
    char temp[50];
    int test;

    do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_data)) && (test == 1));

    if(test != 1)
        return ERROR;
    else
        return 0;
}

int File_caract::get_id()
{
    char temp[50];
    int ID, test;

    do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_id)) && (test == 1));
    if(test != 1)
        return ERROR;

    test = fscanf(m_file_id,"%d",&ID);
    if(test != 1)
        return ERROR;
    else
        return ID;
}

int File_caract::get_rects(caract_t &caract)
{
        char temp[50];
        caract.nb_rect = 0;
        caract.caract.clear();

        rect_t rect;
        int test;

        do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_rect)) && (test == 1));
        if(test != 1)
            return ERROR;

        do
        {
            caract.nb_rect ++;
            caract.caract.resize(caract.nb_rect);
            test = fscanf(m_file_id, "%d %d %d %d %d", &rect.x, &rect.y, &rect.length, &rect.height, &rect.wieght);
            if(test == 5)
            {
                caract.caract[caract.nb_rect - 1] = rect;
                do{fscanf(m_file_id,"%s",temp);}while(strcmp(temp,tag_rect_end));
                fscanf(m_file_id,"%s",temp);
            }
            else
                printf("error !!\n\n");
        }
        while((!strcmp(temp,tag_rect)) && (test == 5));
    return 0;
}

void File_caract::set_rects(caract_t &caract)
{
        register unsigned int i;
        for(i = 0; i < caract.nb_rect; i++)
        {
            fprintf(m_file_id, "<R> %d %d %d %d %d <\\R> ", caract.caract[i].x, caract.caract[i].y, caract.caract[i].length,caract.caract[i].height, caract.caract[i].wieght);
        }
}

void File_caract::set_nb_caract(unsigned int nb_caract)
{
    char temp[50];
    int test;

    do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_car)) && (test == 1));

    fprintf(m_file_id, " %d", nb_caract);
}

unsigned int File_caract::get_nb_caracteristics()
{
    char temp[30];
    unsigned int test;
    go_to_origin();
    do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_car)) && (test == 1));
    fscanf(m_file_id,"%u",&test);

    return test;
}

bool File_caract::compare_caracts(caract_t caract1, caract_t caract2, int ID_1, int ID_2)
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
