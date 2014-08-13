/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "GLXOSD.hpp"
#include "GLLoader.hpp"
#include "VertexBuffer.hpp"
namespace glxosd {

std::string vertex_source =
		"#version 130\n"
				"uniform float posX;"
				"uniform float posY;"
				"uniform float width;"
				"uniform float height;"
				"uniform float imageWidth;"
				"uniform float imageHeight;"
				"in vec4 vposition;\n"
				"in vec2 vtexcoord;\n"
				"out vec2 ftexcoord;\n"
				"void main() {\n"
				"   ftexcoord = vtexcoord;\n"
				"   gl_Position = (vposition*vec4(2*imageWidth/width, 2*imageHeight/height, 0, 1))+vec4(-1,1,0,0)+vec4(2*posX/width, 2*posY/height, 0, 0);\n"
				"}\n";

std::string fragment_source = "#version 130\n"
		"uniform sampler2D tex;\n"
		"in vec2 ftexcoord;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"   FragColor = texture(tex, ftexcoord);\n"
		"}\n";

VertexBuffer::VertexBuffer(std::vector<GLfloat> vertices,
		std::vector<GLuint> indices, int imageWidth, int imageHeight,
		std::vector<ColourRGBA> image, GLenum type) :
		indices(indices), imageWidth(imageWidth), imageHeight(imageHeight) {
	shaderProgram = ShaderProgram::getOrBuild("basicTexturedVertexBuffer",
			[](ShaderProgram *program) {
				program->
				addShader(GL_VERTEX_SHADER, vertex_source)->
				addShader(GL_FRAGMENT_SHADER, fragment_source)->
				build();
			});

	texture_location = shaderProgram->getUniformLocation("tex");
	posXLocation = shaderProgram->getUniformLocation("posX");
	posYLocation = shaderProgram->getUniformLocation("posY");
	widthLocation = shaderProgram->getUniformLocation("width");
	heightLocation = shaderProgram->getUniformLocation("height");
	imageWidthLocation = shaderProgram->getUniformLocation("imageWidth");
	imageHeightLocation = shaderProgram->getUniformLocation("imageHeight");

	GLint vpositionAttrib = shaderProgram->getAttribLocation("vposition");
	GLint vtexcoordAttrib = shaderProgram->getAttribLocation("vtexcoord");

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
				vertices.data(),
				GL_STATIC_DRAW);

		glEnableVertexAttribArray(vpositionAttrib);

		glVertexAttribPointer(vpositionAttrib, 3, GL_FLOAT, GL_FALSE,
				5 * sizeof(GLfloat), (char*) 0 + 0 * sizeof(GLfloat));

		glEnableVertexAttribArray(vtexcoordAttrib);
		glVertexAttribPointer(vtexcoordAttrib, 2, GL_FLOAT, GL_FALSE,
				5 * sizeof(GLfloat), (char*) 0 + 3 * sizeof(GLfloat));

		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
				indices.data(), GL_STATIC_DRAW);

	}
	glBindVertexArray(0);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0,
	GL_RGBA,
	GL_UNSIGNED_BYTE, image.data());

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexBuffer::render(int x, int y, int width, int height) {
	shaderProgram->start();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shaderProgram->setUniform1i(texture_location, 0);
	shaderProgram->setUniform1f(posXLocation, x);
	shaderProgram->setUniform1f(posYLocation, y);
	shaderProgram->setUniform1f(widthLocation, width);
	shaderProgram->setUniform1f(heightLocation, height);
	shaderProgram->setUniform1f(imageWidthLocation, imageWidth);
	shaderProgram->setUniform1f(imageHeightLocation, imageHeight);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shaderProgram->stop();
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &vao);
}

} /* namespace glxosd */
