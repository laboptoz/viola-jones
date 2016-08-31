#ifndef TRAINING_H_INCLUDED
#define TRAINING_H_INCLUDED

#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../config.h"
#include "caract.h"
#include "../files/File_caract.h"

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

    File_caract m_true_file;
    File_caract m_false_file;
    char m_weak_file[100];
    char m_threshold_file[100];

    void set_image_caract(char* file_name, caracteristic_type_t caracteristic_type);
    void clean_file(char* file_name);
    void generate_thresholds();
    void compute_errors();
};


#endif // TRAINING_H_INCLUDED
