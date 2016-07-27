#include "training.h"
#include "../bitmap/bmp.h"
#include "caract.h"

#include <cmath>
#include <string.h>
#include <iostream>

Training::Training(char* p_folder_file)
{
    m_folder_file = fopen(p_folder_file, "r");
    if(m_folder_file == NULL)
    {
        printf("can't open folder file : %s\n",p_folder_file);
        exit(-1);
    }
    define_all_caract_type();
}

Training::~Training()
{
    fclose(m_folder_file);
}

void Training::generate_caracteristic_file()
{
    char folder[100];
    char tmp[100];
    char tag[] = "<CARACT>";
    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag));
    while(fscanf(m_folder_file,"\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",tag) == 0);
    strcpy(tmp, folder);
    strcpy(m_true_file, strcat(tmp,tag));

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",tag) == 0);
    strcpy(tmp, folder);
    strcpy(m_false_file, strcat(tmp,tag));

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",tag) == 0);
    strcpy(tmp, folder);
    strcpy(m_tmp_file, strcat(tmp,tag));

    printf("generate %s\n", m_true_file);
    generate_caracteristics(m_true_file);
    traine_true_image();

    /*printf("\ngenerate %s\n", m_false_file);
    generate_caracteristics_from_file(m_false_file, m_true_file);
    traine_false_image();

    simplify_true_caract();*/
}

void Training::traine_true_image()
{
    /* files names and folders */
    char folder[100];
    char file_image[100];
    char tmp[100];

    /* tags */
    char tag1[] = "<IMG>";
    char tag2[] = "<TRUE>";
    char img_tag[] = "<FILE>";
    char end_of_true_tag[] = "<\\TRUE>";

    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag1));
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag2));
    while(fscanf(m_folder_file,"\t\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    printf("true image processing in %s folder\n", folder);

    int nb_of_trainning = 0;
    m_last_file_used = caract;
    do
    {
        fseek(m_folder_file, 0, 0);
        do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,tag2));
        for(int i =0; i < nb_of_trainning + 1; i++)
            do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,img_tag) && strcmp(file_image,end_of_true_tag));

        if(strcmp(file_image,end_of_true_tag))
        {
            fscanf(m_folder_file,"%s",file_image);
            printf("%s processing : ",file_image);
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), true_caract);
        }
        nb_of_trainning++;
    }
    while(strcmp(file_image,end_of_true_tag));

    cpy_temp(true_caract);
}

void Training::traine_false_image()
{
    /* files names and folders */
    char folder[100];
    char file_image[100];
    char tmp[100];

    /* tags */
    char tag1[] = "<IMG>";
    char tag2[] = "<FALSE>";
    char img_tag[] = "<FILE>";
    char end_of_true_tag[] = "<\\FALSE>";

    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag1));
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag2));
    while(fscanf(m_folder_file,"\t\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    printf("false image processing in %s folder\n", folder);

    int nb_of_trainning = 0;
    m_last_file_used = caract;
    do
    {
        fseek(m_folder_file, 0, 0);
        do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,tag2));
        for(int i =0; i < nb_of_trainning + 1; i++)
            do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,img_tag) && strcmp(file_image,end_of_true_tag));

        if(strcmp(file_image,end_of_true_tag))
        {
            fscanf(m_folder_file,"%s",file_image);
            printf("%s processing\n",file_image);
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), false_caract);
        }
        nb_of_trainning++;
    }
    while(strcmp(file_image,end_of_true_tag));

    cpy_temp(false_caract);
}

