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
    loadSettings : function()
    {
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorSaveKey, Qt.Key_S);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorLoadKey, Qt.Key_L);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorNewMapKey, Qt.Key_N);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditMapKey, Qt.Key_E);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorResizeMapKey, Qt.Key_R);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorChangePlaceSelectionKey, Qt.Key_P);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditUnitsKey, Qt.Key_U);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditTerrainsKey, Qt.Key_T);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditPlayersKey, Qt.Key_P);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditRulesKey, Qt.Key_R);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorOptimizePlayersKey, Qt.Key_O);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorPlaceLineKey, Qt.Key_L);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorPlaceEllipseKey, Qt.Key_E);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorUpdateSpritesKey, Qt.Key_U);
        settings.addSetting(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorToggleIgnorePlacementKey, Qt.Key_I);
        return true;
    }
}
