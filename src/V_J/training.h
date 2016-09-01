#ifndef TRAINING_H_INCLUDED
#define TRAINING_H_INCLUDED

#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../config.h"
#include "caract.h"
#include "../bitmap/bmp.h"
#include "../files/File_caract.h"
#include "../files/File_threshold.h"
#include "../files/File_weak.h"
#include "../files/File_folder.h"

enum last_file_used_t
{
    caract,
    temp
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

    std::vector< std::vector<unsigned long int> > integral_image_0d;
    std::vector< std::vector<unsigned long int> > integral_image_45d;

    File_folder m_folder_file;
    File_caract m_true_file;
    File_caract m_false_file;
    File_weak m_weak_file;
    File_threshold m_threshold_file;

    void compute_image(bmp& BMP, char* image_name);
    float get_sum(int ID, bmp& BMP, caract_t caracteristics);
    void set_min_and_max(float data, float& mini, float& maxi, int count_image);
    void set_image_caract(char* file_name, caracteristic_type_t caracteristic_type);
    void generate_thresholds();
    void compute_errors();
};


#endif // TRAINING_H_INCLUDED
