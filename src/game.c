#include "game.h"

#include "gl_loader.h"
#include "rhi.h"

typedef struct game_state {
    rhi_shader ForwardShader;
    rhi_input_layout InputLayout;
    rhi_buffer VertexBuffer;
    rhi_buffer IndexBuffer;
} game_state;

internal game_state GameState;

global const f32 Vertices[] = {
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 
    -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f 
};

global const u32 Indices[] = {
    0, 1, 3,
    1, 2, 3
};
    
void GameInit()
{
    RHIShaderInit(&GameState.ForwardShader, "Assets/Shaders/ForwardVertex.glsl", "Assets/Shaders/ForwardFragment.glsl");

    RHIInputLayoutInit(&GameState.InputLayout);
    RHIInputLayoutBind(&GameState.InputLayout);

    RHIBufferInit(&GameState.VertexBuffer, RHIBufferUsage_Vertex, sizeof(Vertices));
    RHIBufferInit(&GameState.IndexBuffer, RHIBufferUsage_Index, sizeof(Indices));
    RHIBufferUpload(&GameState.VertexBuffer, sizeof(Vertices), Vertices);
    RHIBufferUpload(&GameState.IndexBuffer, sizeof(Indices), Indices);

    RHIInputLayoutAdd(&GameState.InputLayout, 0, 3, sizeof(f32) * 6, 0);
    RHIInputLayoutAdd(&GameState.InputLayout, 1, 3, sizeof(f32) * 6, 3 * sizeof(f32));
}

void GameUpdate(f32 DeltaTime)
{
    gl.Clear(GL_COLOR_BUFFER_BIT);
    gl.ClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    RHIShaderBind(&GameState.ForwardShader);
    RHIInputLayoutBind(&GameState.InputLayout);
    RHIBufferBind(&GameState.VertexBuffer);
    RHIBufferBind(&GameState.IndexBuffer);
    gl.DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameExit()
{
    RHIShaderFree(&GameState.ForwardShader);
}

void GameResize(u32 Width, u32 Height)
{
    
}
