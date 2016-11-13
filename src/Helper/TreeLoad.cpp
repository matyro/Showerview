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

		char buffer[sizeof(double) + 1];

		while (!f.eof())
		{
			f.read(buffer, sizeof(double) );
			const double* p = reinterpret_cast<const double*>(buffer);
			
			m_fileContent.push_back(*p);
						
		}

		f.close();		
	}

	void TreeLoad::reload(float p_time, double scale)
	{
		m_tree.clear();

		for(int i=0; i < m_fileContent.size()-34; i = i+34)
		{

			const double time1 = m_fileContent[i + 6];
			const double time2 = m_fileContent[i + 6 + 17];

			cl_float4 start;
			cl_float4 end;

			//std::cout << "DTime: " << time2 - time1 << std::endl;

			//std::cout << "Zeit: " << time << std::endl;
			if (time1 > p_time)
			{
				continue;
			}
			else if (time2 > p_time)
			{
				double dx = m_fileContent[i + 7 + 17] - m_fileContent[i + 7];
				double dh = m_fileContent[i + 5 + 17] - m_fileContent[i + 5];
				double dy = m_fileContent[i + 8 + 17] - m_fileContent[i + 8];

				double dt = time2 - time1;
				double t = p_time - time1;

				double x = t / dt;



				start = { static_cast<float>(m_fileContent[i + 7] * scale),
					static_cast<float>(m_fileContent[i + 5] * scale),
					static_cast<float>(m_fileContent[i + 8] * scale),
					0.0f };

				end = { static_cast<float>((m_fileContent[i + 7] + dx*x) * scale),
					static_cast<float>((m_fileContent[i + 5] + dh*x) * scale),
					static_cast<float>((m_fileContent[i + 8] + dy*x) * scale),
					0.0f };
			}
			else
			{
				start = { static_cast<float>(m_fileContent[i + 7] * scale),
						static_cast<float>(m_fileContent[i + 5] * scale),
						static_cast<float>(m_fileContent[i + 8] * scale),
						0.0f };

				end = { static_cast<float>(m_fileContent[i + 7 + 17] * scale),
						static_cast<float>(m_fileContent[i + 5 + 17] * scale),
						static_cast<float>(m_fileContent[i + 8 + 17] * scale),
						0.0f };
			}


			



			cl_float4 col;
			switch (static_cast<int>(m_fileContent[i + 0]))
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
				col = { 0.4f, 0.1f, 0.8f, 1.0f };	// Weißgrau
				break;
			case 14: // Proton
				col = { 0.8f, 0.0f, 0.0f, 1.0f };	// Rot
				break;
			default:
				col = { 0.0f, 0.8f, 0.0f, 1.0f };	// Grün
				break;
			}

			m_tree.addLine(start, end, col);

			//std::cout << "Add line from " << start.x << ", " << start.y << ", " << start.z;
			//std::cout << " to " << end.x << ", " << end.y << ", " << end.z << std::endl;

		}

	}
	


}