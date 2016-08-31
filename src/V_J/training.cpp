#include "training.h"
#include "../bitmap/bmp.h"

#include <cmath>
#include <string.h>
#include <string>
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

    char folder[100];
    char tmp[100];
    char file[100];
    char tag[] = "<CARACT>";
    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag));
    while(fscanf(m_folder_file,"\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, folder);
    strcpy(m_true_file, strcat(tmp,file));

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, folder);
    strcpy(m_false_file, strcat(tmp,file));

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, folder);
    strcpy(m_weak_file, strcat(tmp,file));

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, folder);
    strcpy(m_threshold_file, strcat(tmp,file));
}

Training::~Training()
{
    fclose(m_folder_file);
}

using std::string;

void Training::generate_caracteristic_file()
{
    clean_file(m_weak_file);

    printf("generate %s\n", m_true_file);
    generate_caracteristics(m_true_file);
    traine_true_image();

    printf("\ngenerate %s \n", m_false_file);
    generate_caracteristics(m_false_file);
    traine_false_image();

    printf("\ncompute variances : \n");
    compute_variances(m_true_file);
    printf("\t%s done\n", m_true_file);
    compute_variances(m_false_file);
    printf("\t%s done\n", m_false_file);

    printf("\nGenerate thresholds\n");
    generate_thresholds();

    printf("Compute errors\n");
    compute_errors();
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

    int nb_of_trainning = 0;

    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag1));
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag2));
    while(fscanf(m_folder_file,"\t\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    printf("true image processing in %s folder\n", folder);

    do
    {
        fseek(m_folder_file, 0, 0);
        do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,tag2));
        for(int i =0; i < nb_of_trainning + 1; i++)
            do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,img_tag) && strcmp(file_image,end_of_true_tag));

        if(strcmp(file_image,end_of_true_tag))
        {
            fscanf(m_folder_file,"%s",file_image);
            #ifdef LOG
            printf("%s : ",file_image);
            #endif
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), true_caract);
        }
        nb_of_trainning++;
    }
    while(strcmp(file_image,end_of_true_tag));
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
    char end_of_false_tag[] = "<\\FALSE>";

    int nb_of_trainning = 0;

    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag1));
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag2));
    while(fscanf(m_folder_file,"\t\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    printf("false image processing in %s folder\n", folder);
    do
    {
        fseek(m_folder_file, 0, 0);
        do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,tag2));
        for(int i =0; i < nb_of_trainning + 1; i++)
            do{fscanf(m_folder_file,"%s",file_image);}while(strcmp(file_image,img_tag) && strcmp(file_image,end_of_false_tag));

        if(strcmp(file_image,end_of_false_tag))
        {
            fscanf(m_folder_file,"%s",file_image);
            #ifdef LOG
            printf("%s : ",file_image);
            #endif
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), false_caract);
        }
        nb_of_trainning++;
    }
    while(strcmp(file_image,end_of_false_tag));
}

void Training::compute_variances(char* file_name)
{
    errno = 0;
    FILE* file = fopen(file_name, "r+");

    if(file != NULL)
    {
        int eof, count_image;
        float sum, scare_sum, variance, mean, mini, maxi;
        fpos_t position1, position2;

        rewind(file);
        do
        {
            eof = go_to_data(file);
            if(eof != ERROR)
            {
                fgetpos (file, &position1);
                fscanf(file, "%d %f %f %f %f %f <\\D>", &count_image, &mini, &maxi, &sum, &scare_sum, &variance);
                fgetpos (file, &position2);

                mean = sum / (float)count_image;
                variance = scare_sum - mean*mean;
                variance = variance >= 0 ? sqrtf(variance) : VARIANCE_ERROR;

                fsetpos (file, &position1);
                fprintf(file, " %03d %07.1f %07.1f %07.1f %07.1f %05.1f", count_image, mini, maxi, sum, scare_sum, variance);
                fsetpos (file, &position2);
            }
        }
        while(eof != ERROR);
    }
    else
        printf("Error to open %s error code %d \n", file_name, errno);
}

