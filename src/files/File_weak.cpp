#include "File_weak.h"

File_weak::File_weak()
{
    strcpy(tag_parity, "<P>");
}

File_weak::~File_weak()
{

}

void File_weak::set_next_image()
{
    fprintf(m_file_id,"\n");
}

int File_weak::get_next_data(int& data)
{
    return(fscanf(m_file_id, "%d", &data));
}

void File_weak::set_next_data(float data)
{
    fprintf(m_file_id,"%04d ",(int)data);
}

void File_weak::set_parity(caracteristic_type_t caracteristic_type)
{
    if(caracteristic_type == true_caract)
        fprintf(m_file_id,"<P> %d ", TRUE);
    else
        fprintf(m_file_id,"<P> %d ", FALSE);
}

int File_weak::go_to_parity()
{
    char temp[50];
    int test;

    do{test = fscanf(m_file_id,"%s",temp);}while((strcmp(temp,tag_parity)) && (test == 1));

    if(test != 1)
        return ERROR;
    else
    {
        int parity;
        test = fscanf(m_file_id, "%d", &parity);
        if(test != 1)
            return ERROR;
        else
            return parity;
    }
}
