#ifndef TRAINING_H_INCLUDED
#define TRAINING_H_INCLUDED

#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../config.h"

enum last_file_used_t
{
    caract,
    temp
};

enum caracteristic_type_t
{
    true_caract,
    false_caract
};

class Training
{
public :
    Training(char* p_folder_file);
    ~Training();

    void generate_caracteristic_file();
    void traine_true_image();
    void traine_false_image();

private :
    FILE* m_folder_file;
    enum last_file_used_t m_last_file_used;

    char m_true_file[100];
    char m_false_file[100];
    char m_tmp_file[100];

    void set_image_caract(char* file_name, caracteristic_type_t caracteristic_type);
    void generate_caracteristics(char* file_name);
    void generate_caracteristics_from_file(char* file_name_new, char* file_name_base);
    void simplify_true_caract();
    void cpy_temp(caracteristic_type_t caracteristic_type);
};


#endif // TRAINING_H_INCLUDED