void Training::set_image_caract(char* image_name, caracteristic_type_t caracteristic_type)
{
    /* files names and folders */
    char file_txt[100];

    /* compute image */
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(image_name);
    BMP.compute_integral_image_0d(bmps.image, integral_image_0d);
    BMP.compute_integral_image_45d(bmps.image, integral_image_45d);

    #ifdef LOG
    printf("%5d/%5d pxl\n", (int)bmps.width, (int)bmps.height);
    #endif

    if(caracteristic_type == true_caract)
        strcpy(file_txt, m_true_file);
    else
        strcpy(file_txt, m_false_file);

    errno = 0;
    FILE * file = fopen(file_txt, "r+");
    if(file != NULL)
    {
        FILE* weak = fopen(m_weak_file, "a");
        if(weak != NULL)
        {
            /* internal variable */
            int count_image, ID, error;
            float sum, scare_sum, result, mini, maxi;
            fpos_t position1, position2;
            caract_t caracteristics;

            /* go to begining of file */
            rewind(file);
            fseek(file, 0, SEEK_SET);

            if(caracteristic_type == true_caract)
                fprintf(weak,"<P> %d ", TRUE);
            else
                fprintf(weak,"<P> %d ", FALSE);

            do
            {
                ID = get_id(file);
                error = get_rects(file, caracteristics);

                if(error != ERROR)
                {
                    fgetpos (file, &position1);
                    fscanf(file, "%d %f %f %f %f", &count_image, &mini, &maxi, &sum, &scare_sum);
                    fgetpos (file, &position2);
                    count_image ++;

                    if(ID < 10)
                        result = BMP.get_sum_0d(integral_image_0d, caracteristics);
                    else
                        result = BMP.get_sum_45d(integral_image_45d, caracteristics);

                    result = result*10000;
                    sum += result;
                    scare_sum = (scare_sum*(count_image - 1) + result*result)/count_image;

                    if(count_image == 1)
                        mini = maxi = result;
                    else
                    {
                        if(result < mini)
                            mini = result;
                        else if(result > maxi)
                            maxi = result;
                    }

                    fsetpos (file, &position1);
                    fprintf(file, " %03d %07.1f %07.1f %07.1f %07.1f", count_image, mini, maxi, sum, scare_sum);
                    fsetpos (file, &position2);

                    fprintf(weak,"%04d ",(int)result);
                }
            }
            while(error != ERROR);
            fprintf(weak,"\n");
            fclose(weak);
        }
        else
            printf("Error to open %s error code %d \n", m_weak_file, errno);

        fclose(file);
    }
    else
        printf("Error to open %s error code %d \n", file_txt, errno);
}

void Training::clean_file(char* file_name)
{
    errno = 0;
    FILE* file = fopen(file_name, "w");
    if(file == NULL)
        printf("Error to open %s error code %d \n", file_name, errno);
}

void Training::generate_caracteristics(char* file_name)
{
    FILE * file = fopen(file_name, "w");

    unsigned int nb_caracts = 0;

    if(file != NULL)
    {
        int x, y, length, height;

        fseek(file, 0, 0);
        for(x = 0; x < INIT_SIZE - 1; x++)
        {
            for(y = 0; y < INIT_SIZE - 1; y ++)
            {
                for(length = 2; length <= INIT_SIZE - x; length++)
                {
                    for(height = 2; height <= INIT_SIZE - y; height++)
                    {
                       generate_caracteristics_0d(file, x, y, length, height, nb_caracts);
                       generate_caracteristics_45d(file, x, y, length, height, nb_caracts);
                    }
                }
            }
        }
        printf("%d characteristics generated\n", nb_caracts);
        fclose(file);
    }
    else
        printf("Error to open %s error code %d \n", file_name, errno);
}

