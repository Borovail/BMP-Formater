# BMP-Formater

#BMP graphic format
BMP (BitMap) is a simple lossless image storage format. It has been with us for more than 30 years (it was created around 1987) and nowadays it is of course already surpassed and replaced by other formats. During its existence, it has seen 5 versions and allows storing images in both compressed and uncompressed form. It was very popular and used mainly in the Windows OS, as it was created as a product of IBM and Microsoft. This is a format that was very often used, but at the same time very unpopular among users, because it was able to compress some images so much that they were larger after compression than before.

#An image file in this format consists of:

from the header , which contains metadata, such as file size, dimensions
and from image data , i.e. pixels.
Your task this time will be to create a program that will be able to work with the BMP graphic format - it will be able to load and save files of this type and perform several transformations with them. Specifically, you create

module BMPin which you load and save a file in BMP format,
a module transformationsin which you implement individual transformations with a file, and
file main.c, which, based on command line parameters, will perform appropriate operations with the file.
You can find detailed information about the format and its structure, e.g. on Wikipedia or in the series on the pages of the root.cz server . In this assignment, you will work with 24-bit color depth. This means that 3B are used to express the color of each pixel. You can find the file header defined in the header file bmp.h.

#Base64
The BMP graphics format is a binary format. Since we want to represent the binary data in an understandable way in the Arena results, it will be encoded using the Base64 format . You can create an original file from the data encoded in this way at any time using the tool base64. For example if he is streamrepresented in the Arena as follows:

FILE* stream = "Qk1WAAAAAAAAADYAAAAoAAAAAgAAAAQAAAABABgAAAAAACAAAAAjLgAAIy4AAAAAAAAAAAAA////AAAAAAD/AAAA/wAAAAD/AAAA/wAAAAAA/wAAAAA="; // base64 encoded stream
You can create a reverse file from it with the following command:

$ echo "Qk1WAAAAAAAAADYAAAAoAAAAAgAAAAQAAAABABgAAAAAACAAAAAjLgAAIy4AAAAAAAAAAAAA////AAAAAAD/AAAA/wAAAAD/AAAA/wAAAAAA/wAAAAA=" | base64 -d > file.bmp
Subsequently, you can work with the file on which the tests took place.
