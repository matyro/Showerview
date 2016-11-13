
#include "Renderer/window_render.h"




opencl_window::opencl_window(const int width, const int height)
	:m_width(width), m_height(height)
{
	m_frameCounter = 0;
}


void opencl_window::init(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines)
{
	m_ocl = std::make_shared<OpenCL>("../Shader/kernel.cl");
	std::cout << "Init Kernel" << std::endl;

	m_ocl->loadKernelFunction("transform_perspective");

	m_ocl->addBuffer(0, lines, 0);
	m_ocl->setArg(0, 1, static_cast<unsigned int>(lines.size()));

	m_ocl->writeData(0);



	m_ocl->loadKernelFunction("writeTexture");

	m_ocl->addOpenGL(1, textureFlag, textureID);
	m_ocl->setArg(1, 1, m_ocl->getKernel(0).getBuffer(0));
	m_ocl->setArg(1, 2, static_cast<unsigned int>(lines.size()));	
}


void opencl_window::draw(const int lineCount, cl_float16 view, cl_float16 projection)
{
	//std::cout << "Start OCL: " << frameCount << std::endl;

	cl_int error = 0;


	int lineCount_rounded = static_cast<int>((static_cast<float>(lineCount) / 256.0) + 1.0) * 256;

	m_ocl->setArg(0, 2, view);
	m_ocl->setArg(0, 3, projection);

	m_ocl->writeData(0);

	m_ocl->deploy(0, cl::NDRange(0), cl::NDRange(lineCount_rounded), cl::NDRange(256));


	//kernel2.setArg(3, cam.plane());	

	//std::cout << "Init" << std::endl;
	m_ocl->acquireOGL(1);

	//std::cout << "Kernel Start" << std::endl;
	m_ocl->deploy(1, cl::NDRange(0, 0), cl::NDRange(m_width, m_height), cl::NDRange(16, 16));

	m_ocl->releaseOGL(1);

	m_ocl->finish();

	m_frameCounter++;
}

