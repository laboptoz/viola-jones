#include "File.h"

File::File()
{
    strcpy(write_mode, "w");
    strcpy(read_mode, "r");
    strcpy(append_mode, "a");
    strcpy(write_update_mode, "w+");
    strcpy(read_update_mode, "r+");
    strcpy(append_update_mode, "a+");
}

File::~File()
{

}

void File::set_name(char* name)
{
    strcpy(m_file_name, name);
}

void File::get_name(char* name)
{
    strcpy(name, m_file_name);
}

void File::go_to_origin()
{
    fseek(m_file_id, 0, SEEK_SET);
}

void File::go_to_end()
{
    fseek(m_file_id, 0, SEEK_END);
}

int File::file_open(char* mode)
{
    errno = 0;
    strcpy(m_mode, mode);
    m_file_id = fopen(m_file_name, mode);
    if(m_file_id == NULL)
    {
        printf("Error to open %s error code %d \n", m_file_name, errno);
        return ERROR;
    }
    else
        return 0;
}

void File::file_close()
{
    if(m_file_id != NULL)
    {
        fclose(m_file_id);
        strcpy(m_mode, "");
    }
}

int File::cpy_into_this_file(File origin)
{
    if((strcmp(get_mode(),write_mode) != 0) && (strcmp(origin.get_mode(),read_mode) != 0))
    {
        return ERROR;
    }
    origin.go_to_origin();
    go_to_origin();
    register char car;
    do
    {
        car = fgetc(origin.get_file_id());
        if(car != EOF)
            fputc(car, m_file_id);
    }
    while (car != EOF);
    return 0;
}

int File::cpy_from_this_file(File destination)
{
    if((strcmp(get_mode(),read_mode) != 0) && (strcmp(destination.get_mode(),write_mode) != 0))
    {
        return ERROR;
    }
    destination.go_to_origin();
    go_to_origin();
    register char car;
    do
    {
        car = fgetc(m_file_id);
        if(car != EOF)
            fputc(car, destination.get_file_id());
    }
    while (car != EOF);
    return 0;
}

int File::clean_file()
{
    int test1, test2;
    if(m_file_id == NULL)
    {
        test1 = file_open(read_mode);
        file_close();
    }
    else
    {
       file_close();
       test1 = file_open(read_mode);
       file_close();
       test2 = file_open(m_mode);
    }
    if((test1 == ERROR) || (test2 == ERROR))
        return ERROR;
    else
        return 0;
}
