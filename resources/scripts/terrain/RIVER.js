var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(RIVER.getName());
        terrain.setHasFlowDirection(true);
    };
    this.getName = function()
    {
        return qsTr("River");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {        
        terrain.loadBaseSprite("river+0+land+N+E+S+W");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_river";
    };
    this.getDescription = function()
    {
        return qsTr("Most infantry and Hovercraft units are the only ground unit that can cross rivers.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 1);        
        var weatherModifier = TERRAIN.getWeatherModifier();
        var baseId = terrain.getBaseTerrainID();        
        return "back_" + weatherModifier + "river+" + rand.toString();
    };
    this.getFlowTiles = function()
    {
        return ["RIVER", "BRIDGE"];
    };
    this.updateFlowSprites = function(terrain, pPfs)
    {
        var flowData = pPfs.getFlowData();        
        var length = flowData.size()
        for (var i  = 0; i < length; ++i)
        {
            var pos = flowData.getPosition(i);
            var currentTerrain = map.getTerrain(pos.x, pos.y);
            if (currentTerrain.getID() === "RIVER")
            {
                currentTerrain.unloadSprites();
                currentTerrain.loadBaseTerrainSprites();
                RIVER.loadSpriteFromFlowData(currentTerrain, pos, flowData, i);
            }
        }
        RIVER.loadSeaOverlays(flowData);
    };
    this.loadSpriteFromFlowData = function(terrain, pos, flowData, index)
    {
        var flow = flowData.getFlowString(index);
        var surroundingsDirect = terrain.getSurroundings("RIVER,BRIDGE,SEA,REAF", false, true, GameEnums.Directions_Direct);
        if (surroundingsDirect === "+N+E+S+W" ||
            surroundingsDirect === "+E+S+W" ||
            surroundingsDirect === "+N+S+W" ||
            surroundingsDirect === "+N+E+W" ||
            surroundingsDirect === "+N+E+S")
        {
            var random = globals.randInt(0, 1);
            terrain.loadBaseSprite("river+" + random.toString() + "+land" + surroundingsDirect);
        }
        else
        {
            var surroundingsDiagonal = terrain.getSurroundings("RIVER,BRIDGE,SEA,REAF", false, true, GameEnums.Directions_Diagnonal);
            if (surroundingsDirect.includes("+N"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+NE", "").replace("+NW", "");
            }
            if (surroundingsDirect.includes("+S"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+SE", "").replace("+SW", "");
            }
            if (surroundingsDirect.includes("+E"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+SE", "").replace("+NE", "");
            }
            if (surroundingsDirect.includes("+W"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+SW", "").replace("+NW", "");
            }
            var landEnding = "";
            if (surroundingsDirect !== "" ||
                surroundingsDiagonal !== "")
            {
                landEnding = "+land" + surroundingsDirect + surroundingsDiagonal;
            }
            var animName = "river" + flow + landEnding;
            if (terrain.existsResAnim(animName))
            {
                terrain.loadBaseSprite(animName);
            }
            else
            {
                var flowDirection = flowData.getFlowDirection(index);
                var altFlows = flowData.getAlternateFlowString(flowDirection);
                var length = altFlows.length;
                var loaded = false;
                for (var i = 0; i < length; ++i)
                {
                    var flowString = altFlows[i];
                    animName = "river" + flowString + landEnding;
                    if (terrain.existsResAnim(animName))
                    {
                        terrain.loadBaseSprite(animName);
                        loaded = true;
                        break;
                    }
                }
                if (!loaded)
                {
                    animName = "river+S" + landEnding;
                    if (terrain.existsResAnim(animName))
                    {
                        terrain.loadBaseSprite(animName);
                    }
                    else
                    {
                        var fallbackAnim = terrain.getFittingResAnim("river", landEnding);
                        if (fallbackAnim !== "")
                        {
                            terrain.loadBaseSprite(fallbackAnim);
                        }
                        else
                        {
                            // fallback so at least a river is shown
                            terrain.loadBaseSprite("river+S+land+NE+SE+SW+NW");
                        }
                    }
                }
            }
        }
    };
    this.loadSeaOverlays = function(flowData)
    {
        var overlayTiles = flowData.getOverlayTiles(["SEA", "REAF"]);
        var tileMapping = flowData.getOverlayTileMapping();
        var length = overlayTiles.length
        for (var i  = 0; i < length; ++i)
        {
            var pos = overlayTiles[i];
            var terrain = map.getTerrain(pos.x, pos.y);
            var surroundingsRiverDirect = terrain.getSurroundings("RIVER,BRIDGE", false, false, GameEnums.Directions_Direct);
            var surroundingsLandDirect = terrain.getSurroundings("RIVER,BRIDGE,SEA,REAF", false, true, GameEnums.Directions_Direct);
            var surroundingsLandDiagonal = terrain.getSurroundings("RIVER,BRIDGE,SEA,REAF", false, true, GameEnums.Directions_Diagnonal);
            GameConsole.print("River: " + surroundingsRiverDirect, 1);
            GameConsole.print("Land: " + surroundingsLandDirect + surroundingsLandDiagonal, 1);
            for (var i2 = 1; i2 <= 4; ++i2)
            {
                var endString = "+" + i2.toString();
                switch (i2)
                {
                case 1:
                {
                    if (surroundingsRiverDirect.includes("+N") ||
                        surroundingsRiverDirect.includes("+W"))
                    {
                        if (surroundingsLandDirect.includes("+N") &&
                            surroundingsLandDirect.includes("+W"))
                        {
                            endString += "+land+N+W";
                        }
                        else if (surroundingsLandDirect.includes("+N"))
                        {
                            endString += "+land+N";
                        }
                        else if (surroundingsLandDirect.includes("+W"))
                        {
                            endString += "+land+W";
                        }
                        else if (surroundingsLandDiagonal.includes("+NW"))
                        {
                            endString += "+land+NW";
                        }
                    }
                    else
                    {
                        continue;
                    }
                    break;
                }
                case 2:
                {
                    if (surroundingsRiverDirect.includes("+N") ||
                        surroundingsRiverDirect.includes("+E"))
                    {
                        if (surroundingsLandDirect.includes("+N+E"))
                        {
                            endString += "+land+N+E";
                        }
                        else if (surroundingsLandDirect.includes("+N"))
                        {
                            endString += "+land+N";
                        }
                        else if (surroundingsLandDirect.includes("+E"))
                        {
                            endString += "+land+E";
                        }
                        else if (surroundingsLandDiagonal.includes("+NE"))
                        {
                            endString += "+land+NE";
                        }
                    }
                    else
                    {
                        continue;
                    }
                    break;
                }
                case 3:
                {
                    if (surroundingsRiverDirect.includes("+S") ||
                        surroundingsRiverDirect.includes("+E"))
                    {
                        if (surroundingsLandDirect.includes("+E+S"))
                        {
                            endString += "+land+E+S";
                        }
                        else if (surroundingsLandDirect.includes("+S"))
                        {
                            endString += "+land+S";
                        }
                        else if (surroundingsLandDirect.includes("+E"))
                        {
                            endString += "+land+E";
                        }
                        else if (surroundingsLandDiagonal.includes("+SE"))
                        {
                            endString += "+land+SE";
                        }
                    }
                    else
                    {
                        continue;
                    }
                    break;
                }
                case 4:
                {
                    if (surroundingsRiverDirect.includes("+S") ||
                        surroundingsRiverDirect.includes("+W"))
                    {
                        if (surroundingsLandDirect.includes("+S+W"))
                        {
                            endString += "+land+S+W";
                        }
                        else if (surroundingsLandDirect.includes("+S"))
                        {
                            endString += "+land+S";
                        }
                        else if (surroundingsLandDirect.includes("+W"))
                        {
                            endString += "+land+W";
                        }
                        else if (surroundingsLandDiagonal.includes("+SW"))
                        {
                            endString += "+land+SW";
                        }
                    }
                    else
                    {
                        continue;
                    }
                    break;
                }
                }
                var index = tileMapping[i];
                var flow = flowData.getFlowString(index);
                var animName = "riverend" + flow + endString;
                if (terrain.existsResAnim(animName))
                {
                    terrain.loadOverlaySprite(animName);
                    GameConsole.print("Loaded: " + animName, 1);
                }
                else
                {
                    var flowDirection = flowData.getFlowDirection(index);
                    var altFlows = flowData.getAlternateFlowString(flowDirection);
                    var altFlowlength = altFlows.length;
                    var loaded = false;
                    for (var i3 = 0; i3 < altFlowlength; ++i3)
                    {
                        var flowString = altFlows[i3];
                        animName = "riverend" + flowString + endString;
                        GameConsole.print("Trying: " + animName, 1);
                        if (terrain.existsResAnim(animName))
                        {
                            terrain.loadOverlaySprite(animName);
                            GameConsole.print("Fallback 1 Loaded: " + animName, 1);
                            loaded = true;
                            break;
                        }
                    }
                    if (!loaded)
                    {
                        animName = terrain.getFittingResAnim("riverend", endString);
                        if (animName !== "")
                        {
                            terrain.loadOverlaySprite(animName);
                            GameConsole.print("Fallback 2 Loaded: " + animName, 1);
                        }
                    }
                }
            }
        }
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
