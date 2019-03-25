//
// Created by matt on 16/06/17.
//

#include <utils/Logging.h>
#include <cstring>
#include <Types.h>
#include "image/Image.h"

namespace mathernogl {

/*static*/ std::unique_ptr<Image> Image::create(const uint width, const uint height, const uint bpp) {
    FIBITMAP* fiBitmap = FreeImage_Allocate(width, height, bpp);
    if(fiBitmap != nullptr)
        return std::make_unique<Image>(fiBitmap);
    else
        return std::unique_ptr<Image>();
}

/*static*/ std::unique_ptr<Image> Image::load(const std::string &filePath) {
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filePath.c_str(), 0);
    if (imageFormat == FIF_UNKNOWN)
        imageFormat = FreeImage_GetFIFFromFilename(filePath.c_str());


    FIBITMAP* bitmap = nullptr;
    bool loadFailed = true;
    if (FreeImage_FIFSupportsReading(imageFormat)) {
        bitmap = FreeImage_Load(imageFormat, filePath.c_str());
        if (bitmap != nullptr)
            loadFailed = false;
    }

    if(loadFailed)
        throw std::runtime_error("Failed to load image file: " + filePath + ". ");
    else
        return std::make_unique<Image>(bitmap);
}

/*static*/ void Image::save(const Image *image, const std::string &filePath) {
    FREE_IMAGE_FORMAT fiFormat = FreeImage_GetFileType(filePath.c_str(), 0);
    if (fiFormat == FIF_UNKNOWN)
        fiFormat = FreeImage_GetFIFFromFilename(filePath.c_str());
    if (fiFormat == FIF_UNKNOWN)
        throw std::runtime_error("Unable to save image file, unsupported format, " + filePath + ". ");

    if(FreeImage_FIFSupportsWriting(fiFormat)) {
        if (!FreeImage_Save(fiFormat, image->fiBitmap, filePath.c_str()))
            throw std::runtime_error("Unable to save image file, " + filePath + ". ");
    }
}


Image::Image(FIBITMAP *fiBitmap) : fiBitmap(fiBitmap) {

}

Image::~Image() {
    FreeImage_Unload(fiBitmap);
}

uint Image::getWidth() const {
    return FreeImage_GetWidth(fiBitmap);
}

uint Image::getHeight() const {
    return FreeImage_GetHeight(fiBitmap);
}

uint Image::getBPP() const {
    return FreeImage_GetBPP(fiBitmap);
}

uint Image::getWidthBytes() const {
    return FreeImage_GetPitch(fiBitmap);
}

uint Image::getColourDataBytes() const {
    return getHeight() * getWidthBytes();
}

bool Image::readColourData(ColourVector* colourData) const {
    FIBITMAP* fiBitmap32 = FreeImage_ConvertTo32Bits(fiBitmap);
    if(fiBitmap32 != nullptr){
        uint width = FreeImage_GetWidth(fiBitmap32);
        uint height = FreeImage_GetHeight(fiBitmap32);
        RGBQUAD colour;
        for(int y = height-1; y >= 0; --y){
            for(int x = 0; x < width; ++x){
                if(FreeImage_GetPixelColor(fiBitmap32, (uint)x, (uint)y, &colour)) {
                    colourData->emplace_back((float) colour.rgbRed / 255, (float) colour.rgbGreen / 255, (float) colour.rgbBlue / 255, (float) colour.rgbReserved / 255);
                }
            }
        }
        FreeImage_Unload(fiBitmap32);
        return true;
    }
    else
        return false;
}

bool Image::writeColourData(const ColourVector* colourData, uint dataOffset /*= 0*/) {
    FIBITMAP* fiBitmapDest = nullptr;
    if(getBPP() == 24 || getBPP() == 32)
        fiBitmapDest = fiBitmap;
    else if(getBPP() == 8)
        fiBitmapDest = FreeImage_Allocate(getWidth(), getHeight(), 24);

    if(fiBitmapDest != nullptr) {
        for (uint y = 0; y < getHeight(); ++y) {
            for (uint x = 0; x < getWidth(); ++x) {
                uint index = x + y * getWidth() + dataOffset;
                if (index < colourData->size()) {
                    const Vector4D& colour = (*colourData)[index];
                    RGBQUAD fiColour;
                    fiColour.rgbRed =       (BYTE) std::min( std::max(colour.x*255, 0.0f), 255.0f );
                    fiColour.rgbGreen =     (BYTE) std::min( std::max(colour.y*255, 0.0f), 255.0f );
                    fiColour.rgbBlue =      (BYTE) std::min( std::max(colour.z*255, 0.0f), 255.0f );
                    fiColour.rgbReserved =  (BYTE) std::min( std::max(colour.w*255, 0.0f), 255.0f );
                    FreeImage_SetPixelColor(fiBitmapDest, x, getHeight() - y - 1, &fiColour);
                }
            }
        }

        if(getBPP() == 8){
            FIBITMAP* fiBitmap8 = FreeImage_ConvertTo8Bits(fiBitmapDest);
            std::memcpy(FreeImage_GetBits(fiBitmap), FreeImage_GetBits(fiBitmap8), getColourDataBytes());
            FreeImage_Unload(fiBitmap8);
            FreeImage_Unload(fiBitmapDest);
        }
        return  true;
    }
    else
        return false;
}

std::unique_ptr<Image> Image::clone() const {
    return std::make_unique<Image>(FreeImage_Clone(fiBitmap));
}

bool Image::convert(uint bpp) {
    if(getBPP() == bpp)
        return true;

    FIBITMAP* fiBitmapConvert = nullptr;
    if(bpp == 8)
        fiBitmapConvert = FreeImage_ConvertTo8Bits(fiBitmap);
    else if(bpp == 24)
        fiBitmapConvert = FreeImage_ConvertTo24Bits(fiBitmap);
    else if(bpp == 32)
        fiBitmapConvert = FreeImage_ConvertTo32Bits(fiBitmap);

    if(fiBitmap != nullptr){
        FreeImage_Unload(fiBitmap);
        fiBitmap = fiBitmapConvert;
        return true;
    }
    else
        return false;
}


}
