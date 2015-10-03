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

#include "OGLWrapper\Vertex.h"

namespace render
{
	struct LineVertex
	{
		float pos[3];
		float width;
		float color[4];		
	};
	
	
	class MultiLineRenderUnit: public RenderUnit
	{
	private:

		std::vector<LineVertex> m_std_lines;	

		std::unique_ptr<Shader> m_o_Shader;

		Vertex<3,LineVertex> m_o_Vertex;		

		void activateContext() const;
		void deactivateContext() const;

		void init();

	public:

		MultiLineRenderUnit();

		void draw(glm::mat4 projection, glm::mat4 view) const;

		void updateLines();

		void addLine(const LineVertex start, const LineVertex end );

		void setBufferSize(const unsigned int numberOfLines);		
	};

} /* namespace render */

#endif /* RENDERUNITS_MULTILINERENDERUNIT_H_ */
