#include "training.h"
#include "../bitmap/bmp.h"

#include <cmath>
#include <string.h>
#include <string>
#include <iostream>

Training::Training(char* p_folder_file)
{
    m_folder_file.set_name(p_folder_file);
    if(m_folder_file.file_open(m_folder_file.read_mode) == ERROR)
        exit(-1);

    m_folder_file.get_txt_folder();

    char tmp[100];
    m_folder_file.get_next_txt(tmp);
    m_true_file.set_name(tmp);

    m_folder_file.get_next_txt(tmp);
    m_false_file.set_name(tmp);

    m_folder_file.get_next_txt(tmp);
    m_weak_file.set_name(tmp);

    m_folder_file.get_next_txt(tmp);
    m_threshold_file.set_name(tmp);
}

Training::~Training()
{
    m_folder_file.file_close();
}

using std::string;

void Training::generate_caracteristic_file()
{
    char names[100];

    m_weak_file.clean_file();

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

    int test;
    int nb_of_trainning = 0;

    m_folder_file.go_to_origin();
    m_folder_file.get_images_folder(TRUE, folder);

    printf("\ttrue image processing in %s folder\n", folder);

    do
    {
        test = m_folder_file.get_img(nb_of_trainning, file_image, TRUE);
        if(test != ERROR)
        {
            #ifdef LOG
            printf("\t%s : ",file_image);
            #endif
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), true_caract);
        }
        nb_of_trainning++;
    }
    while(test != ERROR);
}

void Training::traine_false_image()
{
    /* files names and folders */
    char folder[100];
    char file_image[100];
    char tmp[100];
    int test;

    int nb_of_trainning = 0;

    m_folder_file.go_to_origin();
    m_folder_file.get_images_folder(FALSE, folder);

    printf("\tfalse image processing in %s folder\n", folder);
    do
    {
        test = m_folder_file.get_img(nb_of_trainning, file_image, FALSE);
        if(test != ERROR)
        {
            #ifdef LOG
            printf("\t%s : ",file_image);
            #endif
            strcpy(tmp,folder);
            set_image_caract(strcat(tmp,file_image), false_caract);
        }
        nb_of_trainning++;
    }
    while(test != ERROR);
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
        if(m_weak_file.file_open(m_weak_file.append_mode) != ERROR)
        {
            /* internal variable */
            int count_image, ID, error;
            float sum, scare_sum, result, mini, maxi;
            fpos_t position1, position2;
            caract_t caracteristics;

            /* go to begining of file */
            rewind(file_caract.get_file_id());
            file_caract.go_to_origin();

            m_weak_file.set_parity(caracteristic_type);

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

                    m_weak_file.set_next_data(result);
                }
            }
            while(error != ERROR);
            m_weak_file.set_next_image();
            m_weak_file.file_close();
        }

        file_caract.file_close();
    }
}

void Training::generate_thresholds()
{
    if(m_threshold_file.file_open(m_threshold_file.write_mode) != ERROR)
    {
        if(m_true_file.file_open(m_true_file.read_mode) != ERROR)
        {
            if(m_false_file.file_open(m_false_file.read_mode) != ERROR)
            {
                m_true_file.go_to_origin();
                m_false_file.go_to_origin();

                char data_end_true, data_end_false;
                float min1, min2, max1, max2;
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

                        m_threshold_file.compute_treshold(min1,max1);
                        m_threshold_file.write_datas();
                    }
                }
                while((data_end_true != ERROR) || (data_end_false != ERROR));

                m_false_file.file_close();
            }
            m_true_file.file_close();
        }
        m_threshold_file.file_close();
    }
}

void Training::compute_errors()
{
    int data, test, parity;
    unsigned int nb_caract;

    m_threshold_file.get_datas();

    if(m_weak_file.file_open(m_weak_file.read_mode) != ERROR)
    {
        m_weak_file.go_to_origin();
        do
        {
            parity = m_weak_file.go_to_parity();
            if(parity != ERROR)
            {
                nb_caract = 0;
                do
                {
                    test = m_weak_file.get_next_data(data);
                    if(test == 1)
                    {
                        if(parity == TRUE)
                        {
                            if(data <= m_threshold_file.get_threshold(nb_caract))
                                m_threshold_file.T_neg[nb_caract]++;
                            else
                                m_threshold_file.T_pos[nb_caract]++;
                        }
                        else
                        {
                            if(data <= m_threshold_file.get_threshold(nb_caract))
                                m_threshold_file.F_neg[nb_caract]++;
                            else
                                m_threshold_file.F_pos[nb_caract]++;
                        }
                        nb_caract++;
                    }
                }
                while(test == 1);
            }
        }
        while(parity != ERROR);
        printf("get errors done\n");
        m_weak_file.file_close();

        m_threshold_file.set_datas();
    }
}
