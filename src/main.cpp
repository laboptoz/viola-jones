#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string.h>

#include "bitmap/bmp.h"
#include "V_J/V_and_J.h"
#include "V_J/training.h"
//#include "time/timer_t.hpp"

void aff_vect(std::vector< std::vector<unsigned long int> > &vect )
{
  std::cout << "vect is:" << std::endl;
  for (std::vector<std::vector<unsigned long int> >::iterator it = vect.begin(); it != vect.end(); ++it)
  {
    for (std::vector<unsigned long int>::iterator its = it->begin(); its != it->end(); ++its)
      std::cout << ' ' << (unsigned long int)*its;
    std::cout << '\n';
  }
}

void out(std::vector<std::vector<unsigned long int> > &image, float x, float y, float l, float w)
{
    int mini = std::min(image.size(), image[0].size());
    float factor = mini / INIT_SIZE;
    //0 0 6 12//5 5 4 12//5 5 14 14//5 6 4 11//5 7 4 9
    //5 7 11 1//7 3 12//7 6 4 10//7 9 7 6//7 10 10 9//7 15 3 4//8 3 2 16//8 3 6 8
    //9 6 4 13//11 13 7 2//17 4 2 2//17 2 2 5//16 12 2 6//16 10 2 4//16 1 3 14//12 2 4 14

    //11 14 2 5
    x = x*factor;
    y = y*factor;
    l = l*factor;
    w = w*factor;
    for (unsigned int i = 0; i < image.size(); i++)
    {
        for (unsigned int j = 0; j < image[0].size(); j++)
        {
            if((i > x) && (i < (x+l)) && (j > y) && (j < (y+w)))
                image[i][j] = 0;
        }
    }

}

void find_best()
{
    char file_cara[] = "D:/projet/AI/viola&jones/file/caract_true.txt";
    unsigned long int i;

    char file_in[] = "D:/projet/AI/viola&jones/file/true/004.bmp"; //chemin à changer
    char file_out[] = "D:/projet/AI/viola&jones/file/true/out.bmp"; //chemin à changer
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(file_in);

    errno = 0;
    FILE * file_input = fopen(file_cara, "r");
    if(file_input != NULL)
    {
            fseek(file_input, 0, 0);

            int x,y,length1, hieght1, wieght1,length2,hieght2,wieght2;
            int count_image;
            float variance, sum, scare_sum;

            do
            {
                i = fscanf(file_input, "<rect> %d %d %d %d %d <\\rect> <rect> %d %d %d %d %d <\\rect> <data> %d %f %f %f <\\data> ", &x, &y, &length1, &hieght1, &wieght1, &x, &y, &length2, &hieght2, &wieght2,
                                &count_image, &sum, &scare_sum, &variance);

                out(BMP.input_bmp.image,x,y,length1,hieght1);

            }
            while(i == 14);

        BMP.write_bmp(file_out);
        fclose(file_input);
    }
    else
        printf("Error to open %s error code %d \n", file_in, errno);

}

int main ()
{
  char file_folder[] = "D:/projet/AI/viola&jones/file_folder.txt"; //chemin à changer

  Training training(file_folder);

  training.generate_caracteristic_file();

  //find_best();

    /*char file_in[] = "D:/projet/AI/viola&jones/file/true/007.bmp"; //chemin à changer
    char file_out[] = "D:/projet/AI/viola&jones/file/true/out.bmp"; //chemin à changer
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(file_in);
    /*out(BMP.input_bmp.image, 13, 5, 3, 12);
    out(BMP.input_bmp.image, 14, 7, 4, 9);
    out(BMP.input_bmp.image, 5, 3, 5, 4);
    out(BMP.input_bmp.image, 5, 9, 4, 2);
    out(BMP.input_bmp.image, 5, 11, 11, 4);*/

    /*out(BMP.input_bmp.image, 9, 4, 2, 11);
    out(BMP.input_bmp.image, 9, 9, 2, 10);
    //out(BMP.input_bmp.image, 15, 10, 2, 2);
    //out(BMP.input_bmp.image, 3, 7, 2, 7);
    //out(BMP.input_bmp.image, 8, 12, 2, 4);
    BMP.write_bmp(file_out);*/
  return 0;
}
