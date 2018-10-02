#pragma once

class ImageMaker
{
public:
	ImageMaker(UINT width, UINT height);
	~ImageMaker();

	void Set(UINT x, UINT y, RGBQUAD color);
	void SaveBMP(wstring path);
	uint8_t ToBYTE(double x);

private:
	bool InBounds(UINT x, UINT y);
	

	template<class Type>
	void Clamp(const Type& x, const Type& min, const Type& max)
	{
		return (x < min) ? min : ((max < x) ? max : x);
	}

	struct BMPHeader
	{
		std::uint16_t bfType;
		std::uint32_t bfSize;
		std::uint16_t bfReserved1;
		std::uint16_t bfReserved2;
		std::uint32_t bfOffBits;
		std::uint32_t biSize;
		std::int32_t  biWidth;
		std::int32_t  biHeight;
		std::uint16_t biPlanes;
		std::uint16_t biBitCount;
		std::uint32_t biCompression;
		std::uint32_t biSizeImage;
		std::int32_t  biXPelsPerMeter;
		std::int32_t  biYPelsPerMeter;
		std::uint32_t biClrUsed;
		std::uint32_t biClrImportant;
	};


private:
	UINT width, height;
	vector<RGBQUAD> data;
};