#include "stdafx.h"
#include "ImageMaker.h"

ImageMaker::ImageMaker(UINT width, UINT height)
	: width(width), height(height)
{
	data.resize((size_t)(width * height));
}

ImageMaker::~ImageMaker()
{
}

void ImageMaker::Set(UINT x, UINT y, RGBQUAD color)
{
	if (!InBounds(x,y))
	{
		return;
	}

	data[y * width + x] = color;
}

void ImageMaker::SaveBMP(wstring path)
{
	BITMAPFILEHEADER bitmapfile;
	BITMAPINFO bitmapinfo;

	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = width;
	bitmapinfo.bmiHeader.biHeight = height;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biSizeImage = height * width * (bitmapinfo.bmiHeader.biBitCount / 8);
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;

	bitmapfile.bfType = 0x4d42;
	bitmapfile.bfSize = (((width * bitmapinfo.bmiHeader.biBitCount + 31) & ~31) / 8 * height) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bitmapfile.bfReserved1 = 0;
	bitmapfile.bfReserved2 = 0;
	bitmapfile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD);

	bitmapinfo.bmiColors->rgbBlue = 0;
	bitmapinfo.bmiColors->rgbGreen = 0;
	bitmapinfo.bmiColors->rgbRed = 0;
	bitmapinfo.bmiColors->rgbReserved = 0;

	string strPath = String::WStringToString(path);
	FILE * fp;
	fopen_s(&fp, strPath.c_str(), "wb");

	fwrite(&bitmapfile, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bitmapinfo, sizeof(BITMAPINFO), 1, fp);
	fwrite((void*)&data[0], sizeof(RGBQUAD) * data.size(), 1, fp);

	fclose(fp);
}

bool ImageMaker::InBounds(UINT x, UINT y)
{
	return (0 <= y) && (y < height) && (0 <= x) && (x < width);
}

uint8_t ImageMaker::ToBYTE(double x)
{
	return x >= 1.0 ? 255 : x <= 0.0 ? 0 : (BYTE)x;
}
