/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include<GL/gl.h>
#include<GL/glx.h>
namespace glxosd {

#define DEFINE_GL_FUNC_TYPE(ret, name, arg) typedef ret ( * name##_t) arg
#define DEFINE_GL_FUNC(returns, name, arg) DEFINE_GL_FUNC_TYPE(returns, name, arg); const name##_t name __attribute__((unused))=  (name##_t) glXGetProcAddress((const GLubyte*) #name)

typedef void (APIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message,
		void* userParam);

DEFINE_GL_FUNC(GLuint, glCreateShader, (GLenum));
DEFINE_GL_FUNC(void, glShaderSource,
		(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length));
DEFINE_GL_FUNC(void, glCompileShader, (GLuint shader));
DEFINE_GL_FUNC(GLuint, glCreateProgram, ());
DEFINE_GL_FUNC(void, glAttachShader, (GLuint, GLuint));
DEFINE_GL_FUNC(void, glLinkProgram, (GLuint));
DEFINE_GL_FUNC(void, glUseProgram, (GLuint));
DEFINE_GL_FUNC(void, glBindBuffer, (GLuint, GLuint));
DEFINE_GL_FUNC(void, glGetShaderiv,
		(GLuint shader, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetShaderInfoLog,
		(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog));
DEFINE_GL_FUNC(void, glReadBuffer, (GLenum mode));
DEFINE_GL_FUNC(void, glDrawRangeElements,
		(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices));
DEFINE_GL_FUNC(void, glTexImage3D,
		(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels));
DEFINE_GL_FUNC(void, glTexSubImage3D,
		(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels));
DEFINE_GL_FUNC(void, glCopyTexSubImage3D,
		(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glCompressedTexImage3D,
		(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data));
DEFINE_GL_FUNC(void, glCompressedTexSubImage3D,
		(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data));
DEFINE_GL_FUNC(void, glGenQueries, (GLsizei n, GLuint *ids));
DEFINE_GL_FUNC(void, glDeleteQueries, (GLsizei n, const GLuint *ids));
DEFINE_GL_FUNC(GLboolean, glIsQuery, (GLuint id));
DEFINE_GL_FUNC(void, glBeginQuery, (GLenum target, GLuint id));
DEFINE_GL_FUNC(void, glEndQuery, (GLenum target));
DEFINE_GL_FUNC(void, glGetQueryiv,
		(GLenum target, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetQueryObjectuiv,
		(GLuint id, GLenum pname, GLuint *params));
DEFINE_GL_FUNC(GLboolean, glUnmapBuffer, (GLenum target));
DEFINE_GL_FUNC(void, glGetBufferPointerv,
		(GLenum target, GLenum pname, void **params));
DEFINE_GL_FUNC(void, glDrawBuffers, (GLsizei n, const GLenum *bufs));
DEFINE_GL_FUNC(void, glUniformMatrix2x3fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix3x2fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix2x4fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix4x2fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix3x4fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix4x3fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glBlitFramebuffer,
		(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter));
DEFINE_GL_FUNC(void, glRenderbufferStorageMultisample,
		(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glFramebufferTextureLayer,
		(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer));
DEFINE_GL_FUNC(void, glFlushMappedBufferRange,
		(GLenum target, GLintptr offset, GLsizeiptr length));
DEFINE_GL_FUNC(void, glBindVertexArray, (GLuint array));
DEFINE_GL_FUNC(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays));
DEFINE_GL_FUNC(void, glGenVertexArrays, (GLsizei n, GLuint *arrays));
DEFINE_GL_FUNC(GLboolean, glIsVertexArray, (GLuint array));
DEFINE_GL_FUNC(void, glBeginTransformFeedback, (GLenum primitiveMode));
DEFINE_GL_FUNC(void, glEndTransformFeedback, (void));
DEFINE_GL_FUNC(void, glBindBufferRange,
		(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size));
DEFINE_GL_FUNC(void, glBindBufferBase,
		(GLenum target, GLuint index, GLuint buffer));
DEFINE_GL_FUNC(void, glTransformFeedbackVaryings,
		(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode));
DEFINE_GL_FUNC(void, glGetTransformFeedbackVarying,
		(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name));
DEFINE_GL_FUNC(void, glVertexAttribIPointer,
		(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer));
DEFINE_GL_FUNC(void, glGetVertexAttribIiv,
		(GLuint index, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetVertexAttribIuiv,
		(GLuint index, GLenum pname, GLuint *params));
DEFINE_GL_FUNC(void, glVertexAttribI4i,
		(GLuint index, GLint x, GLint y, GLint z, GLint w));
DEFINE_GL_FUNC(void, glVertexAttribI4ui,
		(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w));
DEFINE_GL_FUNC(void, glVertexAttribI4iv, (GLuint index, const GLint *v));
DEFINE_GL_FUNC(void, glVertexAttribI4uiv, (GLuint index, const GLuint *v));
DEFINE_GL_FUNC(void, glGetUniformuiv,
		(GLuint program, GLint location, GLuint *params));
DEFINE_GL_FUNC(GLint, glGetFragDataLocation,
		(GLuint program, const GLchar *name));
DEFINE_GL_FUNC(void, glUniform1ui, (GLint location, GLuint v0));
DEFINE_GL_FUNC(void, glUniform2ui, (GLint location, GLuint v0, GLuint v1));
DEFINE_GL_FUNC(void, glUniform3ui,
		(GLint location, GLuint v0, GLuint v1, GLuint v2));
DEFINE_GL_FUNC(void, glUniform4ui,
		(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3));
DEFINE_GL_FUNC(void, glUniform1uiv,
		(GLint location, GLsizei count, const GLuint *value));
DEFINE_GL_FUNC(void, glUniform2uiv,
		(GLint location, GLsizei count, const GLuint *value));
DEFINE_GL_FUNC(void, glUniform3uiv,
		(GLint location, GLsizei count, const GLuint *value));
DEFINE_GL_FUNC(void, glUniform4uiv,
		(GLint location, GLsizei count, const GLuint *value));
DEFINE_GL_FUNC(void, glClearBufferiv,
		(GLenum buffer, GLint drawbuffer, const GLint *value));
DEFINE_GL_FUNC(void, glClearBufferuiv,
		(GLenum buffer, GLint drawbuffer, const GLuint *value));
DEFINE_GL_FUNC(void, glClearBufferfv,
		(GLenum buffer, GLint drawbuffer, const GLfloat *value));
DEFINE_GL_FUNC(void, glClearBufferfi,
		(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil));
DEFINE_GL_FUNC(void, glCopyBufferSubData,
		(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size));
DEFINE_GL_FUNC(void, glGetUniformIndices,
		(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices));
DEFINE_GL_FUNC(void, glGetActiveUniformsiv,
		(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params));
DEFINE_GL_FUNC(GLuint, glGetUniformBlockIndex,
		(GLuint program, const GLchar *uniformBlockName));
DEFINE_GL_FUNC(void, glGetActiveUniformBlockiv,
		(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetActiveUniformBlockName,
		(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName));
DEFINE_GL_FUNC(void, glUniformBlockBinding,
		(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding));
DEFINE_GL_FUNC(void, glDrawArraysInstanced,
		(GLenum mode, GLint first, GLsizei count, GLsizei instancecount));
DEFINE_GL_FUNC(void, glDrawElementsInstanced,
		(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount));
DEFINE_GL_FUNC(GLsync, glFenceSync, (GLenum condition, GLbitfield flags));
DEFINE_GL_FUNC(GLboolean, glIsSync, (GLsync sync));
DEFINE_GL_FUNC(void, glDeleteSync, (GLsync sync));
DEFINE_GL_FUNC(GLenum, glClientWaitSync,
		(GLsync sync, GLbitfield flags, GLuint64 timeout));
DEFINE_GL_FUNC(void, glWaitSync,
		(GLsync sync, GLbitfield flags, GLuint64 timeout));
DEFINE_GL_FUNC(void, glGetInteger64v, (GLenum pname, GLint64 *data));
DEFINE_GL_FUNC(void, glGetSynciv,
		(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values));
DEFINE_GL_FUNC(void, glGetBufferParameteri64v,
		(GLenum target, GLenum pname, GLint64 *params));
DEFINE_GL_FUNC(void, glGenSamplers, (GLsizei count, GLuint *samplers));
DEFINE_GL_FUNC(void, glDeleteSamplers, (GLsizei count, const GLuint *samplers));
DEFINE_GL_FUNC(GLboolean, glIsSampler, (GLuint sampler));
DEFINE_GL_FUNC(void, glBindSampler, (GLuint unit, GLuint sampler));
DEFINE_GL_FUNC(void, glSamplerParameteri,
		(GLuint sampler, GLenum pname, GLint param));
DEFINE_GL_FUNC(void, glSamplerParameteriv,
		(GLuint sampler, GLenum pname, const GLint *param));
DEFINE_GL_FUNC(void, glSamplerParameterf,
		(GLuint sampler, GLenum pname, GLfloat param));
DEFINE_GL_FUNC(void, glSamplerParameterfv,
		(GLuint sampler, GLenum pname, const GLfloat *param));
DEFINE_GL_FUNC(void, glGetSamplerParameteriv,
		(GLuint sampler, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetSamplerParameterfv,
		(GLuint sampler, GLenum pname, GLfloat *params));
DEFINE_GL_FUNC(void, glVertexAttribDivisor, (GLuint index, GLuint divisor));
DEFINE_GL_FUNC(void, glBindTransformFeedback, (GLenum target, GLuint id));
DEFINE_GL_FUNC(void, glDeleteTransformFeedbacks,
		(GLsizei n, const GLuint *ids));
DEFINE_GL_FUNC(void, glGenTransformFeedbacks, (GLsizei n, GLuint *ids));
DEFINE_GL_FUNC(GLboolean, glIsTransformFeedback, (GLuint id));
DEFINE_GL_FUNC(void, glPauseTransformFeedback, (void));
DEFINE_GL_FUNC(void, glResumeTransformFeedback, (void));
DEFINE_GL_FUNC(void, glGetProgramBinary,
		(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary));
DEFINE_GL_FUNC(void, glProgramBinary,
		(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length));
DEFINE_GL_FUNC(void, glProgramParameteri,
		(GLuint program, GLenum pname, GLint value));
DEFINE_GL_FUNC(void, glInvalidateFramebuffer,
		(GLenum target, GLsizei numAttachments, const GLenum *attachments));
DEFINE_GL_FUNC(void, glInvalidateSubFramebuffer,
		(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glTexStorage2D,
		(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glTexStorage3D,
		(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth));
DEFINE_GL_FUNC(void, glGetInternalformativ,
		(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params));
DEFINE_GL_FUNC(void, glActiveTexture, (GLenum texture));
DEFINE_GL_FUNC(void, glBindAttribLocation,
		(GLuint program, GLuint index, const GLchar *name));
DEFINE_GL_FUNC(void, glBindFramebuffer, (GLenum target, GLuint framebuffer));
DEFINE_GL_FUNC(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer));
DEFINE_GL_FUNC(void, glBindTexture, (GLenum target, GLuint texture));
DEFINE_GL_FUNC(void, glBlendColor,
		(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
DEFINE_GL_FUNC(void, glBlendEquation, (GLenum mode));
DEFINE_GL_FUNC(void, glBlendEquationSeparate,
		(GLenum modeRGB, GLenum modeAlpha));
DEFINE_GL_FUNC(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
DEFINE_GL_FUNC(void, glBlendFuncSeparate,
		(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha));
DEFINE_GL_FUNC(void, glBufferData,
		(GLenum target, GLsizeiptr size, const void *data, GLenum usage));
DEFINE_GL_FUNC(void, glBufferSubData,
		(GLenum target, GLintptr offset, GLsizeiptr size, const void *data));
DEFINE_GL_FUNC(GLenum, glCheckFramebufferStatus, (GLenum target));
DEFINE_GL_FUNC(void, glClear, (GLbitfield mask));
DEFINE_GL_FUNC(void, glClearColor,
		(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
DEFINE_GL_FUNC(void, glClearDepthf, (GLfloat d));
DEFINE_GL_FUNC(void, glClearStencil, (GLint s));
DEFINE_GL_FUNC(void, glColorMask,
		(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha));
DEFINE_GL_FUNC(void, glCompressedTexImage2D,
		(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data));
DEFINE_GL_FUNC(void, glCompressedTexSubImage2D,
		(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data));
DEFINE_GL_FUNC(void, glCopyTexImage2D,
		(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border));
DEFINE_GL_FUNC(void, glCopyTexSubImage2D,
		(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glCullFace, (GLenum mode));
DEFINE_GL_FUNC(void, glDeleteBuffers, (GLsizei n, const GLuint *buffers));
DEFINE_GL_FUNC(void, glDeleteFramebuffers,
		(GLsizei n, const GLuint *framebuffers));
DEFINE_GL_FUNC(void, glDeleteProgram, (GLuint program));
DEFINE_GL_FUNC(void, glDeleteRenderbuffers,
		(GLsizei n, const GLuint *renderbuffers));
DEFINE_GL_FUNC(void, glDeleteShader, (GLuint shader));
DEFINE_GL_FUNC(void, glDeleteTextures, (GLsizei n, const GLuint *textures));
DEFINE_GL_FUNC(void, glDepthFunc, (GLenum func));
DEFINE_GL_FUNC(void, glDepthMask, (GLboolean flag));
DEFINE_GL_FUNC(void, glDepthRangef, (GLfloat n, GLfloat f));
DEFINE_GL_FUNC(void, glDetachShader, (GLuint program, GLuint shader));
DEFINE_GL_FUNC(void, glDisable, (GLenum cap));
DEFINE_GL_FUNC(void, glDisableVertexAttribArray, (GLuint index));
DEFINE_GL_FUNC(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
DEFINE_GL_FUNC(void, glDrawElements,
		(GLenum mode, GLsizei count, GLenum type, const void *indices));
DEFINE_GL_FUNC(void, glEnable, (GLenum cap));
DEFINE_GL_FUNC(void, glEnableVertexAttribArray, (GLuint index));
DEFINE_GL_FUNC(void, glFinish, (void));
DEFINE_GL_FUNC(void, glFlush, (void));
DEFINE_GL_FUNC(void, glFramebufferRenderbuffer,
		(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer));
DEFINE_GL_FUNC(void, glFramebufferTexture2D,
		(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level));
DEFINE_GL_FUNC(void, glFrontFace, (GLenum mode));
DEFINE_GL_FUNC(void, glGenBuffers, (GLsizei n, GLuint *buffers));
DEFINE_GL_FUNC(void, glGenerateMipmap, (GLenum target));
DEFINE_GL_FUNC(void, glGenFramebuffers, (GLsizei n, GLuint *framebuffers));
DEFINE_GL_FUNC(void, glGenRenderbuffers, (GLsizei n, GLuint *renderbuffers));
DEFINE_GL_FUNC(void, glGenTextures, (GLsizei n, GLuint *textures));
DEFINE_GL_FUNC(void, glGetActiveAttrib,
		(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name));
DEFINE_GL_FUNC(void, glGetActiveUniform,
		(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name));
DEFINE_GL_FUNC(void, glGetAttachedShaders,
		(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders));
DEFINE_GL_FUNC(GLint, glGetAttribLocation,
		(GLuint program, const GLchar *name));
DEFINE_GL_FUNC(void, glGetBooleanv, (GLenum pname, GLboolean *data));
DEFINE_GL_FUNC(void, glGetBufferParameteriv,
		(GLenum target, GLenum pname, GLint *params));
DEFINE_GL_FUNC(GLenum, glGetError, (void));
DEFINE_GL_FUNC(void, glGetFloatv, (GLenum pname, GLfloat *data));
DEFINE_GL_FUNC(void, glGetFramebufferAttachmentParameteriv,
		(GLenum target, GLenum attachment, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetIntegerv, (GLenum pname, GLint *data));
DEFINE_GL_FUNC(void, glGetProgramiv,
		(GLuint program, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetProgramInfoLog,
		(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog));
DEFINE_GL_FUNC(void, glGetShaderPrecisionFormat,
		(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision));
DEFINE_GL_FUNC(void, glGetShaderSource,
		(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source));
DEFINE_GL_FUNC(void, glGetTexParameterfv,
		(GLenum target, GLenum pname, GLfloat *params));
DEFINE_GL_FUNC(void, glGetTexParameteriv,
		(GLenum target, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetUniformfv,
		(GLuint program, GLint location, GLfloat *params));
DEFINE_GL_FUNC(void, glGetUniformiv,
		(GLuint program, GLint location, GLint *params));
DEFINE_GL_FUNC(GLint, glGetUniformLocation,
		(GLuint program, const GLchar *name));
DEFINE_GL_FUNC(void, glGetVertexAttribfv,
		(GLuint index, GLenum pname, GLfloat *params));
DEFINE_GL_FUNC(void, glGetVertexAttribiv,
		(GLuint index, GLenum pname, GLint *params));
DEFINE_GL_FUNC(void, glGetVertexAttribPointerv,
		(GLuint index, GLenum pname, void **pointer));
DEFINE_GL_FUNC(void, glHint, (GLenum target, GLenum mode));
DEFINE_GL_FUNC(GLboolean, glIsBuffer, (GLuint buffer));
DEFINE_GL_FUNC(GLboolean, glIsEnabled, (GLenum cap));
DEFINE_GL_FUNC(GLboolean, glIsFramebuffer, (GLuint framebuffer));
DEFINE_GL_FUNC(GLboolean, glIsProgram, (GLuint program));
DEFINE_GL_FUNC(GLboolean, glIsRenderbuffer, (GLuint renderbuffer));
DEFINE_GL_FUNC(GLboolean, glIsShader, (GLuint shader));
DEFINE_GL_FUNC(GLboolean, glIsTexture, (GLuint texture));
DEFINE_GL_FUNC(void, glLineWidth, (GLfloat width));
DEFINE_GL_FUNC(void, glPixelStorei, (GLenum pname, GLint param));
DEFINE_GL_FUNC(void, glPolygonOffset, (GLfloat factor, GLfloat units));
DEFINE_GL_FUNC(void, glReadPixels,
		(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels));
DEFINE_GL_FUNC(void, glReleaseShaderCompiler, (void));
DEFINE_GL_FUNC(void, glRenderbufferStorage,
		(GLenum target, GLenum internalformat, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glSampleCoverage, (GLfloat value, GLboolean invert));
DEFINE_GL_FUNC(void, glScissor,
		(GLint x, GLint y, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glShaderBinary,
		(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length));
DEFINE_GL_FUNC(void, glStencilFunc, (GLenum func, GLint ref, GLuint mask));
DEFINE_GL_FUNC(void, glStencilFuncSeparate,
		(GLenum face, GLenum func, GLint ref, GLuint mask));
DEFINE_GL_FUNC(void, glStencilMask, (GLuint mask));
DEFINE_GL_FUNC(void, glStencilMaskSeparate, (GLenum face, GLuint mask));
DEFINE_GL_FUNC(void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass));
DEFINE_GL_FUNC(void, glStencilOpSeparate,
		(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass));
DEFINE_GL_FUNC(void, glTexImage2D,
		(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels));
DEFINE_GL_FUNC(void, glTexParameterf,
		(GLenum target, GLenum pname, GLfloat param));
DEFINE_GL_FUNC(void, glTexParameterfv,
		(GLenum target, GLenum pname, const GLfloat *params));
DEFINE_GL_FUNC(void, glTexParameteri,
		(GLenum target, GLenum pname, GLint param));
DEFINE_GL_FUNC(void, glTexParameteriv,
		(GLenum target, GLenum pname, const GLint *params));
DEFINE_GL_FUNC(void, glTexSubImage2D,
		(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels));
DEFINE_GL_FUNC(void, glUniform1f, (GLint location, GLfloat v0));
DEFINE_GL_FUNC(void, glUniform1fv,
		(GLint location, GLsizei count, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniform1i, (GLint location, GLint v0));
DEFINE_GL_FUNC(void, glUniform1iv,
		(GLint location, GLsizei count, const GLint *value));
DEFINE_GL_FUNC(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1));
DEFINE_GL_FUNC(void, glUniform2fv,
		(GLint location, GLsizei count, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniform2i, (GLint location, GLint v0, GLint v1));
DEFINE_GL_FUNC(void, glUniform2iv,
		(GLint location, GLsizei count, const GLint *value));
DEFINE_GL_FUNC(void, glUniform3f,
		(GLint location, GLfloat v0, GLfloat v1, GLfloat v2));
DEFINE_GL_FUNC(void, glUniform3fv,
		(GLint location, GLsizei count, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniform3i,
		(GLint location, GLint v0, GLint v1, GLint v2));
DEFINE_GL_FUNC(void, glUniform3iv,
		(GLint location, GLsizei count, const GLint *value));
DEFINE_GL_FUNC(void, glUniform4f,
		(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3));
DEFINE_GL_FUNC(void, glUniform4fv,
		(GLint location, GLsizei count, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniform4i,
		(GLint location, GLint v0, GLint v1, GLint v2, GLint v3));
DEFINE_GL_FUNC(void, glUniform4iv,
		(GLint location, GLsizei count, const GLint *value));
DEFINE_GL_FUNC(void, glUniformMatrix2fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix3fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glUniformMatrix4fv,
		(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));
DEFINE_GL_FUNC(void, glValidateProgram, (GLuint program));
DEFINE_GL_FUNC(void, glVertexAttrib1f, (GLuint index, GLfloat x));
DEFINE_GL_FUNC(void, glVertexAttrib1fv, (GLuint index, const GLfloat *v));
DEFINE_GL_FUNC(void, glVertexAttrib2f, (GLuint index, GLfloat x, GLfloat y));
DEFINE_GL_FUNC(void, glVertexAttrib2fv, (GLuint index, const GLfloat *v));
DEFINE_GL_FUNC(void, glVertexAttrib3f,
		(GLuint index, GLfloat x, GLfloat y, GLfloat z));
DEFINE_GL_FUNC(void, glVertexAttrib3fv, (GLuint index, const GLfloat *v));
DEFINE_GL_FUNC(void, glVertexAttrib4f,
		(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w));
DEFINE_GL_FUNC(void, glVertexAttrib4fv, (GLuint index, const GLfloat *v));
DEFINE_GL_FUNC(void, glVertexAttribPointer,
		(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer));
DEFINE_GL_FUNC(void, glViewport,
		(GLint x, GLint y, GLsizei width, GLsizei height));
DEFINE_GL_FUNC(void, glBindFragDataLocation,
		(GLuint program, GLuint colorNumber, const char * name));
DEFINE_GL_FUNC(GLuint, glGetDebugMessageLog,
		( GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog));
DEFINE_GL_FUNC(void, glDebugMessageCallback,
		( DEBUGPROC callback, void * userParam));
DEFINE_GL_FUNC(void, glDebugMessageControl,
		( GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled));

}
