/**
 * @file pngimage.hxx
 * @desc PNG image header.
 */
#ifndef PNGIMAGE_HXX
#define PNGIMAGE_HXX

 /**
  * A PNG image class.
  */
class PNGImage
{
public:
	/**
	 * Destructs a PNG image.
	 */
	~PNGImage();

	/**
	 * Constructs a PNG image.
	 */
	PNGImage();

	/**
	 * Constructs a PNG image with filename.
	 */
	PNGImage(const char *fileName);

	/**
	 * Initializes a PNG image with filename.
	 */
	bool init(const char *fileName);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getBitDepth() const { return bitDepth; }
	int getColorType() const { return colorType; }
	size_t getImageSize() const { return imageSize; }
	size_t getRowPointersSize() const { return rowPointersSize; }
	const void *getImage() const { return image; }
	const void *getRowPointers() const { return rowPointers; }

private:
	int width;
	int height;
	int bitDepth;
	int colorType;
	size_t imageSize;
	size_t rowPointersSize;
	void *image;
	void *rowPointers;

	PNGImage(const PNGImage&);
	PNGImage& operator=(const PNGImage&);
};

#endif // PNGIMAGE_HXX