void Training::set_image_caract(char* file_name, caracteristic_type_t caracteristic_type)
{
    /* files names and folders */
    char file_in[100];
    char file_out[100];

    int nb_caract = 0;
    unsigned long int i;

    /* compute image */
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(file_name);
    BMP.compute_integral_image(bmps.image);

    printf("%5d per %5d pixels : ", (int)bmps.width, (int)bmps.height);

    if(m_last_file_used == caract)
    {
        if(caracteristic_type == true_caract)
            strcpy(file_in, m_true_file);
        else
            strcpy(file_in, m_false_file);
        strcpy(file_out, m_tmp_file);
    }
    else
    {
        if(caracteristic_type == true_caract)
            strcpy(file_out, m_true_file);
        else
            strcpy(file_out, m_false_file);
        strcpy(file_in, m_tmp_file);
    }

    errno = 0;

    FILE * file_input = fopen(file_in, "r");
    FILE * file_output = fopen(file_out, "w");

    if(file_input != NULL)
    {
        fseek(file_input, 0, 0);
        if(file_output != NULL)
        {
            fseek(file_output, 0, 0);

            int x,y,length1, hieght1, wieght1,length2,hieght2,wieght2;
            int count_image;
            float variance, sum, scare_sum;

            do
            {
                i = fscanf(file_input, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data> ", &x, &y, &length1, &hieght1, &wieght1, &x, &y, &length2, &hieght2, &wieght2,
                                &count_image, &sum, &scare_sum, &variance);

                if(i == 14)
                {
                    count_image ++;

                    float result = BMP.get_sum(bmps.image, x, y , length1, hieght1, wieght1, x
                                           , y, length2, hieght2, wieght2);
                    sum += result;
                    scare_sum = (scare_sum*(count_image - 1) + result*result)/count_image;
                    float mean = sum / (float)count_image;
                    variance = scare_sum - mean*mean;
                    variance = variance >= 0 ? sqrt(variance) : 1.0f;
                    if((count_image <= MIN_IMAGE) || ((variance < VARIANCE_THREHOLD) && (variance != VARIANCE_ERROR)))
                    {
                        fprintf(file_output, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data> \n", x, y, length1, hieght1, wieght1, x, y, length2, hieght2, wieght2,
                                count_image, sum, scare_sum, variance);
                        nb_caract++;
                    }
                }
            }
            while(i == 14);
            printf(" %d caracteristics found\n", nb_caract);
            fprintf(file_output, "nb avaible caract = %d", nb_caract);

            if(m_last_file_used == caract)
                m_last_file_used = temp;
            else
                m_last_file_used = caract;

            fclose(file_output);
        }
        else
            printf("Error to open %s error code %d \n", file_out, errno);

        fclose(file_input);
    }
    else
        printf("Error to open %s error code %d \n", file_in, errno);
}

void Training::generate_caracteristics(char* file_name)
{
    FILE * file = fopen(file_name, "w");

    int count_image = 0;
    float sum = 0;
    float variance = 0.f;
    float scare_sum = 0.f;

    unsigned int nb_caract;
    unsigned int nb_rectangle;

    if(file != NULL)
    {
        fseek(file, 0, 0);
        for(int x = 0; x < INIT_SIZE - 2; x++)
        {
            for(int y = 0; y < INIT_SIZE - 2; y ++)
            {
                for(int length = 3; length < INIT_SIZE - x; length++)
                {
                    for(int hieght = 3; hieght < INIT_SIZE - y; hieght++)
                    {
                        for(nb_caract = 0; nb_caract < all_caract_type.size(); nb_caract++)
                        {
                            for(nb_rectangle = 0; nb_rectangle < all_caract_type[nb_caract].nb_rect; nb_rectangle++)
                            {
                                fprintf(file, "<rect> ");
                                if(nb_rectangle == 0)
                                    fprintf(file, "%d %d %d %d %d ", x, y, length, hieght, FIRST_RECT_WIEGHT);
                                else
                                {
                                    define_caract_t define_caract = all_caract_type[nb_caract];
                                    define_rect_caract_t rect_caract = define_caract.caract[nb_rectangle - 1];
                                    int temp_length = length / rect_caract.length_factor;
                                    int temp_hieght = hieght / rect_caract.height_factor;
                                    int temp_x;
                                    if(rect_caract.offset_x == SAME_POSITION)
                                        temp_x = x;
                                    else
                                        temp_x = x + length / rect_caract.offset_x;
                                    int temp_y;
                                    if(rect_caract.offset_y == SAME_POSITION)
                                        temp_y = y;
                                    else
                                        temp_y = y + length / rect_caract.offset_y;

                                    fprintf(file, "%d %d %d %d %d ", temp_x, temp_y, temp_length, temp_hieght, rect_caract.wieght);
                                }
                                fprintf(file, "<\\rect> ");
                            }
                            fprintf(file, "<data> ");
                            fprintf(file, "%d %f %f %f", count_image, sum, scare_sum, variance);
                            fprintf(file, " <\\data> \n");
                        }

                        /*fprintf(file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect>", x, y, length, hieght, FIRST_RECT_WIEGHT, x, y, length, hieght/2, 2);
                        fprintf(file, " <data> ");
                        fprintf(file, "%d %f %f %f", count_image, sum, scare_sum, variance);
                        fprintf(file, " <\\data> \n");
                        fprintf(file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect>", x, y, length, hieght, FIRST_RECT_WIEGHT, x, y, length/2, hieght, 2);
                        fprintf(file, " <data> ");
                        fprintf(file, "%d %f %f %f", count_image, sum, scare_sum, variance);
                        fprintf(file, " <\\data> \n");*/
                    }
                }
            }
        }
        fclose(file);
    }
    else
        printf("Error to open %s error code %d \n", file_name, errno);
}

