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

void aff_vect(std::vector< std::vector<unsigned long int> > &vect )
{
  std::cout << "vect is:" << std::endl;
  for (auto& row : vect)
  {
    for (auto& element : row)
      std::cout << ' ' << (unsigned long int)element;
    std::cout << '\n';
  }
}

using VectI = std::vector<int>;

void out(std::vector<std::vector<unsigned long int> > &image, int x, int y, int l, int w)
{
    int mini = std::min(image.size(), image[0].size());
    float factor = mini / INIT_SIZE;

    x = x*factor;
    y = y*factor;
    l = l*factor;
    w = w*factor;

    int j,i;

    for (i = x; i < x+l; i++)
    {
        for (j = y; j < y+w; j++)
        {
            if( (i == x) || (i == x+l-1) || (j == y) || (j == y+w-1) )
                image[i][j] = 0;
        }
    }

}

void find_best()
{
    char file_cara[] = "D:/projet/AI/viola&jones/file/tmp.txt";
    unsigned long int i;

    char file_in[] = "D:/projet/AI/viola&jones/file/true/045.bmp"; //chemin à changer
    char file_out[] = "D:/projet/AI/viola&jones/file/true/out.bmp"; //chemin à changer
    Caracteristics caracteristic;
    caract_t caracteristics;
    bmp BMP;
    bmp_t bmps = BMP.read_bmp(file_in);

    errno = 0;
    FILE * file_input = fopen(file_cara, "r");
    if(file_input != NULL)
    {
            unsigned int test = caracteristic.get_nb_caracteristics(file_input);

            if(test != 0)
            {
                fseek(file_input, 0, 0);

                do
                {
                    i = caracteristic.get_rects(file_input, caracteristics);
                    out(BMP.input_bmp.image,caracteristics.caract[0].x,caracteristics.caract[0].y,caracteristics.caract[0].length,caracteristics.caract[0].height);
                }
                while(i == 0);
            }
            else
                printf("\nNo caracteristic found to generate output\n");

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

  find_best();

  return 0;
}
