#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "bmp.h"
#include "log.h"


struct bmp_header *read_bmp_header(FILE *stream)
{
   LOG_INFO("Reading BMP header with stream %p\n", (void *)stream);

   if (stream == NULL){
      LOG_WARNING("Stream is NULL\n");
      return NULL;
   }

   struct bmp_header *header = (struct bmp_header *)malloc(sizeof(struct bmp_header));
   if (header == NULL){
      LOG_ERROR("Memory allocation failed\n");
      return NULL;
   }

   if (fseek(stream, 0, SEEK_SET) != 0)
   {
      LOG_ERROR("Failed to seek to the beginning of the file\n");
      free(header);
      return NULL;
   }

   if (fread(header, sizeof(struct bmp_header), 1, stream) != 1)
   {
      LOG_ERROR("Failed to read the BMP header\n");
      free(header);
      return NULL;
   }

   if (header->type != 0x4D42)
   {
      LOG_ERROR("This is not a BMP file\n");
      free(header);
      return NULL;
   }

   LOG_INFO("BMP header read successfully\n");
   return header;
}

struct pixel *read_data(FILE *stream, const struct bmp_header *header)
{
   LOG_INFO("Reading BMP data with stream %p and header %p\n", (void *)stream, (void *)header);

   if (!stream || !header ){
      LOG_WARNING("Stream or header is NULL\n");
      return NULL;
   }

   if (fseek(stream, header->offset, SEEK_SET) != 0){
      LOG_ERROR("Failed to seek to the beginning of the data\n");
      return NULL;
   }

   uint32_t width = header->width,height = header->height;

   size_t bytes_per_pixel = header->bpp/ 8;
   size_t line_size = width * bytes_per_pixel;
   int padding = (4 - (line_size) % 4) % 4;


   struct pixel *data = (struct pixel *)malloc(width *height * sizeof(struct pixel));
   if (!data){
      LOG_ERROR("Memory allocation failed\n");
      return NULL;
   }

 
   uint8_t *current_position = (uint8_t *)data;
    for (size_t y = 0; y < height; y++) {
        if (fread(current_position, 1, line_size, stream) != line_size) {
            LOG_ERROR("Failed to read the data\n");
            free(data);
            return NULL;
        }
        current_position += line_size;
        fseek(stream, padding, SEEK_CUR);
    }

   LOG_INFO("BMP data read successfully\n");
   return data;
}

struct bmp_image *read_bmp(FILE *stream)
{
   LOG_INFO("Reading BMP image with stream %p\n", (void *)stream);

   if(!stream) {
      LOG_WARNING("Stream is NULL\n");
      return NULL;
   }

   struct bmp_header *header = read_bmp_header(stream);

   if(!header){
      LOG_ERROR("Failed to read BMP header\n");
      return NULL;     
   } 

   struct pixel *data = read_data(stream, header);

   if(!data){
      LOG_ERROR("Failed to read BMP data\n");
      free(header);
      return NULL;
   }

   struct bmp_image *image = (struct bmp_image *)malloc(sizeof(struct bmp_image));
   if(!image){
      LOG_ERROR("Memory allocation failed\n");
      free(header);
      free(data);
      return NULL;
   }

   image->header = header;
   image->data = data;

   LOG_INFO("BMP image read successfully\n"); 
   return image;
}

bool write_bmp(FILE *stream, const struct bmp_image *image)
{
   LOG_INFO("Writing BMP image with stream %p and image %p\n", (void *)stream, (void *)image);

   if(!stream||!image || !image->data || !image->header ) {
      LOG_WARNING("Stream or image is NULL\n");
      return false;
   }

   struct bmp_header *header = image->header;

   if(fseek(stream,0,SEEK_SET) !=0) {
      LOG_ERROR("Failed to seek to the beginning of the file\n");
      return false;
   }

   if(fwrite(header,sizeof(struct bmp_header),1,stream) != 1){
      LOG_ERROR("Failed to write the BMP header\n");
      return false;
   }

   if(fseek(stream,header->offset,SEEK_SET) != 0){
      LOG_ERROR("Failed to seek to the beginning of the data\n");
      return false;
   }

   uint32_t width = header->width,height = header->height;

   size_t bytes_per_pixel = (int)header->bpp/8;
   size_t line_size = (size_t)width * (size_t)bytes_per_pixel;
   size_t padding = (4 - (line_size) % 4) % 4;

   uint8_t* current_possition = (uint8_t*)image->data;
   for (size_t i = 0; i < (size_t)height; i++)
   {
      if(fwrite(current_possition,1,line_size,stream) != line_size) {
         LOG_ERROR("Failed to write the data\n");
         return false;
      }
      current_possition += line_size;
      for (size_t j = 0; j < padding; j++)
      {
         if(fputc(0,stream) == EOF) {
            LOG_ERROR("Failed to write padding\n");
            return false;
         }
      }
   }
   
   LOG_INFO("BMP image written successfully\n");
   return true;
  
}

void free_bmp_image(struct bmp_image *image)
{
   LOG_INFO("Freeing BMP image %p\n", (void *)image);
   if(!image) {
      LOG_WARNING("Image is NULL\n");
      return;
   }

   free(image->header);
   free(image->data);
   free(image);
   LOG_INFO("BMP image freed successfully\n");
}



