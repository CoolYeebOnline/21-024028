#include<stdafx.h>
#pragma once

#ifndef  __IMAGE_LOADER_H__
#define __IMAGE_LOADER_H__

typedef enum ImageType {
	IM_BITMAP,
	IM_PCX,
	IM_PPM,

	IM_MAX
}ImageType;

class ImageLoader
{
public:
	ImageLoader() {};		//Default constructor - no real functionality
	~ImageLoader() {};		//Default destructor - no additional functionality

	//A Static function to load an image from a provided file
	//Both the returned image pixel data and the palette data will need to be freed by the caller with a delete[] operator
	/*!
		\param a_filename The filename and path of the image to load 
		\param a_width a reference that will be used to store the width in pixels of the image
		\param a_height a reference that will be used to store the height of the image in pixels
		a_bpp a refrence to store the number of bits per pixel for the image
		\param a_paletteDAta a reference pointer to the palette data that may be loaded with the image,
		for no-paletteised images pass in nullptr
		\return returns a void pointer to the image pixel data that has been extracted from the file
	*/
	static void* LoadFromFile(const char* a_filename, u32 a_imageType, u32& a_height, u8& a_bpp, void*& a_paletteData);
private:
};

#endif // ! __IMAGE_LOADER_H__


