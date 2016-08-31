#ifndef FILE_WEAK_H_INCLUDED
#define FILE_WEAK_H_INCLUDED

#include "File.h"

class File_weak : public File
{
public:
    File_weak();
    ~File_weak();

    void set_next_image();
    int get_next_data(int& data);
    void set_next_data(float data);
    void set_parity(caracteristic_type_t caracteristic_type);
    int go_to_parity();

private:
    char tag_parity[5];
};


#endif // FILE_WEAK_H_INCLUDED
