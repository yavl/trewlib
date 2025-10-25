#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "testgputext/SDL_math3d.h"
#include <memory>

#define MAX_VERTEX_COUNT 4000
#define MAX_INDEX_COUNT  6000
#define SUPPORTED_SHADER_FORMATS (SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL)

namespace trew {
    typedef enum
    {
        VertexShader,
        PixelShader,
        PixelShader_SDF,
    } Shader;

    typedef SDL_FPoint Vec2;

    typedef struct Vec3
    {
        float x, y, z;
    } Vec3;

    typedef struct Vertex
    {
        Vec3 pos;
        SDL_FColor colour;
        Vec2 uv;
    } Vertex;

    typedef struct Context
    {
        SDL_GPUDevice* device;
        SDL_Window* window;
        SDL_GPUGraphicsPipeline* pipeline;
        SDL_GPUBuffer* vertex_buffer;
        SDL_GPUBuffer* index_buffer;
        SDL_GPUTransferBuffer* transfer_buffer;
        SDL_GPUSampler* sampler;
        SDL_GPUCommandBuffer* cmd_buf;
    } Context;

    typedef struct GeometryData
    {
        Vertex* vertices;
        int vertex_count;
        int* indices;
        int index_count;
    } GeometryData;

    class TextRenderer {
    public:
        TextRenderer();
        ~TextRenderer();
    private:
        Context context{};
        SDL_GPUShader* load_shader(
            SDL_GPUDevice* device,
            Shader shader,
            Uint32 sampler_count,
            Uint32 uniform_buffer_count,
            Uint32 storage_buffer_count,
            Uint32 storage_texture_count);
        void queue_text_sequence(GeometryData* geometry_data, TTF_GPUAtlasDrawSequence* sequence, SDL_FColor* colour);
        void queue_text(GeometryData* geometry_data, TTF_GPUAtlasDrawSequence* sequence, SDL_FColor* colour);
        void set_geometry_data(Context* context, GeometryData* geometry_data);
        void transfer_data(Context* context, GeometryData* geometry_data);
        void draw(Context* context, SDL_Mat4X4* matrices, int num_matrices, TTF_GPUAtlasDrawSequence* draw_sequence);
        void DOIT();
    };
}