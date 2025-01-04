#include "ASManager.hpp"
#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <fmt/core.h>
#include <trew/Logger.hpp>
#include <trew/AssetManager.hpp>
#include <trew/drawables/impl_glfw/Texture.hpp>
#include <cassert>
#include "Globals.hpp"

using namespace trew;

const std::string logTag = "AngelScript";

namespace ASFunction {
    void print(const std::string& msg) {
        fmt::print("{}\n", msg.c_str());
    }

    void loadTexture(const std::string& path) {
        auto assets = Globals::assets.get();
        assets->load(path, AssetType::TEXTURE);
        if (auto texture = assets->getTexture(path); auto tex = texture.value()) {
            if (auto shader = assets->getShader("default"); auto sh = shader.value()) {
                tex->setShader(sh);
            }
            auto cam = Globals::camera.get();
            tex->setCamera(cam);
        }
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
    logError(logTag, fmt::format("{} ({}, {}) : {} : {}", msg->section, msg->row, msg->col, type, msg->message));
}

ASManager::ASManager(std::shared_ptr<AssetManager> assets): assets(assets) {
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    RegisterStdString(engine);
    r = engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(ASFunction::print), asCALL_CDECL);
    r = engine->RegisterGlobalFunction("void loadTexture(const string& in)", asFUNCTION(ASFunction::loadTexture), asCALL_CDECL);
    assert(r >= 0);
}

void ASManager::registerScript(const char* path) {
    CScriptBuilder builder;
    int r = builder.StartNewModule(engine, path);
    if (r < 0)
    {
        // If the code fails here it is usually because there
        // is no more memory to allocate the module
        logError(logTag, "Unrecoverable error while starting a new module.");
        return;
    }
    r = builder.AddSectionFromFile(path);
    if (r < 0)
    {
        // The builder wasn't able to load the file. Maybe the file
        // has been removed, or the wrong name was given, or some
        // preprocessing commands are incorrectly written.
        logError(logTag, "Please correct the errors in the script and try again.");
        return;
    }
    r = builder.BuildModule();
    if (r < 0)
    {
        // An error occurred. Instruct the script writer to fix the 
        // compilation errors that were listed in the output stream.
        logError(logTag, "Please correct the errors in the script and try again.");
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
        logError(logTag, "The script must have the function 'void main()'. Please add it and try again.");
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
            logError(logTag, fmt::format("An exception '{}' occurred. Please correct the code and try again.", ctx->GetExceptionString()));
        }
    }
}