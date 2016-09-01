#include "File_folder.h"

File_folder::File_folder()
{
    strcpy(img_tag, "<IMG>");
    strcpy(true_tag, "<TRUE>");
    strcpy(false_tag, "<FALSE>");
    strcpy(file_tag, "<FILE>");
    strcpy(eo_false_tag, "<\\FALSE>");
    strcpy(eo_true_tag, "<\\TRUE>");
    strcpy(caract_tag, "<CARACT>");
}

File_folder::~File_folder()
{

}

void File_folder::get_images_folder(int type, char* folder)
{
    go_to_origin();
    do{fscanf(m_file_id,"%s",folder);}while(strcmp(folder,img_tag));
    if(type == TRUE)
        do{fscanf(m_file_id,"%s",folder);}while(strcmp(folder,true_tag));
    else
        do{fscanf(m_file_id,"%s",folder);}while(strcmp(folder,false_tag));
    while(fscanf(m_file_id,"\t\t<FOLDER> %s <\\FOLDER>",folder) == 0);
}

void File_folder::get_txt_folder()
{
    go_to_origin();
    do{fscanf(m_file_id,"%s",txt_folder);}while(strcmp(txt_folder, caract_tag));
    while(fscanf(m_file_id,"\t<FOLDER> %s <\\FOLDER>",txt_folder) == 0);
}

void File_folder::get_next_txt(char* file)
{
    char tmp[100];
    while(fscanf(m_file_id,"\t\t<FILE> %s <\\FILE>",file) == 0);
    strcpy(tmp, txt_folder);
    strcat(tmp,file);
    strcpy(file, tmp);
}

int File_folder::get_img(int img_i, char* file_image, int type)
{
    char tag[10];
    char eo_tag[10];

    if(type == TRUE)
    {
        strcpy(tag, true_tag);
        strcpy(eo_tag, eo_true_tag);
    }
    else
    {
        strcpy(tag, false_tag);
        strcpy(eo_tag, eo_false_tag);
    }

    go_to_origin();
    do{fscanf(m_file_id,"%s",file_image);}while(strcmp(file_image,tag));
    for(int i =0; i < img_i + 1; i++)
        do{fscanf(m_file_id,"%s",file_image);}while(strcmp(file_image,file_tag) && strcmp(file_image,eo_tag));

    if(strcmp(file_image,eo_tag))
    {
        fscanf(m_file_id,"%s",file_image);
        return 0;
    }
    else
        return ERROR;
}
