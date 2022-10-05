#include "ellie_rhi.h"
#include "ellie_platform.h"

#include <stdlib.h>
#include <stdio.h>
#include "glad/glad.h"

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
    
    glGenBuffers(1, &Buffer->Handle);
    glBindBuffer(RHIBufferUsageToGL(Usage), Buffer->Handle);
    glBufferData(RHIBufferUsageToGL(Usage), size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(RHIBufferUsageToGL(Usage), 0);
}

void RHIBufferFree(rhi_buffer* Buffer)
{
    glDeleteBuffers(1, &Buffer->Handle);
}

void RHIBufferUpload(rhi_buffer* Buffer, u64 Size, const void* Data)
{
    glBindBuffer(RHIBufferUsageToGL(Buffer->Usage), Buffer->Handle);
    glBufferSubData(RHIBufferUsageToGL(Buffer->Usage), 0, Size, Data);
}

void RHIBufferBind(rhi_buffer* Buffer)
{
    glBindBuffer(RHIBufferUsageToGL(Buffer->Usage), Buffer->Handle);
}

void RHIShaderInit(rhi_shader* Shader, const char* Vertex, const char* Fragment)
{
    i32 VertexShaderSize = 0;
    i32 FragmentShaderSize = 0;
    char* VertexSource = PlatformReadFile(Vertex, &VertexShaderSize);
    char* FragmentSource = PlatformReadFile(Fragment, &FragmentShaderSize);
    
    u32 VertexShader, FragmentShader = 0;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, (const GLchar**)&VertexSource, NULL);
    glCompileShader(VertexShader);
    {
        i32 Success = 0;
        char InfoLog[1024] = {0};

        glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            glGetShaderInfoLog(VertexShader, 1024, NULL, InfoLog);
            printf("Failed to compile vertex shader!\n");
            printf("%s\n", InfoLog);
        }
    }
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, (const GLchar**)&FragmentSource, NULL);
    glCompileShader(FragmentShader);
    {
        i32 Success = 0;
        char InfoLog[1024] = {0};

        glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            glGetShaderInfoLog(FragmentShader, 1024, NULL, InfoLog);
            printf("Failed to compile fragment shader!\n");
            printf("%s\n", InfoLog);
        }
    }

    Shader->Handle = glCreateProgram();
    glAttachShader(Shader->Handle, VertexShader);
    glAttachShader(Shader->Handle, FragmentShader);
    glLinkProgram(Shader->Handle);
    {
        i32 Success = 0;
        char InfoLog[1024] = {0};

        glGetProgramiv(Shader->Handle, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            glGetProgramInfoLog(Shader->Handle, 1024, NULL, InfoLog);
            printf("Failed to link shader program!\n");
            printf("%s\n", InfoLog);
        }
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    free(VertexSource);
    free(FragmentSource);
}

void RHIShaderFree(rhi_shader* Shader)
{
    glDeleteProgram(Shader->Handle);
}

void RHIShaderBind(rhi_shader* Shader)
{
    glUseProgram(Shader->Handle);
}

void RHIShaderUniformInt(rhi_shader* Shader, const char* Uniform, i32 Value)
{
    u32 Location = glGetUniformLocation(Shader->Handle, Uniform);
    glUniform1i(Location, Value);
}

void RHIShaderUniformFloat(rhi_shader* Shader, const char* Uniform, f32 Value)
{
    u32 Location = glGetUniformLocation(Shader->Handle, Uniform);
    glUniform1f(Location, Value);
}

void RHIShaderUniformVec2(rhi_shader* Shader, const char* Uniform, hmm_vec2 Value)
{
    u32 Location = glGetUniformLocation(Shader->Handle, Uniform);
    glUniform2f(Location, Value.X, Value.Y);
}

void RHIShaderUniformVec3(rhi_shader* Shader, const char* Uniform, hmm_vec3 Value)
{
    u32 Location = glGetUniformLocation(Shader->Handle, Uniform);
    glUniform3f(Location, Value.X, Value.Y, Value.Z);
}

void RHIShaderUniformVec4(rhi_shader* Shader, const char* Uniform, hmm_vec4 Value)
{
    u32 Location = glGetUniformLocation(Shader->Handle, Uniform);
    glUniform4f(Location, Value.X, Value.Y, Value.Z, Value.W);
}

void RHIShaderUniformMat4(rhi_shader* Shader, const char* Uniform, hmm_mat4 Value)
{
    u32 Location = glGetUniformLocation(Shader->Handle, Uniform);
    glUniformMatrix4fv(Location, 1, GL_FALSE, (const GLfloat*)Value.Elements);
}

void RHIInputLayoutInit(rhi_input_layout* InputLayout)
{
    glGenVertexArrays(1, &InputLayout->Handle);
}

void RHIInputLayoutFree(rhi_input_layout* InputLayout)
{
    glDeleteVertexArrays(1, &InputLayout->Handle);
}

void RHIInputLayoutBind(rhi_input_layout* InputLayout)
{
    glBindVertexArray(InputLayout->Handle);
}

void RHIInputLayoutAdd(rhi_input_layout* InputLayout, i32 Index, u32 ElementCount, i64 Stride, u64 Offset)
{
    glBindVertexArray(InputLayout->Handle);
    glEnableVertexAttribArray(Index);
    glVertexAttribPointer(Index, ElementCount, GL_FLOAT, GL_FALSE, Stride, (void*)Offset);
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
    
    glGenTextures(1, &Texture->Handle);
    glBindTexture(GL_TEXTURE_2D, Texture->Handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture->Width, Texture->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(Data);
}

void RHITextureFree(rhi_texture* Texture)
{
    glDeleteTextures(1, &Texture->Handle);
}

void RHITextureBind(rhi_texture* Texture, u32 Slot)
{
    glActiveTexture(GL_TEXTURE0 + Slot);
    glBindTexture(GL_TEXTURE_2D, Texture->Handle);
}
