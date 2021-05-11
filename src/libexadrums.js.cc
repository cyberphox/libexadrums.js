#include "libexadrums.js.h"
#include <string>

using namespace Napi;

LibexadrumsJs::LibexadrumsJs(const Napi::CallbackInfo& info) : ObjectWrap(info)
{
    Napi::Env env = info.Env();

    if(info.Length() < 1) 
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return;
    }

    if(!info[0].IsString()) 
    {
        Napi::TypeError::New(env, "You need to name yourself").ThrowAsJavaScriptException();
        return;
    }

    const std::string dataLocation = info[0].As<Napi::String>().Utf8Value();
    this->drumKit = std::make_unique<eXaDrumsApi::eXaDrums>(dataLocation.data());
}

void LibexadrumsJs::Start(const Napi::CallbackInfo& info) 
{
    drumKit->Start();
}

void LibexadrumsJs::Stop(const Napi::CallbackInfo& info) 
{
    drumKit->Stop();
}

Napi::Value LibexadrumsJs::IsStarted(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    bool isStarted = drumKit->IsStarted();
    return Napi::Value::From(env, isStarted);
}

Napi::Value LibexadrumsJs::GetDataLocation(const Napi::CallbackInfo& info)
{
    return Napi::String::From(info.Env(), drumKit->GetDataLocation());
}

Napi::Value LibexadrumsJs::GetVersion(const Napi::CallbackInfo& info)
{
    return Napi::String::From(info.Env(), eXaDrumsApi::eXaDrums::GetVersion());
}

void LibexadrumsJs::EnableMetronome(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if(info.Length() < 1) 
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return;
    }

    if(!info[0].IsBoolean())
    {
        Napi::TypeError::New(env, "Wrong argument type").ThrowAsJavaScriptException();
        return;
    }

    const auto arg = static_cast<bool>(info[0].As<Napi::Boolean>());
    drumKit->EnableMetronome(arg);
}

void LibexadrumsJs::ChangeClickVolume(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if(info.Length() < 1) 
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return;
    }

    if(!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong argument type").ThrowAsJavaScriptException();
        return;
    }
    const std::size_t arg = static_cast<uint32_t>(info[0].As<Napi::Number>());
    drumKit->ChangeClickVolume(arg);
}

Napi::Value LibexadrumsJs::GetKitsNames(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const auto kitsNames = drumKit->GetKitsNames();
    auto kitsNamesArray = Napi::Array::New(env, kitsNames.size());
    for(size_t i = 0; i < kitsNames.size(); ++i)
    {
        kitsNamesArray[i] = Napi::String::From(env, kitsNames[i]);
    }

    return kitsNamesArray;
}

Napi::Function LibexadrumsJs::GetClass(Napi::Env env) 
{
    return DefineClass(env, "LibexadrumsJs", 
    {
        LibexadrumsJs::InstanceMethod("start", &LibexadrumsJs::Start),
        LibexadrumsJs::InstanceMethod("stop", &LibexadrumsJs::Stop),
        LibexadrumsJs::InstanceMethod("isStarted", &LibexadrumsJs::IsStarted),
        LibexadrumsJs::InstanceMethod("getDataLocation", &LibexadrumsJs::GetDataLocation),
        LibexadrumsJs::InstanceMethod("getVersion", &LibexadrumsJs::GetVersion),
        LibexadrumsJs::InstanceMethod("enableMetronome", &LibexadrumsJs::EnableMetronome),
        LibexadrumsJs::InstanceMethod("changeClickVolume", &LibexadrumsJs::ChangeClickVolume),
        LibexadrumsJs::InstanceMethod("getKitsNames", &LibexadrumsJs::GetKitsNames),
    });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
    Napi::String name = Napi::String::New(env, "LibexadrumsJs");
    exports.Set(name, LibexadrumsJs::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
