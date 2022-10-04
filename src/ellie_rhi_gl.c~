#include "rhi.h"
#include "gl_loader.h"
#include "platform.h"

#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

void RHIShaderUniformInt(rhi_shader* Shader, const char* Uniform, i32 Value)
{
    u32 Location = gl.GetUniformLocation(Shader->Handle, Uniform);
    gl.Uniform1i(Location, Value);
}

void RHIShaderUniformFloat(rhi_shader* Shader, const char* Uniform, f32 Value)
{
    u32 Location = gl.GetUniformLocation(Shader->Handle, Uniform);
    gl.Uniform1f(Location, Value);
}

void RHIShaderUniformVec2(rhi_shader* Shader, const char* Uniform, hmm_vec2 Value)
{
    u32 Location = gl.GetUniformLocation(Shader->Handle, Uniform);
    gl.Uniform2f(Location, Value.X, Value.Y);
}

void RHIShaderUniformVec3(rhi_shader* Shader, const char* Uniform, hmm_vec3 Value)
{
    u32 Location = gl.GetUniformLocation(Shader->Handle, Uniform);
    gl.Uniform3f(Location, Value.X, Value.Y, Value.Z);
}

void RHIShaderUniformVec4(rhi_shader* Shader, const char* Uniform, hmm_vec4 Value)
{
    u32 Location = gl.GetUniformLocation(Shader->Handle, Uniform);
    gl.Uniform4f(Location, Value.X, Value.Y, Value.Z, Value.W);
}

void RHIShaderUniformMat4(rhi_shader* Shader, const char* Uniform, hmm_mat4 Value)
{
    u32 Location = gl.GetUniformLocation(Shader->Handle, Uniform);
    gl.UniformMatrix4fv(Location, 1, GL_FALSE, (const GLfloat*)Value.Elements);
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

void RHITextureLoad(rhi_texture* Texture, const char* Path)
{
    stbi_set_flip_vertically_on_load(true);
    u8* Data = stbi_load(Path, &Texture->Width, &Texture->Height, &Texture->Channels, STBI_rgb_alpha);
    if (!Data)
    {
        printf("Failed to load texture! %s", Path);
        return;
    }
    
    gl.GenTextures(1, &Texture->Handle);
    gl.BindTexture(GL_TEXTURE_2D, Texture->Handle);

    gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gl.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture->Width, Texture->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
    gl.GenerateMipmap(GL_TEXTURE_2D);

    free(Data);
}

void RHITextureFree(rhi_texture* Texture)
{
    gl.DeleteTextures(1, &Texture->Handle);
}

void RHITextureBind(rhi_texture* Texture, u32 Slot)
{
    gl.ActiveTexture(GL_TEXTURE0 + Slot);
    gl.BindTexture(GL_TEXTURE_2D, Texture->Handle);
}
