#include "bmp.h"
#include "transformations.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>


int main() {
    FILE *stream = fopen("assets/adventurer.bmp", "rb");
    if (!stream) {
        fprintf(stderr, "Error opening file 'adventurer.bmp'\n");
        return EXIT_FAILURE;
    }

    struct bmp_image* image = read_bmp(stream);
    if (!image) {
        fprintf(stderr, "Failed to read BMP image\n");
        fclose(stream);
        return EXIT_FAILURE;
    }

    image->header=NULL;

    struct bmp_image* flipped_horizontally = flip_horizontally(image);
    if (!flipped_horizontally) {
        fprintf(stderr, "Failed to flip the image horizontally\n");
        free_bmp_image(image);
        fclose(stream);
        return EXIT_FAILURE;
    }


    FILE *flipped_stream = fopen("flipped.bmp", "wb");
    if (!flipped_stream) {
        fprintf(stderr, "Error opening file 'flipped.bmp'\n");
        free_bmp_image(image);
        free_bmp_image(flipped_horizontally);
        fclose(stream);
        return EXIT_FAILURE;
    }

    write_bmp(flipped_stream, flipped_horizontally);

    fclose(stream);
    fclose(flipped_stream);

    free_bmp_image(image);
    free_bmp_image(flipped_horizontally);

    printf("Image flipped horizontally and saved to 'flipped.bmp'\n");

    return EXIT_SUCCESS;
}

