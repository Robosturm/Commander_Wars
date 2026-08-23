EditorMenu =
             {
    // constants
    PLACE_LINE_MODE : GameEnums.EditorModes_Last,
    PLACE_ELLIPSE_MODE : GameEnums.EditorModes_Last + 1,

    // state variables
    startPoint : Qt.point(-1, -1),

    topBarEvent : function(input)
    {
        if (input === "EXIT")
        {
            currentMenu.showExit();
        }
        else if (input === "SAVEMAP")
        {
            currentMenu.showSaveMap();
        }
        else if (input === "LOADMAP")
        {
            currentMenu.showLoadMap();
        }
        else if (input === "UNDO")
        {
            currentMenu.editorUndo();
        }
        else if (input === "REDO")
        {
            currentMenu.editorRedo();
        }
        else if (input === "EDITSCRIPT")
        {
            currentMenu.showEditScript();
        }
        else if (input === "EDITCAMPAIGN")
        {
            currentMenu.showEditCampaign();
        }
        else if (input === "IMPORTCOWTXT")
        {
            currentMenu.showImportCoWTxTMap();
        }
        else if (input === "IMPORTAWDSAWS")
        {
            currentMenu.showImportAwdsAws();
        }
        else if (input === "EXPORTAWDSAWS")
        {
            currentMenu.showExportAwdsAws();
        }
        else if (input === "IMPORTAW4AW4")
        {
            currentMenu.showImportAwdsAw4();
        }
        else if (input === "IMPORTAWBYWEB")
        {
            currentMenu.showImportAwByWeb();
        }
        else if (input === "IMPORTAWBYWEBBYMAPID")
        {
            currentMenu.showImportAwByWebByMapId();
        }
        else if (input === "NEWMAP")
        {
            currentMenu.showNewMap();
        }
        else if (input === "EDITMAP")
        {
            currentMenu.showEditMap();
        }
        else if (input === "EXTENDMAP")
        {
            currentMenu.showExtendMap();
        }
        else if (input === "FLIPX")
        {
            currentMenu.flipX();
        }
        else if (input === "FLIPY")
        {
            currentMenu.flipY();
        }
        else if (input === "ROTATEX")
        {
            currentMenu.rotateX();
        }
        else if (input === "ROTATEY")
        {
            currentMenu.rotateY();
        }
        else if (input === "ROTATEX90")
        {
            currentMenu.rotateX90();
        }
        else if (input === "ROTATEY90")
        {
            currentMenu.rotateY90();
        }
        else if (input === "ROTATE90")
        {
            currentMenu.rotate90();
        }
        else if (input === "RANDOMMAP")
        {
            currentMenu.showRandomMap();
        }
        else if (input === "PLACESELECTION")
        {
            currentMenu.changePlaceSelection();
        }
        else if (input === "DELETEUNITS")
        {
            currentMenu.deleteUnits();
        }
        else if (input === "EDITUNITS")
        {
            currentMenu.editUnits();
        }
        else if (input === "EDITTERRAIN")
        {
            currentMenu.editTerrains();
        }
        else if (input === "OPTIMIZEPLAYERS")
        {
            currentMenu.optimizePlayers();
        }
        else if (input === "EDITPLAYERS")
        {
            currentMenu.showEditPlayers();
        }
        else if (input === "EDITRULES")
        {
            currentMenu.showEditRules();
        }
        else if (input === "COPY")
        {
            currentMenu.copy();
        }
        else if (input === "PASTE")
        {
        }
        else if (input === "PASTEALL")
        {
        }
        else  if (input === "RESIZEMAP")
        {
            currentMenu.showResizeMap();
        }
        else if (input === "TOGGLEGRID")
        {
            currentMenu.toggleGridLayout();
        }
        else if (input === "TOGGLEMIDDLECROSS")
        {
            currentMenu.toggleMiddleCrossGrid();
        }
        else  if (input === "VIEWMAPSTATS")
        {
            currentMenu.viewMapStats();
        }
        else if (input === "EDITBIOMES")
        {
            currentMenu.showEditBiomes();
        }
        else if (input === "UPDATESPRITES")
        {
            currentMenu.updateSprites();
        }
        else if (input === "PLACELINE")
        {
            EditorMenu.startPoint = Qt.point(-1, -1);
            currentMenu.changeCursor("cursor+default");
            currentMenu.setEditorMode(EditorMenu.PLACE_LINE_MODE);
        }
        else if (input === "PLACEELLIPSE")
        {
            EditorMenu.startPoint = Qt.point(-1, -1);
            currentMenu.changeCursor("cursor+default");
            currentMenu.setEditorMode(EditorMenu.PLACE_ELLIPSE_MODE);
        }
    },
    keyEvent : function(menu, mode, map, modifiers, key)
    {
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
