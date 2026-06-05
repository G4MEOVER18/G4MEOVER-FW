#include "../g4meover_app.h"

enum VarItemListIndex {
    VarItemListIndexGraphics,
    VarItemListIndexMainmenu,
    VarItemListIndexLockscreen,
    VarItemListIndexStatusbar,
    VarItemListIndexFileBrowser,
    VarItemListIndexGeneral,
};

void g4meover_app_scene_interface_var_item_list_callback(void* context, uint32_t index) {
    G4MEOVERApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void g4meover_app_scene_interface_on_enter(void* context) {
    G4MEOVERApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(var_item_list, "Graphics", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "Mainmenu", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "Lockscreen", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "Statusbar", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "File Browser", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "General", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    variable_item_list_set_enter_callback(
        var_item_list, g4meover_app_scene_interface_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, G4MEOVERAppSceneInterface));

    view_dispatcher_switch_to_view(app->view_dispatcher, G4MEOVERAppViewVarItemList);
}

bool g4meover_app_scene_interface_on_event(void* context, SceneManagerEvent event) {
    G4MEOVERApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneInterface, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexGraphics:
            scene_manager_set_scene_state(
                app->scene_manager, G4MEOVERAppSceneInterfaceGraphics, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneInterfaceGraphics);
            break;
        case VarItemListIndexMainmenu:
            scene_manager_set_scene_state(
                app->scene_manager, G4MEOVERAppSceneInterfaceMainmenu, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneInterfaceMainmenu);
            break;
        case VarItemListIndexLockscreen:
            scene_manager_set_scene_state(
                app->scene_manager, G4MEOVERAppSceneInterfaceLockscreen, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneInterfaceLockscreen);
            break;
        case VarItemListIndexStatusbar:
            scene_manager_set_scene_state(
                app->scene_manager, G4MEOVERAppSceneInterfaceStatusbar, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneInterfaceStatusbar);
            break;
        case VarItemListIndexFileBrowser:
            scene_manager_set_scene_state(
                app->scene_manager, G4MEOVERAppSceneInterfaceFilebrowser, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneInterfaceFilebrowser);
            break;
        case VarItemListIndexGeneral:
            scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneInterfaceGeneral, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneInterfaceGeneral);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void g4meover_app_scene_interface_on_exit(void* context) {
    G4MEOVERApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
