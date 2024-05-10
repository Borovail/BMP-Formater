// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <getopt.h>
// #include <stdbool.h>

// #include "bmp.h"
// #include "transformations.h"
// #include "log.h"

// void print_usage()
// {
//     printf("Usage: bmp [OPTION]... [FILE]...\n");
//     printf("Simple BMP transformation tool.\n");
//     printf("\nOptions:\n");
//     printf("  -r, --rotate <l|r>            Rotate image left or right\n");
//     printf("  -f, --flip <h|v>              Flip image horizontally or vertically\n");
//     printf("  -c, --crop <y,x,h,w>          Crop image from position [y,x] with height h and width w\n");
//     printf("  -s, --scale <factor>          Scale image by factor\n");
//     printf("  -b, --brightness <factor>     Adjust brightness by factor\n");
//     printf("  -t, --transparency <factor>   Adjust transparency by factor\n");
//     printf("  -o, --output <file>           Write output to file\n");
//     printf("  -i, --input <file>            Read input from file\n");
//     printf("  -h, --help                    Display this help and exit\n");
// }

// int main(int argc, char *argv[])
// {
//     LOG_INFO("Program started\n");
//     if (argc == 1)
//     {
//         print_usage();
//         LOG_INFO("Program finished successfully\n");
//         return 0;
//     }

//     char *input_file = NULL;
//     char *output_file = NULL;
//     struct bmp_image *temp_image = NULL;
//     struct bmp_image *image = NULL;
//     FILE *input_stream = NULL;
//     FILE *output_stream = NULL;

//     int opt;
//     static struct option long_options[] = {
//         {"rotate", required_argument, 0, 'r'},
//         {"flip", required_argument, 0, 'f'},
//         {"crop", required_argument, 0, 'c'},
//         {"scale", required_argument, 0, 's'},
//         {"brightness", required_argument, 0, 'b'},
//         {"transparency", required_argument, 0, 't'},
//         {"output", required_argument, 0, 'o'},
//         {"input", required_argument, 0, 'i'},
//         {"help", no_argument, 0, 'h'},
//         {0, 0, 0, 0}};

//     while ((opt = getopt_long(argc, argv, "r:f:c:s:b:t:o:i:h", long_options, NULL)) != -1)
//     {
//         switch (opt)
//         {
//         case 'o':
//             output_file = optarg;
//             break;
//         case 'i':
//             input_file = optarg;
//             break;
//         case 'h':
//             print_usage();
//             LOG_INFO("Program finished successfully\n");
//             return 0;
//         }
//     }

//     if (input_file)
//     {
//         input_stream = fopen(input_file, "rb");
//         if (!input_stream)
//         {
//             LOG_ERROR("Failed to open input file %s\n", input_file);
//             return EXIT_FAILURE;
//         }
//         LOG_INFO("File opened: %s", input_file);

//         image = read_bmp(input_stream);
//         fclose(input_stream);
//         LOG_INFO("File closed: %s", input_file);

//         if (!image)
//         {
//             LOG_ERROR("Failed to read BMP from %s\n", input_file);
//             return EXIT_FAILURE;
//         }
//     }
//     else
//     {
//         fprintf(stderr, "Input file must be specified\n");
//         return EXIT_FAILURE;
//     }

//     optind = 1;
//     while ((opt = getopt_long(argc, argv, "r:f:c:s:b:t:o:i:h", long_options, NULL)) != -1)
//     {
//         switch (opt)
//         {
//         case 'r':
//             if (strcmp(optarg, "l") == 0 || strcmp(optarg, "left") == 0)
//                 temp_image = rotate_left(image);
//             else if (strcmp(optarg, "r") == 0 || strcmp(optarg, "right") == 0)
//                 temp_image = rotate_right(image);
//             else
//             {
//                 fprintf(stderr, "Invalid option for rotate: %s\n", optarg);
//                 free_bmp_image(image);
//                 return EXIT_FAILURE;
//             }

//             free_bmp_image(image);
//             image = temp_image;
//             break;
//         case 'f':
//             if (strcmp(optarg, "h") == 0 || strcmp(optarg, "horizontal") == 0)
//                 temp_image = flip_horizontally(image);
//             else if (strcmp(optarg, "v") == 0 || strcmp(optarg, "vertical") == 0)
//                 temp_image = flip_vertically(image);
//             else
//             {
//                 fprintf(stderr, "Invalid option for flip: %s\n", optarg);
//                 free_bmp_image(image);
//                 return EXIT_FAILURE;
//             }

//             free_bmp_image(image);
//             image = temp_image;
//             break;
//         case 'c':
//         {
//             int y, x, h, w;
//             if (sscanf(optarg, "%d,%d,%d,%d", &y, &x, &h, &w) == 4)
//             {
//                 if (y < 0 || x < 0 || h < 0 || w < 0)
//                 {
//                     fprintf(stderr, "Crop dimensions and positions must be non-negative\n");
//                     return EXIT_FAILURE;
//                 }
//                 temp_image = crop(image, (uint32_t)y, (uint32_t)x, (uint32_t)h, (uint32_t)w);
//             }
//             else
//             {
//                 fprintf(stderr, "Invalid format for crop: %s\n", optarg);
//                 return EXIT_FAILURE;
//             }

//             free_bmp_image(image);
//             image = temp_image;
//             break;
//         }
//         case 's':
//         {
//             float factor = (float)atof(optarg);
//             if (factor > 0)
//                 temp_image = scale(image, factor);
//             else
//             {
//                 fprintf(stderr, "Invalid scale factor: %s\n", optarg);
//                 free_bmp_image(image);
//                 return EXIT_FAILURE;
//             }

//             free_bmp_image(image);
//             image = temp_image;
//             break;
//         }
//         case 'b':
//             temp_image = brightness(image, (float)atof(optarg));

//             free_bmp_image(image);
//             image = temp_image;
//             break;
//         case 't':
//             temp_image = transparency(image, (float)atof(optarg));

//             free_bmp_image(image);
//             image = temp_image;
//             break;
//         }
//     }

//     if (!output_file)
//         output_file = "output.bmp";
    

//     output_stream = fopen(output_file, "wb");
//     if (!output_stream)
//     {
//         LOG_ERROR("Failed to open output file %s", output_file);
//         free_bmp_image(image);
//         return EXIT_FAILURE;
//     }
//     LOG_INFO("File opened: %s", output_file);
//     write_bmp(output_stream, image);
//     fclose(output_stream);
//     LOG_INFO("File closed: %s", output_file);

//     free_bmp_image(image);

//     LOG_INFO("Program finished successfully");
//     return EXIT_SUCCESS;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "bmp.h"
#include "transformations.h"
#include "log.h"

int main(){
    FILE *input_stream = fopen("assets/freddy.bmp", "rb");
    struct bmp_image *image = read_bmp(input_stream);
    display_image_info(image);
    struct bmp_image *temp_image = rotate_left(image);
    display_image_info(temp_image);
    FILE *output_stream = fopen("output.bmp", "wb");
    write_bmp(output_stream, temp_image);
    fclose(input_stream);
    fclose(output_stream);
    free_bmp_image(image);
    // free_bmp_image(temp_image);
    return 0;
}