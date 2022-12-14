#include "ellie.h"
#include "ellie_rhi.h"

#include "glad/glad.h"

typedef struct game_state {
    rhi_shader ForwardShader;
    rhi_input_layout InputLayout;
    rhi_buffer VertexBuffer;
    rhi_buffer IndexBuffer;
    rhi_texture Texture;
} game_state;

static game_state GameState;

static const f32 Vertices[] = {
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f
};

static const u32 Indices[] = {
    0, 1, 3,
    1, 2, 3
};
    
void GameInit()
{
    RHIShaderInit(&GameState.ForwardShader, "Assets/Shaders/ForwardVertex.glsl", "Assets/Shaders/ForwardFragment.glsl");
    RHITextureLoad(&GameState.Texture, "Assets/Textures/epitech.png");
    
    RHIInputLayoutInit(&GameState.InputLayout);
    RHIInputLayoutBind(&GameState.InputLayout);

    RHIBufferInit(&GameState.VertexBuffer, RHIBufferUsage_Vertex, sizeof(Vertices));
    RHIBufferInit(&GameState.IndexBuffer, RHIBufferUsage_Index, sizeof(Indices));
    RHIBufferUpload(&GameState.VertexBuffer, sizeof(Vertices), Vertices);
    RHIBufferUpload(&GameState.IndexBuffer, sizeof(Indices), Indices);

    RHIInputLayoutAdd(&GameState.InputLayout, 0, 3, sizeof(f32) * 8, 0);
    RHIInputLayoutAdd(&GameState.InputLayout, 1, 3, sizeof(f32) * 8, 3 * sizeof(f32));
    RHIInputLayoutAdd(&GameState.InputLayout, 2, 2, sizeof(f32) * 8, 6 * sizeof(f32));
}

void GameUpdate(f32 DeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    RHIShaderBind(&GameState.ForwardShader);

    RHITextureBind(&GameState.Texture, 0);
    RHIShaderUniformInt(&GameState.ForwardShader, "Texture", 0);
    
    RHIInputLayoutBind(&GameState.InputLayout);
    RHIBufferBind(&GameState.VertexBuffer);
    RHIBufferBind(&GameState.IndexBuffer);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void GameExit()
{
    RHIShaderFree(&GameState.ForwardShader);
}

void GameResize(u32 Width, u32 Height)
{
    
}
