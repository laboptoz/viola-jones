#include "File_threshold.h"

File_threshold::File_threshold()
{
    strcpy(tag_data, "<T>");
}

File_threshold::~File_threshold()
{

}

float File_threshold::compute_treshold(int nb1, int nb2, float sum1, float sum2, float scare_sum1, float scare_sum2)
{
   float mean1 = sum1/nb1;
   float mean2 = sum2/nb2;
    float deviation1 = scare_sum1 - mean1*mean1;
    float deviation2 = scare_sum2 - mean2*mean2;

    if(deviation1>=0)
        deviation1 = sqrtf(deviation1);
    else
        deviation1 = 1;
    if(deviation2>=0)
        deviation2 = sqrtf(deviation2);
    else
        deviation2 = 1;

    float A = (1/(2*deviation2*deviation2)) - (1/(2*deviation1*deviation1));
    float B = (mean1/(deviation1*deviation1)) - (mean2/(deviation2*deviation2));
    float C = (mean2*mean2/(2*deviation2*deviation2)) - (mean1*mean1/(2*deviation1*deviation1));

    float delta = B*B - 4.*A*C;

    if(delta >= 0.)
    {
        float x1 = (-B - sqrtf(delta)) / (2.*A);
        float x2 = (-B + sqrtf(delta)) / (2.*A);
        //printf("%f = %f || %f | %f || %f = %f\n",mean1,x1,x1bis, x2, x2bis,mean2);

        if(((x1 > mean1) && (x1 < mean2)) || ((x1 < mean1) && (x1 > mean2)))
            tmp_threshold = x1;
        else if(((x2 > mean1) && (x2 < mean2)) || ((x2 < mean1) && (x2 > mean2)))
            tmp_threshold = x2;
        else
            tmp_threshold = - 1;
    }
    else
        tmp_threshold = -1;

    return tmp_threshold;
}

void File_threshold::write_datas(int T_pos, int T_neg, int F_pos, int F_neg)
{
    fprintf(m_file_id,"%s %06.1f %03d %03d %03d %03d\n",tag_data, tmp_threshold, T_pos, T_neg, F_pos, F_neg);
}

int File_threshold::get_datas()
{
    float threshold_tmp;
    int T_pos_tmp, T_neg_tmp, F_pos_tmp, F_neg_tmp;

    int test = file_open(read_update_mode);
    if(test != ERROR)
    {
        rewind(m_file_id);
        go_to_origin();
        do
        {
            test = go_to_threshold();
            if(test != ERROR)
            {
                fscanf(m_file_id, "%f %d %d %d %d\n", &threshold_tmp, &T_pos_tmp, &T_neg_tmp, &F_pos_tmp, &F_neg_tmp);
                threshold.push_back(threshold_tmp);
                T_pos.push_back(T_pos_tmp);
                T_neg.push_back(T_neg_tmp);
                F_pos.push_back(F_pos_tmp);
                F_neg.push_back(F_neg_tmp);
            }
        }
        while(test != ERROR);
        printf("get data done\n");
        file_close();

        return 0;
    }
    else
        return ERROR;
}

int File_threshold::set_datas()
{
    int test = file_open(write_mode);
    if(test != ERROR)
    {
        rewind(m_file_id);
        go_to_origin();
        unsigned int nb_caract = 0;

        for(nb_caract = 0; nb_caract < threshold.size(); nb_caract++)
        {
            if(threshold[nb_caract] != -1.)
            {
                int total = T_pos[nb_caract]+ T_neg[nb_caract]+ F_pos[nb_caract]+ F_neg[nb_caract];
                float e1 = (T_pos[nb_caract] + F_neg[nb_caract])*100 / total;
                float e2 = (F_pos[nb_caract] + T_neg[nb_caract])*100 / total;
                fprintf(m_file_id, "%s %06.1f %03d %03d %03d %03d %6.2f %6.2f\n",tag_data, threshold[nb_caract], T_pos[nb_caract], T_neg[nb_caract], F_pos[nb_caract], F_neg[nb_caract], e1, e2);
            }
            else
                fprintf(m_file_id, "%s %06.1f %03d %03d %03d %03d\n",tag_data, threshold[nb_caract], T_pos[nb_caract], T_neg[nb_caract], F_pos[nb_caract], F_neg[nb_caract]);
        }

        printf("set data done\n");
        file_close();
        return 0;
    }
    else
        return ERROR;
}

float File_threshold::get_threshold(int i)
{
    return threshold[i];
}

int File_threshold::go_to_threshold()
{
    char temp[50];
    int test;

    do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_data)) && (test == 1));

    if(test != 1)
        return ERROR;
    else
        return 0;
}

std::vector<int> File_threshold::get_index_sup(float p_error)
{
    std::vector<int> index;

    int test = file_open(read_mode);
    if(test != ERROR)
    {
        go_to_origin();

        int i = 0;
        float threshold_tmp;
        int T_pos_tmp, T_neg_tmp, F_pos_tmp, F_neg_tmp;

        do
        {
            test = go_to_threshold();
            if(test != ERROR)
            {
                fscanf(m_file_id, "%f %d %d %d %d\n", &threshold_tmp, &T_pos_tmp, &T_neg_tmp, &F_pos_tmp, &F_neg_tmp);
                float total = T_pos_tmp + T_neg_tmp + F_pos_tmp + F_neg_tmp;
                float e1 = (T_pos_tmp + F_neg_tmp)*100. / total;
                float e2 = (F_pos_tmp + T_neg_tmp)*100. / total;
                if((e1 >= p_error) || (e2 >= p_error))
                    index.push_back(i);

                i++;
            }
        }
        while(test != ERROR);
        printf("get index done\n");
        file_close();
    }

    return index;
}
