#include "bmp.h"
#include "transformations.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *stream = fopen("assets/adventurer.bmp", "rb");
    if (!stream)
    {
        LOG_ERROR("Error opening file 'adventurer.bmp'\n");
        return EXIT_FAILURE;
    }
    LOG_INFO("File 'adventurer.bmp' opened successfully\n");

    struct bmp_image *image = read_bmp(stream);
    if (!image)
    {
        LOG_ERROR("Failed to read BMP image\n");
        fclose(stream);
        return EXIT_FAILURE;
    }

    FILE *stream1 = fopen("result.bmp", "wb");
    if (!stream)
    {
        LOG_ERROR("Error opening file 'result.bmp'\n");
        return EXIT_FAILURE;
    }
    LOG_INFO("File 'result.bmp' opened successfully\n");

    if(write_bmp(stream1, image) == false)
    {
        LOG_ERROR("Failed to write BMP image\n");
        fclose(stream1);
        return EXIT_FAILURE;
    }

    free_bmp_image(image);
    fclose(stream);
    fclose(stream1);

    return EXIT_SUCCESS;
}
