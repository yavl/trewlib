#pragma once

#include <trew/trew.hpp>
#include <trew/Asset.hpp>
#include <string>
#include <SDL3/SDL.h>

namespace trew {
SDL_GPUShader* loadShader(SDL_GPUDevice* device,
                          const char* assetsDirectory,
                          const char* shaderFilename,
                          Uint32 samplerCount,
                          Uint32 uniformBufferCount,
                          Uint32 storageBufferCount,
                          Uint32 storageTextureCount
                          );
}
