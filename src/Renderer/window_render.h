#pragma once


#include<memory>

#include "OpenCL.h"

#include "Renderer/render.h"

class opencl_window //: render
{
private:
	std::shared_ptr<OpenCL> m_ocl;

	double m_frameCounter;
	const int m_width;
	const int m_height;

public:
	opencl_window(const int width, const int height);

	void init(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines);

	void draw(const int lineCount, cl_float16 view, cl_float16 projection);

};
