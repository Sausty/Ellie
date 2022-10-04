#include "ellie_gl.h"

#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>

internal void* GLLibrary;

gl_state gl;

void LoadGL()
{
    GLLibrary = dlopen("libGL.so", RTLD_NOW);

    gl.Viewport = dlsym(GLLibrary, "glViewport");
    gl.Clear = dlsym(GLLibrary, "glClear");
    gl.ClearColor = dlsym(GLLibrary, "glClearColor");
    gl.GenBuffers = dlsym(GLLibrary, "glGenBuffers");
    gl.BindBuffer = dlsym(GLLibrary, "glBindBuffer");
    gl.BufferData = dlsym(GLLibrary, "glBufferData");
    gl.BufferSubData = dlsym(GLLibrary, "glBufferSubData");
    gl.DeleteBuffers = dlsym(GLLibrary, "glDeleteBuffers");
    gl.CreateShader = dlsym(GLLibrary, "glCreateShader");
    gl.ShaderSource = dlsym(GLLibrary, "glShaderSource");
    gl.CompileShader = dlsym(GLLibrary, "glCompileShader");
    gl.GetShaderiv = dlsym(GLLibrary, "glGetShaderiv");
    gl.GetShaderInfoLog = dlsym(GLLibrary, "glGetShaderInfoLog");
    gl.DeleteShader = dlsym(GLLibrary, "glDeleteShader");
    gl.CreateProgram = dlsym(GLLibrary, "glCreateProgram");
    gl.AttachShader = dlsym(GLLibrary, "glAttachShader");
    gl.LinkProgram = dlsym(GLLibrary, "glLinkProgram");
    gl.GetProgramiv = dlsym(GLLibrary, "glGetProgramiv");
    gl.GetProgramInfoLog = dlsym(GLLibrary, "glGetProgramInfoLog");
    gl.UseProgram = dlsym(GLLibrary, "glUseProgram");
    gl.DeleteProgram = dlsym(GLLibrary, "glDeleteProgram");
    gl.GenVertexArrays = dlsym(GLLibrary, "glGenVertexArrays");
    gl.BindVertexArray = dlsym(GLLibrary, "glBindVertexArray");
    gl.DeleteVertexArrays = dlsym(GLLibrary, "glDeleteVertexArrays");
    gl.EnableVertexAttribArray = dlsym(GLLibrary, "glEnableVertexAttribArray");
    gl.VertexAttribPointer = dlsym(GLLibrary, "glVertexAttribPointer");
    gl.DrawArrays = dlsym(GLLibrary, "glDrawArrays");
    gl.DrawElements = dlsym(GLLibrary, "glDrawElements");
    gl.GenTextures = dlsym(GLLibrary, "glGenTextures");
    gl.BindTexture = dlsym(GLLibrary, "glBindTexture");
    gl.ActiveTexture = dlsym(GLLibrary, "glActiveTexture");
    gl.TexParameteri = dlsym(GLLibrary, "glTexParameteri");
    gl.TexImage2D = dlsym(GLLibrary, "glTexImage2D");
    gl.GenerateMipmap = dlsym(GLLibrary, "glGenerateMipmap");
    gl.DeleteTextures = dlsym(GLLibrary, "glDeleteTextures");
    gl.GetUniformLocation = dlsym(GLLibrary, "glGetUniformLocation");
    gl.Uniform1i = dlsym(GLLibrary, "glUniform1i");
    gl.Uniform1f = dlsym(GLLibrary, "glUniform1f");
    gl.Uniform2f = dlsym(GLLibrary, "glUniform2f");
    gl.Uniform3f = dlsym(GLLibrary, "glUniform3f");
    gl.Uniform4f = dlsym(GLLibrary, "glUniform4f");
    gl.UniformMatrix4fv = dlsym(GLLibrary, "glUniformMatrix4fv");
    
    assert(gl.Viewport);
    assert(gl.Clear);
    assert(gl.ClearColor);
    assert(gl.GenBuffers);
    assert(gl.BindBuffer);
    assert(gl.BufferData);
    assert(gl.BufferSubData);
    assert(gl.DeleteBuffers);
    assert(gl.CreateShader);
    assert(gl.ShaderSource);
    assert(gl.CompileShader);
    assert(gl.GetShaderiv);
    assert(gl.GetShaderInfoLog);
    assert(gl.DeleteShader);
    assert(gl.CreateProgram);
    assert(gl.AttachShader);
    assert(gl.LinkProgram);
    assert(gl.GetProgramiv);
    assert(gl.GetProgramInfoLog);
    assert(gl.UseProgram);
    assert(gl.DeleteProgram);
    assert(gl.GenVertexArrays);
    assert(gl.BindVertexArray);
    assert(gl.DeleteVertexArrays);
    assert(gl.EnableVertexAttribArray);
    assert(gl.VertexAttribPointer);
    assert(gl.DrawArrays);
    assert(gl.DrawElements);
    assert(gl.GenTextures);
    assert(gl.BindTexture);
    assert(gl.ActiveTexture);
    assert(gl.TexParameteri);
    assert(gl.TexImage2D);
    assert(gl.GenerateMipmap);
    assert(gl.DeleteTextures);
    assert(gl.GetUniformLocation);
    assert(gl.Uniform1i);
    assert(gl.Uniform1f);
    assert(gl.Uniform2f);
    assert(gl.Uniform3f);
    assert(gl.Uniform4f);
    assert(gl.UniformMatrix4fv);
    
    printf("Loaded libGL.so and the following functions:\n");
    printf("\t- glViewport\n");
    printf("\t- glClear\n");
    printf("\t- glClearColor\n");
    printf("\t- glGenBuffers\n");
    printf("\t- glBindBuffer\n");
    printf("\t- glBufferData\n");
    printf("\t- glBufferSubData\n");
    printf("\t- glDeleteBuffers\n");
    printf("\t- glCreateShader\n");
    printf("\t- glShaderSource\n");
    printf("\t- glCompileShader\n");
    printf("\t- glGetShaderiv\n");
    printf("\t- glGetShaderInfoLog\n");
    printf("\t- glDeleteShader\n");
    printf("\t- glCreateProgram\n");
    printf("\t- glAttachShader\n");
    printf("\t- glLinkProgram\n");
    printf("\t- glGetProgramiv\n");
    printf("\t- glGetProgramInfoLog\n");
    printf("\t- glUseProgram\n");
    printf("\t- glDeleteProgram\n");
    printf("\t- glGenVertexArrays\n");
    printf("\t- glBindVertexArray\n");
    printf("\t- glDeleteVertexArrays\n");
    printf("\t- glEnableVertexAttribArray\n");
    printf("\t- glVertexAttribPointer\n");
    printf("\t- glDrawArrays\n");
    printf("\t- glDrawElements\n");
    printf("\t- glGenTextures\n");
    printf("\t- glBindTexture\n");
    printf("\t- glActiveTexture\n");
    printf("\t- glTexParameteri\n");
    printf("\t- glTexImage2D\n");
    printf("\t- glGenerateMipmap\n");
    printf("\t- glDeleteTextures\n");
    printf("\t- glGetUniformLocation\n");
    printf("\t- glUniform1i\n");
    printf("\t- glUniform1f\n");
    printf("\t- glUniform2f\n");
    printf("\t- glUniform3f\n");
    printf("\t- glUniform4f\n");
    printf("\t- glUniformMatrix4fv\n");
}

void FreeGL()
{
    dlclose(GLLibrary);
}
