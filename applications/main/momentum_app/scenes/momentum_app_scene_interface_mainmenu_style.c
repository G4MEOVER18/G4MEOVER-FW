#include "../g4meover_app.h"

// Reference the menu style names from mainmenu scene
extern const char* const menu_style_names[MenuStyleCount];

void g4meover_app_scene_interface_mainmenu_style_submenu_callback(void* context, uint32_t index) {
    G4MEOVERApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void g4meover_app_scene_interface_mainmenu_style_on_enter(void* context) {
    G4MEOVERApp* app = context;
    Submenu* submenu = app->submenu;

    for(size_t i = 0; i < MenuStyleCount; i++) {
        submenu_add_item(
            submenu,
            menu_style_names[i],
            i,
            g4meover_app_scene_interface_mainmenu_style_submenu_callback,
            app);
    }

    submenu_set_header(submenu, "Choose Menu Style:");
    submenu_set_selected_item(submenu, g4meover_settings.menu_style);
    view_dispatcher_switch_to_view(app->view_dispatcher, G4MEOVERAppViewSubmenu);
}

bool g4meover_app_scene_interface_mainmenu_style_on_event(void* context, SceneManagerEvent event) {
    G4MEOVERApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        g4meover_settings.menu_style = event.event;
        app->save_settings = true;
        scene_manager_previous_scene(app->scene_manager);
    }

    return consumed;
}

void g4meover_app_scene_interface_mainmenu_style_on_exit(void* context) {
    G4MEOVERApp* app = context;
    submenu_reset(app->submenu);
}
