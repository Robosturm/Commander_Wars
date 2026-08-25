var SETTINGSLOADER =
{
    EditorGroup : "EDITOR_KEYS",
    EditorSaveKey : "EDITOR_SAVE_KEY",
    EditorLoadKey : "EDITOR_LOAD_KEY",
    EditorNewMapKey : "EDITOR_NEW_MAP_KEY",
    EditorEditMapKey : "EDITOR_EDIT_MAP_KEY",
    EditorChangePlaceSelectionKey : "EDITOR_CHANGE_PLACE_SELECTION_KEY",
    EditorEditUnitsKey : "EDITOR_EDIT_UNITS_KEY",
    EditorEditTerrainsKey : "EDITOR_EDIT_TERRAIN_KEY",
    EditorEditPlayersKey : "EDITOR_EDIT_PLAYERS_KEY",
    EditorResizeMapKey : "EDITOR_RESIZE_MAP_KEY",
    EditorEditRulesKey : "EDITOR_EDIT_RULES_KEY",
    EditorOptimizePlayersKey : "EDITOR_OPTIMIZE_PLAYERS_KEY",
    EditorPlaceLineKey : "EDITOR_PLACE_LINE_KEY",
    EditorPlaceEllipseKey : "EDITOR_PLACE_ELLIPSE_KEY",
    EditorUpdateSpritesKey : "EDITOR_UPDATE_SPRITES_KEY",
    EditorToggleIgnorePlacementKey : "EDITOR_TOGGLE_IGNORE_PLACEMENT_KEY",

    settingMapping : [
    ],

    loadSettings : function()
    {
        for (var i = 0; i < SETTINGSLOADER.settingMapping.length; ++i)
        {
            var setting = SETTINGSLOADER.settingMapping[i];
            settings.addSetting(setting[0], setting[1], setting[2]);
        }
        return true;
    }
}

SETTINGSLOADER.settingMapping.push(
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorSaveKey, Qt.Key_S],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorLoadKey, Qt.Key_L],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorNewMapKey, Qt.Key_N],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditMapKey, Qt.Key_E],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorResizeMapKey, Qt.Key_R],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorChangePlaceSelectionKey, Qt.Key_P],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditUnitsKey, Qt.Key_U],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditTerrainsKey, Qt.Key_T],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditPlayersKey, Qt.Key_P],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditRulesKey, Qt.Key_R],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorOptimizePlayersKey, Qt.Key_O],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorPlaceLineKey, Qt.Key_L],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorPlaceEllipseKey, Qt.Key_E],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorUpdateSpritesKey, Qt.Key_U],
    [SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorToggleIgnorePlacementKey, Qt.Key_I]
);