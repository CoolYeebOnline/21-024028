#include <stdafx.h>
#include "rc_PCXLoader.h"


int PCX_getEncodedByte(u8& a_value, u8& a_frequency, std::fstream* a_stream) {
	if (a_stream-> peek() == EOF) {
		return EOF;
	}
	a_frequency = 1;
	a_stream->read((char*)(&a_value), 1);
	if ((a_value & PCX_RLE_MASK) == PCX_RLE_MASK) {
		a_frequency = a_value & PCX_RLE_FREQ_MASK;
		if (a_stream->peek() == EOF) {
			return EOF;
		}
		a_stream->read((char*)(&a_value), 1);
	}
	return 0;
}
//! A function for loading a PCX image, returns pointer to raw image data.
/*!
	\param a_stream an constant void pointer to the filestream to read from.
	\param a_width an integer reference for the width in pixels of the image the load function will populate this value
	\param a_height an integer reference for the width in pixels of the image the load fucntion will populate this value
	\param a_bitsPerPixel an integer reference for the number of bits per pixel
	\param a_imagePalette pointer to image palette data stored in RGB format, for non palettised images pass nullptr as parameter
	\return The raw image data
*/
void* PCXLoader::LoadFromFile(std::fstream* a_stream, u32& a_w, u32& a_h, u8& a_bpp, void*& a_palette) {
	std::fstream* file = a_stream; //get pointer to file stream
	PCXHeader header;
	//read PCX header (read 128 bytes of the file)
	file->read((char*)(&header), sizeof(PCXHeader));
	//validate that this is a PCX header by testing the identuifer and encoding bytes
	if (header.identifier != PCX_VALID_HEADER || header.encoding != PCX_RLE_ENCODING) {
		file->close();
		return nullptr;
	}
	//this is a valid PCX file continue to load

	//get palette information if present
	//PCX version 3.0 and on there is a possible 256 colour palette 769 bytes from end of the file
	if (header.version == 3) {
		//Version 3 of the PCX file format contained no palette information and made use of the default palette
		u8* egaPalette = (u8*)(header.colourpalette);
		for (int i = 0; i < 48; ++i) {
			egaPalette[i] = PCX_defaultPalette[i];
		}
	}
	if (header.bitsPerPixel == 8) {
		//if there are less than 8 bits per pixel then the file contains n oadditional palette information otherwise read in VGA palette data
		file->seekg(PCX_VGA_PALETTE_OFFSET, std::ios_base::end);
		char paletteIndicator = 0;
		file->read(&paletteIndicator, 1);
		if (paletteIndicator == 0xc) {
			//we have a palette at the end of the file proceed to read palette data from file
			a_palette = new PCXHeader::PCXPaletteColour[256];
			file->read(&paletteIndicator, 1);
			file->read((char*)(a_palette), 256 * sizeof(PCXHeader::PCXPaletteColour));
		}
		file->clear();
		file->seekg(0, std::ios_base::beg);
		file->seekg(sizeof(PCXHeader), std::ios_base::beg);			//seek from the beginning of the file to the end of the PCX header
	}
	//if we get here and a_palette is still null then allocate memory for 16 colour palette
	if (!a_palette && (header.numColourPlanes * header.bitsPerPixel) < 24) {
		a_palette = new PCXHeader::PCXPaletteColour[16]; //allocate enough memory for a 16 colour palette (malloc(48));
		memcpy(a_palette, header.colourpalette, 48); //copy the palette data into the palette buffer
	}
	//get the pixel size of the image
	a_w = header.dimensions.right - header.dimensions.left + 1;
	a_h = header.dimensions.bottom - header.dimensions.top + 1;
	a_bpp = header.bitsPerPixel * header.numColourPlanes;
	//size of the decompressed image in bytes
	u32 bytesInRow = (u32)(a_w * (float)(a_bpp / 8.f));
	u32 decompImageSize = a_h * bytesInRow;
	//The way we will process the image data is to decompress on image scanline at a time.
	//number of bytes in a decompressed scanline (when colour planes greater than 1 bytes per scanline give split between R/G/B values)
	u32 decompScanLine = header.bytesPerScanline * header.numColourPlanes;
	//PCX images may contain some line padding - calculate lien padding
	u32 scanlinePadding = decompScanLine - bytesInRow;
	u32 actualBytesPerImageRow = decompScanLine - scanlinePadding;
	//create a data buffer large enough to hold the decompressed image
	u8* imageData = new u8[decompImageSize];
	memset(imageData, 0, decompImageSize);
	u8* scanlineData = new u8[decompScanLine];
	memset(scanlineData, 0, decompScanLine);

	//create some stack variables to hold the value and frequency for the data read from file
	u8 value = 0; //The current pixel value to be decoded 
	u8 frequency = 0; //the frequency/occurances of this pixel value
	//while all of the image data has not been extracted 
	u32 bytesProcessed = 0;
	std::streamsize streamLocation;
	u32 row = 0;
	while (row < a_h - 1) {
		streamLocation = file->tellg();
		//for each row of the image decode the compressed image data 
		for (u8* slp = scanlineData; slp < (scanlineData + decompScanLine);) {
			//test for premature end of file
			if (EOF == PCX_getEncodedByte(value, frequency, file)) {
				//if file ends suddenly release and null data
				delete[] imageData;
				imageData = nullptr;
				if (!a_palette) {
					delete[] a_palette;
					a_palette = nullptr;
				}
				//for the number of runs insert the value into our image data
				for (u8 i = 0; i < frequency; ++i) {
					*slp++ = value;
				}
			}
			++row;
			//completing the above for loop gives us one scanline of data decompressed to copy into our Image buffer
		}
		//now copy based off number of colour planes
		if (header.numColourPlanes != 1) {
			//scan line is broken down into rrrrr... ggggg... bbbb... (aaaa)...
			//need to iterate through this image and copy across data to appropriate rgb channels
			u8* red = scanlineData;
			u8* green = scanlineData + header.bytesPerScanline;
			u8* blue = scanlineData + (header.bytesPerScanline * 2);
			u8* alpha = header.numColourPlanes == 4 ? scanlineData + (header.bytesPerScanline * 3) : nullptr;

			for(u32 processedBytes = bytesProcessed; processedBytes < (bytesProcessed + actua
				))
		}
	}

}
