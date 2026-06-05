#include "../g4meover_app.h"

enum VarItemListIndex {
    VarItemListIndexScreen,
    VarItemListIndexDolphin,
    VarItemListIndexSpoof,
    VarItemListIndexVgm,
    VarItemListIndexShowG4MEOVERIntro,
};

void g4meover_app_scene_misc_var_item_list_callback(void* context, uint32_t index) {
    G4MEOVERApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void g4meover_app_scene_misc_on_enter(void* context) {
    G4MEOVERApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(var_item_list, "Screen", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "Dolphin", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "Spoofing Options", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    item = variable_item_list_add(var_item_list, "VGM Options", 0, NULL, app);
    variable_item_set_current_value_text(item, ">");

    variable_item_list_add(var_item_list, "Show G4MEOVER Intro", 0, NULL, app);

    variable_item_list_set_enter_callback(
        var_item_list, g4meover_app_scene_misc_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list, scene_manager_get_scene_state(app->scene_manager, G4MEOVERAppSceneMisc));

    view_dispatcher_switch_to_view(app->view_dispatcher, G4MEOVERAppViewVarItemList);
}

bool g4meover_app_scene_misc_on_event(void* context, SceneManagerEvent event) {
    G4MEOVERApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneMisc, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexScreen:
            scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneMiscScreen, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneMiscScreen);
            break;
        case VarItemListIndexDolphin:
            scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneMiscDolphin, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneMiscDolphin);
            break;
        case VarItemListIndexSpoof:
            scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneMiscSpoof, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneMiscSpoof);
            break;
        case VarItemListIndexVgm:
            scene_manager_set_scene_state(app->scene_manager, G4MEOVERAppSceneMiscVgm, 0);
            scene_manager_next_scene(app->scene_manager, G4MEOVERAppSceneMiscVgm);
            break;
        case VarItemListIndexShowG4MEOVERIntro: {
            for(int i = 0; i < 10; i++) {
                if(storage_common_copy(
                       app->storage, EXT_PATH("dolphin/firstboot.bin"), SLIDESHOW_FS_PATH)) {
                    app->show_slideshow = true;
                    g4meover_app_apply(app);
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    return consumed;
}

void g4meover_app_scene_misc_on_exit(void* context) {
    G4MEOVERApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
