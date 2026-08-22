EditorMenu =
             {
    topBarEvent : function(input)
    {        
        currentMenu.autoFocus();
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
    },
    keyEvent : function(menu, modifiers, key)
    {
        return false;
    },
    mapClickLeft : function(menu, x, y)
    {
        return false;
    },
    mapClickLeftUp : function(menu, x, y)
    {
        return false;
    },
    mapClickLeftDown : function(menu, x, y)
    {
        return false;
    },
    mapClickRight : function(menu, x, y)
    {
        return false;
    }
};
