/*
 * LineRender.h
 *
 *  Created on: 03.09.2015
 *      Author: dominik
 */

#ifndef LINERENDER_H_
#define LINERENDER_H_

#include "Shader.h"

class LineRender
{
private:

	Shader* shader;

	GLuint vertexBufferObject;
	GLuint vertexArrayObject;

	static inline double GET_ABS(double x) {return x>0?x:-x;}

	void init(float width, float height);

	void activateContext();
	void deactivateContext();
public:

	LineRender(float width, float height);
	~LineRender();

	void line( double x1, double y1, double z1, double x2, double y2, double z2, //coordinates of the line
	    float w,                            //width/thickness of the line in pixel
	    float Cr, float Cg, float Cb,    //RGB color components
	    float alpha    					// alpha channel
		);



};

#endif /* LINERENDER_H_ */
