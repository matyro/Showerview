
#include "file_render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
// for Linux platform, plz make sure the size of data type is correct for BMP spec.
// if you use this on Windows or other platforms, plz pay attention to this.
typedef int LONG;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;

// __attribute__((packed)) on non-Intel arch may cause some unexpected error, plz be informed.

typedef struct BITMAPFILEHEADER
{
	WORD    bfType; // 2  /* Magic identifier */
	DWORD   bfSize; // 4  /* File size in bytes */
	WORD    bfReserved1; // 2
	WORD    bfReserved2; // 2
	DWORD   bfOffBits; // 4 /* Offset to image data, bytes */ 
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER
{
	DWORD    biSize; // 4 /* Header size in bytes */
	LONG     biWidth; // 4 /* Width of image */
	LONG     biHeight; // 4 /* Height of image */
	WORD     biPlanes; // 2 /* Number of colour planes */
	WORD     biBitCount; // 2 /* Bits per pixel */
	DWORD    biCompression; // 4 /* Compression type */
	DWORD    biSizeImage; // 4 /* Image size in bytes */
	LONG     biXPelsPerMeter; // 4
	LONG     biYPelsPerMeter; // 4 /* Pixels per meter */
	DWORD    biClrUsed; // 4 /* Number of colours */ 
	DWORD    biClrImportant; // 4 /* Important colours */ 
} __attribute__((packed)) BITMAPINFOHEADER;

/*
typedef struct tagRGBQUAD
{
unsigned char    rgbBlue;
unsigned char    rgbGreen;
unsigned char    rgbRed;
unsigned char    rgbReserved;
} RGBQUAD;
* for biBitCount is 16/24/32, it may be useless
*/

#endif



opencl_file::opencl_file(const int width, const int height) 
	:m_width(width), m_height(height)
{
	m_frameCounter = 0;

	for (int i = 0; i < width*height * 4; i++)
	{
		m_image.push_back(0.0f);
	}
}


void opencl_file::init(std::vector<Line>& lines)
{
	m_ocl = std::make_shared<OpenCL>("../Shader/output_kernel.cl");
	std::cout << "Init Kernel" << std::endl;

	m_ocl->loadKernelFunction("transform_perspective");

	m_ocl->addBuffer(0, lines, 0);
	m_ocl->setArg(0, 1, static_cast<unsigned int>(lines.size()));

	m_ocl->writeData(0);



	m_ocl->loadKernelFunction("writeTexture");

	m_ocl->addBuffer(1, m_image, 0);
	m_ocl->setArg(1, 1, m_ocl->getKernel(0).getBuffer(0));
	m_ocl->setArg(1, 2, static_cast<unsigned int>(lines.size()));
}

void opencl_file::update(std::vector<Line>& lines)
{
	m_ocl->replaceBuffer(0, 0, lines, 0);
	m_ocl->setArg(0, 1, static_cast<unsigned int>(lines.size()));

	m_ocl->setArg(1, 1, m_ocl->getKernel(0).getBuffer(0));
	m_ocl->setArg(1, 2, static_cast<unsigned int>(lines.size()));
}

void opencl_file::draw(const int lineCount, cl_float16 view, cl_float16 projection)
{
	//std::cout << "Start OCL: " << frameCount << std::endl;

	cl_int error = 0;

	for (int i = 0; i < m_image.size(); i++)
	{
		m_image[i] = 0.0f;
	}

	int lineCount_rounded = static_cast<int>((static_cast<float>(lineCount) / 256.0) + 1.0) * 256;

	m_ocl->setArg(0, 2, view);
	m_ocl->setArg(0, 3, projection);

	m_ocl->writeData(0);

	m_ocl->deploy(0, cl::NDRange(0), cl::NDRange(lineCount_rounded), cl::NDRange(256));


	//kernel2.setArg(3, cam.plane());	

	//std::cout << "Init" << std::endl;


	//std::cout << "Kernel Start" << std::endl;
	m_ocl->deploy(1, cl::NDRange(0, 0), cl::NDRange(m_width, m_height), cl::NDRange(16, 16));

	m_ocl->readData(1, 0);

	m_ocl->finish();

	
	std::stringstream sstr;
	sstr << m_frameCounter;
	this->writeBitmap("image_" + sstr.str() + ".bmp");

	m_frameCounter++;
}



void opencl_file::writeBitmap(std::string filename)
{
	BITMAPINFOHEADER bmpInfoHeader = { 0 };

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;

	bmpInfoHeader.biCompression = 0L; //BI_RGB;

	bmpInfoHeader.biHeight = m_width;
	bmpInfoHeader.biWidth = m_height;

	bmpInfoHeader.biPlanes = 1;

	bmpInfoHeader.biSizeImage = bmpInfoHeader.biWidth* bmpInfoHeader.biHeight * (bmpInfoHeader.biBitCount / 8);

	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfType = 'B' + ('M' << 8);
	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;


	std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	file.write(reinterpret_cast<const char*>(&bfh), sizeof(bfh));
	file.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			unsigned char coli[4];
			float col = 0;

			col = m_image[4*(x + y*m_width) + 0] * 255.0;
			//std::cout << "Color in " << x << "," << y << " -> " << col;
			coli[0] = static_cast<unsigned char>(col);
			col = m_image[4 * (x + y*m_width) + 1] * 255.0;
			//std::cout << "," << col;
			coli[1] = static_cast<unsigned char>(col);
			col = m_image[4 * (x + y*m_width) + 2] * 255.0;
			//std::cout << "," << col << std::endl;
			coli[2] = static_cast<unsigned char>(col);

			file.write((char*)coli, 3);

		}
	}
	

	file.close();

}
