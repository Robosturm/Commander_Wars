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
        return ["river+0+land+N+E+S+W",
                "river+1+land+N+E+S+W",
                "river+0+land+E+S+W",
                "river+1+land+E+S+W",
                "river+0+land+N+E+S",
                "river+1+land+N+E+S",
                "river+0+land+N+E+W",
                "river+1+land+N+E+W",
                "river+0+land+N+S+W",
                "river+1+land+N+S+W",
                "river+S+W+land+N+W",
                "river+S+W+land+N+W+SE",
                "river+E+S+land+N+E",
                "river+E+S+land+N+E+SW",
                "river+N+E+land+N+W",
                "river+N+E+land+N+W+SE",
                "river+E+S+land+S+W",
                "river+E+S+land+S+W+NE",
                "river+N+E+land+E+S",
                "river+N+E+land+E+S+NW",
                "river+N+W+land+S+W",
                "river+N+W+land+S+W+NE",
                "river+N+W+land+N+E",
                "river+N+W+land+N+E+SW",
                "river+S+W+land+E+S",
                "river+S+W+land+E+S+NW",
                "river+S+land+E+W",
                "river+N+land+E+W",
                "river+E+land+N+S",
                "river+W+land+N+S",
                "river+S+land+E+W+bridge",
                "river+E+land+N+SE",
                "river+E+land+S+NE",
                "river+E+land+S+NE+NW",
                "river+W+land+S+NW",
                "river+W+land+N+SW",
                "river+E+land+S",
                "river+E+land+N",
                "river+E",
                "river+W",
                "river+N",
                "river+S",
                "river+N+land+W+NE+SE",
                "river+E+S+land+N+SE+SW",
                "river+E+S+land+N+SW",
                "river+E+S+land+W+NE",
                "river+E+S+land+W+NE+SE",
                "river+E+W+land+S+NE+NW",
                "river+N+E+land+S+NE+NW",
                "river+N+E+land+S+NW",
                "river+N+E+land+W+NE+SE",
                "river+N+E+land+W+SE",
                "river+N+land+E",
                "river+N+land+E+NW",
                "river+N+land+E+SW+NW",
                "river+N+land+W+NE",
                "river+N+W+land+E+SW",
                "river+N+W+land+E+SW+NW",
                "river+N+W+land+S+NE",
                "river+N+W+land+S+NE+NW",
                "river+S+land+E",
                "river+S+land+E+SW",
                "river+S+land+E+SW+NW",
                "river+S+land+N+SE+SW",
                "river+S+land+NE+SE+SW+NW",
                "river+S+land+W",
                "river+S+land+W+NE+SE",
                "river+S+land+W+SE",
                "river+S+W+land+E+NW",
                "river+S+W+land+E+SW+NW",
                "river+S+W+land+N+SE",
                "river+S+W+land+N+SE+SW",
                "river+W+land+N",
                "river+W+land+S",
                "river+W+land+S+NE+NW",];
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
            var variables = terrain.getVariables();
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
    this.updateFlowSprites = function(terrain, pPfs, map)
    {
        var flowData = pPfs.getFlowData();        
        var length = flowData.size()
        for (var i  = 0; i < length; ++i)
        {
            var pos = flowData.getPosition(i);
            var currentTerrain = map.getTerrain(pos.x, pos.y);
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
                        var fallbackAnim = terrain.getFittingResAnim("river+", landEnding);
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
            var animName = "riverend" + flowString + "+" + i2.toString() + landname;
            if (terrain.existsResAnim(animName))
            {
                terrain.loadBaseSprite(animName);
            }
        }
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