void Training::generate_caracteristics_0d(FILE* file, int x, int y, int length, int height, unsigned int &nb_caracts)
{
    unsigned int nb_caract, nb_rectangle;

     for(nb_caract = 0; nb_caract < all_caract_type.size(); nb_caract++)
    {
        fprintf(file, "<ID> %u <\\ID> ", nb_caract);
        for(nb_rectangle = 0; nb_rectangle < all_caract_type[nb_caract].nb_rect; nb_rectangle++)
        {
            fprintf(file, "<R> ");
            if(nb_rectangle == 0)
                fprintf(file, "%d %d %d %d %d ", x, y, length, height, FIRST_RECT_WIEGHT);
            else
            {
                define_caract_t define_caract = all_caract_type[nb_caract];
                define_rect_caract_t rect_caract = define_caract.caract[nb_rectangle - 1];

                int temp_length = length / rect_caract.length_factor;
                int temp_hieght = height / rect_caract.height_factor;

                int temp_x = x;
                if(rect_caract.offset_x != SAME_POSITION)
                    temp_x += length / rect_caract.offset_x;

                int temp_y = y;
                if(rect_caract.offset_y != SAME_POSITION)
                    temp_y += height / rect_caract.offset_y;

                fprintf(file, "%d %d %d %d %d ", temp_x, temp_y, temp_length, temp_hieght, rect_caract.wieght);
            }
            fprintf(file, "<\\R> ");
        }
        fprintf(file, "<D> ");
        fprintf(file, "%03d %07.1f %07.1f %07.1f %07.1f %05.3f", 0, 0., 0., 0., 0., 0.);//count image, sum, scare_sum, variance
        fprintf(file, " <\\D> \n");

        nb_caracts++;
    }
}

void Training::generate_caracteristics_45d(FILE* file, int x, int y, int length, int height, unsigned int &nb_caracts)
{
    /*
        L
          --
       -- -- -- H
    xy -- --
       --
    */
    if((x + length + height - 2 <= INIT_SIZE) && (y - length + 1 >= 0) && (y + height - 1 <= INIT_SIZE))
    {
        unsigned int nb_caract, nb_rectangle;

         for(nb_caract = 0; nb_caract < all_caract_type.size(); nb_caract++)
        {
            fprintf(file, "<ID> %u <\\ID> ", nb_caract+10);
            for(nb_rectangle = 0; nb_rectangle < all_caract_type[nb_caract].nb_rect; nb_rectangle++)
            {
                fprintf(file, "<R> ");
                if(nb_rectangle == 0)
                    fprintf(file, "%d %d %d %d %d ", x, y, length, height, FIRST_RECT_WIEGHT);
                else
                {
                    define_caract_t define_caract = all_caract_type[nb_caract];
                    define_rect_caract_t rect_caract = define_caract.caract[nb_rectangle - 1];

                    int temp_length = length / rect_caract.length_factor;
                    int temp_hieght = height / rect_caract.height_factor;
                    int temp_x = x;
                    int temp_y = y;

                    if(rect_caract.offset_x != SAME_POSITION)
                    {
                        temp_x += length / rect_caract.offset_x;
                        temp_y -= length / rect_caract.offset_x;
                    }

                    if(rect_caract.offset_y != SAME_POSITION)
                    {
                        temp_y += height / rect_caract.offset_y;
                        temp_x += height / rect_caract.offset_y;
                    }

                    fprintf(file, "%d %d %d %d %d ", temp_x, temp_y, temp_length, temp_hieght, rect_caract.wieght);
                }
                fprintf(file, "<\\R> ");
            }
            fprintf(file, "<D> ");
            fprintf(file, "%03d %07.1f %07.1f %07.1f %07.1f %05.3f", 0, 0., 0., 0., 0., 0.);//count image, sum, scare_sum, variance
            fprintf(file, " <\\D> \n");

            nb_caracts++;
        }
    }
}

void Training::generate_caracteristics_from_file(char* file_name_new, char* file_name_base)
{
    FILE * new_file = fopen(file_name_new, "w");
    FILE * base_file = fopen(file_name_base, "r");

    if(new_file != NULL)
    {
        fseek(new_file, 0, 0);
        if(base_file != NULL)
        {
            int ID, i =0;
            caract_t caracteristics;
            unsigned int nb_caracts = 0;

            fseek(base_file, 0, 0);

            do
            {
                ID = get_id(base_file);
                i = get_rects(base_file, caracteristics);

                if(i == 0)
                {
                    fprintf(new_file, "<ID> %d <\\ID> ", ID);
                    set_rects(new_file, caracteristics);
                    fprintf(new_file, "<D> %03d %07.1f %07.1f %07.1f %07.1f %05.3f <\\D> \n", 0, 0.f, 0.f, 0.f, 0.f, 0.f);
                    nb_caracts++;
                }
            }
            while(i == 0);

            fprintf(new_file, "<CAR> %d <\\CAR>", nb_caracts);

            fclose(base_file);
        }
        fclose(new_file);
    }
    else
        printf("Error to open %s error code %d \n", file_name_new, errno);
}

