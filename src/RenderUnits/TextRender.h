#ifndef _TEXT_RENDER_H_
#define _TEXT_RENDER_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace render
{
	class textRender: RenderUnit
	{
	private:
		GLuint VAO;
		GLuint VBO;

		struct Character
		{
			GLuint TextureID;  // ID handle of the glyph texture
			glm::ivec2 Size;       // Size of glyph
			glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
			GLuint Advance;    // Offset to advance to next glyph
		};

		std::map<GLchar, Character> Characters;

		const char* font;

		Shader* shader;

		float width;
		float height;

		void init(float width, float height);

		void activateContext();
		void deactivateContext();

	public:

		textRender(float width, float height);
		textRender(const char* fontPath, float width, float height);

		~textRender();

		void loadFont();
		void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

		void draw()
		{

		}

	};
}

#endif
