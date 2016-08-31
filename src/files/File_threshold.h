#ifndef FILE_THRESHOLD_H_INCLUDED
#define FILE_THRESHOLD_H_INCLUDED

#include "File.h"

class File_threshold : public File
{
public:
    File_threshold();
    ~File_threshold();

    float compute_treshold(int mini, int maxi);
    void write_datas(int T_pos = 0, int T_neg = 0, int F_pos = 0, int F_neg = 0);
    int get_datas();
    int set_datas();

    float get_threshold(int i);

    std::vector<int> T_pos, T_neg, F_pos, F_neg;

private:
    std::vector<float> threshold;
    float tmp_threshold;
    char tag_data[5];

    int go_to_threshold();

};

#endif // FILE_THRESHOLD_H_INCLUDED
