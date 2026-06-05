#pragma once
// G4MEOVER-FW v. 1.0.0 — Compatibility Forwarding Header
#include "../momentum/settings.h"

// Variable/Funktions-Aliases (g4meover_* → momentum_*)
#define g4meover_settings       momentum_settings
#define g4meover_settings_save  momentum_settings_save
#define g4meover_settings_load  momentum_settings_load

// Pfad-Alias
#define G4MEOVER_SETTINGS_PATH  MOMENTUM_SETTINGS_PATH
