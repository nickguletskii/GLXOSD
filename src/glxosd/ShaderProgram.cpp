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
#include "ShaderProgram.hpp"
#include "GLLoader.hpp"
#include <GL/gl.h>

namespace glxosd {

std::map<std::string, ShaderProgram*> *shaderPrograms = nullptr;

ShaderProgram::ShaderProgram() {
	shaderProgram = glCreateProgram();
}
void checkShaderCompileStatus(GLuint shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length, '\0');
		glGetShaderInfoLog(shader, length, &length, log.data());
		throw std::runtime_error(
				"Couldn't compile GLXOSD shader: "
						+ std::string(log.begin(), log.end()));
	}
}
void checkProgramLinkStatus(GLuint obj) {
	GLint status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length, '\0');
		glGetProgramInfoLog(obj, length, &length, log.data());
		throw std::runtime_error(
				"Couldn't link GLXOSD shader program: "
						+ std::string(log.begin(), log.end()));
	}
}
ShaderProgram* ShaderProgram::addShader(GLenum type, std::string shaderSource) {
	GLuint shader = glCreateShader(type);
	const GLchar* source = static_cast<const GLchar*>(shaderSource.c_str());
	const int length = shaderSource.size();
	glShaderSource(shader, 1, &source, &length);
	glCompileShader(shader);
	checkShaderCompileStatus(shader);

	glAttachShader(shaderProgram, shader);

	shaders.push_back(shader);

	return this;
}
ShaderProgram* ShaderProgram::build() {
	glLinkProgram(shaderProgram);

	for (auto shader : shaders)
		glDeleteShader(shader);

	checkProgramLinkStatus(shaderProgram);

	return this;
}
GLint ShaderProgram::getUniformLocation(std::string name) {
	return glGetUniformLocation(shaderProgram, name.c_str());
}

void ShaderProgram::start() {
	glUseProgram(shaderProgram);
}

void ShaderProgram::stop() {
	glUseProgram(0);
}

void ShaderProgram::setUniform1i(int loc, GLint v) {
	glUniform1i(loc, v);
}
void ShaderProgram::setUniform1f(int loc, GLfloat v) {
	glUniform1f(loc, v);
}

ShaderProgram::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(shaderProgram);
}

ShaderProgram* ShaderProgram::getOrBuild(std::string name,
		std::function<void(ShaderProgram *shaderProgram)> builder) {
	if (shaderPrograms == nullptr)
		shaderPrograms = new std::map<std::string, ShaderProgram*>();

	std::map<std::string, ShaderProgram*>::iterator it;
	if ((it = shaderPrograms->find(name)) != shaderPrograms->end()) {
		return it->second;
	}

	ShaderProgram* shaderProgram = new ShaderProgram();

	builder(shaderProgram);

	shaderPrograms->insert(
			std::pair<std::string, ShaderProgram*>(name, shaderProgram));
	return shaderProgram;
}

} /* namespace glxosd */
