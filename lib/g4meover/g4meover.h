#pragma once
// G4MEOVER-FW v. 1.0.0 — Hauptheader (Forwarding + Compatibility)
#include "../momentum/momentum.h"

// Compatibility aliases: g4meover_* / g4mw* → momentum_* / mntm* (C-Symbole)
#define g4meover_settings       momentum_settings
#define g4meover_settings_save  momentum_settings_save
#define g4meover_settings_load  momentum_settings_load
#define G4MEOVER_SETTINGS_PATH  MOMENTUM_SETTINGS_PATH

// Enum-Wert Aliases (MNTM → G4MW Umbenennung)
#define MenuStyleG4MW           MenuStyleMNTM
