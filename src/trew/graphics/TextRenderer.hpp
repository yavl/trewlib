#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <trew/trew.hpp>

#define MAX_VERTEX_COUNT 4000
#define MAX_INDEX_COUNT  6000
#define SUPPORTED_SHADER_FORMATS (SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL)

namespace trew {
    enum TextShader {
        VertexShader,
        PixelShader,
        PixelShader_SDF,
    };

    typedef SDL_FPoint Vec2;

    struct Vec3 {
        float x, y, z;
    };

    struct Vertex {
        Vec3 pos;
        SDL_FColor colour;
        Vec2 uv;
    };

    struct TextContext {
        SDL_GPUDevice* device;
        SDL_Window* window;
        SDL_GPUGraphicsPipeline* pipeline;
        SDL_GPUBuffer* vertex_buffer;
        SDL_GPUBuffer* index_buffer;
        SDL_GPUTransferBuffer* transfer_buffer;
        SDL_GPUSampler* sampler;
        SDL_GPUCommandBuffer* commandBuffer;
        SDL_GPUTexture* swapchainTexture;
    };

    struct GeometryData {
        Vertex* vertices;
        int vertex_count;
        int* indices;
        int index_count;
    };

    class Camera;

    class TextRenderer {
    public:
        TextRenderer(SDL_GPUDevice* device, SDL_Window* window, Camera* cam);
        ~TextRenderer();
        void drawText(char str[]);

        TextContext context{};
    private:
        Camera* cam;
        GeometryData geometry_data = { 0 };
        TTF_Font* font;
        TTF_TextEngine* engine;
        TTF_Text* text;
        SDL_GPUShader* loadShader(
            SDL_GPUDevice* device,
            TextShader shader,
            Uint32 sampler_count,
            Uint32 uniform_buffer_count,
            Uint32 storage_buffer_count,
            Uint32 storage_texture_count);
        void queue_text_sequence(GeometryData* geometry_data, TTF_GPUAtlasDrawSequence* sequence, SDL_FColor* colour);
        void queue_text(GeometryData* geometry_data, TTF_GPUAtlasDrawSequence* sequence, SDL_FColor* colour);
        void set_geometry_data(TextContext* context, GeometryData* geometry_data);
        void transfer_data(TextContext* context, GeometryData* geometry_data);
        void draw(TextContext* context, glm::mat4 matrices[], int num_matrices, TTF_GPUAtlasDrawSequence* draw_sequence);
    };
}