void Training::generate_thresholds()
{
    errno = 0;
    FILE* threshold_file = fopen(m_threshold_file, "w");
    if(threshold_file != NULL)
    {
        FILE* true_file = fopen(m_true_file, "r");
        if(true_file != NULL)
        {
            FILE* false_file = fopen(m_false_file, "r");
            if(false_file != NULL)
            {
                fseek(true_file, 0, SEEK_SET);
                fseek(false_file,0, SEEK_SET);

                char data_end_true, data_end_false;
                float min1, min2, max1, max2, threshold;
                int tmp;

                do
                {
                    data_end_true = go_to_data(true_file);
                    data_end_false = go_to_data(false_file);

                    if((data_end_true != ERROR) || (data_end_false != ERROR))
                    {
                        fscanf(true_file, "%d %f %f", &tmp, &min1, &max1);
                        fscanf(false_file, "%d %f %f", &tmp, &min2, &max2);

                        if(min2 < min1)
                            min1 = min2;
                        if(max2 > max1)
                            max1 = max2;

                        threshold = min1 + (max1 - min1)/2.f;
                        fprintf(threshold_file,"<T> %06.1f %03d %03d %03d %03d\n",threshold,0,0,0,0);
                    }
                }
                while((data_end_true != ERROR) || (data_end_false != ERROR));

                fclose(false_file);
            }
            else
                printf("Error to open %s error code %d \n", m_false_file, errno);

            fclose(true_file);
        }
        else
            printf("Error to open %s error code %d \n", m_true_file, errno);

        fclose(threshold_file);
    }
    else
        printf("Error to open %s error code %d \n", m_threshold_file, errno);
}

void Training::compute_errors()
{
    errno = 0;
    FILE* threshold_file = fopen(m_threshold_file, "r+");
    if(threshold_file != NULL)
    {
        std::vector<int> T_pos, T_neg, F_pos, F_neg;
        std::vector<float> threshold;
        int data, test, parity;
        unsigned int nb_caract;

        float threshold_tmp;
        int T_pos_tmp, T_neg_tmp, F_pos_tmp, F_neg_tmp;

        rewind(threshold_file);
        fseek(threshold_file, 0, SEEK_SET);
        do
        {
            test = go_to_threshold(threshold_file);
            if(test != ERROR)
            {
                fscanf(threshold_file, "%f %d %d %d %d\n", &threshold_tmp, &T_pos_tmp, &T_neg_tmp, &F_pos_tmp, &F_neg_tmp);
                threshold.push_back(threshold_tmp);
                T_pos.push_back(T_pos_tmp);
                T_neg.push_back(T_neg_tmp);
                F_pos.push_back(F_pos_tmp);
                F_neg.push_back(F_neg_tmp);
            }
        }
        while(test != ERROR);
        printf("get data done\n");
        fclose(threshold_file);

        FILE* weak_file = fopen(m_weak_file, "r");
        if(weak_file != NULL)
        {
            fseek(weak_file,0, SEEK_SET);
            do
            {
                parity = go_to_parity(weak_file);
                if(parity != ERROR)
                {
                    nb_caract = 0;
                    do
                    {
                        test = fscanf(weak_file, "%d", &data);
                        if(test == 1)
                        {
                            if(parity == TRUE)
                            {
                                if(data <= threshold[nb_caract])
                                    T_neg[nb_caract]++;
                                else
                                    T_pos[nb_caract]++;
                            }
                            else
                            {
                                if(data <= threshold[nb_caract])
                                    F_neg[nb_caract]++;
                                else
                                    F_pos[nb_caract]++;
                            }
                            nb_caract++;
                        }
                    }
                    while(test == 1);
                }
            }
            while(parity != ERROR);
            printf("get errors done\n");
            fclose(weak_file);

            errno = 0;
            threshold_file = fopen(m_threshold_file, "w");
            if(threshold_file != NULL)
            {
                rewind(threshold_file);
                fseek(threshold_file, 0, SEEK_SET);
                fprintf(threshold_file,"b");
                nb_caract = 0;

                for(nb_caract = 0; nb_caract < threshold.size(); nb_caract++)
                {
                    data = fprintf(threshold_file, "<T> %06.1f %03d %03d %03d %03d\n", threshold[nb_caract], T_pos[nb_caract], T_neg[nb_caract], F_pos[nb_caract], F_neg[nb_caract]);
                }

                printf("set data done\n");
                fclose(threshold_file);
            }
            else
                printf("Error to open %s error code %d \n", m_threshold_file, errno);
        }
        else
            printf("Error to open %s error code %d \n", m_weak_file, errno);
    }
    else
        printf("Error to open %s error code %d \n", m_threshold_file, errno);
}

