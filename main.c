
#include "ppm.h"
#include "ppm_utils.h"

int main(int argc, char *argv[]) {

    ppm_image *image = make_image(500, 500);
    set_background_color(image, 255, 255, 255);

    write_image(image, "test.ppm");
    free_image(image);

    return 0;

}