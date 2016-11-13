#pragma once


#include <iostream>

#include <type_traits>
#include <vector>
#include <array>

#include "OCL/OpenCL_Include.h"

/*
struct Line
{
	cl_float4 start;
	cl_float4 direction;
	cl_float4 col;
};*/

typedef cl_float16 Line;


class LineSet
{
private:
	std::vector<Line> m_vLines;


public:

	LineSet() {}


	inline void addLine(const cl_float4 start, const cl_float4 end, const cl_float4 col)
	{
		Line tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp.s[0 + i] = start.s[i];
			tmp.s[4 + i] = end.s[i];// -start.s[i];
			tmp.s[8 + i] = col.s[i];
			tmp.s[12 + i] = 0.0;
		}
		
		m_vLines.push_back(tmp);
	}



	inline const size_t size() const { return m_vLines.size(); }
	inline const Line* const data() const { return m_vLines.data(); }

	inline std::vector<Line>& vec() { return m_vLines; }

	inline void clear() { m_vLines.clear(); }
};

