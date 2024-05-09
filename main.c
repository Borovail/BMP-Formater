#include "bmp.h"
#include "transformations.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>


// int main(){
//     FILE *stream = fopen("assets/adventurer.bmp", "rb");
//     if (!stream) {
//         fprintf(stderr, "Error opening file 'adventurer.bmp'\n");
//         return EXIT_FAILURE;
//     }
// struct bmp_image* image = read_bmp(stream);
// struct bmp_image* flipped_horizontally = flip_horizontally(image);
// struct bmp_image* flipped_vertically = flip_vertically(image);
// struct bmp_image* rotated_right = rotate_right(image);
// struct bmp_image* rotated_left = rotate_left(image);
// struct bmp_image* scaled = scale(image, 2);
// struct bmp_image* cropped = crop(image, 10, 10, 100, 100);
// struct bmp_image* extracted = extract(image, "r");

// free_bmp_image(image);
// free_bmp_image(flipped_horizontally);
// free_bmp_image(flipped_vertically);
// free_bmp_image(rotated_right);
// free_bmp_image(rotated_left);
// free_bmp_image(scaled);
// free_bmp_image(cropped);
// free_bmp_image(extracted);

// fclose(stream);

// return 0;
// }

// int main() {
//     FILE *stream = fopen("assets/adventurer.bmp", "rb");
//     if (!stream) {
//         fprintf(stderr, "Error opening file 'adventurer.bmp'\n");
//         return EXIT_FAILURE;
//     }

//     struct bmp_image* image = read_bmp(stream);
//     if (!image) {
//         fprintf(stderr, "Failed to read BMP image\n");
//         fclose(stream);
//         return EXIT_FAILURE;
//     }

//     image->header=NULL;

//     struct bmp_image* flipped_horizontally = flip_horizontally(image);
//     if (!flipped_horizontally) {
//         fprintf(stderr, "Failed to flip the image horizontally\n");
//         free_bmp_image(image);
//         fclose(stream);
//         return EXIT_FAILURE;
//     }


//     FILE *flipped_stream = fopen("flipped.bmp", "wb");
//     if (!flipped_stream) {
//         fprintf(stderr, "Error opening file 'flipped.bmp'\n");
//         free_bmp_image(image);
//         free_bmp_image(flipped_horizontally);
//         fclose(stream);
//         return EXIT_FAILURE;
//     }

//     write_bmp(flipped_stream, flipped_horizontally);

//     fclose(stream);
//     fclose(flipped_stream);

//     free_bmp_image(image);
//     free_bmp_image(flipped_horizontally);

//     printf("Image flipped horizontally and saved to 'flipped.bmp'\n");

//     return EXIT_SUCCESS;
// }

int main()
{

    read_bmp();
    write_bmp();
    read_bmp_header();
    read_data();
    free_bmp_image();

    flip_horizontally();
    flip_vertically();
    rotate_right();
    rotate_left();
    scale();
    crop();
    extract();

    return 0;
}