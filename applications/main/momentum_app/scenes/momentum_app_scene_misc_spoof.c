#include "../g4meover_app.h"

enum VarItemListIndex {
    VarItemListIndexFlipperName, // TODO: Split into name, mac, serial
    VarItemListIndexShellColor,
};

const char* const shell_color_names[FuriHalVersionColorCount] = {
    "Real",
    "Black",
    "White",
    "Transparent",
};
static void g4meover_app_scene_misc_spoof_shell_color_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, shell_color_names[index]);
    g4meover_settings.spoof_color = index;
    app->save_settings = true;
    app->require_reboot = true;
}

void g4meover_app_scene_misc_spoof_var_item_list_callback(void* context, uint32_t index) {
    G4MEOVERApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void g4meover_app_scene_misc_spoof_on_enter(void* context) {
    G4MEOVERApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(var_item_list, "Flipper Name", 0, NULL, app);
    variable_item_set_current_value_text(item, app->device_name);

    item = variable_item_list_add(
        var_item_list,
        "Shell Color",
        FuriHalVersionColorCount,
        g4meover_app_scene_misc_spoof_shell_color_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.spoof_color);
    variable_item_set_current_value_text(item, shell_color_names[g4meover_settings.spoof_color]);

    variable_item_list_set_enter_callback(
        var_item_list, g4meover_app_scene_misc_spoof_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, G4MEOVERAppSceneMiscSpoof));

    view_dispatcher_switch_to_view(app->view_dispatcher, G4MEOVERAppViewVarItemList);
}

bool g4meover_app_scene_misc_spoof_on_event(void* context, SceneManagerEvent event) {
    G4MEOVERApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneMiscSpoof, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexFlipperName:
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneMiscSpoofName);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void g4meover_app_scene_misc_spoof_on_exit(void* context) {
    G4MEOVERApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