void Training::generate_caracteristics_from_file(char* file_name_new, char* file_name_base)
{
    FILE * new_file = fopen(file_name_new, "w");
    FILE * base_file = fopen(file_name_base, "r");

    int count_image, x1, y1, length1, hieght1, wieght1, x2, y2, length2, hieght2, wieght2;
    float sum,scare_sum,variance;

    if(new_file != NULL)
    {
        fseek(new_file, 0, 0);
        if(base_file != NULL)
        {
            fseek(base_file, 0, 0);
            int i = 0;
            do
            {
                i = fscanf(base_file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data> ",
                       &x1, &y1, &length1, &hieght1, &wieght1, &x2, &y2, &length2, &hieght2, &wieght2, &count_image, &sum, &scare_sum, &variance);
                count_image = 0;
                sum = 0.f;
                variance = 0.f;
                if(i == 14)
                    fprintf(new_file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data>\n",
                            x1, y1, length1, hieght1, wieght1, x2, y2, length2, hieght2, wieght2, count_image, sum, scare_sum, variance);
            }
            while(i == 14);
            fclose(base_file);
        }
        fclose(new_file);
    }
    else
        printf("Error to open %s error code %d \n", file_name_new, errno);
}

void Training::simplify_true_caract()
{
    FILE * true_file = fopen(m_true_file, "r");
    FILE * false_file = fopen(m_false_file, "r");
    FILE * tmp_file = fopen(m_tmp_file, "w");

    int count_image, x1, y1, length1, hieght1, wieght1, x2, y2, length2, hieght2, wieght2;
    int x11, y11, length11, hieght11, wieght11, x12, y12, length12, hieght12, wieght12;
    float sum,scare_sum,variance;
    bool test;
    int count_caract = 0;

    if(false_file != NULL)
    {
        fseek(false_file, 0, 0);
        if(true_file != NULL)
        {
            fseek(true_file, 0, 0);
            if(tmp_file != NULL)
            {
            fseek(tmp_file, 0, 0);
                int in = 0;
                int out = 0;
                do
                {
                    in = fscanf(false_file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data> ",
                                &x1, &y1, &length1, &hieght1, &wieght1, &x2, &y2, &length2, &hieght2, &wieght2, &count_image, &sum, &scare_sum, &variance);
                    do
                    {
                        out = fscanf(true_file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data> ",
                                &x11, &y11, &length11, &hieght11, &wieght11, &x12, &y12, &length12, &hieght12, &wieght12, &count_image, &sum, &scare_sum, &variance);
                        test = (x1 == x11) && (y1 == y11) && (length1 == length11) && (wieght1 == wieght11) &&
                                (x2 == x12) && (y2 == y12) && (length2 == length12) && (wieght2 == wieght12);

                        if(((!test) || (in != 14)) && (out == 14))
                        {
                            fprintf(tmp_file, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data>\n",
                                    x11, y11, length11, hieght11, wieght11, x12, y12, length12, hieght12, wieght12, count_image, sum, scare_sum, variance);
                            count_caract++;
                        }
                    }
                    while(((!test) || (in != 14)) && (out == 14));
                }
                while((in == 14) && (out == 14));
                fprintf(tmp_file, "nb avaible caract = %d", count_caract);

                m_last_file_used = temp;
                //cpy_temp(true_caract);
            }
            fclose(true_file);
        }
        fclose(false_file);
    }
    else
        printf("Error to open %s error code %d \n", m_false_file, errno);
}

void Training::cpy_temp(caracteristic_type_t caracteristic_type)
{
    if(m_last_file_used == temp)
    {
        /* files names and folder */
        char file_caract[100];

        if(caracteristic_type == true_caract)
            strcpy(file_caract, m_true_file);
        else
            strcpy(file_caract, m_false_file);

        errno = 0;
        FILE * FILE_tmp = fopen(m_tmp_file, "r");
        FILE * FILE_caract = fopen(file_caract, "w");

        if(FILE_tmp != NULL)
        {
            fseek(FILE_tmp, 0, 0);
            if(FILE_caract != NULL)
            {
                fseek(FILE_caract, 0, 0);
                char car;
                do
                {
                    car = fgetc(FILE_tmp);
                    if(car != EOF)
                    {
                        fputc(car, FILE_caract);
                    }
                    else
                        printf("EOF\n");
                }
                while (car != EOF);
                fclose(FILE_caract);
            }
            else
                printf("Error to open %s error code %d \n", file_caract, errno);
            fclose(FILE_tmp);
        }
        else
            printf("Error to open %s error code %d \n", m_tmp_file, errno);
    }
}
