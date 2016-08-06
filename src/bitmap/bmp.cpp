#include "../bitmap/bmp.h"

#include <iostream>
#include <cmath>

#define palette1 8
#define palette4 64
#define palette8 1024
//palette = 2^i * 4

#define R_COEF 0.3
#define G_COEF 0.59
#define B_COEF 0.11

constexpr uint8_t MASK_1 = 0x01;
constexpr uint8_t MASK_4 = 0x0f;
constexpr uint8_t MASK_8 = 0xff;

bmp::bmp()
{

}

bmp::~bmp()
{

}

bmp_t bmp::read_bmp(char* file_name)
{
    errno = 0;
    FILE * file = fopen(file_name, "rb");

    if(file != NULL)
    {
        get_bmp_header(file);
        if(input_bmp.palette_size != 0)
            get_image(file);
        else
            get_image_real_color(file);

        fclose(file);
    }
    else
        printf("Error read %d \n", errno);

    return(input_bmp);
}

void bmp::write_bmp(char* file_name)
{
    errno = 0;
    FILE * file = fopen(file_name, "wb");

    if(file != NULL)
    {
        generate_bmp_header(file);
        if(input_bmp.palette_size != 0)
            generate_image(file);
        else
            generate_image_real_color(file);

        fclose(file);
    }
    else
        printf("Error write %d \n", errno);
}

void bmp::compute_integral_image_0d(std::vector<std::vector<unsigned long int> > &image, std::vector<std::vector<unsigned long int> > &integral_image) //the image must be a rectangle !!!
{
    register unsigned int i,j;
    register unsigned long int scare_sum = 0;

    integral_image.resize(image.size());
    for(i = 0; i < integral_image.size(); i++)
        integral_image[i].resize(image[i].size());

    for(i = 0; i < integral_image.size(); i++)
        for(j = 0; j < integral_image[i].size(); j++)
        {
            scare_sum += image[i][j] * image[i][j];
            if((i <= 0) || (j <= 0))
            {
                if(i > 0)
                    integral_image[i][j] = image[i][j] + integral_image[i-1][j];

                if(j > 0)
                    integral_image[i][j] = image[i][j] + integral_image[i][j-1];
                else if((j == 0) && (i == 0))
                    integral_image[i][j] = image[i][j];
            }
            else
                integral_image[i][j] = image[i][j] + integral_image[i-1][j] + integral_image[i][j-1] - integral_image[i-1][j-1];
        }
    image_mean = integral_image[integral_image.size()-1][integral_image[0].size()-1] / (float)(integral_image.size()*integral_image[0].size());
    scare_sum = scare_sum - image_mean*image_mean;
    ecart_type = scare_sum >= 0 ? sqrtf(scare_sum) : 1.0f;
}

void bmp::compute_integral_image_45d(std::vector<std::vector<unsigned long int> > &image, std::vector<std::vector<unsigned long int> > &integral_image) //the image must be a rectangle !!!
{
    register unsigned int i, j;

    integral_image.resize(image.size());
    for(i = 0; i < integral_image.size(); i++)
        integral_image[i].resize(image[i].size());

    for(i = 0; i < integral_image.size(); i++)
        for(j = 0; j < integral_image[i].size(); j++)
        {
            integral_image[i][j] = image[i][j];
            if(i > 0)
            {
                integral_image[i][j] += image[i-1][j];

                if(j > 0)
                    integral_image[i][j] += integral_image[i-1][j -1];
                if(j < integral_image[i].size() - 1)
                    integral_image[i][j] += integral_image[i-1][j +1];
                if(i > 1)
                    integral_image[i][j] -= integral_image[i-2][j];
            }
        }
}

