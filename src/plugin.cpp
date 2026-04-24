#include "Plugin.h"

namespace Plugin
{
    static constexpr auto NAME = "F4SEMenuFrameworkExample"sv;
    static constexpr auto VERSION = REL::Version{ 3, 0, 0 };
}

void OnMessage(F4SE::MessagingInterface::Message* message) {
    if (message->type == F4SE::MessagingInterface::kGameDataReady) {
        UI::Example1::LookupForm();
    }
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
    a_info->infoVersion = F4SE::PluginInfo::kVersion;
    a_info->name = Plugin::NAME.data();
    a_info->version = 1;

    if (a_f4se->IsEditor()) {
        return false;
    }

    const auto ver = a_f4se->RuntimeVersion();
    if (ver < F4SE::RUNTIME_1_10_162) {
        return false;
    }

    return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
    SetupLog();
    logger::info("{} v{} loading", Plugin::NAME, Plugin::VERSION.string());

    F4SE::Init(a_f4se);
    F4SE::GetMessagingInterface()->RegisterListener(OnMessage);

    UI::Register();

    logger::info("{} loaded successfully", Plugin::NAME);
    return true;
}
