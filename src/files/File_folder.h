#ifndef FILE_FOLDER_H_INCLUDED
#define FILE_FOLDER_H_INCLUDED

#include "File.h"

class File_folder : public File
{
public:
    File_folder();
    ~File_folder();

    void get_images_folder(int type, char* folder);
    void get_txt_folder();
    void get_next_txt(char* file);
    int get_img(int i, char* file_image, int type);
//private:

    char img_tag[10];
    char true_tag[10];
    char false_tag[10];
    char file_tag[10];
    char eo_false_tag[10];
    char eo_true_tag[10];
    char caract_tag[10];

    char txt_folder[100];
};


#endif // FILE_FOLDER_H_INCLUDED
