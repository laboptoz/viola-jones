#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string.h>

#include "bitmap/bmp.h"
#include "V_J/V_and_J.h"
#include "V_J/training.h"
#include "V_J/caract.h"

void draw_0d(std::vector<std::vector<unsigned long int> > &image, int x, int y, int l, int w)
{
    int mini = std::min(image.size(), image[0].size());
    float factor = mini / INIT_SIZE;

    x = x*factor;
    y = y*factor;
    l = l*factor;
    w = w*factor;

    int i;

    for(i = x; i < x+l; i++)
    {
        image[i][y] = 0;
        image[i][y+w-1] = 0;
    }

    for(i = y; i < y+w; i++)
    {
        image[x][i] = 0;
        image[x+l-1][i] = 0;
    }
}

void draw_45d(std::vector<std::vector<unsigned long int> > &image, int x, int y, int l, int h)
{
    int mini = std::min(image.size(), image[0].size());
    float factor = mini / INIT_SIZE;

    x = x*factor;
    y = y*factor;
    l = (l-1)*factor;
    h = (h-1)*factor;

    int i;

    for(i = 0; i < l; i++)
    {
        image[x+i][y-i] = 0;
        image[x+h-1+i][y+h-1-i] = 0;
    }

    for(i = 0; i < h; i++)
    {
        image[x+i][y+i] = 0;
        image[x+l-1+i][y-l+1+i] = 0;
    }
}

void find_best()
{
    char file_cara[] = "D:/projet/AI/viola&jones/file/caract_true.txt";

    char file_in[] = "D:/projet/AI/viola&jones/file/true/000.bmp"; //chemin à changer
    char file_out[] = "D:/projet/AI/viola&jones/file/true/out.bmp"; //chemin à changer
    caract_t caracteristics;
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(file_in);

    File_threshold threshold_file;
    threshold_file.set_name("D:/projet/AI/viola&jones/file/threshold.txt");
    std::vector<int> index = threshold_file.get_index_sup(70.);
    std::cout << index.size() << std::endl;

    File_caract caracts;
    caracts.set_name(file_cara);
    caracts.file_open("r");

    for(std::vector<int>::iterator it = index.begin(); it != index.end(); ++it)
    {
        int id = caracts.get_caract_index((*it), caracteristics);
        if(id != ERROR)
        {
            std::cout << id << std::endl;
            if(id<10)
                draw_0d(BMP.input_bmp.image,caracteristics.caract[0].x,caracteristics.caract[0].y,caracteristics.caract[0].length,caracteristics.caract[0].height);
            else
                draw_45d(BMP.input_bmp.image,caracteristics.caract[0].x,caracteristics.caract[0].y,caracteristics.caract[0].length,caracteristics.caract[0].height);
        }
    }

    caracts.file_close();
    BMP.write_bmp(file_out);
}

int main ()
{
  char file_folder[] = "D:/projet/AI/viola&jones/file_folder.txt"; //chemin à changer

  //Training training(file_folder);

  //training.generate_caracteristic_file();

  find_best();



  return 0;
}
