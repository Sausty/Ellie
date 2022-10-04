#include "rhi.h"
#include "gl_loader.h"
#include "platform.h"

#include <stdlib.h>
#include <stdio.h>

GLenum RHIBufferUsageToGL(rhi_buffer_usage Usage)
{
    switch (Usage)
    {
    case RHIBufferUsage_Vertex: return GL_ARRAY_BUFFER;
    case RHIBufferUsage_Index:  return GL_ELEMENT_ARRAY_BUFFER;
    }

    return 0;
}

void RHIBufferInit(rhi_buffer* Buffer, rhi_buffer_usage Usage, u64 size)
{
    Buffer->Usage = Usage;
    
    gl.GenBuffers(1, &Buffer->Handle);
    gl.BindBuffer(RHIBufferUsageToGL(Usage), Buffer->Handle);
    gl.BufferData(RHIBufferUsageToGL(Usage), size, NULL, GL_DYNAMIC_DRAW);
    gl.BindBuffer(RHIBufferUsageToGL(Usage), 0);
}

void RHIBufferFree(rhi_buffer* Buffer)
{
    gl.DeleteBuffers(1, &Buffer->Handle);
}

void RHIBufferUpload(rhi_buffer* Buffer, u64 Size, const void* Data)
{
    gl.BindBuffer(RHIBufferUsageToGL(Buffer->Usage), Buffer->Handle);
    gl.BufferSubData(RHIBufferUsageToGL(Buffer->Usage), 0, Size, Data);
}

void RHIBufferBind(rhi_buffer* Buffer)
{
    gl.BindBuffer(RHIBufferUsageToGL(Buffer->Usage), Buffer->Handle);
}

void RHIShaderInit(rhi_shader* Shader, const char* Vertex, const char* Fragment)
{
    i32 VertexShaderSize = 0;
    i32 FragmentShaderSize = 0;
    char* VertexSource = ReadFile(Vertex, &VertexShaderSize);
    char* FragmentSource = ReadFile(Fragment, &FragmentShaderSize);
    
    u32 VertexShader, FragmentShader = 0;
    VertexShader = gl.CreateShader(GL_VERTEX_SHADER);
    gl.ShaderSource(VertexShader, 1, (const GLchar**)&VertexSource, NULL);
    gl.CompileShader(VertexShader);
    {
        i32 Success = 0;
        char InfoLog[1024] = {0};

        gl.GetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            gl.GetShaderInfoLog(VertexShader, 1024, NULL, InfoLog);
            printf("Failed to compile vertex shader!\n");
            printf("%s\n", InfoLog);
        }
    }
    FragmentShader = gl.CreateShader(GL_FRAGMENT_SHADER);
    gl.ShaderSource(FragmentShader, 1, (const GLchar**)&FragmentSource, NULL);
    gl.CompileShader(FragmentShader);
    {
        i32 Success = 0;
        char InfoLog[1024] = {0};

        gl.GetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            gl.GetShaderInfoLog(FragmentShader, 1024, NULL, InfoLog);
            printf("Failed to compile fragment shader!\n");
            printf("%s\n", InfoLog);
        }
    }

    Shader->Handle = gl.CreateProgram();
    gl.AttachShader(Shader->Handle, VertexShader);
    gl.AttachShader(Shader->Handle, FragmentShader);
    gl.LinkProgram(Shader->Handle);
    {
        i32 Success = 0;
        char InfoLog[1024] = {0};

        gl.GetProgramiv(Shader->Handle, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            gl.GetProgramInfoLog(Shader->Handle, 1024, NULL, InfoLog);
            printf("Failed to link shader program!\n");
            printf("%s\n", InfoLog);
        }
    }

    gl.DeleteShader(VertexShader);
    gl.DeleteShader(FragmentShader);

    free(VertexSource);
    free(FragmentSource);
}

void RHIShaderFree(rhi_shader* Shader)
{
    gl.DeleteProgram(Shader->Handle);
}

void RHIShaderBind(rhi_shader* Shader)
{
    gl.UseProgram(Shader->Handle);
}

void RHIInputLayoutInit(rhi_input_layout* InputLayout)
{
    gl.GenVertexArrays(1, &InputLayout->Handle);
}

void RHIInputLayoutFree(rhi_input_layout* InputLayout)
{
    gl.DeleteVertexArrays(1, &InputLayout->Handle);
}

void RHIInputLayoutBind(rhi_input_layout* InputLayout)
{
    gl.BindVertexArray(InputLayout->Handle);
}

void RHIInputLayoutAdd(rhi_input_layout* InputLayout, i32 Index, u32 ElementCount, i64 Stride, u64 Offset)
{
    gl.BindVertexArray(InputLayout->Handle);
    gl.EnableVertexAttribArray(Index);
    gl.VertexAttribPointer(Index, ElementCount, GL_FLOAT, GL_FALSE, Stride, (void*)Offset);
}
