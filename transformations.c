#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "transformations.h"
#include "log.h"

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

void transform(const struct bmp_image *source_image, struct bmp_image *output_image,
               index_transformer source_index_transformer)
{
     uint32_t width = source_image->header->width, height = source_image->header->height;
     size_t output_index = 0, source_index = 0;

     for (size_t y = 0; y < height; y++)
     {
          for (size_t x = 0; x < width; x++)
          {
               output_index = y * width + x;
               source_index = source_index_transformer(x, y, width, height);
               output_image->data[output_index] = source_image->data[source_index];
          }
     }
}

struct bmp_image *flip_horizontally(const struct bmp_image *image)
{
     LOG_INFO("Flipping image %p horizontally\n", (void *)image);

     if (!image || !image->header || !image->data)
     {
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = create_image_with(image->header, image->header->width, image->header->height);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     transform(image, new_image, horizontal_flip_index);

     LOG_INFO("Image flipped horizontally successfully\n");
     return new_image;
}

struct bmp_image *flip_vertically(const struct bmp_image *image)
{
     LOG_INFO("Flipping image %p vertically\n", (void *)image);

     if (!image || !image->header || !image->data)
     {
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = create_image_with(image->header, image->header->width, image->header->height);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     transform(image, new_image, vertical_flip_index);

     LOG_INFO("Image flipped vertically successfully\n");
     return new_image;
}

struct bmp_image *rotate_right(const struct bmp_image *image)
{
     LOG_INFO("Rotating image %p right\n", (void *)image);

     if (!image || !image->header || !image->data)
     {
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = create_image_with(image->header, image->header->height, image->header->width);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     transform(image, new_image, rotate_right_index);

     LOG_INFO("Image rotated right successfully\n");
     return new_image;
}

struct bmp_image *rotate_left(const struct bmp_image *image)
{
     LOG_INFO("Rotating image %p left\n", (void *)image);

     if (!image || !image->header || !image->data)
     {
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     struct bmp_image *new_image = create_image_with(image->header, image->header->height, image->header->width);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     transform(image, new_image, rotate_left_index);

     LOG_INFO("Image rotated left successfully\n");
     return new_image;
}

struct bmp_image *scale(const struct bmp_image *image, float factor)
{
     LOG_INFO("Scaling image %p by factor %f\n", (void *)image, factor);

     if (factor <= 0 || !image || !image->header || !image->data)
     {
          LOG_WARNING("Factor is less than or equal to 0 or image or header or data is NULL\n");
          return NULL;
     }

     uint32_t new_width = (uint32_t)round((float)image->header->width * factor);
     uint32_t new_height = (uint32_t)round((float)image->header->height * factor);

     struct bmp_image *new_image = create_image_with(image->header, new_width, new_height);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     for (size_t new_y = 0; new_y < new_height; new_y++)
     {
          for (size_t new_x = 0; new_x < new_width; new_x++)
          {
               size_t original_x = (size_t)round((float)new_x / factor);
               size_t original_y = (size_t)round((float)new_y / factor);

               original_x = (original_x >= image->header->width) ? image->header->width - 1 : original_x;
               original_y = (original_y >= image->header->height) ? image->header->height - 1 : original_y;

               new_image->data[new_y * new_width + new_x] = image->data[original_y * image->header->width + original_x];
          }
     }

     LOG_INFO("Image scaled successfully\n");
     return new_image;
}

struct bmp_image *crop(const struct bmp_image *image, const uint32_t start_y, const uint32_t start_x,
                       const uint32_t height, const uint32_t width)
{
     LOG_INFO("Cropping image %p with start_x %u, start_y %u, height %u, width %u\n", (void *)image, start_x, start_y, height, width);
     if (!image || !image->header || !image->data)
     {
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     if (start_x + width > image->header->width || start_y + height > image->header->height)
     {
          LOG_WARNING("Invalid cropping dimensions\n");
          return NULL;
     }

     struct bmp_image *new_image = create_image_with(image->header, width, height);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     for (size_t target_y = 0; target_y < height; target_y++)
     {
          for (size_t target_x = 0; target_x < width; target_x++)
          {
               size_t source_y = start_y + target_y;
               size_t source_x = start_x + target_x;

               size_t target_index = target_y * width + target_x;
               size_t source_index = source_y * image->header->width + source_x;

               new_image->data[target_index] = image->data[source_index];
          }
     }

     LOG_INFO("Image cropped successfully\n");
     return new_image;
}

struct bmp_image *extract(const struct bmp_image *image, const char *colors_to_keep)
{
     LOG_INFO("Extracting colors %s from image %p\n", colors_to_keep, (void *)image);

     if (!image || !image->header || !image->data || !colors_to_keep)
     {
          LOG_WARNING("Image or header or data or colors to keep is NULL\n");
          return NULL;
     }

     bool keep_red = strchr(colors_to_keep, 'r') != NULL;
     bool keep_green = strchr(colors_to_keep, 'g') != NULL;
     bool keep_blue = strchr(colors_to_keep, 'b') != NULL;

     if (!keep_red && !keep_green && !keep_blue)
     {
          LOG_WARNING("No colors to keep\n");
          return NULL;
     }
     uint32_t width = image->header->width, height = image->header->height;

     struct bmp_image *new_image = create_image_with(image->header, width, height);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     for (size_t y = 0; y < height; y++)
     {
          for (size_t x = 0; x < width; x++)
          {
               size_t index = y * width + x;
               new_image->data[index].red = keep_red ? image->data[index].red : 0;
               new_image->data[index].green = keep_green ? image->data[index].green : 0;
               new_image->data[index].blue = keep_blue ? image->data[index].blue : 0;
          }
     }

     LOG_INFO("Colors extracted successfully\n");
     return new_image;
}

struct bmp_image *brightness(const struct bmp_image *image, float factor)
{
     LOG_INFO("Changing brightness of image %p by factor %f\n", (void *)image, factor);

     if (!image || !image->header || !image->data)
     {
          LOG_WARNING("Image or header or data is NULL\n");
          return NULL;
     }

     if (factor < 0)
     {
          LOG_WARNING("Factor is less than 0.\n");
          return NULL;
     }

     struct bmp_image *new_image = create_image_with(image->header, image->header->width, image->header->height);

     if (!new_image)
     {
          LOG_ERROR("Failed to initialize new image\n");
          return NULL;
     }

     for (size_t y = 0; y < image->header->height; y++)
     {
          for (size_t x = 0; x < image->header->width; x++)
          {
               size_t index = y * image->header->width + x;
               new_image->data[index].red = (uint8_t)round((float)image->data[index].red * factor);
               new_image->data[index].green = (uint8_t)round((float)image->data[index].green * factor);
               new_image->data[index].blue = (uint8_t)round((float)image->data[index].blue * factor);
          }
     }

     LOG_INFO("Brightness changed successfully\n");
     return new_image;
}
