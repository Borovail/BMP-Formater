#include "transformations.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct bmp_image *init_image_from(const struct bmp_image *image, size_t new_width, size_t new_height)
{
     LOG_INFO("Initializing new image from image %p with new width %zu and new height %zu\n", (void *)image, new_width, new_height);

     struct bmp_image *new_image = (struct bmp_image *)malloc(sizeof(struct bmp_image));
     if (new_image == NULL){
          LOG_ERROR("Memory allocation failed\n");
     }

     new_image->header = (struct bmp_header *)malloc(sizeof(struct bmp_header));
     if (new_image->header == NULL)
     {
          LOG_ERROR("Memory allocation failed\n");
          free(new_image);
          return NULL;
     }
     *new_image->header = *image->header;
     new_image->header->width = (uint32_t)new_width;
     new_image->header->height = (uint32_t)new_height;

     
     size_t row_size = (new_width * sizeof(struct pixel) + 3) & ~3; 
     new_image->header->image_size = row_size * new_height;        

     new_image->header->size = 54 + new_image->header->image_size; 

     new_image->data = (struct pixel *)malloc(new_image->header->width * new_image->header->height * sizeof(struct pixel));
     if (new_image->data == NULL)
     {
          LOG_ERROR("Memory allocation failed\n");
          free(new_image->header);
          free(new_image);
          return NULL;
     }

     LOG_INFO("New image initialized successfully\n");

     return new_image;
}

typedef size_t (*index_transformer)(size_t x, size_t y, size_t width, size_t height);

size_t horizontal_flip_index(size_t x, size_t y, size_t width, size_t height)
{
     height = height - 1;
     return y * width + (width - x - 1);
}

size_t vertical_flip_index(size_t x, size_t y, size_t width, size_t height)
{
     return (height - y - 1) * width + x;
}

size_t rotate_right_index(size_t x, size_t y, size_t width, size_t height)
{
     width = width - 1;
     return x * height + height - y - 1;
}

size_t rotate_left_index(size_t x, size_t y, size_t width, size_t height)
{
     return (width - x - 1) * height + y;
}

void rotate(const struct bmp_image *source_image, struct bmp_image *output_image, index_transformer transformer)
{
     size_t left_index = 0, index = 0;
     for (size_t y = 0; y < source_image->header->height; y++)
     {
          for (size_t x = 0; x < source_image->header->width; x++)
          {
               left_index = y * source_image->header->width + x;
               index = transformer(x, y, source_image->header->width, source_image->header->height);
               output_image->data[left_index] = source_image->data[index];
          }
     }
}

struct bmp_image *flip_horizontally(const struct bmp_image *image)
{
     LOG_INFO("Flipping image %p horizontally\n", (void *)image);

     if (image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = init_image_from(image, image->header->width, image->header->height);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     rotate(image, new_image, horizontal_flip_index);


     LOG_INFO("Image flipped horizontally successfully\n");
     return new_image;
}

struct bmp_image *flip_vertically(const struct bmp_image *image)
{
     LOG_INFO("Flipping image %p vertically\n", (void *)image);

     if (image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = init_image_from(image, image->header->width, image->header->height);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     rotate(image, new_image, vertical_flip_index);

     LOG_INFO("Image flipped vertically successfully\n");
     return new_image;
}

struct bmp_image *rotate_right(const struct bmp_image *image)
{
     LOG_INFO("Rotating image %p right\n", (void *)image);
     
     if (image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = init_image_from(image, image->header->height, image->header->width);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     rotate(image, new_image, rotate_right_index);

     LOG_INFO("Image rotated right successfully\n");
     return new_image;
}

struct bmp_image *rotate_left(const struct bmp_image *image)
{
     LOG_INFO("Rotating image %p left\n", (void *)image);

     if (image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = init_image_from(image, image->header->height, image->header->width);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     rotate(image, new_image, rotate_left_index);

     LOG_INFO("Image rotated left successfully\n");
     return new_image;
}


struct bmp_image *scale(const struct bmp_image *image, float factor)
{
     LOG_INFO("Scaling image %p by factor %f\n", (void *)image, factor);

     if (factor <= 0 || image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Factor is less than or equal to 0 or image or header or data is NULL\n");
          return NULL;
     }

     size_t new_width = round(image->header->width * factor);
     size_t new_height = round(image->header->height * factor);

     struct bmp_image *new_image = init_image_from(image, new_width, new_height);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     size_t original_x = 0, original_y = 0;
     for (size_t new_y = 0; new_y < new_image->header->height; new_y++)
     {
          for (size_t new_x = 0; new_x < new_image->header->width; new_x++)
          {
               original_x = round(new_x / factor);
               original_y = round(new_y / factor);

               original_x = (original_x >= image->header->width) ? image->header->width - 1 : original_x;
               original_y = (original_y >= image->header->height) ? image->header->height - 1 : original_y;

               new_image->data[new_y * new_image->header->width + new_x] = image->data[original_y * image->header->width + original_x];
          }
     }

     LOG_INFO("Image scaled successfully\n");
     return new_image;
}

struct bmp_image *crop(const struct bmp_image *image, const uint32_t start_y, const uint32_t start_x,
                       const uint32_t height, const uint32_t width)
{
     LOG_INFO("Cropping image %p with start_x %u, start_y %u, height %u, width %u\n", (void *)image, start_x, start_y, height, width);
     if (image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     if (start_x + width > image->header->width || start_y + height > image->header->height){
          LOG_WARNING("Invalid cropping dimensions\n");
          return NULL;
     }

     struct bmp_image *new_image = init_image_from(image, width, height);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     size_t left_index = 0, index = 0;
     for (size_t y = start_y; y < start_y + height; y++)
     {
          for (size_t x = start_x; x < start_x + width; x++)
          {
               left_index = (y - start_y) * width + (x - start_x);
               index = y * image->header->width + x;
               new_image->data[left_index] = image->data[index];
          }
     }

     LOG_INFO("Image cropped successfully\n");
     return new_image;
}


struct bmp_image *extract(const struct bmp_image *image, const char *colors_to_keep)
{
     LOG_INFO("Extracting colors %s from image %p\n", colors_to_keep, (void *)image);

     if (image == NULL || image->header == NULL || image->data == NULL){
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     bool keep_red = strchr(colors_to_keep, 'r') != NULL;
     bool keep_green = strchr(colors_to_keep, 'g') != NULL;
     bool keep_blue = strchr(colors_to_keep, 'b') != NULL;

     if(!keep_red && !keep_green && !keep_blue)
     {
          LOG_WARNING("No colors to keep\n");
          return NULL;
     }
     
     struct bmp_image *new_image = init_image_from(image, image->header->width, image->header->height);

     if (new_image == NULL){
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     for (size_t y = 0; y < image->header->height; y++)
     {
          for (size_t x = 0; x < image->header->width; x++)
          {
               size_t index = y * image->header->width + x;
               new_image->data[index].red = keep_red ? image->data[index].red : 0;
               new_image->data[index].green = keep_green ? image->data[index].green : 0;
               new_image->data[index].blue = keep_blue ? image->data[index].blue : 0;
          }
     }

     LOG_INFO("Colors extracted successfully\n");
     return new_image;
}
