
#include <iostream>


#include "OGL.h"




std::tuple<std::shared_ptr<Texture>, std::shared_ptr<Shader>, std::shared_ptr<Vertex<float>> > initOGL(int width, int height)
{
	//GLuint tex = createTexture(width, height);
	auto texture = std::make_shared<Texture>(width, height);


	auto shader = std::make_shared<Shader>("../Shader/quad.vs", "../Shader/quad.fs");
	shader->addAttribute("position");
	shader->addUniform("tex");

	glUniform1i(shader->uniform("tex"), texture->getTextureID());

	auto vert = std::make_shared<Vertex<float>>();

	vert->moveData(std::vector<float>({	-1.0f, -1.0f, 
										-1.0f,  1.0f,
										 1.0f,  1.0f,
										 1.0f, -1.0f}));
	vert->transmitData(shader->attribute("position"), 2, 0);
	

	return std::make_tuple(texture, shader, vert );
}


void drawOGL( std::tuple<std::shared_ptr<Texture>, std::shared_ptr<Shader>, std::shared_ptr<Vertex<float>> > data)
{	
	// Clear the colorbuffer		

	//std::cout << "Start OGL" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	std::get<1>(data)->Use();
	

	std::get<0>(data)->bindTexture();

	//std::cout << "Start DRAW" << std::endl;
	std::get<2>(data)->draw(GL_TRIANGLE_FAN, 4);	
	//std::cout << "End DRAW" << std::endl;

	glFlush();	
	glFinish();

	//std::cout << "End OGL" << std::endl;
}