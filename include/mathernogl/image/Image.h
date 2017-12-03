 #pragma once
//
// Created by matt on 16/06/17.
//

#include <FreeImage.h>
#include <string>
#include <vector>
#include <memory>
#include <mathernogl/maths/Maths.h>

namespace mathernogl {

class Image {
public:
    typedef std::vector<Vector4D> ColourVector;

    /*
     * creates an image with the given parameters
     */
    static std::unique_ptr<Image> create(const uint width, const uint height, const uint bpp);

    /*
     * loads an image from the given file path
     */
    static std::unique_ptr<Image> load(const std::string &filePath);

    /*
    * saves an image at the given file path
    */
    static void save(const Image *image, const std::string &filePath);

private:
    FIBITMAP *fiBitmap;

public:
    Image(FIBITMAP *fiBitmap);
    virtual ~Image();

    /*
     * returns the image width in pixels
     */
    uint getWidth() const;

    /*
     * returns the image height in pixels
     */
    uint getHeight() const;

    /*
     * returns the number of bits that make up a pixel
     */
    uint getBPP() const;

    /*
     * returns the image width in bytes
     */
    uint getWidthBytes() const;

    /*
     * returns the number bytes that makes up the colour data of this image
     */
    uint getColourDataBytes() const;

    /*
     * writes the colour data from this image into the back of colourData (using emplace_back())
     * starting from the top left of the image, row wise
     * returns success state
     */
    bool readColourData(ColourVector* colourData) const;

    /*
     * writes the given colour data, starting from dataOffset, into this image as per format
     * only valid for images with a bpp of 8, 24, or 32
     * refer to readColourData for more info on the colourData format
     * doesn't write to disk, just this image object
     * returns success state
     */
    bool writeColourData(const ColourVector* colourData, uint dataOffset = 0);

    /*
     * clones this image into another image
     */
    std::unique_ptr<Image> clone() const;

    /*
     * converts this image into a different bpp format
     * only valid to convert to a bpp of 8, 24, or 32
     * returns success state
     */
    bool convert(uint bpp);
};

}
