/*
 * MultiLineRenderUnit.h
 *
 *  Created on: 17.09.2015
 *      Author: dominik
 */

#ifndef RENDERUNITS_MULTILINERENDERUNIT_H_
#define RENDERUNITS_MULTILINERENDERUNIT_H_

#include "RenderUnit.h"

namespace render
{
	struct LineVertex
	{
		float x;
		float y;
		float z;
		float width;
		float r;
		float g;
		float b;
	};
	
	struct Line
	{
		const LineVertex start;
		const LineVertex end;

		Line(LineVertex start_, LineVertex end_) :
						start(start_), end(end_)
		{

		}

		static const Line makeLine(LineVertex start, LineVertex end)
		{
			return Line(start,end);
		}
	};

	class MultiLineRenderUnit: public RenderUnit
	{
	private:

		unsigned int m_uiLineCount;

		void activateContext() const;
		void deactivateContext() const;

		

	public:

		void init();

		void draw(glm::mat4) const;

		void addLine(LineVertex start, LineVertex end );

		void setBufferSize(const unsigned int numberOfLines);

		const Line operator[](unsigned int i) const;
		Line& operator[](unsigned int i);
	};

} /* namespace render */

#endif /* RENDERUNITS_MULTILINERENDERUNIT_H_ */
