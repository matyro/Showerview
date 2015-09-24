/*
 * MultiLineRenderUnit.h
 *
 *  Created on: 17.09.2015
 *      Author: dominik
 */

#ifndef RENDERUNITS_MULTILINERENDERUNIT_H_
#define RENDERUNITS_MULTILINERENDERUNIT_H_

#include "RenderUnit.h"

#include "OGLWrapper/Shader.h"


#include <vector>
#include <memory>

namespace render
{
	struct LineVertex
	{
		float pos[3];
		float width;
		float color[4];
	};
	
	struct Line
	{
		const LineVertex start;
		const LineVertex end;

		Line() :
			start({{0,0,0},0,{0,0,0}}), end({{0,0,0},0,{0,0,0}})
		{

		}

		Line(const LineVertex start_, const LineVertex end_) :
						start(start_), end(end_)
		{

		}

		static const Line makeLine(const LineVertex start, const LineVertex end)
		{
			return Line(start,end);
		}
	};

	class MultiLineRenderUnit: public RenderUnit
	{
	private:

		std::vector<Line> m_std_lines;


		GLuint m_uiVAO;
		GLuint m_uiVBO;

		std::unique_ptr<Shader> m_o_Shader;

		std::unique_ptr<float[]> m_vertexData;

		void activateContext() const;
		void deactivateContext() const;

		

	public:

		void init();

		void draw(glm::mat4) const;

		void updateLines();

		void addLine(const LineVertex start, const LineVertex end );

		void setBufferSize(const unsigned int numberOfLines);

		const Line operator[](unsigned int i) const;
		Line& operator[](unsigned int i);
	};

} /* namespace render */

#endif /* RENDERUNITS_MULTILINERENDERUNIT_H_ */
