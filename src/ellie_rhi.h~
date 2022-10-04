#ifndef ELLIE_RHI_GL_H
#define ELLIE_RHI_GL_H

#include "common.h"

#include <HandmadeMath.h>

// TODO(milo): Shader uniforms with HandmadeMath
// TODO(milo): Textures

typedef enum rhi_buffer_usage {
    RHIBufferUsage_Vertex,
    RHIBufferUsage_Index
} rhi_buffer_usage;

typedef struct rhi_buffer {
    u32 Handle;
    rhi_buffer_usage Usage;
} rhi_buffer;

typedef struct rhi_shader {
    u32 Handle;
} rhi_shader;

typedef struct rhi_input_layout {
    u32 Handle;
} rhi_input_layout;

typedef struct rhi_texture {
    u32 Handle;
    i32 Width;
    i32 Height;
    i32 Channels;
} rhi_texture;

void RHIBufferInit(rhi_buffer* Buffer, rhi_buffer_usage Usage, u64 Size);
void RHIBufferFree(rhi_buffer* Buffer);
void RHIBufferUpload(rhi_buffer* Buffer, u64 Size, const void* Data);
void RHIBufferBind(rhi_buffer* Buffer);

void RHIShaderInit(rhi_shader* Shader, const char* Vertex, const char* Fragment);
void RHIShaderFree(rhi_shader* Shader);
void RHIShaderBind(rhi_shader* Shader);
void RHIShaderUniformInt(rhi_shader* Shader, const char* Uniform, i32 Value);
void RHIShaderUniformFloat(rhi_shader* Shader, const char* Uniform, f32 Value);
void RHIShaderUniformVec2(rhi_shader* Shader, const char* Uniform, hmm_vec2 Value);
void RHIShaderUniformVec3(rhi_shader* Shader, const char* Uniform, hmm_vec3 Value);
void RHIShaderUniformVec4(rhi_shader* Shader, const char* Uniform, hmm_vec4 Value);
void RHIShaderUniformMat4(rhi_shader* Shader, const char* Uniform, hmm_mat4 Value);

void RHIInputLayoutInit(rhi_input_layout* InputLayout);
void RHIInputLayoutFree(rhi_input_layout* InputLayout);
void RHIInputLayoutBind(rhi_input_layout* InputLayout);
void RHIInputLayoutAdd(rhi_input_layout* InputLayout, i32 Index, u32 ElementCount, i64 Stride, u64 Offset);

void RHITextureLoad(rhi_texture* Texture, const char* Path);
void RHITextureFree(rhi_texture* Texture);
void RHITextureBind(rhi_texture* Texture, u32 Slot);

#endif
