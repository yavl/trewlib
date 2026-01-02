//
//  Renderer.hpp
//  Trewlib
//
//  Created by Vladislav Nikolaev on 22.07.2025.
//

#pragma once

#include <trew/trew.hpp>
#include <optional>
#include <unordered_map>
#include <trew/Color.hpp>
#include <trew/graphics/GraphicsTypes.hpp>
#include <memory>

struct SDL_GPUDevice;
struct SDL_Window;
struct SDL_GPUGraphicsPipeline;
struct SDL_GPUCommandBuffer;
struct SDL_GPUBuffer;
struct SDL_GPUTexture;
struct SDL_GPUSampler;
struct ImFont;

namespace trew {
class BaseUI;
class Camera;
class AssetManager;
class ImageSurface;
class TextRenderer;
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
    void clearScreen();
    void drawTriangle(float x, float y, float width, float height);
    void drawRectangle(float x, float y, float width, float height);
    void drawTexture(float x, float y, float width, float height, SDL_GPUTexture* texture, float rotation, float scale, std::optional<Color> color = std::nullopt, std::optional<glm::mat4> parentModelMatrix = std::nullopt, FilterMode filterMode = FilterMode::NEAREST);
    void drawText(const char* str, float x, float y, FontSize fontSize, float rotation);
    void submit();
    SDL_GPUTexture* getTexture(ImageSurface* image);
    void prepareTexture(const char* name);
    SDL_GPUTexture* getSwapchainTexture() const;
    SDL_GPUCommandBuffer* getCommandBuffer() const;
private:
    std::unique_ptr<TextRenderer> textRenderer;
    SDL_GPUGraphicsPipeline* createTrianglePipeline();
    SDL_GPUGraphicsPipeline* createRectanglePipeline();
    SDL_GPUGraphicsPipeline* createTexturePipeline();
    SDL_GPUTexture* createTexture(ImageSurface* image);
    Context context;
    SDL_GPUGraphicsPipeline* trianglePipeline = nullptr;
    SDL_GPUGraphicsPipeline* rectanglePipeline = nullptr;
    SDL_GPUGraphicsPipeline* texturePipeline = nullptr;
    SDL_GPUCommandBuffer* commandBuffer = nullptr;
    float font_size = 24.0f;
    ImFont* font;
    SDL_GPUTexture* swapchainTexture = nullptr;
    Camera* cam;
    AssetManager* assets;
    std::unordered_map<ImageSurface*, SDL_GPUTexture*> textures;

    // Texture pipeline

    SDL_GPUBuffer* vertexBuffer = nullptr;
    SDL_GPUBuffer* indexBuffer = nullptr;
    SDL_GPUSampler* nearestSampler = nullptr;
    SDL_GPUSampler* linearSampler = nullptr;
};
}
