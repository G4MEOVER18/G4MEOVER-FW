#pragma once

#include <gui/scene_manager.h>

// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) G4MEOVERAppScene##id,
typedef enum {
#include "g4meover_app_scene_config.h"
    G4MEOVERAppSceneNum,
} G4MEOVERAppScene;
#undef ADD_SCENE

extern const SceneManagerHandlers g4meover_app_scene_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);
#include "g4meover_app_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "g4meover_app_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);
#include "g4meover_app_scene_config.h"
#undef ADD_SCENE
