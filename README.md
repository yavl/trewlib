# Trewlib
A very tiny cross-platform 2D game library I plan to use for my dream 2D game for Windows & macOS. This is not supposed to be a general-purpose game library. Very WIP, not recommended for use at this time.
### Features
* OpenGL 3.3 renderer (SDL3 GPU renderer planned)
* Compiles on Windows, macOS, Linux
* AngelScript as a scripting language (WIP)
* Does nothing except drawing sprites, text, loading shaders and running scripts
### Dependencies
SDL3, stb_image, glm, glad, fmt
### Building on Windows
Go to `build` folder and run `./generateWin64.bat` (CMake and Visual Studio 2022 must be installed).
### Building on macOS
```
brew install sdl3
brew install freetype
```
Go to `build` folder and run `cmake . -G Xcode` (XCode and command-line tools must be installed). Open and build `Trewlib.xcodeproj`, copy `build/assets` folder next to an executable and run.
### Example
```c++
#include <trew/app/Application.hpp>
#include <trew/drawables/impl_opengl/GLTexture.hpp>
#include <trew/nodes/Sprite.hpp>
#include <trew/Camera.hpp>
#include <trew/Shader.hpp>
#include <trew/InputManager.hpp>
#include <trew/app/SdlWindow.hpp>
#include <trew/AssetManager.hpp>
#include <trew/Logger.hpp>
#include <trew/actions/MoveAction.hpp>
#include <trew/drawables/impl_opengl/GLText.hpp>
#include <trew/scripting/ASManager.hpp>
#include <trew/Color.hpp>
#include <trew/Globals.hpp>
#include <trew/FileHandle.hpp>
#include <nlohmann/json.hpp>
#include <SDL3/SDL.h>

using namespace trew;
using namespace nlohmann;

const char* assetsRootDirectory = "assets";

class HelloWorld : public Application {
public:
    HelloWorld(std::weak_ptr<Window> window) {
        this->window = window;
        Globals::camera = std::make_shared<Camera>(window);
        cam = Globals::camera;
        input = std::make_unique<InputManager>(window);
    }

    ~HelloWorld() {}

    void create() override {
        Globals::assets = std::make_shared<AssetManager>(assetsRootDirectory);;
        assets = Globals::assets;
        assets->load("default", AssetType::SHADER);
        assets->load("text", AssetType::SHADER);

        ASManager as(assets);
        as.registerScript("assets/scripts/main.as");
        as.runScript("assets/scripts/main.as");

        auto sh = assets->getShader("default").value();
        auto tex = assets->getTexture("tex.png").value();
        sprite = std::make_unique<Sprite>(tex);

        auto tex2 = assets->getTexture("tex2.png").value();
        auto sprite2 = new Sprite(tex2);
        sprite2->setXY(400, 300);
        sprite->addChild(sprite2);
        sprite->setRotation(-45.f);
        //sprite->addAction(new MoveAction(0.f, 200.f, 2.f));

        window.lock()->addWindowResizeCallback([this](int width, int height) {
            resize(width, height);
            });

        auto textSh = assets->getShader("text").value();
        text = std::make_unique<Text>(textSh, cam.get());

        auto circleTex = assets->getTexture("circle.png").value();

        {
            auto circleSprite = std::make_unique<Sprite>(circleTex);
            circleSprite->setXY(-1000, 0);
            circleSprite->setColor(Color(0, 0, 1, 1));
            Globals::sprites.push_back(std::move(circleSprite));
        }

        {
            auto circleSprite = std::make_unique<Sprite>(circleTex);
            circleSprite->setXY(-800, 0);
            circleSprite->setColor(Color(1, 1, 0, 1));
            Globals::sprites.push_back(std::move(circleSprite));
        }

        {
            auto circleSprite = std::make_unique<Sprite>(circleTex);
            circleSprite->setXY(-600, 0);
            circleSprite->setColor(Globals::sprites[0].get()->getColor() + Globals::sprites[1].get()->getColor());
            Globals::sprites.push_back(std::move(circleSprite));
        }

        window.lock()->addMouseButtonCallback([=](int button) {
            auto cursorPos = window.lock()->getCursorPos();
            auto world = cam->screenToSpace(cursorPos.x, cursorPos.y);
            auto moveAction = new MoveAction(world.x, world.y, 1.f);
            Globals::sprites[0]->addAction(moveAction);
            log("mouseClick", fmt::format("World pos: {}, {}", world.x, world.y));
            });
    }
    void update(float dt) override {
        input->update();
        cam->update(dt);
        sprite->act(dt);
        for (const auto& sprite : Globals::sprites) {
            sprite.get()->act(dt);
        }
    }

    void render() override {
        glClearColor(0.f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sprite->draw();
        for (const auto& sprite : Globals::sprites) {
            sprite.get()->draw();
        }

        float offset = 100.f;
        float prevY = 200.f;
        for (auto& textStr : texts) {
            text->draw(textStr, -1000.f, prevY + offset, 1.f, glm::vec3(1.f, 1.f, 1.f));
            prevY += offset;
        }
        window.lock()->swapBuffers();
    }

    void resize(int width, int height) override {
        glViewport(0, 0, width, height);
        render();
    }
private:
    std::weak_ptr<Window> window;
    std::shared_ptr<AssetManager> assets;
    std::shared_ptr<Camera> cam;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Text> text;
    std::vector<std::string> texts;
};

int main() {
    Logger::getInstance().setLogLevel(LogLevel::LOG_DEBUG);

    auto file = FileHandle("assets/properties.json").asString();
    json j = json::parse(file);
    auto windowProperties = j["window"];
    int width = windowProperties["width"].get<int>();
    int height = windowProperties["height"].get<int>();
    std::string title = windowProperties["title"].get<std::string>();

    auto window = std::make_shared<SdlWindow>();
    window->createWindow(title, width, height);

    auto m = std::make_unique<HelloWorld>(window);
    //auto m = std::make_unique<HelloSDL3GPU>(window);
    m->create();
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    float dt = 0;
    while (!window->shouldClose()) {
        last = now;
        now = SDL_GetPerformanceCounter();
        dt = static_cast<float>((now - last) / static_cast<float>(SDL_GetPerformanceFrequency()));
        m->update(static_cast<float>(dt));
        m->render();
    }
}
```
