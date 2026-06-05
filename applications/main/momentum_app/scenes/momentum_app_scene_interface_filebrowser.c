#include "../g4meover_app.h"

enum VarItemListIndex {
    VarItemListIndexSortDirsFirst,
    VarItemListIndexShowHiddenFiles,
    VarItemListIndexShowInternalTab,
    VarItemListIndexFavoriteTimeout,
};

const char* const browser_path_names[BrowserPathModeCount] = {
    "OFF",
    "Current",
    "Brief",
    "Full",
};

void g4meover_app_scene_interface_filebrowser_var_item_list_callback(void* context, uint32_t index) {
    G4MEOVERApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void g4meover_app_scene_interface_filebrowser_sort_dirs_first_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "ON" : "OFF");
    g4meover_settings.sort_dirs_first = value;
    app->save_settings = true;
}

static void
    g4meover_app_scene_interface_filebrowser_show_hidden_files_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "ON" : "OFF");
    g4meover_settings.show_hidden_files = value;
    app->save_settings = true;
}

static void
    g4meover_app_scene_interface_filebrowser_show_internal_tab_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "ON" : "OFF");
    g4meover_settings.show_internal_tab = value;
    app->save_settings = true;
}

static void
    g4meover_app_scene_interface_filebrowser_browser_path_mode_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, browser_path_names[index]);
    g4meover_settings.browser_path_mode = index;
    app->save_settings = true;
}

static void g4meover_app_scene_interface_filebrowser_favorite_timeout_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    uint32_t value = variable_item_get_current_value_index(item);
    char text[6];
    snprintf(text, sizeof(text), "%lu S", value);
    variable_item_set_current_value_text(item, value ? text : "OFF");
    g4meover_settings.favorite_timeout = value;
    app->save_settings = true;
}

void g4meover_app_scene_interface_filebrowser_on_enter(void* context) {
    G4MEOVERApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(
        var_item_list,
        "Folders Above Files",
        2,
        g4meover_app_scene_interface_filebrowser_sort_dirs_first_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.sort_dirs_first);
    variable_item_set_current_value_text(item, g4meover_settings.sort_dirs_first ? "ON" : "OFF");

    item = variable_item_list_add(
        var_item_list,
        "Show Hidden Files",
        2,
        g4meover_app_scene_interface_filebrowser_show_hidden_files_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.show_hidden_files);
    variable_item_set_current_value_text(item, g4meover_settings.show_hidden_files ? "ON" : "OFF");

    item = variable_item_list_add(
        var_item_list,
        "Show Internal Tab",
        2,
        g4meover_app_scene_interface_filebrowser_show_internal_tab_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.show_internal_tab);
    variable_item_set_current_value_text(item, g4meover_settings.show_internal_tab ? "ON" : "OFF");

    item = variable_item_list_add(
        var_item_list,
        "Show Path",
        BrowserPathModeCount,
        g4meover_app_scene_interface_filebrowser_browser_path_mode_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.browser_path_mode);
    variable_item_set_current_value_text(
        item, browser_path_names[g4meover_settings.browser_path_mode]);

    item = variable_item_list_add(
        var_item_list,
        "Favorite Timeout",
        61,
        g4meover_app_scene_interface_filebrowser_favorite_timeout_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.favorite_timeout);
    char text[4];
    snprintf(text, sizeof(text), "%lu S", g4meover_settings.favorite_timeout);
    variable_item_set_current_value_text(item, g4meover_settings.favorite_timeout ? text : "OFF");

    variable_item_list_set_enter_callback(
        var_item_list, g4meover_app_scene_interface_filebrowser_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, G4MEOVERAppSceneInterfaceFilebrowser));

    view_dispatcher_switch_to_view(app->view_dispatcher, G4MEOVERAppViewVarItemList);
}

bool g4meover_app_scene_interface_filebrowser_on_event(void* context, SceneManagerEvent event) {
    G4MEOVERApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(
            app->scene_manager, G4MEOVERAppSceneInterfaceFilebrowser, event.event);
        consumed = true;
        switch(event.event) {
        default:
            break;
        }
    }

    return consumed;
}

void g4meover_app_scene_interface_filebrowser_on_exit(void* context) {
    G4MEOVERApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
