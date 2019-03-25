#include <utils/ImageImporter.h>

namespace mathernogl {

//returned image data needs to be freed
ImageData* loadImage(const std::string &filePath) {
	FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;
	imageFormat = FreeImage_GetFileType(filePath.c_str(), 0);
	if (imageFormat == FIF_UNKNOWN)
		imageFormat = FreeImage_GetFIFFromFilename(filePath.c_str());
	if (imageFormat == FIF_UNKNOWN)
		throw std::runtime_error("Unable to read image file, " + filePath + ". ");

	byte* byteMap;
	unsigned int imageWidth = 0;
	unsigned int imageHeight = 0;
	unsigned int bytesPerPixel = 0;
	if (FreeImage_FIFSupportsReading(imageFormat)) {
		FIBITMAP* bitmap = FreeImage_Load(imageFormat, filePath.c_str());
		if (!bitmap){
			throw std::runtime_error("Unable to load image file, " + filePath + ". ");
		}

		BYTE* fiPixels = FreeImage_GetBits(bitmap);
		imageWidth = FreeImage_GetWidth(bitmap);
		imageHeight = FreeImage_GetHeight(bitmap);
		bytesPerPixel = FreeImage_GetBPP(bitmap);
		if (bytesPerPixel % 8 != 0){
			throw std::runtime_error("Image importer hasn't worked as expected, pixels seem to be a number of bits that aren't divisible into bytes. " + filePath + ". ");
		}
		bytesPerPixel /= 8;

		//memory created by free image will be freed
		unsigned int numBytes = imageWidth * imageHeight * bytesPerPixel;
		byteMap = new byte[numBytes];
		std::memcpy(byteMap, fiPixels, numBytes);
		FreeImage_Unload(bitmap);
        return new ImageData(byteMap, imageWidth, imageHeight, bytesPerPixel);
	}

    return nullptr;
}


void saveImage(const std::string &filePath, const ImageData *imageData) {
    FREE_IMAGE_FORMAT fiFormat = FreeImage_GetFileType(filePath.c_str(), 0);
    if (fiFormat == FIF_UNKNOWN)
        fiFormat = FreeImage_GetFIFFromFilename(filePath.c_str());

    if(FreeImage_FIFSupportsWriting(fiFormat)){
        FIBITMAP* fiBitmap = FreeImage_Allocate(imageData->width, imageData->height, imageData->bytesPerPixel * 8);
        BYTE* fiPixels = FreeImage_GetBits(fiBitmap);
        std::memcpy(fiPixels, imageData->byteMap, imageData->width * imageData->height * imageData->bytesPerPixel);
        FreeImage_Save(fiFormat, fiBitmap, filePath.c_str());
        FreeImage_Unload(fiBitmap);
    }
    else
        throw std::runtime_error("Unable to save image file, unsupported format, " + filePath + ". ");

}

void flipImageBothWays(byte* image, unsigned int width, unsigned int height, unsigned int bpp) {
	byte* flippedImage = new byte[width * height * bpp];

	int numPixels = width * height;
	for(int pixelNum = 0; pixelNum < numPixels; pixelNum++){
		for(int byteNum = 0; byteNum < bpp; byteNum++){
			flippedImage[(numPixels-pixelNum-1)*bpp + byteNum] = image[pixelNum*bpp + byteNum];
		}
	}

	for(int byteNum = 0; byteNum < numPixels*bpp; byteNum++){
		image[byteNum] = flippedImage[byteNum];
	}
}

void flipImageVertically(byte* image, unsigned int width, unsigned int height, unsigned int bpp) {
	byte* flippedImage = new byte[width * height * bpp];

	for(int rowNum = 0; rowNum < height; rowNum++){
		for(int byteNum = 0; byteNum < bpp*width; byteNum++){
			flippedImage[(height-rowNum-1)*bpp*width + byteNum] = image[rowNum*bpp*width + byteNum];
		}
	}

	int numPixels = width * height;
	for(int byteNum = 0; byteNum < numPixels*bpp; byteNum++){
		image[byteNum] = flippedImage[byteNum];
	}
}

void flipImageHorizontally(byte* image, unsigned int width, unsigned int height, unsigned int bpp) {
	flipImageBothWays(image, width, height, bpp);
	flipImageVertically(image, width, height, bpp);
}


}
