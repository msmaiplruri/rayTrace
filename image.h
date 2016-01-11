#pragma once
#include <png.h>
#include <cstdio>

int write_png(png_bytepp& image, const char* PNG_FILE_NAME, const int WIDTH, const int HEIGHT)
{
    using namespace std;

    auto fp = fopen(PNG_FILE_NAME, "wb");
    if (!fp) {
        return 1;
    }

    auto png = png_create_write_struct(
            PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png) {
        fclose(fp);
        return 1;
    }

    auto info_png = png_create_info_struct(png);
    if (!info_png) {
        fclose(fp);
        png_destroy_write_struct(&png, 0);
        return 1;
    }

    if (setjmp(png_jmpbuf(png))) {
        fclose(fp);
        png_destroy_write_struct(&png, &info_png);
        return 1;
    }

    png_init_io(png, fp);

    // init info
    png_set_IHDR(png, info_png,
            WIDTH, HEIGHT, 8,
            PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);

    // write image
    png_write_info(png, info_png);
    png_write_image(png, image);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info_png);

    fclose(fp);
    return 0;
}
