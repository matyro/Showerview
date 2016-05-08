#pragma once


#include <memory>

#include <GL/glew.h>

#include "OGLWrapper/Shader.h"
#include "OGLWrapper/Vertex.h"
#include "OGLWrapper/Texture.h"

std::tuple<std::shared_ptr<Texture>, std::shared_ptr<Shader>, std::shared_ptr<Vertex<float>> > initOGL(int width, int height);


void drawOGL(std::tuple<std::shared_ptr<Texture>, std::shared_ptr<Shader>, std::shared_ptr<Vertex<float>> > data);