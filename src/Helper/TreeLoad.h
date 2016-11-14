/*
 * TreeLoad.h
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#pragma once

#include <string>
#include <limits>

#include "Helper/Line.h"

namespace helper
{	
	class TreeLoad
	{
	private:
		
		LineSet m_tree;

		std::vector<double> m_fileContent;

	protected:
		

	public:

		TreeLoad(std::string file);

		void reload(float p_time, double scaleX, double scaleY, double scaleZ);


		inline LineSet& getTree() { return m_tree; }
		
	};
}

