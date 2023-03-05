var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.init = function (terrain)
    {
        terrain.setPalette(RIVER.getDefaultPalette());
        terrain.setTerrainName(RIVER.getName());
        terrain.setHasFlowDirection(true);
    };
    this.getName = function()
    {
        return qsTr("River");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
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
    this.loadBaseSprite = function(terrain, map)
    {        
        terrain.loadBaseSprite("river+0+land+N+E+S+W+mask");
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
        return ["river+0+land+N+E+S+W+mask",
                "river+1+land+N+E+S+W+mask",
                "river+land+E+S+W+mask",
                "river+land+N+E+S+mask",
                "river+land+N+E+W+mask",
                "river+land+N+S+W+mask",
                "river+S+W+land+N+W+mask",
                "river+S+W+land+N+W+SE+mask",
                "river+E+S+land+N+E+mask",
                "river+N+W+land+S+W+NE+mask",
                "river+E+land+N+S+W+mask",
                "river+E+land+N+W+mask",
                "river+E+land+N+W+SE+mask",
                "river+E+land+S+W+mask",
                "river+E+land+S+W+NE+mask",
                "river+E+S+land+W+NE+mask",
                "river+land+E+S+W+mask",
                "river+N+land+E+NW+SW+mask",
                "river+N+land+E+S+mask",
                "river+N+land+E+S+NW+mask",
                "river+N+land+E+S+W+mask",
                "river+N+land+S+W+mask",
                "river+N+W+land+E+NW+SW+mask",
                "river+S+land+E+NW+SW+mask",
                "river+S+land+N+E+SW+mask",
                "river+S+land+N+E+W+mask",
                "river+S+W+land+E+NW+SW+mask",
                "river+W+land+E+S+mask",
                "river+S+land+E+W+mask",
                "river+N+land+E+W+mask",
                "river+E+land+N+S+mask",
                "river+W+land+N+S+mask",
                "river+S+land+E+W+bridge+mask",
                "river+E+land+N+SE+mask",
                "river+E+land+S+NE+mask",
                "river+E+land+S+NE+NW+mask",
                "river+W+land+S+NW+mask",
                "river+W+land+N+SW+mask",
                "river+E+land+S+mask",
                "river+E+land+N+mask",
                "river+E+mask",
                "river+W+mask",
                "river+N+mask",
                "river+S+mask",
                "river+N+land+W+mask",
                "river+N+land+W+NE+SE+mask",
                "river+E+S+land+N+SE+SW+mask",
                "river+E+S+land+N+SW+mask",
                "river+E+S+land+W+NE+mask",
                "river+E+S+land+W+NE+SE+mask",
                "river+E+W+land+S+NE+NW+mask",
                "river+N+E+land+S+NE+NW+mask",
                "river+N+E+land+S+NW+mask",
                "river+N+E+land+W+NE+SE+mask",
                "river+N+E+land+W+SE+mask",
                "river+N+land+E+mask",
                "river+N+land+E+NW+mask",
                "river+N+land+E+SW+NW+mask",
                "river+N+land+W+NE+mask",
                "river+N+W+land+E+SW+mask",
                "river+N+W+land+E+SW+NW+mask",
                "river+N+W+land+S+NE+mask",
                "river+N+W+land+S+NE+NW+mask",
                "river+S+land+E+mask",
                "river+S+land+E+SW+mask",
                "river+S+land+E+SW+NW+mask",
                "river+S+land+N+SE+SW+mask",
                "river+S+land+NE+SE+SW+NW+mask",
                "river+S+land+W+mask",
                "river+S+land+W+NE+SE+mask",
                "river+S+land+W+SE+mask",
                "river+S+W+land+E+NW+mask",
                "river+S+W+land+E+SW+NW+mask",
                "river+S+W+land+N+SE+mask",
                "river+S+W+land+N+SE+SW+mask",
                "river+W+land+N+mask",
                "river+W+land+S+mask",
                "river+W+land+S+NE+NW+mask",
                "river+S+W+land+N+SE+SW+mask",
                "river+W+land+E+S+NW+mask",
                "river+W+land+N+E+mask",
                "river+W+land+N+E+S+mask",
                "river+W+land+N+E+SW+mask",];
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (unit !== null &&
            unit.getMovementType() === "MOVE_SMALL_BOAT")
        {
            return "back_" + weatherModifier + "river+boat";
        }
        else
        {
            var variables = terrain.getAnimationVariables();
            var variable = variables.getVariable("BACKGROUND_ID");
            var rand = 0;
            if (variable === null)
            {
                rand = globals.randInt(0, 1);
                variable = variables.createVariable("BACKGROUND_ID");
                variable.writeDataInt32(rand);
            }
            else
            {
                rand = variable.readDataInt32();
            }
            return "back_" + weatherModifier + "river+" + rand.toString();
        }
    };
    this.getFlowTiles = function()
    {
        return ["RIVER", "BRIDGE", "BRIDGE1", "BRIDGE2"];
    };
    this.updateFlowSprites = function(terrain, pPfs, map, applyRulesPalette)
    {
        var flowData = pPfs.getFlowData();        
        var length = flowData.size()
        for (var i  = 0; i < length; ++i)
        {
            var pos = flowData.getPosition(i);
            var currentTerrain = map.getTerrain(pos.x, pos.y);
            if (applyRulesPalette)
            {
                currentTerrain.setTerrainPaletteGroup(map.getGameRules().getMapPalette());
            }
            if (currentTerrain.getID() === "RIVER")
            {
                if (!currentTerrain.getFixedSprite())
                {
                    currentTerrain.unloadSprites();
                    currentTerrain.loadBaseTerrainSprites();
                    RIVER.loadSpriteFromFlowData(currentTerrain, pos, flowData, i, map);
                }
            }
            else
            {
                currentTerrain = currentTerrain.getBaseTerrain("RIVER");
                currentTerrain.unloadSprites();
                currentTerrain.loadBaseTerrainSprites();
                RIVER.loadSpriteFromFlowData(currentTerrain, pos, flowData, i, map);
            }
        }
    };
    this.loadSpriteFromFlowData = function(terrain, pos, flowData, index, map)
    {
        var flow = flowData.getFlowString(index);
        var surroundingsDirect = terrain.getSurroundings("RIVER,BRIDGE,BRIDGE1,BRIDGE2,LAKE,SEA,REAF", false, true, GameEnums.Directions_Direct, false);
        if (surroundingsDirect === "+N+E+S+W")
        {
            var random = globals.randInt(0, 1);
            terrain.loadBaseSprite("river+" + random.toString() + "+land" + surroundingsDirect + "+mask");
        }
        else
        {
            var surroundingsDiagonal = terrain.getSurroundings("RIVER,BRIDGE,BRIDGE1,BRIDGE2,LAKE,SEA,REAF", false, true, GameEnums.Directions_Diagnonal);
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
                landEnding = "+land" + surroundingsDirect + surroundingsDiagonal + "+mask";
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
                        var fallbackAnim = terrain.getFittingResAnim("river+", landEnding);
                        if (fallbackAnim !== "" && surroundingsDirect !== "")
                        {
                            terrain.loadBaseSprite(fallbackAnim);
                        }
                        else
                        {
                            // fallback so at least a river is shown
                            terrain.loadBaseSprite("river+S+land+NE+SE+SW+NW+mask");
                        }
                    }
                }
            }
        }
    };
    this.loadTerrainSeaOverlay = function(terrain, flowString)
    {
        var surroundingsLandDirect = terrain.getSurroundings("RIVER,BRIDGE,BRIDGE1,BRIDGE2,LAKE,SEA,REAF,BEACH,FOG,ROUGH_SEA", false, true, GameEnums.Directions_Direct);
        var surroundingsLandDiagonal = terrain.getSurroundings("RIVER,BRIDGE,BRIDGE1,BRIDGE2LAKE,SEA,REAF,BEACH,FOG,ROUGH_SEA", false, true, GameEnums.Directions_Diagnonal);
        if (flowString === "+S")
        {
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+NW", "");
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+NE", "");
        }
        if (flowString === "+W")
        {
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+SE", "");
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+NE", "");
        }
        if (flowString === "+N")
        {
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+SW", "");
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+SE", "");
        }
        if (flowString === "+E")
        {
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+NW", "");
            surroundingsLandDiagonal = surroundingsLandDiagonal.replace("+SW", "");
        }
        var south = surroundingsLandDirect.includes("+S");
        var east = surroundingsLandDirect.includes("+E");
        var north = surroundingsLandDirect.includes("+N");
        var west = surroundingsLandDirect.includes("+W");
        for (var i2 = 1; i2 <= 4; ++i2)
        {
            var landname = "";
            switch (i2)
            {
                case 1:
                {
                    if (north && west)
                    {
                        landname = "+land+N+W";
                    }
                    else if (north)
                    {
                        landname = "+land+N";
                    }
                    else if (west)
                    {
                        landname = "+land+W";
                    }
                    else if (surroundingsLandDiagonal.includes("+NW"))
                    {
                        landname = "+land+NW";
                    }
                    break;
                }
                case 2:
                {
                    if (north && east)
                    {
                        landname = "+land+N+E";
                    }
                    else if (north)
                    {
                        landname = "+land+N";
                    }
                    else if (east)
                    {
                        landname = "+land+E";
                    }
                    else if (surroundingsLandDiagonal.includes("+NE"))
                    {
                        landname = "+land+NE";
                    }
                    break;
                }
                case 3:
                {
                    if (south && east)
                    {
                        landname = "+land+E+S";
                    }
                    else if (south)
                    {
                        landname = "+land+S";
                    }
                    else if (east)
                    {
                        landname = "+land+E";
                    }
                    else if (surroundingsLandDiagonal.includes("+SE"))
                    {
                        landname = "+land+SE";
                    }
                    break;
                }
                case 4:
                {
                    if (south && west)
                    {
                        landname = "+land+S+W";
                    }
                    else if (south)
                    {
                        landname = "+land+S";
                    }
                    else if (west)
                    {
                        landname = "+land+W";
                    }
                    else if (surroundingsLandDiagonal.includes("+SW"))
                    {
                        landname = "+land+SW";
                    }
                    break;
                }
            }
            var animName = "riverend" + flowString + "+" + i2.toString() + landname + "+mask";
            if (terrain.existsResAnim(animName))
            {
                terrain.loadOverlaySprite(animName);
            }
        }
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
