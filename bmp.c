#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

struct bmp_header *read_bmp_header(FILE *stream)
{
   if (stream == NULL)
      return NULL;

   struct bmp_header *header = (struct bmp_header *)malloc(sizeof(struct bmp_header));
   if (header == NULL)
      return NULL;

   if (fseek(stream, 0, SEEK_SET) != 0)
   {
      free(header);
      return NULL;
   }

   if (fread(header, sizeof(struct bmp_header), 1, stream) != 1)
   {
      free(header);
      return NULL;
   }

   if (header->type != 0x4D42)
   {
      free(header);
      return NULL;
   }

   return header;
}

struct pixel *read_data(FILE *stream, const struct bmp_header *header)
{
   if (stream == NULL || header == NULL)
      return NULL;

   if (fseek(stream, header->offset, SEEK_SET) != 0) return NULL;

   size_t bytes_per_pixel = header->bpp/ 8;
   size_t line_size = header->width * bytes_per_pixel;
   int padding = (4 - (line_size) % 4) % 4;


   struct pixel *data = (struct pixel *)malloc(header->width * header->height * sizeof(struct pixel));
   if (data == NULL) return NULL;

 
   uint8_t *current_position = (uint8_t *)data;
    for (size_t y = 0; y < header->height; y++) {
        if (fread(current_position, 1, line_size, stream) != line_size) {
            free(data);
            return NULL;
        }
        current_position += line_size;
        fseek(stream, padding, SEEK_CUR);
    }

    return data;
}

struct bmp_image *read_bmp(FILE *stream)
{
   if(stream == NULL) return NULL;

   struct bmp_header *header = read_bmp_header(stream);

   if(header == NULL){
      fprintf(stderr, "Error: This is not a BMP file.\n");
      return NULL;     
   } 

   struct pixel *data = read_data(stream, header);

   if(data == NULL){
      fprintf(stderr, "Error: Corrupted BMP file.\n");
      free(header);
      return NULL;
   }

   struct bmp_image *image = (struct bmp_image *)malloc(sizeof(struct bmp_image));
   if(image == NULL){
      free(header);
      free(data);
      return NULL;
   }

   image->header = header;
   image->data = data;

   return image;
}

bool write_bmp(FILE *stream, const struct bmp_image *image)
{
   if(stream==NULL||image==NULL || image->data==NULL || image->header==NULL ) return false;

   if(fseek(stream,0,SEEK_SET) !=0) return false;

   if(fwrite(image->header,sizeof(struct bmp_header),1,stream) != 1) return false;

   if(fseek(stream,image->header->offset,SEEK_SET) != 0) return false;

   size_t bytes_per_pixel = (int)image->header->bpp/8;
   size_t line_size = (size_t)image->header->width * (size_t)bytes_per_pixel;
   size_t padding = (4 - (line_size) % 4) % 4;

   uint8_t* current_possition = (uint8_t*)image->data;
   for (size_t i = 0; i < (size_t)image->header->height; i++)
   {
      if(fwrite(current_possition,1,line_size,stream) != line_size) return false;
      current_possition += line_size;
      for (size_t j = 0; j < padding; j++)
      {
         if(fputc(0,stream) == EOF) return false;
      }
   }
   
   return true;
  
}

void free_bmp_image(struct bmp_image *image)
{
   if(image == NULL) return;

   free(image->header);
   free(image->data);
   free(image);
}



