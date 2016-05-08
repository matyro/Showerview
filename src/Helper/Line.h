#pragma once


#include <iostream>

#include <type_traits>
#include <vector>
#include <array>


#include <CL/cl.hpp>



struct Line
{
	cl_float4 start;
	cl_float4 direction;
	cl_float4 col;
};


class LineSet
{
private:
	std::vector<Line> m_vLines;


public:

	LineSet() {}


	void addLine(cl_float4 start, cl_float4 end, cl_float4 col)
	{
		Line tmp;
		tmp.start = start;
		tmp.col = col;

		for(int i=0; i<4; i++)
			tmp.direction.s[i] = end.s[i] - start.s[i];	
		
		m_vLines.push_back(tmp);
	}



	inline const size_t size() const { return m_vLines.size(); }
	inline const Line* const data() const { return m_vLines.data(); }

	inline std::vector<Line>& vec() { return m_vLines; }
};

