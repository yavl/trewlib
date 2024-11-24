#include "ASManager.hpp"
#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <fmt/core.h>
#include <cassert>

using namespace trew;

namespace ASFunction {
    void printee(std::string& msg) {
        fmt::print("{}\n", msg.c_str());
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

    fmt::print("[ASManager] {} ({}, {}) : {} : {}\n", msg->section, msg->row, msg->col, type, msg->message);
}

ASManager::ASManager() {
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    RegisterStdString(engine);
    r = engine->RegisterGlobalFunction("void printee(const string& in)", asFUNCTION(ASFunction::printee), asCALL_CDECL);
    assert(r >= 0);
}

void ASManager::registerScript(const char* path) {
    CScriptBuilder builder;
    int r = builder.StartNewModule(engine, path);
    if (r < 0)
    {
        // If the code fails here it is usually because there
        // is no more memory to allocate the module
        fmt::print("Unrecoverable error while starting a new module.\n");
        return;
    }
    r = builder.AddSectionFromFile(path);
    if (r < 0)
    {
        // The builder wasn't able to load the file. Maybe the file
        // has been removed, or the wrong name was given, or some
        // preprocessing commands are incorrectly written.
        fmt::print("Please correct the errors in the script and try again.\n");
        return;
    }
    r = builder.BuildModule();
    if (r < 0)
    {
        // An error occurred. Instruct the script writer to fix the 
        // compilation errors that were listed in the output stream.
        fmt::print("Please correct the errors in the script and try again.\n");
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
        fmt::print("The script must have the function 'void main()'. Please add it and try again.\n");
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
            fmt::print("An exception '{}' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
        }
    }
}