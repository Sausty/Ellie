#ifndef ELLIE_GL_LOADER_H
#define ELLIE_GL_LOADER_H

#include "common.h"

#include <GL/gl.h>

typedef void (*PFN_glViewport)(i32 x, i32 y, i32 width, i32 height);
typedef void (*PFN_glClear)(i32 mask);
typedef void (*PFN_glClearColor)(f32 r, f32 g, f32 b, f32 a);
typedef void (*PFN_glGenBuffers)(u32 n, u32* buffers);
typedef void (*PFN_glBindBuffer)(i32 target, u32 buffer);
typedef void (*PFN_glBufferData)(i32 target, u64 size, const void* data, i32 usage);
typedef void (*PFN_glBufferSubData)(i32 target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (*PFN_glDeleteBuffers)(GLsizei n, const GLuint* bufffers);
typedef u32  (*PFN_glCreateShader)(GLenum shaderType);
typedef void (*PFN_glShaderSource)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void (*PFN_glCompileShader)(GLuint shader);
typedef void (*PFN_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
typedef void (*PFN_glGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (*PFN_glDeleteShader)(GLuint shader);
typedef u32  (*PFN_glCreateProgram)();
typedef void (*PFN_glAttachShader)(GLuint program, GLuint shader);
typedef void (*PFN_glLinkProgram)(GLuint program);
typedef void (*PFN_glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
typedef void (*PFN_glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (*PFN_glUseProgram)(GLuint program);
typedef void (*PFN_glDeleteProgram)(GLuint program);
typedef void (*PFN_glGenVertexArrays)(u32 n, u32* buffers);
typedef void (*PFN_glBindVertexArray)(GLuint vao);
typedef void (*PFN_glDeleteVertexArrays)(u32 n, u32* buffers);
typedef void (*PFN_glEnableVertexAttribArray)(u32 index);
typedef void (*PFN_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (*PFN_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
typedef void (*PFN_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void (*PFN_glGenTextures)(GLint n, GLuint* textures);
typedef void (*PFN_glBindTexture)(GLenum target, GLuint texture);
typedef void (*PFN_glActiveTexture)(GLuint slot);
typedef void (*PFN_glTexParameteri)(GLenum target, GLenum parameter, GLenum value);
typedef void (*PFN_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data);
typedef void (*PFN_glGenerateMipmap)(GLenum target);
typedef void (*PFN_glDeleteTextures)(GLint n, GLuint* textures);
typedef u32  (*PFN_glGetUniformLocation)(GLuint program, const GLchar* name);
typedef void (*PFN_glUniform1i)(GLint location, i32 value);
typedef void (*PFN_glUniform1f)(GLint location, f32 value);
typedef void (*PFN_glUniform2f)(GLint location, f32 x, f32 y);
typedef void (*PFN_glUniform3f)(GLint location, f32 x, f32 y, f32 z);
typedef void (*PFN_glUniform4f)(GLint location, f32 x, f32 y, f32 z, f32 w);
typedef void (*PFN_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

typedef struct gl_state {
    PFN_glViewport Viewport;
    PFN_glClear Clear;
    PFN_glClearColor ClearColor;
    PFN_glGenBuffers GenBuffers;
    PFN_glBindBuffer BindBuffer;
    PFN_glBufferData BufferData;
    PFN_glBufferSubData BufferSubData;
    PFN_glDeleteBuffers DeleteBuffers;
    PFN_glCreateShader CreateShader;
    PFN_glShaderSource ShaderSource;
    PFN_glCompileShader CompileShader;
    PFN_glGetShaderiv GetShaderiv;
    PFN_glGetShaderInfoLog GetShaderInfoLog;
    PFN_glDeleteShader DeleteShader;
    PFN_glCreateProgram CreateProgram;
    PFN_glAttachShader AttachShader;
    PFN_glLinkProgram LinkProgram;
    PFN_glGetProgramiv GetProgramiv;
    PFN_glGetProgramInfoLog GetProgramInfoLog;
    PFN_glUseProgram UseProgram;
    PFN_glDeleteProgram DeleteProgram;
    PFN_glGenVertexArrays GenVertexArrays;
    PFN_glBindVertexArray BindVertexArray;
    PFN_glDeleteVertexArrays DeleteVertexArrays;
    PFN_glEnableVertexAttribArray EnableVertexAttribArray;
    PFN_glVertexAttribPointer VertexAttribPointer;
    PFN_glDrawArrays DrawArrays;
    PFN_glDrawElements DrawElements;
    PFN_glGenTextures GenTextures;
    PFN_glBindTexture BindTexture;
    PFN_glActiveTexture ActiveTexture;
    PFN_glTexParameteri TexParameteri;
    PFN_glTexImage2D TexImage2D;
    PFN_glGenerateMipmap GenerateMipmap;
    PFN_glDeleteTextures DeleteTextures;
    PFN_glGetUniformLocation GetUniformLocation;
    PFN_glUniform1i Uniform1i;
    PFN_glUniform1f Uniform1f;
    PFN_glUniform2f Uniform2f;
    PFN_glUniform3f Uniform3f;
    PFN_glUniform4f Uniform4f;
    PFN_glUniformMatrix4fv UniformMatrix4fv;
} gl_state;

extern gl_state gl;

void LoadGL();
void FreeGL();

#endif
