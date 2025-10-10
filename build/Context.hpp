//
//  Context.hpp
//  Trewlib
//
//  Created by Vladislav Nikolaev on 22.07.2025.
//

namespace trew {
struct Context {
    struct SDL_GPUDevice;
    struct SDL_Window;
    
    struct Context {
        SDL_GPUDevice* device;
        SDL_Window* window;
    };
}
}