float bmp::get_sum_0d(std::vector<std::vector<unsigned long int> > &image, caract_t &p_caract)
{
    int mini = std::min(image.size(), image[0].size());
    float factor = (float)(mini - 1) / INIT_SIZE;
    std::vector<long int> results;
    results.resize(p_caract.nb_rect);
    float result = 0.f;

    int x_min;
    int y_min;
    int x_max;
    int y_max;

    register int i;

    for(i = p_caract.nb_rect - 1; i >= 0; i--)
    {
        x_min = p_caract.caract[i].x*factor;
        y_min = p_caract.caract[i].y*factor;
        x_max = x_min + (p_caract.caract[i].length)*factor;
        y_max = y_min + (p_caract.caract[i].height)*factor;

        results[i] = image[x_min][y_min] - image[x_max][y_min];
        results[i] += -image[x_min][y_max] + image[x_max][y_max];
        result += results[i]*p_caract.caract[i].wieght;
    }
    unsigned long int nb_pixel = p_caract.caract[0].length*p_caract.caract[0].height*factor*factor;
    return((result-image_mean*nb_pixel)/ecart_type/(float)nb_pixel);
}

float bmp::get_sum_45d(std::vector<std::vector<unsigned long int> > &image, caract_t &p_caract)
{
    /*    2
       1    3
         4
    */
    int mini = std::min(image.size(), image[0].size());
    float factor = (float)(mini - 1) / INIT_SIZE;
    std::vector<long int> results;
    results.resize(p_caract.nb_rect);
    float result = 0.f;

    int x1, y1;
    int x2, y2;
    int x3, y3;
    int x4, y4;

    int height_1_factor;

    register int i;

    for(i = p_caract.nb_rect - 1; i >= 0; i--)
    {
        height_1_factor = (p_caract.caract[i].height -1)*factor;

        x1 = p_caract.caract[i].x*factor;
        y1 = p_caract.caract[i].y*factor;

        x2 = (p_caract.caract[i].x + p_caract.caract[i].length - 1)*factor;
        y2 = (p_caract.caract[i].y - p_caract.caract[i].length + 1)*factor;

        x3 = x2 + height_1_factor;
        y3 = y2 + height_1_factor;

        x4 = x1 + height_1_factor;
        y4 = y1 + height_1_factor;

        results[i] = image[x3][y3] - image[x2][y2];
        results[i] += -image[x4][y4] + image[x1][y1];
        result += results[i]*p_caract.caract[i].wieght;
    }
    unsigned long int nb_pixel = ((float)p_caract.caract[0].length*(float)p_caract.caract[0].height + (float)(p_caract.caract[0].length-1)*(float)(p_caract.caract[0].height -1))*factor*factor;
    return((result-image_mean*nb_pixel)/ecart_type/(float)nb_pixel);
}

/*
*
*   private functions
*
*/

void bmp::get_bmp_header(FILE* file)
{
    unsigned char tmp[4];
    fread (input_bmp.type, sizeof(char), 2, file);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.file_size = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.start_addr = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.header_size = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.width = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.height = four_to_int(tmp);
    fread (tmp, sizeof(char), 2, file);
    input_bmp.nb_plans = two_to_int(tmp);
    fread (tmp, sizeof(char), 2, file);
    input_bmp.nb_bit_per_pixel = two_to_int(tmp);
    input_bmp.mask = get_mask(input_bmp.nb_bit_per_pixel);
    fread (tmp, sizeof(char), 3, file);
    input_bmp.compression_type = three_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.image_size = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.resolution_h = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.resolution_v = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.nb_colors_in_palette = four_to_int(tmp);
    fread (tmp, sizeof(char), 4, file);
    input_bmp.nb_significant_colors = four_to_int(tmp);
    if(input_bmp.nb_bit_per_pixel == 1)
        input_bmp.palette_size = palette1;
    else if(input_bmp.nb_bit_per_pixel == 4)
        input_bmp.palette_size = palette4;
    else if(input_bmp.nb_bit_per_pixel == 8)
        input_bmp.palette_size = palette8;
    else
        input_bmp.palette_size = 0;

    input_bmp.palette.resize(input_bmp.palette_size);
    for(unsigned int i = 0; i < input_bmp.palette_size/4; i++)
    {
        fread (tmp, sizeof(char), 4, file);
        input_bmp.palette[i] = (unsigned char)((float)(tmp[1]*R_COEF) + (float)(tmp[2]*G_COEF) + (float)(tmp[3]*B_COEF));
    }
}

