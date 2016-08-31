#include "../files/File_caract.h"

void File_caract::define_all_caract_type()
{
    all_caract_type.clear();
    all_caract_type.reserve(6);

    define_rect_caract_t rect2;
    define_rect_caract_t rect3;

    define_caract_t define_caract;

    /* carct 1 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = FULL_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

     /* carct 2 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = FULL_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

     /* carct 3 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 2;

    rect3.offset_x = HALF_POSITION;
    rect3.offset_y = HALF_POSITION;
    rect3.length_factor = HALF_SIZE;
    rect3.height_factor = HALF_SIZE;
    rect3.wieght = 2;

    define_caract.nb_rect = 3;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    define_caract.caract[1] = rect3;
    all_caract_type.push_back(define_caract);

    /* carct 4 */
    rect2.offset_x = QUARTER_POSITION;
    rect2.offset_y = QUARTER_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 4;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

    /* carct 5 */
    rect2.offset_x = QUARTER_POSITION;
    rect2.offset_y = SAME_POSITION;
    rect2.length_factor = HALF_SIZE;
    rect2.height_factor = FULL_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);

    /* carct 6 */
    rect2.offset_x = SAME_POSITION;
    rect2.offset_y = QUARTER_POSITION;
    rect2.length_factor = FULL_SIZE;
    rect2.height_factor = HALF_SIZE;
    rect2.wieght = 2;

    define_caract.nb_rect = 2;
    define_caract.caract.resize(define_caract.nb_rect - 1);
    define_caract.caract[0] = rect2;
    all_caract_type.push_back(define_caract);


    std::cout << "nb caracteristic patern : " << all_caract_type.size() << std::endl;
}
