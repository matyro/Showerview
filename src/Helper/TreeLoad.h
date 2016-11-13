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

		void reload(float p_time = std::numeric_limits<float>::max(), double scale = 1.0);


		inline LineSet& getTree() { return m_tree; }
		
	};
}

