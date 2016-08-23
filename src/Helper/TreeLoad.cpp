/*
 * TreeLoad.cpp
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#include "TreeLoad.h"

#include <iostream>
#include <fstream>

namespace helper
{

	TreeLoad::TreeLoad(std::string file)
	{

		std::fstream f(file, std::ios::in | std::ios::binary);

		if (!f.good())
		{
			std::cerr << "Could not open " << file << std::endl;
			std::cin.get();
			exit(-1);
		}

		const int particleSize = 17 * sizeof(double);

		char buffer[2* particleSize];
		
		while (!f.eof())
		{
			f.read(buffer, 2 * particleSize);

			const double* p = reinterpret_cast<const double*>(buffer);

			const cl_float4 start = {p[7]/10000.0, p[5] / 10000.0, p[8] / 10000.0, 0};
			const cl_float4 end = { p[7 + 17] / 10000.0, p[5 + 17] / 10000.0, p[8 + 17] / 10000.0, 0 };



			cl_float4 col;
			switch (static_cast<int>(p[0]))
			{
			case 1: // Gamma
				col = { 0.0f, 0.0f, 1.0f, 1.0f };	// Dunkelblau
				break;
			case 2:	// e-
			case 3:	// e+
				col = { 0.0f, 1.0f, 1.0f, 1.0f };	// Hellblau
				break;
			case 5: // µ+
			case 6: // µ-
				col = { 0.5f, 0.0f, 0.5f, 1.0f };	// Lila
				break;
			case 7: // pi0
			case 8: // pi+
			case 9: // pi-
				col = { 1.0f, 1.0f, 0.0f, 1.0f };	// Gelb
				break;
			case 13: // Neutron
				col = { 0.8f, 0.8f, 0.8f, 1.0f };	// Weißgrau
				break;
			case 14: // Proton
				col = { 0.8f, 0.0f, 0.0f, 1.0f };	// Rot
				break;
			default:
				col = { 0.0f, 0.8f, 0.0f, 1.0f };	// Grün
				break;
			}

			m_tree.addLine(start, end, col);

			std::cout << "Add line from " << start.x << ", " << start.y << ", " << start.z;
			std::cout << " to " << end.x << ", " << end.y << ", " << end.z << std::endl;

		}



	}


	


}