void Training::simplify_true_caract()
{
    FILE * true_file = fopen(m_true_file, "r");
    FILE * false_file = fopen(m_false_file, "r");
    FILE * tmp_file = fopen(m_weak_file, "w");

    if(tmp_file != NULL)
    {
        fseek(tmp_file, 0, 0);
        if(true_file != NULL)
        {
            int nb_caract_in_file = get_nb_caracteristics(true_file);
            if(nb_caract_in_file != 0)
            {
                fseek(true_file, 0, 0);
                if(false_file != NULL)
                {
                    caract_t caracteristics_true;
                    caract_t caracteristics_false;
                    int count_image;
                    float sum,scare_sum,variance_true, variance_false, mini, maxi;
                    bool test;
                    int ID_true, ID_false, count_caract = 0;

                    nb_caract_in_file = get_nb_caracteristics(false_file);

                    count_caract = 0;
                    fseek(false_file, 0, 0);
                    int out = 0;
                    do
                    {
                        ID_false = get_id(false_file);
                        get_rects(false_file, caracteristics_false);

                        if(nb_caract_in_file != 0)
                            fscanf(false_file, "%d %f %f %f %f %f <\\D> ", &count_image, &mini, &maxi, &sum, &scare_sum, &variance_false);

                        do
                        {
                            ID_true = get_id(true_file);
                            out = get_rects(true_file, caracteristics_true);
                            if(out == 0)
                            {
                                fscanf(true_file, "%d %f %f %f %f %f <\\D> ", &count_image, &mini, &maxi, &sum, &scare_sum, &variance_true);
                                test =  compare_caracts(caracteristics_true, caracteristics_false, ID_true, ID_false);
                                if(nb_caract_in_file == 0)
                                    test = 0;

                                if((out == 0) || (test == 1) || (variance_true <= variance_false))
                                {
                                    fprintf(tmp_file,"<ID> %d <\\ID> ", ID_true);
                                    set_rects(tmp_file, caracteristics_true);
                                    fprintf(tmp_file, "<D> %03d %07.1f %07.1f %07.1f %07.1f %05.3f <\\D>\n", count_image, mini, maxi, sum, scare_sum, variance_true);
                                    count_caract++;
                                }
                            }
                        }
                        while((!test) && (out == 0));
                    }
                    while(out == 0);
                    fprintf(tmp_file, "<CAR> %d <\\CAR>", count_caract);
                    #ifdef LOG
                    printf("%d caracteristics found\n",count_caract);
                    #endif

                    fclose(false_file);
                }
            }
            else
                fprintf(tmp_file, "<CAR> %d <\\CAR>", 0);
            fclose(true_file);
        }
        else
            printf("Error to open %s error code %d \n", m_false_file, errno);
        fclose(tmp_file);
    }
}

void Training::cpy_file(char* input, char* output)
{
    errno = 0;
    FILE * FILE_input = fopen(input, "r");
    FILE * FILE_output = fopen(output, "w");

    if(FILE_input != NULL)
    {
        fseek(FILE_input, 0, 0);
        if(FILE_output != NULL)
        {
            fseek(FILE_output, 0, 0);
            register char car;
            do
            {
                car = fgetc(FILE_input);
                if(car != EOF)
                    fputc(car, FILE_output);
            }
            while (car != EOF);
            fclose(FILE_output);
        }
        else
            printf("Error to open %s error code %d \n", output, errno);
        fclose(FILE_input);
    }
    else
        printf("Error to open %s error code %d \n", input, errno);
}
