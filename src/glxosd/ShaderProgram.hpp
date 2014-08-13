/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SHADER_HPP_
#define SHADER_HPP_
#include<map>
#include<functional>
#include<vector>
#include<GL/gl.h>
namespace glxosd {

class ShaderProgram {
public:
	ShaderProgram();

	ShaderProgram* addShader(GLenum type, std::string shaderSource);
	ShaderProgram* build();

	GLint getUniformLocation(std::string name);
	void setUniform1i(int loc, GLint v);
	void setUniform1f(int loc, GLfloat v);

	GLint getAttribLocation(std::string name) ;

	void start();
	void stop();

	virtual ~ShaderProgram();

	static ShaderProgram* getOrBuild(std::string name,
			std::function<void(ShaderProgram *shaderProgram)> builder);
private:
	GLuint shaderProgram;
	std::vector<GLuint> shaders;
};

}
/* namespace glxosd */

#endif /* SHADER_HPP_ */
