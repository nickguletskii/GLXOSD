/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "glinject.hpp"
#include "GLXOSD.hpp"
#include "ShaderProgram.hpp"
#include <GL/gl.h>

namespace glxosd {

std::map<std::string, ShaderProgram*> *shaderPrograms = nullptr;

ShaderProgram::ShaderProgram() {
	shaderProgram = rgl(CreateProgram)();
}
void checkShaderCompileStatus(GLuint shader) {
	GLint status;
	rgl(GetShaderiv)(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		rgl(GetShaderiv)(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length, '\0');
		rgl(GetShaderInfoLog)(shader, length, &length, log.data());
		throw std::runtime_error(
				"Couldn't compile GLXOSD shader: "
						+ std::string(log.begin(), log.end()));
	}
}
void checkProgramLinkStatus(GLuint obj) {
	GLint status;
	rgl(GetProgramiv)(obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		rgl(GetProgramiv)(obj, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length, '\0');
		rgl(GetProgramInfoLog)(obj, length, &length, log.data());
		throw std::runtime_error(
				"Couldn't link GLXOSD shader program: "
						+ std::string(log.begin(), log.end()));
	}
}
ShaderProgram* ShaderProgram::addShader(GLenum type, std::string shaderSource) {
	GLuint shader = rgl(CreateShader)(type);
	const GLchar* source = static_cast<const GLchar*>(shaderSource.c_str());
	const int length = shaderSource.size();
	rgl(ShaderSource)(shader, 1, &source, &length);
	rgl(CompileShader)(shader);
	checkShaderCompileStatus(shader);

	rgl(AttachShader)(shaderProgram, shader);

	shaders.push_back(shader);

	return this;
}
ShaderProgram* ShaderProgram::build() {
	rgl(LinkProgram)(shaderProgram);

	for (auto shader : shaders)
		rgl(DeleteShader)(shader);

	checkProgramLinkStatus(shaderProgram);

	return this;
}
GLint ShaderProgram::getUniformLocation(std::string name) {
	return rgl(GetUniformLocation)(shaderProgram, name.c_str());
}

void ShaderProgram::start() {
	rgl(UseProgram)(shaderProgram);
}

void ShaderProgram::stop() {
	rgl(UseProgram)(0);
}

void ShaderProgram::setUniform1i(int loc, GLint v) {
	rgl(Uniform1i)(loc, v);
}
void ShaderProgram::setUniform1f(int loc, GLfloat v) {
	rgl(Uniform1f)(loc, v);
}

GLint ShaderProgram::getAttribLocation(std::string name) {
	return rgl(GetAttribLocation)(shaderProgram, name.c_str());
}

ShaderProgram::ShaderProgram::~ShaderProgram() {
	rgl(DeleteProgram)(shaderProgram);
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

