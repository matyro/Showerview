/*
 * LineRender.cpp
 *
 *  Created on: 03.09.2015
 *      Author: dominik
 */

#include "SingleLineRenderUnit.h"

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>

namespace render
{
	LineRender::LineRender()
	{
		this->shader = nullptr;
		init();
	}

	LineRender::~LineRender()
	{
		if (this->shader != nullptr)
			delete this->shader;

		glDeleteVertexArrays(1, &this->vertexArrayObject);
	}

	void LineRender::init()
	{
		shader = new Shader("../../Shader/line.vs", "../../Shader/line.frag");

		ModelMatrix = glm::mat4(1.0f);

		GLint inPosLocation = shader->addAttribute("in_Pos");
		GLint inColLocation = shader->addAttribute("in_Col");

		std::cout << inPosLocation << ", " << inColLocation << std::endl;

		// Set up a vertex buffer object to hold the vertex position data
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float) + 4 * 8 * sizeof(float), NULL, GL_STATIC_DRAW);

		GLuint VaoID;
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		glEnableVertexAttribArray(inPosLocation);
		// Set up the vertex attribute pointer for the vVertex attribute
		glVertexAttribPointer(inPosLocation,  		// Attribute location
				3,                    	// Number of elements per vertex, here (x,y,z), so 3
				GL_FLOAT,               // Data type of each element
				GL_FALSE,               // Normalised?
				7 * sizeof(float),                      // Stride
				0                       // Offset
				);

		glEnableVertexAttribArray(inColLocation);
		glVertexAttribPointer(inColLocation, 4,
		GL_FLOAT,
		GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));

		glBindVertexArray(0);
	}

	void LineRender::activateContext()
	{
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader->Use();
	}

	void LineRender::deactivateContext()
	{
		//other drawing code...
		glDisable(GL_BLEND); //restore blending options

		glDisable(GL_DEPTH_TEST);
	}

	void LineRender::line(double x1, double y1, double z1, double x2, double y2, double z2, //coordinates of the line
			float w,                            //width/thickness of the line in pixel
			float Cr, float Cg, float Cb,    //RGB color components
			float alpha    					// alpha channel
			)
	{

		/* Local frame: z->Length Axis, x->Width axis, y->Depth

		 2____4__(1)__6____8
		 |\   |\      |\   |
		 ||   | \     ||   |
		 | \  |  \    | \  | y
		 | |  |   \   | |  |	 __	z
		 |  \ |    \  |  \ |   /|
		 |  | |     \ |  | |  /
		 |___\| _____\|___\| /
		 1    3  (2)  5    7
		 x
		 */
		float xMid = (x2 + x1) / 2.0;
		float yMid = (y2 + y1) / 2.0;

		float w2 = w / 2.0;
		float fade = w2 / 4.0;

		//Modellspace
		float para_vertex[8 * 3 * sizeof(float) + 8 * 4 * sizeof(float)] =
		{ x2 - w2 - fade, y2, z2,		//1 Vert
				Cr, Cg, Cb, 0,   			//1 Col

				x1 - w2 - fade, y1, z1,		//2
				Cr, Cg, Cb, 0,			//2

				x2 - w2, y2, z2,		//3
				Cr, Cg, Cb, alpha,    	//3

				x1 - w2, y1, z1,		//4
				Cr, Cg, Cb, alpha,		//4

				x2 + w2, y2, z2,		//5
				Cr, Cg, Cb, alpha,    	//5

				x1 + w2, y1, z1,		//6
				Cr, Cg, Cb, alpha,		//6

				x2 + w2 + fade, y2, z2,		//7
				Cr, Cg, Cb, 0,			//7

				x1 + w2 + fade, y1, z1,		//8
				Cr, Cg, Cb, 0				//8
				};

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float) + 8 * 4 * sizeof(float), para_vertex, GL_STATIC_DRAW);
	}

	void LineRender::draw(glm::mat4 camMatrix)
	{
		activateContext();

		glm::mat4 MVP = camMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around

		shader->Use();
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(this->vertexArrayObject);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

		deactivateContext();
	}
}
