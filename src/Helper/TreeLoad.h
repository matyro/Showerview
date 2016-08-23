/*
 * TreeLoad.h
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#pragma once

#include <string>

#include "Helper/Line.h"

namespace helper
{	
	class TreeLoad
	{
	private:
		
		LineSet m_tree;

	protected:
		

	public:

		TreeLoad(std::string file);


		inline LineSet& getTree() { return m_tree; }
		
	};
}

