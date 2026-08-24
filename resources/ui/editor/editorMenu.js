EditorMenu =
             {
    // constants
    PLACE_LINE_MODE : GameEnums.EditorModes_Last,
    PLACE_ELLIPSE_MODE : GameEnums.EditorModes_Last + 1,
    EXIT : "EXIT",
    SAVEMAP: "SAVEMAP",
    LOADMAP: "LOADMAP",
    UNDO: "UNDO",
    REDO: "REDO",
    EDITSCRIPT: "EDITSCRIPT",
    EDITCAMPAIGN: "EDITCAMPAIGN",
    IMPORTCOWTXT: "IMPORTCOWTXT",
    IMPORTAWDSAWS: "IMPORTAWDSAWS",
    EXPORTAWDSAWS: "EXPORTAWDSAWS",
    IMPORTAW4AW4: "IMPORTAW4AW4",
    IMPORTAWBYWEB: "IMPORTAWBYWEB",
    IMPORTAWBYWEBBYMAPID: "IMPORTAWBYWEBBYMAPID",
    NEWMAP: "NEWMAP",
    EDITMAP: "EDITMAP",
    EXTENDMAP: "EXTENDMAP",
    FLIPX: "FLIPX",
    FLIPY: "FLIPY",
    ROTATEX: "ROTATEX",
    ROTATEY: "ROTATEY",
    ROTATEX90: "ROTATEX90",
    ROTATEY90: "ROTATEY90",
    ROTATE90: "ROTATE90",
    RANDOMMAP: "RANDOMMAP",
    PLACESELECTION: "PLACESELECTION",
    DELETEUNITS: "DELETEUNITS",
    EDITUNITS: "EDITUNITS",
    EDITTERRAIN: "EDITTERRAIN",
    OPTIMIZEPLAYERS: "OPTIMIZEPLAYERS",
    EDITPLAYERS: "EDITPLAYERS",
    EDITRULES: "EDITRULES",
    COPY: "COPY",
    PASTE: "PASTE",
    PASTEALL: "PASTEALL",
    RESIZEMAP: "RESIZEMAP",
    TOGGLEGRID: "TOGGLEGRID",
    TOGGLEMIDDLECROSS: "TOGGLEMIDDLECROSS",
    VIEWMAPSTATS: "VIEWMAPSTATS",
    EDITBIOMES: "EDITBIOMES",
    UPDATESPRITES: "UPDATESPRITES",
    PLACELINE: "PLACELINE",
    PLACEELLIPSE: "PLACEELLIPSE",

    // state variables
    startPoint : Qt.point(-1, -1),

    topBarEvent : function(input)
    {
        if (input === EditorMenu.EXIT)
        {
            currentMenu.showExit();
        }
        else if (input === EditorMenu.SAVEMAP)
        {
            currentMenu.showSaveMap();
        }
        else if (input === EditorMenu.LOADMAP)
        {
            currentMenu.showLoadMap();
        }
        else if (input === EditorMenu.UNDO)
        {
            currentMenu.editorUndo();
        }
        else if (input === EditorMenu.REDO)
        {
            currentMenu.editorRedo();
        }
        else if (input === EditorMenu.EDITSCRIPT)
        {
            currentMenu.showEditScript();
        }
        else if (input === EditorMenu.EDITCAMPAIGN)
        {
            currentMenu.showEditCampaign();
        }
        else if (input === EditorMenu.IMPORTCOWTXT)
        {
            currentMenu.showImportCoWTxTMap();
        }
        else if (input === EditorMenu.IMPORTAWDSAWS)
        {
            currentMenu.showImportAwdsAws();
        }
        else if (input === EditorMenu.EXPORTAWDSAWS)
        {
            currentMenu.showExportAwdsAws();
        }
        else if (input === EditorMenu.IMPORTAW4AW4)
        {
            currentMenu.showImportAwdsAw4();
        }
        else if (input === EditorMenu.IMPORTAWBYWEB)
        {
            currentMenu.showImportAwByWeb();
        }
        else if (input === EditorMenu.IMPORTAWBYWEBBYMAPID)
        {
            currentMenu.showImportAwByWebByMapId();
        }
        else if (input === EditorMenu.NEWMAP)
        {
            currentMenu.showNewMap();
        }
        else if (input === EditorMenu.EDITMAP)
        {
            currentMenu.showEditMap();
        }
        else if (input === EditorMenu.EXTENDMAP)
        {
            currentMenu.showExtendMap();
        }
        else if (input === EditorMenu.FLIPX)
        {
            currentMenu.flipX();
        }
        else if (input === EditorMenu.FLIPY)
        {
            currentMenu.flipY();
        }
        else if (input === EditorMenu.ROTATEX)
        {
            currentMenu.rotateX();
        }
        else if (input === EditorMenu.ROTATEY)
        {
            currentMenu.rotateY();
        }
        else if (input === EditorMenu.ROTATEX90)
        {
            currentMenu.rotateX90();
        }
        else if (input === EditorMenu.ROTATEY90)
        {
            currentMenu.rotateY90();
        }
        else if (input === EditorMenu.ROTATE90)
        {
            currentMenu.rotate90();
        }
        else if (input === EditorMenu.RANDOMMAP)
        {
            currentMenu.showRandomMap();
        }
        else if (input === EditorMenu.PLACESELECTION)
        {
            currentMenu.changePlaceSelection();
        }
        else if (input === EditorMenu.DELETEUNITS)
        {
            currentMenu.deleteUnits();
        }
        else if (input === EditorMenu.EDITUNITS)
        {
            currentMenu.editUnits();
        }
        else if (input === EditorMenu.EDITTERRAIN)
        {
            currentMenu.editTerrains();
        }
        else if (input === EditorMenu.OPTIMIZEPLAYERS)
        {
            currentMenu.optimizePlayers();
        }
        else if (input === EditorMenu.EDITPLAYERS)
        {
            currentMenu.showEditPlayers();
        }
        else if (input === EditorMenu.EDITRULES)
        {
            currentMenu.showEditRules();
        }
        else if (input === EditorMenu.COPY)
        {
            currentMenu.copy();
        }
        else if (input === EditorMenu.PASTE)
        {
        }
        else if (input === EditorMenu.PASTEALL)
        {
        }
        else  if (input === EditorMenu.RESIZEMAP)
        {
            currentMenu.showResizeMap();
        }
        else if (input === EditorMenu.TOGGLEGRID)
        {
            currentMenu.toggleGridLayout();
        }
        else if (input === EditorMenu.TOGGLEMIDDLECROSS)
        {
            currentMenu.toggleMiddleCrossGrid();
        }
        else if (input === "IGNOREPLACEMENTRULES")
        {
            currentMenu.toggleIgnorePlacementRestrictions();
        }
        else  if (input === EditorMenu.VIEWMAPSTATS)
        {
            currentMenu.viewMapStats();
        }
        else if (input === EditorMenu.EDITBIOMES)
        {
            currentMenu.showEditBiomes();
        }
        else if (input === EditorMenu.UPDATESPRITES)
        {
            currentMenu.updateSprites();
        }
        else if (input === EditorMenu.PLACELINE)
        {
            EditorMenu.startPlaceLine(currentMenu);
        }
        else if (input === EditorMenu.PLACEELLIPSE)
        {
            EditorMenu.startPlaceEllipse(currentMenu);
        }
    },
    startPlaceLine : function(menu)
    {
        EditorMenu.startPoint = Qt.point(-1, -1);
        menu.changeCursor("cursor+default");
        menu.setEditorMode(EditorMenu.PLACE_LINE_MODE);
    },
    startPlaceEllipse : function(menu)
    {
        EditorMenu.startPoint = Qt.point(-1, -1);
        menu.changeCursor("cursor+default");
        menu.setEditorMode(EditorMenu.PLACE_ELLIPSE_MODE);
    },
    keyEvent : function(menu, mode, map, key, controlPressed, shiftPressed, altPressed)
    {
        if (controlPressed)
        {
            if (shiftPressed)
            {
                if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditPlayersKey))
                {
                    menu.showEditPlayers();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditRulesKey))
                {
                    menu.showEditRules();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorOptimizePlayersKey))
                {
                    menu.optimizePlayers();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorPlaceLineKey))
                {
                    EditorMenu.startPlaceLine(menu);
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorPlaceEllipseKey))
                {
                    EditorMenu.startPlaceEllipse(menu);
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorUpdateSpritesKey))
                {
                    menu.updateSprites();
                    return true;
                }
                case Qt.Key_I:
                {
                    menu.toggleIgnorePlacementRestrictions();
                    return true;
                }
            }
            else
            {
                if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorLoadKey))
                {
                    menu.showLoadMap();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorSaveKey))
                {
                    menu.showSaveMap();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorNewMapKey))
                {
                    menu.showNewMap();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditMapKey))
                {
                    menu.showEditMap();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorResizeMapKey))
                {
                    menu.showResizeMap();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorChangePlaceSelectionKey))
                {
                    menu.changePlaceSelection();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditUnitsKey))
                {
                    menu.editUnits();
                    return true;
                }
                else if (key === settings.getSettingInt(SETTINGSLOADER.EditorGroup, SETTINGSLOADER.EditorEditTerrainsKey))
                {
                    menu.editTerrains();
                    return true;
                }
            }
        }
        return false;
    },
    mapClickLeft : function(menu, mode, map, x, y)
    {
        if (mode === EditorMenu.PLACE_LINE_MODE)
        {
            if (EditorMenu.startPoint.x < 0)
            {
                EditorMenu.startPoint = Qt.point(x, y);
                menu.clearMarkedFields();
                menu.createMarkedField(EditorMenu.startPoint, "#0000FF");
            }
            else
            {
                EditorMenu.placeLine(menu, x, y);
            }
            return true;
        }
        else if (mode === EditorMenu.PLACE_ELLIPSE_MODE)
        {
            if (EditorMenu.startPoint.x < 0)
            {
                EditorMenu.startPoint = Qt.point(x, y);
            }
            else
            {
                EditorMenu.placeEllipse(menu, x, y);
            }
            return true;
        }
        return false;
    },
    mapClickLeftUp : function(menu, mode, map, x, y)
    {
        return false;
    },
    mapClickLeftDown : function(menu, mode, map, x, y)
    {
        return false;
    },
    mapClickRight : function(menu, mode, map, x, y)
    {
        if (mode === EditorMenu.PLACE_LINE_MODE ||
            mode === EditorMenu.PLACE_ELLIPSE_MODE)
        {
            EditorMenu.startPoint = Qt.point(-1, -1);
            menu.clearMarkedFields();
        }
        return false;
    },
    cursorMoved : function(menu, mode, map, x, y)
    {
        if (mode === EditorMenu.PLACE_LINE_MODE)
        {
            if (EditorMenu.startPoint.x >= 0)
            {
                menu.clearMarkedFields();
                menu.createMarkedFields(EditorMenu.getLineVector(EditorMenu.startPoint.x, EditorMenu.startPoint.y, x, y), "#0000FF")
            }
            return true;
        }
        else if (mode === EditorMenu.PLACE_ELLIPSE_MODE)
        {
            if (EditorMenu.startPoint.x >= 0)
            {
                menu.clearMarkedFields();
                menu.createMarkedFields(EditorMenu.getEllipseVector(EditorMenu.startPoint.x, EditorMenu.startPoint.y, x, y), "#0000FF")
            }
            return true;
        }
        return false;
    },
    getLineVector : function (x1, y1, x2, y2)
    {
        var points = [Qt.point(x1, y1)];
        while (x1 !== x2 || y1 !== y2)
        {
            var xDiff = Math.abs(x1 - x2);
            var yDiff = Math.abs(y1 - y2);
            if (xDiff > yDiff)
            {
                if (x1 < x2)
                {
                    x1++;
                }
                else
                {
                    x1--;
                }
            }
            else
            {
                if (y1 < y2)
                {
                    y1++;
                }
                else
                {
                    y1--;
                }
            }
            points.push(Qt.point(x1, y1));
        }
        return points;
    },
    placeLine : function(menu, x, y)
    {
        menu.placeBasedOnMode(EditorMenu.getLineVector(EditorMenu.startPoint.x, EditorMenu.startPoint.y, x, y));
        menu.clearMarkedFields();
        EditorMenu.startPoint = Qt.point(-1, -1);
    },
    getEllipseVector : function (x1, y1, x2, y2)
    {
        var points = [];
        var minX = Math.min(x1, x2);
        var maxX = Math.max(x1, x2);
        var minY = Math.min(y1, y2);
        var maxY = Math.max(y1, y2);
        var width = maxX - minX;
        var height = maxY - minY;
        var centerX = (minX + maxX) / 2;
        var centerY = (minY + maxY) / 2;
        var firstX = Math.ceil(minX);
        var lastX = Math.ceil(maxX);
        var firstY = Math.ceil(minY);
        var lastY = Math.ceil(maxY);

        for (var y = firstY; y <= lastY; y++)
        {
            for (var x = firstX; x <= lastX; x++)
            {
                var insideEllipse = false;
                if (width === 0)
                {
                    insideEllipse = x === centerX;
                }
                else if (height === 0)
                {
                    insideEllipse = y === centerY;
                }
                else
                {
                    var xRadius = (width / 2);
                    var yRadius = (height / 2);
                    var xDistance = (x - centerX) / xRadius;
                    var yDistance = (y - centerY) / yRadius;
                    insideEllipse = xDistance * xDistance + yDistance * yDistance <= 1;
                }

                if (insideEllipse)
                {
                    points.push(Qt.point(x, y));
                }
            }
        }
        return points;
    },
    placeEllipse : function(menu, x, y)
    {
        menu.placeBasedOnMode(EditorMenu.getEllipseVector(EditorMenu.startPoint.x, EditorMenu.startPoint.y, x, y));
        menu.clearMarkedFields();
        EditorMenu.startPoint = Qt.point(-1, -1);
    },

};
