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
    unsigned long int i;
    int ID;

    char file_in[] = "D:/projet/AI/viola&jones/file/true/094.bmp"; //chemin à changer
    char file_out[] = "D:/projet/AI/viola&jones/file/true/out.bmp"; //chemin à changer
    caract_t caracteristics;
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(file_in);

    int count_image, nb_charact = 0;
    float sum, scare_sum, variance;

    File_caract file_input;
    file_input.set_name(file_cara);
    char mode[] = "r";
    if(file_input.file_open(mode) != ERROR)
    {
            unsigned int test = file_input.get_nb_caracteristics();

            if(test != 0)
            {
                file_input.go_to_origin();

                do
                {
                    ID = file_input.get_id();
                    i = file_input.get_rects(caracteristics);
                    fscanf(file_input.get_file_id(), "%d %f %f %f <\\D> ", &count_image, &sum, &scare_sum, &variance);
                    if(((ID == 5) || (ID == 5))&& (variance <= 17.5))
                    {
                        if(ID<10)
                            draw_0d(BMP.input_bmp.image,caracteristics.caract[0].x,caracteristics.caract[0].y,caracteristics.caract[0].length,caracteristics.caract[0].height);
                        else
                            draw_45d(BMP.input_bmp.image,caracteristics.caract[0].x,caracteristics.caract[0].y,caracteristics.caract[0].length,caracteristics.caract[0].height);
                        nb_charact ++;
                    }
                }
                while(i == 0);
                printf("\n%d characteristics draws\n\n", nb_charact);
            }
            else
                printf("\nNo caracteristic found to generate output\n");

        BMP.write_bmp(file_out);
        file_input.file_close();
    }

}

int main ()
{
  char file_folder[] = "D:/projet/AI/viola&jones/file_folder.txt"; //chemin à changer

  Training training(file_folder);

  training.generate_caracteristic_file();

  //find_best();



  return 0;
}
