/*
 * Copyright (C) 2016 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include"gl-dynamic-loader.h"
#include<stdbool.h>
#include<stdio.h>
#include<dlfcn.h>

void glinject_dynamic_gl_initialise(const char* libGL_path) {
	static bool initialised = false;
	if (initialised) {
		return;
	}
	initialised = true;
	void *gl = dlopen(libGL_path, RTLD_NOW | RTLD_LOCAL);
	void *(*glXGetProcAddress)(const GLubyte *) = dlsym(gl, "glXGetProcAddress");

	glCreateShader = glXGetProcAddress((const GLubyte *) "glCreateShader");
	glCreateProgram = glXGetProcAddress((const GLubyte *) "glCreateProgram");
	glShaderSource = glXGetProcAddress((const GLubyte *) "glShaderSource");
	glCompileShader = glXGetProcAddress((const GLubyte *) "glCompileShader");
	glAttachShader = glXGetProcAddress((const GLubyte *) "glAttachShader");
	glLinkProgram = glXGetProcAddress((const GLubyte *) "glLinkProgram");
	glGetShaderiv = glXGetProcAddress((const GLubyte *) "glGetShaderiv");
	glGetShaderInfoLog = glXGetProcAddress(
			(const GLubyte *) "glGetShaderInfoLog");
	glDrawRangeElements = glXGetProcAddress(
			(const GLubyte *) "glDrawRangeElements");
	glTexImage3D = glXGetProcAddress((const GLubyte *) "glTexImage3D");
	glTexSubImage3D = glXGetProcAddress((const GLubyte *) "glTexSubImage3D");
	glCopyTexSubImage3D = glXGetProcAddress(
			(const GLubyte *) "glCopyTexSubImage3D");
	glCompressedTexImage3D = glXGetProcAddress(
			(const GLubyte *) "glCompressedTexImage3D");
	glCompressedTexSubImage3D = glXGetProcAddress(
			(const GLubyte *) "glCompressedTexSubImage3D");
	glGenQueries = glXGetProcAddress((const GLubyte *) "glGenQueries");
	glDeleteQueries = glXGetProcAddress((const GLubyte *) "glDeleteQueries");
	glIsQuery = glXGetProcAddress((const GLubyte *) "glIsQuery");
	glBeginQuery = glXGetProcAddress((const GLubyte *) "glBeginQuery");
	glEndQuery = glXGetProcAddress((const GLubyte *) "glEndQuery");
	glGetQueryiv = glXGetProcAddress((const GLubyte *) "glGetQueryiv");
	glGetQueryObjectuiv = glXGetProcAddress(
			(const GLubyte *) "glGetQueryObjectuiv");
	glUnmapBuffer = glXGetProcAddress((const GLubyte *) "glUnmapBuffer");
	glGetBufferPointerv = glXGetProcAddress(
			(const GLubyte *) "glGetBufferPointerv");
	glDrawBuffers = glXGetProcAddress((const GLubyte *) "glDrawBuffers");
	glUniformMatrix2x3fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix2x3fv");
	glUniformMatrix3x2fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix3x2fv");
	glUniformMatrix2x4fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix2x4fv");
	glUniformMatrix4x2fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix4x2fv");
	glUniformMatrix3x4fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix3x4fv");
	glUniformMatrix4x3fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix4x3fv");
	glBlitFramebuffer = glXGetProcAddress((const GLubyte *) "glBlitFramebuffer");
	glRenderbufferStorageMultisample = glXGetProcAddress(
			(const GLubyte *) "glRenderbufferStorageMultisample");
	glFramebufferTextureLayer = glXGetProcAddress(
			(const GLubyte *) "glFramebufferTextureLayer");
	glFlushMappedBufferRange = glXGetProcAddress(
			(const GLubyte *) "glFlushMappedBufferRange");
	glDeleteVertexArrays = glXGetProcAddress(
			(const GLubyte *) "glDeleteVertexArrays");
	glIsVertexArray = glXGetProcAddress((const GLubyte *) "glIsVertexArray");
	glBeginTransformFeedback = glXGetProcAddress(
			(const GLubyte *) "glBeginTransformFeedback");
	glEndTransformFeedback = glXGetProcAddress(
			(const GLubyte *) "glEndTransformFeedback");
	glBindBufferRange = glXGetProcAddress((const GLubyte *) "glBindBufferRange");
	glBindBufferBase = glXGetProcAddress((const GLubyte *) "glBindBufferBase");
	glTransformFeedbackVaryings = glXGetProcAddress(
			(const GLubyte *) "glTransformFeedbackVaryings");
	glGetTransformFeedbackVarying = glXGetProcAddress(
			(const GLubyte *) "glGetTransformFeedbackVarying");
	glVertexAttribIPointer = glXGetProcAddress(
			(const GLubyte *) "glVertexAttribIPointer");
	glGetVertexAttribIiv = glXGetProcAddress(
			(const GLubyte *) "glGetVertexAttribIiv");
	glGetVertexAttribIuiv = glXGetProcAddress(
			(const GLubyte *) "glGetVertexAttribIuiv");
	glVertexAttribI4i = glXGetProcAddress((const GLubyte *) "glVertexAttribI4i");
	glVertexAttribI4ui = glXGetProcAddress(
			(const GLubyte *) "glVertexAttribI4ui");
	glVertexAttribI4iv = glXGetProcAddress(
			(const GLubyte *) "glVertexAttribI4iv");
	glVertexAttribI4uiv = glXGetProcAddress(
			(const GLubyte *) "glVertexAttribI4uiv");
	glGetUniformuiv = glXGetProcAddress((const GLubyte *) "glGetUniformuiv");
	glGetFragDataLocation = glXGetProcAddress(
			(const GLubyte *) "glGetFragDataLocation");
	glUniform1ui = glXGetProcAddress((const GLubyte *) "glUniform1ui");
	glUniform2ui = glXGetProcAddress((const GLubyte *) "glUniform2ui");
	glUniform3ui = glXGetProcAddress((const GLubyte *) "glUniform3ui");
	glUniform4ui = glXGetProcAddress((const GLubyte *) "glUniform4ui");
	glUniform1uiv = glXGetProcAddress((const GLubyte *) "glUniform1uiv");
	glUniform2uiv = glXGetProcAddress((const GLubyte *) "glUniform2uiv");
	glUniform3uiv = glXGetProcAddress((const GLubyte *) "glUniform3uiv");
	glUniform4uiv = glXGetProcAddress((const GLubyte *) "glUniform4uiv");
	glClearBufferiv = glXGetProcAddress((const GLubyte *) "glClearBufferiv");
	glClearBufferuiv = glXGetProcAddress((const GLubyte *) "glClearBufferuiv");
	glClearBufferfv = glXGetProcAddress((const GLubyte *) "glClearBufferfv");
	glClearBufferfi = glXGetProcAddress((const GLubyte *) "glClearBufferfi");
	glCopyBufferSubData = glXGetProcAddress(
			(const GLubyte *) "glCopyBufferSubData");
	glGetUniformIndices = glXGetProcAddress(
			(const GLubyte *) "glGetUniformIndices");
	glGetActiveUniformsiv = glXGetProcAddress(
			(const GLubyte *) "glGetActiveUniformsiv");
	glGetUniformBlockIndex = glXGetProcAddress(
			(const GLubyte *) "glGetUniformBlockIndex");
	glGetActiveUniformBlockiv = glXGetProcAddress(
			(const GLubyte *) "glGetActiveUniformBlockiv");
	glGetActiveUniformBlockName = glXGetProcAddress(
			(const GLubyte *) "glGetActiveUniformBlockName");
	glUniformBlockBinding = glXGetProcAddress(
			(const GLubyte *) "glUniformBlockBinding");
	glDrawArraysInstanced = glXGetProcAddress(
			(const GLubyte *) "glDrawArraysInstanced");
	glDrawElementsInstanced = glXGetProcAddress(
			(const GLubyte *) "glDrawElementsInstanced");
	glFenceSync = glXGetProcAddress((const GLubyte *) "glFenceSync");
	glIsSync = glXGetProcAddress((const GLubyte *) "glIsSync");
	glDeleteSync = glXGetProcAddress((const GLubyte *) "glDeleteSync");
	glClientWaitSync = glXGetProcAddress((const GLubyte *) "glClientWaitSync");
	glWaitSync = glXGetProcAddress((const GLubyte *) "glWaitSync");
	glGetInteger64v = glXGetProcAddress((const GLubyte *) "glGetInteger64v");
	glGetSynciv = glXGetProcAddress((const GLubyte *) "glGetSynciv");
	glGetBufferParameteri64v = glXGetProcAddress(
			(const GLubyte *) "glGetBufferParameteri64v");
	glGenSamplers = glXGetProcAddress((const GLubyte *) "glGenSamplers");
	glDeleteSamplers = glXGetProcAddress((const GLubyte *) "glDeleteSamplers");
	glIsSampler = glXGetProcAddress((const GLubyte *) "glIsSampler");
	glBindSampler = glXGetProcAddress((const GLubyte *) "glBindSampler");
	glSamplerParameteri = glXGetProcAddress(
			(const GLubyte *) "glSamplerParameteri");
	glSamplerParameteriv = glXGetProcAddress(
			(const GLubyte *) "glSamplerParameteriv");
	glSamplerParameterf = glXGetProcAddress(
			(const GLubyte *) "glSamplerParameterf");
	glSamplerParameterfv = glXGetProcAddress(
			(const GLubyte *) "glSamplerParameterfv");
	glGetSamplerParameteriv = glXGetProcAddress(
			(const GLubyte *) "glGetSamplerParameteriv");
	glGetSamplerParameterfv = glXGetProcAddress(
			(const GLubyte *) "glGetSamplerParameterfv");
	glVertexAttribDivisor = glXGetProcAddress(
			(const GLubyte *) "glVertexAttribDivisor");
	glBindTransformFeedback = glXGetProcAddress(
			(const GLubyte *) "glBindTransformFeedback");
	glDeleteTransformFeedbacks = glXGetProcAddress(
			(const GLubyte *) "glDeleteTransformFeedbacks");
	glGenTransformFeedbacks = glXGetProcAddress(
			(const GLubyte *) "glGenTransformFeedbacks");
	glIsTransformFeedback = glXGetProcAddress(
			(const GLubyte *) "glIsTransformFeedback");
	glPauseTransformFeedback = glXGetProcAddress(
			(const GLubyte *) "glPauseTransformFeedback");
	glResumeTransformFeedback = glXGetProcAddress(
			(const GLubyte *) "glResumeTransformFeedback");
	glGetProgramBinary = glXGetProcAddress(
			(const GLubyte *) "glGetProgramBinary");
	glProgramBinary = glXGetProcAddress((const GLubyte *) "glProgramBinary");
	glProgramParameteri = glXGetProcAddress(
			(const GLubyte *) "glProgramParameteri");
	glInvalidateFramebuffer = glXGetProcAddress(
			(const GLubyte *) "glInvalidateFramebuffer");
	glInvalidateSubFramebuffer = glXGetProcAddress(
			(const GLubyte *) "glInvalidateSubFramebuffer");
	glTexStorage2D = glXGetProcAddress((const GLubyte *) "glTexStorage2D");
	glTexStorage3D = glXGetProcAddress((const GLubyte *) "glTexStorage3D");
	glGetInternalformativ = glXGetProcAddress(
			(const GLubyte *) "glGetInternalformativ");
	glActiveTexture = glXGetProcAddress((const GLubyte *) "glActiveTexture");
	glBindAttribLocation = glXGetProcAddress(
			(const GLubyte *) "glBindAttribLocation");
	glBindFramebuffer = glXGetProcAddress((const GLubyte *) "glBindFramebuffer");
	glBindRenderbuffer = glXGetProcAddress(
			(const GLubyte *) "glBindRenderbuffer");
	glBlendColor = glXGetProcAddress((const GLubyte *) "glBlendColor");
	glBlendEquation = glXGetProcAddress((const GLubyte *) "glBlendEquation");
	glBlendEquationSeparate = glXGetProcAddress(
			(const GLubyte *) "glBlendEquationSeparate");
	glBlendFuncSeparate = glXGetProcAddress(
			(const GLubyte *) "glBlendFuncSeparate");
	glBufferData = glXGetProcAddress((const GLubyte *) "glBufferData");
	glBufferSubData = glXGetProcAddress((const GLubyte *) "glBufferSubData");
	glCheckFramebufferStatus = glXGetProcAddress(
			(const GLubyte *) "glCheckFramebufferStatus");
	glClearDepthf = glXGetProcAddress((const GLubyte *) "glClearDepthf");
	glCompressedTexImage2D = glXGetProcAddress(
			(const GLubyte *) "glCompressedTexImage2D");
	glCompressedTexSubImage2D = glXGetProcAddress(
			(const GLubyte *) "glCompressedTexSubImage2D");
	glDeleteBuffers = glXGetProcAddress((const GLubyte *) "glDeleteBuffers");
	glDeleteFramebuffers = glXGetProcAddress(
			(const GLubyte *) "glDeleteFramebuffers");
	glDeleteProgram = glXGetProcAddress((const GLubyte *) "glDeleteProgram");
	glDeleteRenderbuffers = glXGetProcAddress(
			(const GLubyte *) "glDeleteRenderbuffers");
	glDeleteShader = glXGetProcAddress((const GLubyte *) "glDeleteShader");
	glDepthRangef = glXGetProcAddress((const GLubyte *) "glDepthRangef");
	glDetachShader = glXGetProcAddress((const GLubyte *) "glDetachShader");
	glDisableVertexAttribArray = glXGetProcAddress(
			(const GLubyte *) "glDisableVertexAttribArray");
	glEnableVertexAttribArray = glXGetProcAddress(
			(const GLubyte *) "glEnableVertexAttribArray");
	glFramebufferRenderbuffer = glXGetProcAddress(
			(const GLubyte *) "glFramebufferRenderbuffer");
	glFramebufferTexture2D = glXGetProcAddress(
			(const GLubyte *) "glFramebufferTexture2D");
	glGenerateMipmap = glXGetProcAddress((const GLubyte *) "glGenerateMipmap");
	glGenFramebuffers = glXGetProcAddress((const GLubyte *) "glGenFramebuffers");
	glGenRenderbuffers = glXGetProcAddress(
			(const GLubyte *) "glGenRenderbuffers");
	glGetActiveAttrib = glXGetProcAddress((const GLubyte *) "glGetActiveAttrib");
	glGetActiveUniform = glXGetProcAddress(
			(const GLubyte *) "glGetActiveUniform");
	glGetAttachedShaders = glXGetProcAddress(
			(const GLubyte *) "glGetAttachedShaders");
	glGetAttribLocation = glXGetProcAddress(
			(const GLubyte *) "glGetAttribLocation");
	glGetBufferParameteriv = glXGetProcAddress(
			(const GLubyte *) "glGetBufferParameteriv");
	glGetFramebufferAttachmentParameteriv = glXGetProcAddress(
			(const GLubyte *) "glGetFramebufferAttachmentParameteriv");
	glGetProgramiv = glXGetProcAddress((const GLubyte *) "glGetProgramiv");
	glGetProgramInfoLog = glXGetProcAddress(
			(const GLubyte *) "glGetProgramInfoLog");
	glGetShaderPrecisionFormat = glXGetProcAddress(
			(const GLubyte *) "glGetShaderPrecisionFormat");
	glGetShaderSource = glXGetProcAddress((const GLubyte *) "glGetShaderSource");
	glGetUniformfv = glXGetProcAddress((const GLubyte *) "glGetUniformfv");
	glGetUniformiv = glXGetProcAddress((const GLubyte *) "glGetUniformiv");
	glGetUniformLocation = glXGetProcAddress(
			(const GLubyte *) "glGetUniformLocation");
	glGetVertexAttribfv = glXGetProcAddress(
			(const GLubyte *) "glGetVertexAttribfv");
	glGetVertexAttribiv = glXGetProcAddress(
			(const GLubyte *) "glGetVertexAttribiv");
	glGetVertexAttribPointerv = glXGetProcAddress(
			(const GLubyte *) "glGetVertexAttribPointerv");
	glIsBuffer = glXGetProcAddress((const GLubyte *) "glIsBuffer");
	glIsFramebuffer = glXGetProcAddress((const GLubyte *) "glIsFramebuffer");
	glIsProgram = glXGetProcAddress((const GLubyte *) "glIsProgram");
	glIsRenderbuffer = glXGetProcAddress((const GLubyte *) "glIsRenderbuffer");
	glIsShader = glXGetProcAddress((const GLubyte *) "glIsShader");
	glReleaseShaderCompiler = glXGetProcAddress(
			(const GLubyte *) "glReleaseShaderCompiler");
	glRenderbufferStorage = glXGetProcAddress(
			(const GLubyte *) "glRenderbufferStorage");
	glSampleCoverage = glXGetProcAddress((const GLubyte *) "glSampleCoverage");
	glShaderBinary = glXGetProcAddress((const GLubyte *) "glShaderBinary");
	glStencilFuncSeparate = glXGetProcAddress(
			(const GLubyte *) "glStencilFuncSeparate");
	glStencilMaskSeparate = glXGetProcAddress(
			(const GLubyte *) "glStencilMaskSeparate");
	glStencilOpSeparate = glXGetProcAddress(
			(const GLubyte *) "glStencilOpSeparate");
	glUniform1f = glXGetProcAddress((const GLubyte *) "glUniform1f");
	glUniform1fv = glXGetProcAddress((const GLubyte *) "glUniform1fv");
	glUniform1i = glXGetProcAddress((const GLubyte *) "glUniform1i");
	glUniform1iv = glXGetProcAddress((const GLubyte *) "glUniform1iv");
	glUniform2f = glXGetProcAddress((const GLubyte *) "glUniform2f");
	glUniform2fv = glXGetProcAddress((const GLubyte *) "glUniform2fv");
	glUniform2i = glXGetProcAddress((const GLubyte *) "glUniform2i");
	glUniform2iv = glXGetProcAddress((const GLubyte *) "glUniform2iv");
	glUniform3f = glXGetProcAddress((const GLubyte *) "glUniform3f");
	glUniform3fv = glXGetProcAddress((const GLubyte *) "glUniform3fv");
	glUniform3i = glXGetProcAddress((const GLubyte *) "glUniform3i");
	glUniform3iv = glXGetProcAddress((const GLubyte *) "glUniform3iv");
	glUniform4f = glXGetProcAddress((const GLubyte *) "glUniform4f");
	glUniform4fv = glXGetProcAddress((const GLubyte *) "glUniform4fv");
	glUniform4i = glXGetProcAddress((const GLubyte *) "glUniform4i");
	glUniform4iv = glXGetProcAddress((const GLubyte *) "glUniform4iv");
	glUniformMatrix2fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix2fv");
	glUniformMatrix3fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix3fv");
	glUniformMatrix4fv = glXGetProcAddress(
			(const GLubyte *) "glUniformMatrix4fv");
	glValidateProgram = glXGetProcAddress((const GLubyte *) "glValidateProgram");
	glVertexAttrib1f = glXGetProcAddress((const GLubyte *) "glVertexAttrib1f");
	glVertexAttrib1fv = glXGetProcAddress((const GLubyte *) "glVertexAttrib1fv");
	glVertexAttrib2f = glXGetProcAddress((const GLubyte *) "glVertexAttrib2f");
	glVertexAttrib2fv = glXGetProcAddress((const GLubyte *) "glVertexAttrib2fv");
	glVertexAttrib3f = glXGetProcAddress((const GLubyte *) "glVertexAttrib3f");
	glVertexAttrib3fv = glXGetProcAddress((const GLubyte *) "glVertexAttrib3fv");
	glVertexAttrib4f = glXGetProcAddress((const GLubyte *) "glVertexAttrib4f");
	glVertexAttrib4fv = glXGetProcAddress((const GLubyte *) "glVertexAttrib4fv");
	glVertexAttribPointer = glXGetProcAddress(
			(const GLubyte *) "glVertexAttribPointer");
	glBindFragDataLocation = glXGetProcAddress(
			(const GLubyte *) "glBindFragDataLocation");
	glGetDebugMessageLog = glXGetProcAddress(
			(const GLubyte *) "glGetDebugMessageLog");
	glDebugMessageControl = glXGetProcAddress(
			(const GLubyte *) "glDebugMessageControl");
	glGenVertexArrays = glXGetProcAddress((const GLubyte *) "glGenVertexArrays");
	glGenBuffers = glXGetProcAddress((const GLubyte *) "glGenBuffers");
	glBindBuffer = glXGetProcAddress((const GLubyte *) "glBindBuffer");
	glBindVertexArray = glXGetProcAddress((const GLubyte *) "glBindVertexArray");
	glUseProgram = glXGetProcAddress((const GLubyte *) "glUseProgram");
}
