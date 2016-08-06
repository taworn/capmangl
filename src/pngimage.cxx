/**
 * @file pngimage.cxx
 * @desc PNG image module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <png.h>
#include "pngimage.hxx"

PNGImage::~PNGImage()
{
	if (rowPointers)
		free(rowPointers);
	if (image)
		free(image);
}

PNGImage::PNGImage()
	: width(0), height(0), bitDepth(0), colorType(0)
	, imageSize(0), rowPointersSize(0), image(NULL), rowPointers(NULL)
{
}

PNGImage::PNGImage(const char *fileName)
	: width(0), height(0), bitDepth(0), colorType(0)
	, imageSize(0), rowPointersSize(0), image(NULL), rowPointers(NULL)
{
	init(fileName);
}

bool PNGImage::init(const char *fileName)
{
	// This function was originally written by David Grayson for
	// https://github.com/DavidEGrayson/ahrs-visualizer
	FILE *fp = fopen(fileName, "rb");
	if (fp == 0) {
		BOOST_LOG_TRIVIAL(error) << "File cannot open.";
		return false;
	}

	// read the header
	png_byte header[8] = { 0 };
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8)) {
		BOOST_LOG_TRIVIAL(error) << "File is not PNG image.";
		fclose(fp);
		return false;
	}

	// create png info struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		BOOST_LOG_TRIVIAL(error) << "png_create_info_struct() returned NULL.";
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return false;
	}

	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		BOOST_LOG_TRIVIAL(error) << "png_create_info_struct() returned NULL.";
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return false;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr)) != 0) {
		BOOST_LOG_TRIVIAL(error) << "libpng setjmp() error.";
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	// init png reading
	png_init_io(png_ptr, fp);

	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	// variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type, NULL, NULL, NULL);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes - 1) % 4);

	// Allocate the image_data as a big block, to be given to opengl
	size_t image_data_size = rowbytes * temp_height * sizeof(png_byte) + 15;
	png_byte *image_data = (png_byte*)malloc(image_data_size);
	if (image_data == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Not enough memory.";
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	size_t row_pointers_size = temp_height * sizeof(png_byte*);
	png_byte **row_pointers = (png_byte**)malloc(row_pointers_size);
	if (row_pointers == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Not enough memory for PNG row pointers.";
		free(image_data);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned int i = 0; i < temp_height; i++) {
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	// deletes old image
	if (this->rowPointers)
		free(this->rowPointers);
	if (this->image)
		free(this->image);

	// copied new image
	this->width = png_get_image_width(png_ptr, info_ptr);
	this->height = png_get_image_height(png_ptr, info_ptr);
	this->bitDepth = png_get_bit_depth(png_ptr, info_ptr);
	this->colorType = png_get_color_type(png_ptr, info_ptr);
	this->imageSize = image_data_size;
	this->rowPointersSize = row_pointers_size;
	this->image = image_data;
	this->rowPointers = row_pointers;

	// closes resources 
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fp);

	return true;
}

