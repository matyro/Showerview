
#include "Renderer/file_render.h"



std::shared_ptr<OpenCL> initOCL(std::vector<Line>& lines, std::vector<float>& pixel)
{
	auto OCL = std::make_shared<OpenCL>("../Shader/output_kernel.cl");
	std::cout << "Init Kernel" << std::endl;

	OCL->loadKernelFunction("transform_perspective");

	OCL->addBuffer(0, lines, 0);
	OCL->setArg(0, 1, static_cast<unsigned int>(lines.size()));

	OCL->writeData(0);

	

	OCL->loadKernelFunction("writeTexture");
	
	OCL->addBuffer(1, pixel , 0);	
	OCL->setArg(1, 1, OCL->getKernel(0).getBuffer(0) );
	OCL->setArg(1, 2, static_cast<unsigned int>(lines.size()));

	
	return std::move(OCL);
}


void calcOCL(std::shared_ptr<OpenCL> OCL, const float frameCount, const Camera& cam, const int lineCount, const int width, const int height, cl_float16 view, cl_float16 projection)
{
	//std::cout << "Start OCL: " << frameCount << std::endl;

	cl_int error = 0;


	int lineCount_rounded = static_cast<int>( (static_cast<float>(lineCount) / 256.0) + 1.0) * 256;

	OCL->setArg(0, 2, view);
	OCL->setArg(0, 3, projection);

	OCL->writeData(0);

	OCL->deploy(0, cl::NDRange(0), cl::NDRange(lineCount_rounded), cl::NDRange(256));
	

	//kernel2.setArg(3, cam.plane());	

	//std::cout << "Init" << std::endl;
	

	//std::cout << "Kernel Start" << std::endl;
	OCL->deploy(1, cl::NDRange(0,0), cl::NDRange(width, height), cl::NDRange(16,16));
	
	OCL->readData(1, 0);

	OCL->finish();
}


void writeToFile(std::string filename, std::vector<float>& imageData)
{
	BITMAPINFOHEADER bmpInfoHeader = { 0 };

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;

	bmpInfoHeader.biCompression = BI_RGB;

	bmpInfoHeader.biHeight = 1024;
	bmpInfoHeader.biWidth = 976;

	bmpInfoHeader.biPlanes = 1;

	bmpInfoHeader.biSizeImage = bmpInfoHeader.biWidth* bmpInfoHeader.biHeight * (bmpInfoHeader.biBitCount / 8);

	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfType = 'B' + ('M' << 8);	
	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	
	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
	

	std::ofstream file(filename, std::ios::out | std::ios::binary);

	file.write(reinterpret_cast<const char*>(&bfh), sizeof(bfh));
	file.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

	for (float itr : imageData)
	{
		uint8_t col = itr * 255;
		file.write(reinterpret_cast<const char*>(&col), 1);
	}
	
	file.close();
	
}