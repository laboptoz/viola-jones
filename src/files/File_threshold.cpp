#include "File_threshold.h"

File_threshold::File_threshold()
{
    strcpy(tag_data, "<T>");
}

File_threshold::~File_threshold()
{

}

float File_threshold::compute_treshold(int mini, int maxi)
{
    tmp_threshold =  mini + (maxi - mini)/2.f;
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
