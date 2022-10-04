#ifndef ELLIE_RHI_GL_H
#define ELLIE_RHI_GL_H

#include "common.h"

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

void RHIBufferInit(rhi_buffer* Buffer, rhi_buffer_usage Usage, u64 Size);
void RHIBufferFree(rhi_buffer* Buffer);
void RHIBufferUpload(rhi_buffer* Buffer, u64 Size, const void* Data);
void RHIBufferBind(rhi_buffer* Buffer);

void RHIShaderInit(rhi_shader* Shader, const char* Vertex, const char* Fragment);
void RHIShaderFree(rhi_shader* Shader);
void RHIShaderBind(rhi_shader* Shader);

void RHIInputLayoutInit(rhi_input_layout* InputLayout);
void RHIInputLayoutFree(rhi_input_layout* InputLayout);
void RHIInputLayoutBind(rhi_input_layout* InputLayout);
void RHIInputLayoutAdd(rhi_input_layout* InputLayout, i32 Index, u32 ElementCount, i64 Stride, u64 Offset);

#endif
