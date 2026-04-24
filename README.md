# F4SE Menu Framework 3 — Example Plugin

A reference / starter plugin that demonstrates every feature of [F4SE Menu Framework 3](https://github.com/DCCStudios/F4SEMenuFramework), itself a Fallout 4 port of [SKSE Menu Framework 3](https://github.com/QTR-Modding/SKSE-Menu-Framework-3) by QTR-Modding.

Clone this repository as a starting point for your own F4SE plugin that uses the framework.

---

## What's demonstrated

| Example | File | Feature |
|---|---|---|
| Example 1 | `src/UI.cpp` | Form lookup and `AddObjectToContainer` |
| Example 2 | `src/UI.cpp` | Popout window with a menu bar |
| Example 3 | `src/UI.cpp` | Text filter / search |
| Example 4 | `src/UI.cpp` | Font Awesome icons |
| Example 5 | `src/UI.cpp` | Non-blocking HUD overlay + `AddInputEvent` + `AddHudElement` |

---

## Starting a new plugin from this template

1. **Clone** this repository (or your own fork)
2. **Edit `CMakeLists.txt`** — change the author and product names:

```cmake
set(AUTHOR_NAME   "YourName")
set(PRODUCT_NAME  "YourPluginName")
set(BEAUTIFUL_NAME "Your Plugin Display Name")
```

3. **Edit `cmake/version.rc.in`** — update the copyright/licence text embedded in the DLL
4. **Delete `LICENSE`** and pick a licence that suits your mod
5. **Copy `resources/F4SEMenuFramework.h`** from the [framework repo](https://github.com/DCCStudios/F4SEMenuFramework) into your `include/` folder (or keep the one already in `include/`)
6. Write your UI in `src/UI.cpp` and register it in `UI::Register()`

---

## Building

### Prerequisites

- Visual Studio 2022
- [CMake](https://cmake.org/) 3.21+
- [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set
- [CommonLibF4](https://github.com/Ryan-rsm-McKenzie/CommonLibF4)
- **F4SE Menu Framework 3** already built (the example links against `F4SEMenuFramework.h` at runtime via `GetProcAddress` — no import lib needed)

### Environment variables

| Variable | Purpose |
|---|---|
| `VCPKG_ROOT` | Path to your vcpkg installation |
| `FALLOUT4_FOLDER` | Game root — DLL is copied to `Data\F4SE\Plugins\` on build |
| `FALLOUT4_MODS_FOLDER` | Optional MO2 mods folder for automatic deployment |

### Configure & build

```powershell
cmake --preset release
cmake --build build/release --config Release
```

Output: `Compile/F4SE/Plugins/F4SEMenuFrameworkExample.dll`

---

## Plugin structure

```
src/
  plugin.cpp   — F4SEPlugin_Query / F4SEPlugin_Load entry points
  UI.cpp       — all ImGui render functions and UI::Register()
include/
  UI.h
  F4SEMenuFramework.h   — public header (GetProcAddress-based, no linking needed)
```

### Minimal `plugin.cpp`

```cpp
extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(
    const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
    a_info->infoVersion = F4SE::PluginInfo::kVersion;
    a_info->name        = "MyPlugin";
    a_info->version     = 1;
    return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(
    const F4SE::LoadInterface* a_f4se)
{
    F4SE::Init(a_f4se);
    auto* msg = F4SE::GetMessagingInterface();
    msg->RegisterListener([](F4SE::MessagingInterface::Message* m) {
        if (m->type == F4SE::MessagingInterface::kGameDataReady)
            UI::Register();
    });
    return true;
}
```

### Minimal `UI.cpp`

```cpp
#include "F4SEMenuFramework.h"

void __stdcall MyMod::Render() {
    ImGuiMCP::Text("Hello from MyMod!");
}

void UI::Register() {
    if (!F4SEMenuFramework::IsInstalled()) return;
    F4SEMenuFramework::SetSection("My Mod");
    F4SEMenuFramework::AddSectionItem("Settings", MyMod::Render);
}
```

---

## Non-blocking overlay example

```cpp
MENU_WINDOW MyOverlay = nullptr;

void __stdcall MyMod::RenderOverlay() {
    if (F4SEMenuFramework::IsAnyBlockingWindowOpened()) return;
    auto* dl = ImGuiMCP::GetForegroundDrawList();
    ImGuiMCP::ImVec2 pos{20, 20};
    ImGuiMCP::ImDrawListManager::AddText(dl, pos, IM_COL32(255,255,255,255), "Hello HUD");
}

void UI::Register() {
    if (!F4SEMenuFramework::IsInstalled()) return;
    F4SEMenuFramework::SetSection("My Mod");

    // Persistent HUD overlay — survives menu close
    MyOverlay = F4SEMenuFramework::AddWindow(MyMod::RenderOverlay, false);
    MyOverlay->IsOpen = true;

    F4SEMenuFramework::AddHudElement(MyMod::RenderOverlay);
}
```

---

## Credits

- **[QTR-Modding](https://github.com/QTR-Modding)** — original [SKSE Menu Framework 3](https://github.com/QTR-Modding/SKSE-Menu-Framework-3) and its example plugin, on which this is based
- **[DCCStudios](https://github.com/DCCStudios)** — [F4SE Menu Framework 3](https://github.com/DCCStudios/F4SEMenuFramework) (the framework this example targets)
- **[Ryan McKenzie](https://github.com/Ryan-rsm-McKenzie/CommonLibF4)** — CommonLibF4
