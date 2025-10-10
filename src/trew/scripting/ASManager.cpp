#include "ASManager.hpp"
#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <fmt/core.h>
#include <trew/Logger.hpp>
#include <trew/AssetManager.hpp>
#include <trew/drawables/impl_opengl/GLTexture.hpp>
#include <trew/nodes/Sprite.hpp>
#include <trew/Globals.hpp>
#include <trew/Camera.hpp>
#include <cassert>
#include <memory>

using namespace trew;

#define LOGTAG "AngelScript"

namespace ASFunction {
    void print(const char* msg) {
        fmt::print("{}\n", msg);
    }

    void loadTexture(const char* path) {
        auto assets = Globals::assets;
        assets->load(path, AssetType::TEXTURE);
        if (auto texture = assets->getTexture(path); auto tex = texture) {
            if (auto shader = assets->getShader("default"); auto sh = shader) {
                tex->setShader(sh);
            }
            auto cam = Globals::camera;
            tex->setCamera(cam);
        }
    }
    
    void createSprite(const char* texturePath) {
        auto tex = Globals::assets->getTexture(texturePath);
        auto circleSprite = std::make_unique<Sprite>(tex);
        circleSprite->setXY(1100, 0);
        circleSprite->setColor(Color(1, 1, 1, 1));
        Globals::sprites.push_back(std::move(circleSprite));
    }
}

static void MessageCallback(const asSMessageInfo* msg, void* param) {
    const char* type = "UNKNOWN";
    if (msg->type == asMSGTYPE_ERROR) {
        type = "ERROR";
    } else if (msg->type == asMSGTYPE_WARNING) {
        type = "WARN";
    } else if (msg->type == asMSGTYPE_INFORMATION) {
        type = "INFO";
    }
    logError(LOGTAG, fmt::format("{} ({}, {}) : {} : {}", msg->section, msg->row, msg->col, type, msg->message));
}

ASManager::ASManager(AssetManager* assets, Camera* cam) :
    assets(assets),
    cam(cam)
{
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    RegisterStdString(engine);
    r = engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(ASFunction::print), asCALL_CDECL);
    r = engine->RegisterGlobalFunction("void loadTexture(const string& in)", asFUNCTION(ASFunction::loadTexture), asCALL_CDECL);
    r = engine->RegisterGlobalFunction("void createSprite(const string& in)", asFUNCTION(ASFunction::createSprite), asCALL_CDECL);
    assert(r >= 0);
}

void ASManager::registerScript(const char* path) {
    CScriptBuilder builder;
    int r = builder.StartNewModule(engine, path);
    if (r < 0)
    {
        // If the code fails here it is usually because there
        // is no more memory to allocate the module
        logError(LOGTAG, "Unrecoverable error while starting a new module.");
        return;
    }
    r = builder.AddSectionFromFile(path);
    if (r < 0)
    {
        // The builder wasn't able to load the file. Maybe the file
        // has been removed, or the wrong name was given, or some
        // preprocessing commands are incorrectly written.
        logError(LOGTAG, "Please correct the errors in the script and try again.");
        return;
    }
    r = builder.BuildModule();
    if (r < 0)
    {
        // An error occurred. Instruct the script writer to fix the 
        // compilation errors that were listed in the output stream.
        logError(LOGTAG, "Please correct the errors in the script and try again.");
        return;
    }
}

void ASManager::runScript(const char* path) {
    // Find the function that is to be called. 
    asIScriptModule* mod = engine->GetModule(path);
    asIScriptFunction* func = mod->GetFunctionByDecl("void main()");
    if (func == 0)
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        logError(LOGTAG, "The script must have the function 'void main()'. Please add it and try again.");
        return;
    }

    // Create our context, prepare it, and then execute
    asIScriptContext* ctx = engine->CreateContext();
    ctx->Prepare(func);
    int r = ctx->Execute();
    if (r != asEXECUTION_FINISHED)
    {
        // The execution didn't complete as expected. Determine what happened.
        if (r == asEXECUTION_EXCEPTION)
        {
            // An exception occurred, let the script writer know what happened so it can be corrected.
            logError(LOGTAG, fmt::format("An exception '{}' occurred. Please correct the code and try again.", ctx->GetExceptionString()));
        }
    }
}