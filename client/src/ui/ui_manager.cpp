#include "ui/connect_menu.h"
#include "ui/screen_settings_menu.h"

#include <common/ui/ui_manager.h>

UiManager UiManager::s_instance{};

void UiManager::Initialise()
{
    // Register UIs here.
    RegisterUi<ConnectMenu>();
    RegisterUi<ScreenSettingsMenu>();
}
