
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

#include <GL/glew.h>



#include "Helper/Line.h"
#include "Helper/TreeLoad.h"
#include "Helper/CameraControll.h"

#include "Camera/Camera.h"




#include "Interface/GLFW_Startup.h"

#include "Renderer/file_render.h"
#include "Renderer/window_render.h"


#include "OGL.h"





int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Set Tree file as first parameter!" << std::endl;
		std::cin.get();
		exit(-1);
	}
	else
	{
		std::cout << "Set Tree file to: " << argv[1] << std::endl;
	}

	const int width = 2*2048;
	const int height = 2*2048;
	GLFW_control glfw(200, 200);
	

	// Inits

	// dist, fov, ratio
	auto cam = std::make_shared<Camera>(1.0f, 100.0f, static_cast<float>(width) / static_cast<float>(height) );
	
	SCamControll().setCamera(cam);	

	cam->moveCam(-10000.0, 0.0, -0.9);
		

	//initOCL();
	auto oglData = initOGL(width, height);
	std::get<0>(oglData)->bindTexture();


	helper::TreeLoad tree(argv[1]);
	tree.reload(1.0001, 1.0 / 1000000.0);

	LineSet lines = tree.getTree();

	//LineSet lines;

	//lines.addLine({ 0.0, 30.0, 0.0, 1.0 }, { 0.0, -30.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 0.5 });

		
	std::cout << "Init OCL: " << lines.size() << std::endl;
	opencl_file render(width, height);
	render.init(GL_TEXTURE_2D, std::get<0>(oglData)->getTextureID(), lines.vec() );

	std::cout << "Init ready, start rendering..." << std::endl;
	
	
	////////////////////////////////////////////////
	//Mainloop	
	std::chrono::high_resolution_clock::time_point time_start = std::chrono::high_resolution_clock::now();

	double time, timeCache = glfwGetTime();
	double timeCache2 = 0;
	double time2, timeCache3 = 0, timeCache4 = 0;


	unsigned long long counter = 0;

	
	while (glfw.closed())
	{		

		double phi = ( 2.0 * 3.1415926 ) * (counter / 360.0); 

		double x = 10000 * sin(phi);
		double y = 10000 * cos(phi);

		
		cam->setPosition(x, 0.8, y);

		cam->setViewDir(0.0, -phi);


		auto time_now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_sinceStart = time_now - time_start;

		SCamControll().setTime( time_sinceStart.count() );
		SCamControll().update();

		

		render.draw(lines.size(), cam->getViewMatrix(), cam->getProjectionMatrix());
		
		timeCache2 += glfwGetTime() - time2;

		drawOGL( oglData );

		
		counter++;		

		glfw.loop( time_sinceStart.count() );		

		
		//tree.reload((0.0001) + (200* 0.00000083) + (static_cast<double>(counter)*0.00000040));

		//tree.reload(10.0);
		//lines = tree.getTree();
		std::cout << lines.size() << " Line to draw!" << std::endl;
		render.update(lines.vec());

		std::cout << "Frame: " << counter << std::endl;
		if (counter > 359)
			break;
		//break;
		///std::cin.get();
	}

	//Terminate everything

	
	glfw.close();
	

	std::cout << "Press enter to terminate" << std::endl;
	std::cin.get();
	std::cin.get();

	return 0;
}

