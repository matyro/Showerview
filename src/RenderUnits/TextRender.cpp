/*
 * TextRender.cpp
 *
 *  Created on: 01.09.2015
 *      Author: dominik
 */

#include "TextRender.h"

#include "Shader.h"

textRender::textRender(float pwidth, float pheight)
	:textRender("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf", pwidth, pheight)
{

}

textRender::textRender(const char* fontPath, float pwidth, float pheight)
	:width(pwidth), height(pheight)
{
	this->font = fontPath;
	this->shader = nullptr;

	this->init(width, height);
}

textRender::~textRender()
{
	if(this->shader != nullptr)
		delete this->shader;
}

void textRender::init(float width, float height)
{
	shader = new Shader("/home/dominik/workspace/OpenGL-Viewer/Shader/text.vs", "/home/dominik/workspace/OpenGL-Viewer/Shader/text.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
	shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void textRender::activateContext()
{
	// Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->Use();
}

void textRender::deactivateContext()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}


void textRender::loadFont()
{

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
 		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, this->font, 0, &face))
    		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
    		// Load character glyph
    		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    		{
        		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        		continue;
    		}
    		// Generate texture
    		GLuint texture;
    		glGenTextures(1, &texture);
    		glBindTexture(GL_TEXTURE_2D, texture);
    		glTexImage2D(
        		GL_TEXTURE_2D,
        		0,
        		GL_RED,
        		face->glyph->bitmap.width,
        		face->glyph->bitmap.rows,
        		0,
        		GL_RED,
        		GL_UNSIGNED_BYTE,
        		face->glyph->bitmap.buffer
    		);

		// Set texture options
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
    		Character character = {
        		texture,
        		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        		face->glyph->advance.x
    		};
    		Characters.insert(std::pair<GLchar, Character>(c, character));
	}


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void textRender::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    this->shader->Use();
    glUniform3f(glGetUniformLocation(this->shader->Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
