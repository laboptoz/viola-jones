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
    char folder[100];
    char tmp[100];
    char file[100];
    char tag[] = "<CARACT>";
    fseek(m_folder_file, 0, 0);
    do{fscanf(m_folder_file,"%s",folder);}while(strcmp(folder,tag));
    while(fscanf(m_folder_file,"\t<FOLDER> %s <\\FOLDER>",folder) == 0);

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, folder);
    m_true_file.set_name(strcat(tmp,file));

    while(fscanf(m_folder_file,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, folder);
    m_false_file.set_name(strcat(tmp,file));

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
    char names[100];

    clean_file(m_weak_file);

    m_true_file.get_name(names);
    printf("generate %s\n", names);
    m_true_file.generate_caracteristics();
    traine_true_image();

    m_false_file.get_name(names);
    printf("\ngenerate %s \n", names);
    m_false_file.generate_caracteristics();
    traine_false_image();

    printf("\ncompute variances : \n");
    m_true_file.compute_variances();
    m_true_file.get_name(names);
    printf("\t%s done\n", names);
    m_false_file.compute_variances();
    m_false_file.get_name(names);
    printf("\t%s done\n", names);

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

    printf("\ttrue image processing in %s folder\n", folder);

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
            printf("\t%s : ",file_image);
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

    printf("\tfalse image processing in %s folder\n", folder);
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
            printf("\t%s : ",file_image);
            #endif
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), false_caract);
        }
        nb_of_trainning++;
    }
    while(strcmp(file_image,end_of_false_tag));
}

void Training::set_image_caract(char* image_name, caracteristic_type_t caracteristic_type)
{
    /* compute image */
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(image_name);
    BMP.compute_integral_image_0d(bmps.image, integral_image_0d);
    BMP.compute_integral_image_45d(bmps.image, integral_image_45d);

    #ifdef LOG
    printf("%5d/%5d pxl\n", (int)bmps.width, (int)bmps.height);
    #endif

    File_caract file_caract;

    if(caracteristic_type == true_caract)
        file_caract = m_true_file;
    else
        file_caract = m_false_file;

    char mode[] = "r+";
    if(file_caract.file_open(mode) != ERROR)
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
            rewind(file_caract.get_file_id());
            file_caract.go_to_origin();

            if(caracteristic_type == true_caract)
                fprintf(weak,"<P> %d ", TRUE);
            else
                fprintf(weak,"<P> %d ", FALSE);

            do
            {
                ID = file_caract.get_id();
                error = file_caract.get_rects(caracteristics);

                if(error != ERROR)
                {
                    fgetpos (file_caract.get_file_id(), &position1);
                    fscanf(file_caract.get_file_id(), "%d %f %f %f %f", &count_image, &mini, &maxi, &sum, &scare_sum);
                    fgetpos (file_caract.get_file_id(), &position2);
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

                    fsetpos (file_caract.get_file_id(), &position1);
                    fprintf(file_caract.get_file_id(), " %03d %07.1f %07.1f %07.1f %07.1f", count_image, mini, maxi, sum, scare_sum);
                    fsetpos (file_caract.get_file_id(), &position2);

                    fprintf(weak,"%04d ",(int)result);
                }
            }
            while(error != ERROR);
            fprintf(weak,"\n");
            fclose(weak);
        }
        else
            printf("Error to open %s error code %d \n", m_weak_file, errno);

        file_caract.file_close();
    }
}

void Training::clean_file(char* file_name)
{
    errno = 0;
    FILE* file = fopen(file_name, "w");
    if(file == NULL)
        printf("Error to open %s error code %d \n", file_name, errno);
}


void Training::generate_thresholds()
{
    errno = 0;
    FILE* threshold_file = fopen(m_threshold_file, "w");
    if(threshold_file != NULL)
    {
        char mode[] = "r";
        if(m_true_file.file_open(mode) != ERROR)
        {
            if(m_false_file.file_open(mode) != ERROR)
            {
                m_true_file.go_to_origin();
                m_false_file.go_to_origin();

                char data_end_true, data_end_false;
                float min1, min2, max1, max2, threshold;
                int tmp;

                do
                {
                    data_end_true = m_true_file.go_to_data();
                    data_end_false = m_false_file.go_to_data();

                    if((data_end_true != ERROR) || (data_end_false != ERROR))
                    {
                        fscanf(m_true_file.get_file_id(), "%d %f %f", &tmp, &min1, &max1);
                        fscanf(m_false_file.get_file_id(), "%d %f %f", &tmp, &min2, &max2);

                        if(min2 < min1)
                            min1 = min2;
                        if(max2 > max1)
                            max1 = max2;

                        threshold = min1 + (max1 - min1)/2.f;
                        fprintf(threshold_file,"<T> %06.1f %03d %03d %03d %03d\n",threshold,0,0,0,0);
                    }
                }
                while((data_end_true != ERROR) || (data_end_false != ERROR));

                m_false_file.file_close();
            }
            m_true_file.file_close();
        }
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
