#ifndef TRAINING_H_INCLUDED
#define TRAINING_H_INCLUDED

#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../config.h"
#include "caract.h"

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

    std::vector< std::vector<unsigned long int> > integral_image_0d;
    std::vector< std::vector<unsigned long int> > integral_image_45d;
    std::vector<define_caract_t> all_caract_type;

    char m_true_file[100];
    char m_false_file[100];
    char m_tmp_file[100];

    void set_image_caract(char* file_name, caracteristic_type_t caracteristic_type);
    void generate_caracteristics(char* file_name);
    void generate_caracteristics_0d(FILE* file, int x, int y, int length, int height, unsigned int &nb_caracts);
    void generate_caracteristics_45d(FILE* file, int x, int y, int length, int height, unsigned int &nb_caracts);
    void generate_caracteristics_from_file(char* file_name_new, char* file_name_base);
    void simplify_true_caract();
    void cpy_file(char* input, char* output);

    void define_all_caract_type();
};


#endif // TRAINING_H_INCLUDED
