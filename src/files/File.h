#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../config.h"
#include "../V_J/caract.h"

class File
{
public:
    File();
    ~File();

    void set_name(char* name);
    void get_name(char* name);
    FILE* get_id() {return m_file_id;};
    char* get_mode() {return m_mode;};

    void go_to_origin();
    void go_to_end();

    int file_open(char* mode);
    void file_close();

    int cpy_into_this_file(File origin);
    int cpy_from_this_file(File destination);

    int clean_file();

private:
    char m_file_name[100];
    char m_mode[5];
    FILE* m_file_id;
};

#endif // FILE_H_INCLUDED
