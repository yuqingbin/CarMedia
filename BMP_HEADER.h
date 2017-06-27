#ifndef BMP_HEADER_H
#define BMP_HEADER_H
class BMP_HEADER
{
        public:
     long unsigned int biClrUsed;
     int biXPelsPerMeter;
     int biYPelsPerMeter;
     int biSizeImage;
     int biCompression;
     long unsigned int biBitCount;
     int biPlanes;
     long unsigned int biHeight;
     long unsigned int biWidth;
     long unsigned int biSize;
     long unsigned int bfOffBits;
     int bfReserved2;
     int bfReserved1;
     long unsigned int bfSize;
     int biClrImportant;
     char bfTypeC1;
      char bfTypeC2;
};
#endif
