//
//  Renderer.hpp
//  Trewlib
//
//  Created by Vladislav Nikolaev on 22.07.2025.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

struct SDL_GPUDevice;
struct SDL_Window;
struct SDL_GPUGraphicsPipeline;
struct SDL_GPUCommandBuffer;
struct SDL_GPUBuffer;
struct SDL_GPUTexture;
struct SDL_GPUSampler;
struct ImFont;

namespace trew {
class Hud;
class Camera;
class AssetManager;
class ImageSurface;
struct Context {
    SDL_GPUDevice* device;
    SDL_Window* window;
};

class Renderer {
public:
    Renderer(SDL_GPUDevice* device, SDL_Window* window, Camera* cam, AssetManager* assets);
    ~Renderer();
    void init();
    void render();
    void render(Hud* hud);
    void clearScreen();
    void drawTriangle(float x, float y, float width, float height);
    void drawRectangle(float x, float y, float width, float height);
    void drawTexture(float x, float y, ImageSurface* surface, float rotation, std::optional<glm::mat4> parentModelMatrix = std::nullopt);
    void submit();
private:
    SDL_GPUGraphicsPipeline* createTrianglePipeline();
    SDL_GPUGraphicsPipeline* createRectanglePipeline();
    SDL_GPUGraphicsPipeline* createTexturePipeline();
    Context context;
    SDL_GPUDevice* device;
    SDL_Window* window;
    SDL_GPUGraphicsPipeline* trianglePipeline = nullptr;
    SDL_GPUGraphicsPipeline* rectanglePipeline = nullptr;
    SDL_GPUGraphicsPipeline* texturePipeline = nullptr;
    SDL_GPUCommandBuffer* commandBuffer = nullptr;
    float font_size = 24.0f;
    ImFont* font;
    SDL_GPUTexture* swapchainTexture = nullptr;
    Camera* cam;
    AssetManager* assets;

    // Texture pipeline

};
}

static SDL_GPUBuffer* vertexBuffer = nullptr;
static SDL_GPUBuffer* indexBuffer = nullptr;
static SDL_GPUTexture* texture = nullptr;
static SDL_GPUSampler* sampler = nullptr;