void bmp::generate_bmp_header(FILE* file)
{
    output_bmp = input_bmp;
    for(unsigned int i = 0; i < output_bmp.palette_size/4; i++)
        output_bmp.palette[i] = 255*i/(output_bmp.palette_size/4 - 1);

    unsigned char empty_tab[4] = {0,0,0,0};
    unsigned char tmp[4];
    fwrite (output_bmp.type, sizeof(char), 2, file);
    int_to_four(output_bmp.file_size, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    fwrite (empty_tab, sizeof(char), 4, file);
    int_to_four(output_bmp.start_addr, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.header_size, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.width, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.height, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_two(output_bmp.nb_plans, tmp);
    fwrite (tmp, sizeof(char), 2, file);
    int_to_two(output_bmp.nb_bit_per_pixel, tmp);
    fwrite (tmp, sizeof(char), 2, file);
    int_to_three(output_bmp.compression_type, tmp);
    fwrite (tmp, sizeof(char), 3, file);
    int_to_four(output_bmp.image_size, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.resolution_h, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.resolution_v, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.nb_colors_in_palette, tmp);
    fwrite (tmp, sizeof(char), 4, file);
    int_to_four(output_bmp.nb_significant_colors, tmp);
    fwrite (tmp, sizeof(char), 4, file);

    for(unsigned int i = 0; i < output_bmp.palette_size/4; i++)
    {
        tmp[0] = 0;
        tmp[1] = output_bmp.palette[i];
        tmp[2] = output_bmp.palette[i];
        tmp[3] = output_bmp.palette[i];
        fwrite (tmp, sizeof(char), 4, file);
    }
}

void bmp::generate_image(FILE* file)
{
    int nb_byte_per_pixel = 8/output_bmp.nb_bit_per_pixel;
    int bytes_per_line = (output_bmp.width / (nb_byte_per_pixel*4)) *4;
    if(output_bmp.width % (4*nb_byte_per_pixel) != 0)
        bytes_per_line += 4;

    char data;
    int bytes, pixel;
    for(long int line = output_bmp.height - 1; line >= 0 ; line--)
    {
        fseek(file, output_bmp.start_addr + bytes_per_line*line, 0);
        for(bytes = 0; bytes < bytes_per_line; bytes++)
        {
            data = 0x00;
            for(pixel = nb_byte_per_pixel - 1; pixel >= 0 ; pixel--)
            {
                if(nb_byte_per_pixel*bytes + pixel < (int)output_bmp.width)
                    data = data | ((output_bmp.image[output_bmp.height - line - 1][nb_byte_per_pixel*bytes + pixel] * (output_bmp.palette_size/4) / 255) << (((8/input_bmp.nb_bit_per_pixel - pixel - 1))*input_bmp.nb_bit_per_pixel));
                else
                    data = data & ~(output_bmp.mask << ((8/input_bmp.nb_bit_per_pixel - pixel - 1))*input_bmp.nb_bit_per_pixel);
            }
            fwrite (&data, sizeof(char), 1, file);
        }
    }
}

void bmp::get_image(FILE* file)
{
  std::vector<std::vector<unsigned long int> > image(input_bmp.height, std::vector<unsigned long int>(input_bmp.width));
  int nb_bytes_per_pixel = 8/input_bmp.nb_bit_per_pixel;
  int bytes_per_line = (input_bmp.width / (nb_bytes_per_pixel*4)) *4;
  if(input_bmp.width % (4*nb_bytes_per_pixel) != 0)
    bytes_per_line += 4;

  char data;
  int bytes, pixel;
  for(long int line = input_bmp.height - 1; line >= 0 ; line--)
  {
    fseek(file, input_bmp.start_addr + bytes_per_line*line, 0);
    for(bytes = 0; bytes < bytes_per_line; bytes++)
    {
      fread (&data, sizeof(char), 1, file);
      for(pixel = nb_bytes_per_pixel - 1; pixel >= 0 ; pixel--)
      {
        if(nb_bytes_per_pixel*bytes + pixel < (int)input_bmp.width)
              image[input_bmp.height - line - 1][nb_bytes_per_pixel*bytes + pixel] = (unsigned char)input_bmp.palette[(data >> ((nb_bytes_per_pixel - pixel - 1)*input_bmp.nb_bit_per_pixel)) & input_bmp.mask];
      }
    }
  }

  input_bmp.image = image;
}

void bmp::get_image_real_color(FILE* file)
{
  std::vector<std::vector<unsigned long int> > image(input_bmp.height, std::vector<unsigned long int>(input_bmp.width));
  int bytes_per_line = ((input_bmp.width * 3) / 4) * 4;
  if((input_bmp.width * 3) % 4 != 0)
    bytes_per_line += 4;

  unsigned char data[3];
  int bytes;
  for(long int line = input_bmp.height - 1; line >= 0 ; line--)
  {
    fseek(file, input_bmp.start_addr + bytes_per_line*line, 0);
    for(bytes = 0; bytes < bytes_per_line; bytes = bytes + 3)
    {
        if(8*bytes/input_bmp.nb_bit_per_pixel < input_bmp.width)
        {
            fread (&data, sizeof(char), 3, file);
            image[input_bmp.height - line - 1][8*bytes/input_bmp.nb_bit_per_pixel] = (unsigned char)(((float)(data[0]*R_COEF)) + ((float)(data[1]*G_COEF)) + ((float)(data[2]*B_COEF)));
        }
    }
  }

  input_bmp.image = image;
}

void bmp::generate_image_real_color(FILE* file)
{
    int bytes_per_line = ((input_bmp.width * 3) / 4) * 4;
      if((input_bmp.width * 3) % 4 != 0)
        bytes_per_line += 4;

    unsigned char data;
    int bytes;
    for(long int line = output_bmp.height - 1; line >= 0 ; line--)
    {
        fseek(file, output_bmp.start_addr + bytes_per_line*line, 0);
        for(bytes = 0; bytes < bytes_per_line; bytes++)
        {
            data = 0x00;
            if(8*bytes/input_bmp.nb_bit_per_pixel < input_bmp.width)
                data = input_bmp.image[output_bmp.height - line - 1][8*bytes/output_bmp.nb_bit_per_pixel];
            else
                data = 0;
            fwrite (&data, sizeof(char), 1, file);
        }
    }
}

unsigned long int bmp::four_to_int(unsigned char data[4])
{
  return ((data[3]&0xff)<<(3*8) | (data[2]&0xff)<<(2*8) | (data[1]&0xff)<<(1*8) | (data[0]&0xff));
}

unsigned long int  bmp::three_to_int(unsigned char data[3])
{
  return ((data[2]&0xff)<<(2*8) | (data[1]&0xff)<<(1*8) | (data[0]&0xff));
}

unsigned long int bmp::two_to_int(unsigned char data[2])
{
  return ((data[1]&0xff)<<(1*8) | (data[0]&0xff));
}

void bmp::int_to_four(unsigned long int data, unsigned char converted[4])
{
  converted[3] = (data >> (3*8)) & 0xff;
  converted[2] = (data >> (2*8)) & 0xff;
  converted[1] = (data >> (1*8)) & 0xff;
  converted[0] = data & 0xff;
}

void bmp::int_to_three(unsigned long int data, unsigned char converted[3])
{
  converted[2] = (data >> (2*8)) & 0xff;
  converted[1] = (data >> (1*8)) & 0xff;
  converted[0] = data & 0xff;
}

void bmp::int_to_two(unsigned long int data,unsigned char converted[2])
{
  converted[1] = (data >> (1*8)) & 0xff;
  converted[0] = data & 0xff;
}

char bmp::get_mask(unsigned long int nb_pixel_bit_per_pixel)
{
    switch(nb_pixel_bit_per_pixel)
    {
    case 1: return MASK_1;
    case 4: return MASK_4;
    case 8: return MASK_8;
    default: return 0;
    }
}
