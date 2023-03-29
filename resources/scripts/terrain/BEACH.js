var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };

    this.init = function (terrain)
    {
        if (terrain.getPalette() === "")
        {
            terrain.setPalette(BEACH.getDefaultPalette());
        }
        terrain.setTerrainName(BEACH.getName());
    };

    this.getName = function()
    {
        return qsTr("Beach");
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID, map, currentPalette)
    {
        if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE", currentPalette);
        }
        else if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA", currentPalette);
        }
        else
        {
            terrain.loadBaseTerrain("SEA");
        }
    };
    this.isSeaTile = function(x, y, map)
    {
        if (map.onMap(x, y))
        {
            if (map.getTerrain(x, y).getBaseTerrainID() === "SEA")
            {
                return true;
            }
        }
        else
        {
            return true;
        }
        return false;
    };
    this.loadBaseSprite = function (terrain, map)
    {
        var surroundingsLandDiagnonal = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_Diagnonal, false);
        var surroundingsLand = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_Direct, false);
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
        if (surroundingsLand !== "")
        {
            if (surroundingsLand === "+S" && surroundingsBeach.includes("+N"))
            {
                surroundingsBeach = surroundingsBeach.replace("+N", "");
            }
            if (surroundingsLand === "+N" && surroundingsBeach.includes("+S"))
            {
                surroundingsBeach = surroundingsBeach.replace("+S", "");
            }
            if (surroundingsLand === "+E" && surroundingsBeach.includes("+W"))
            {
                surroundingsBeach = surroundingsBeach.replace("+W", "");
            }
            if (surroundingsLand === "+W" && surroundingsBeach.includes("+E"))
            {
                surroundingsBeach = surroundingsBeach.replace("+E", "");
            }
            terrain.loadBaseSprite("beach" + surroundingsBeach + "+land" + surroundingsLand + "+mask");
        }
        else
        {
            var x = terrain.getX();
            var y = terrain.getY();
            if ((surroundingsLandDiagnonal === "+NE" && (surroundingsBeach === "+N" || surroundingsBeach === "+E" || surroundingsBeach === "+N+E" || surroundingsBeach === "")) ||
                (surroundingsLandDiagnonal === "+NW" && (surroundingsBeach === "+N" || surroundingsBeach === "+W" || surroundingsBeach === "+N+W" || surroundingsBeach === "")) ||
                (surroundingsLandDiagnonal === "+SE" && (surroundingsBeach === "+S" || surroundingsBeach === "+E" || surroundingsBeach === "+E+S" || surroundingsBeach === "")) ||
                (surroundingsLandDiagnonal === "+SW" && (surroundingsBeach === "+S" || surroundingsBeach === "+W" || surroundingsBeach === "+S+W" || surroundingsBeach === "")))
            {
                terrain.loadBaseSprite("beach" + surroundingsBeach + "+land" + surroundingsLandDiagnonal + "+mask");
            }
            else if (x >= 0 && y >= 0)
            {
                if (!BEACH.isSeaTile(x, y - 2, map))
                {
                    surroundingsBeach = surroundingsBeach.replace("+N", "");
                }
                if (!BEACH.isSeaTile(x, y + 2, map))
                {
                    surroundingsBeach = surroundingsBeach.replace("+S", "");
                }
                if (!BEACH.isSeaTile(x + 2, y, map))
                {
                    surroundingsBeach = surroundingsBeach.replace("+E", "");
                }
                if (!BEACH.isSeaTile(x - 2, y, map))
                {
                    surroundingsBeach = surroundingsBeach.replace("+W", "");
                }
                terrain.loadBaseSprite("beach" + surroundingsBeach + "+mask");
            }
            else
            {
                terrain.loadBaseSprite("beach+mask");
            }
        }
    };
    this.canBePlaced = function (x, y, map)
    {
        var terrain = map.getTerrain(x, y);
        var surroundingsLand = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_Direct, false);
        var surroundingsSea = terrain.getSurroundings("SEA,LAKE", false, true, GameEnums.Directions_All, false);
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
        // var c = (surroundingsLand === "" && surroundingsBeach === "");
        var c = (false);
        var d = (surroundingsLand.includes("+S") && surroundingsLand.includes("+E") && surroundingsSea.includes("+NW"));
        var e = (surroundingsLand.includes("+S") && surroundingsLand.includes("+W") && surroundingsSea.includes("+NE"));
        var f = (surroundingsLand.includes("+W") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NE"));
        var g = (surroundingsLand.includes("+W") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SE"));
        var h = (surroundingsLand.includes("+N") && surroundingsLand.includes("+W") && surroundingsSea.includes("+SE"));
        var i = (surroundingsLand.includes("+N") && surroundingsLand.includes("+E") && surroundingsSea.includes("+SW"));
        var j = (surroundingsLand.includes("+E") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NW"));
        var k = (surroundingsLand.includes("+E") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SW"));
        if (c) // || d || e || f || g || h || i || j || k)
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    this.loadOverlaySprite = function(terrain, map)
    {
        let surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow !== "")
        {
            terrain.loadOverlaySprite("sea+snow" + surroundingsSnow);
        }
        let surroundingsWaste = terrain.getSurroundings("WASTE", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsWaste !== "")
        {
            terrain.loadOverlaySprite("sea+waste" + surroundingsWaste);
        }
        let surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert !== "")
        {
            terrain.loadOverlaySprite("sea+desert" + surroundingsDesert);
        }
        // load coast overlay
        var surroundings = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_Direct, false);
        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            let surroundingsSE = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSE + "+overlay+mask");
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            let surroundingsNE = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNE + "+overlay+mask");
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            let surroundingsSW = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSW + "+overlay+mask");
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            let surroundingsNW = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNW + "+overlay+mask");
            }
        }

        if (map !== null)
        {
            let surroundingsBeach = terrain.getSurroundings("BEACH,SEA,LAKE", true, false, GameEnums.Directions_Direct, false);
            let seaBeach = surroundingsBeach === "+N+E+S+W"
            let circle = globals.getCircle(1, 1);
            let size = circle.size();
            let x = terrain.getX();
            let y = terrain.getY();
            for (var i = 0; i < size; i++)
            {
                let point = circle.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    let terrain2 = map.getTerrain(x + point.x, y + point.y);
                    if (terrain2.getID() === "BEACH")
                    {
                        let surroundingsBeach2 = terrain2.getSurroundings("BEACH,SEA,LAKE", true, false, GameEnums.Directions_Direct, false);
                        let seaBeach2 = surroundingsBeach2 === "+N+E+S+W";
                        if ((seaBeach2 && !seaBeach) ||
                                (!seaBeach2 && seaBeach))
                        {
                            if (point.x > 0)
                            {
                                terrain.loadOverlaySprite("beach+overlay+E+mask");
                            }
                            else if (point.x < 0)
                            {
                                terrain.loadOverlaySprite("beach+overlay+W+mask");
                            }
                            else if (point.y > 0)
                            {
                                terrain.loadOverlaySprite("beach+overlay+S+mask");
                            }
                            else if (point.y < 0)
                            {
                                terrain.loadOverlaySprite("beach+overlay+N+mask");
                            }
                        }
                    }
                }
            }
        }
    };

    this.getMiniMapIcon = function()
    {
        return "minimap_beach";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (typeof defender !== 'undefined' &&
            defender !== null &&
            defender.getUnitType() === GameEnums.UnitType_Naval)
        {
            return "back_" + weatherModifier + "beach+sea";
        }
        return "back_" + weatherModifier + "beach";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return ""  ;
    };

    this.getDescription = function()
    {
        return qsTr("Beachs over great landing places for landing ground troops.");
    };

    this.isLoadingTile =  function(terrain)
    {
        return true;
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["beach+E+land+N+mask",
                "beach+E+land+N+S+mask",
                "beach+E+land+N+S+W+mask",
                "beach+E+land+N+W+mask",
                "beach+E+land+S+mask",
                "beach+E+land+S+W+mask",
                "beach+E+S+land+N+W+mask",
                "beach+E+W+land+N+mask",
                "beach+E+W+land+N+S+mask",
                "beach+E+W+land+S+mask",
                "beach+land+E+mask",
                "beach+land+E+S+mask",
                "beach+land+E+S+W+mask",
                "beach+land+E+W+mask",
                "beach+land+N+mask",
                "beach+land+N+E+mask",
                "beach+land+N+E+S+mask",
                "beach+land+N+E+S+W+mask",
                "beach+land+N+E+W+mask",
                "beach+land+N+S+mask",
                "beach+land+N+S+W+mask",
                "beach+land+N+W+mask",
                "beach+land+S+mask",
                "beach+land+S+W+mask",
                "beach+land+W+mask",
                "beach+N+E+land+S+W+mask",
                "beach+N+land+E+mask",
                "beach+N+land+E+S+W+mask",
                "beach+N+land+E+W+mask",
                "beach+N+land+E+S+mask",
                "beach+N+land+S+W+mask",
                "beach+N+land+W+mask",
                "beach+N+S+land+E+mask",
                "beach+N+S+land+E+W+mask",
                "beach+N+S+land+W+mask",
                "beach+N+W+land+E+S+mask",
                "beach+S+land+E+mask",
                "beach+S+land+W+mask",
                "beach+S+land+E+W+mask",
                "beach+S+land+N+E+mask",
                "beach+S+land+N+E+W+mask",
                "beach+S+land+N+W+mask",
                "beach+W+land+E+S+mask",
                "beach+W+land+N+mask",
                "beach+W+land+N+E+mask",
                "beach+S+W+land+N+E+mask",
                "beach+W+land+N+E+S+mask",
                "beach+W+land+N+S+mask",
                "beach+W+land+S+mask",
                "beach+mask",
                "beach+E+mask",
                "beach+E+S+mask",
                "beach+E+S+W+mask",
                "beach+N+mask",
                "beach+N+E+mask",
                "beach+N+E+S+mask",
                "beach+N+E+S+W+mask",
                "beach+N+E+S+W+sea+NE+mask",
                "beach+N+E+S+W+sea+NW+mask",
                "beach+N+E+S+W+sea+SE+mask",
                "beach+N+E+S+W+sea+SW+mask",
                "beach+N+E+W+mask",
                "beach+N+S+W+mask",
                "beach+N+W+mask",
                "beach+S+mask",
                "beach+S+W+mask",
                "beach+W+mask",
                "beach+N+S+mask",
                "beach+E+W+mask",];
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return ["sea+snow+E",
                "sea+snow+E+S",
                "sea+snow+E+S+W",
                "sea+snow+E+W",
                "sea+snow+N",
                "sea+snow+N+E",
                "sea+snow+N+E+S",
                "sea+snow+N+E+S+W",
                "sea+snow+N+E+W",
                "sea+snow+N+S",
                "sea+snow+N+S+W",
                "sea+snow+N+W",
                "sea+snow+S",
                "sea+snow+S+W",
                "sea+snow+W",
                "sea+desert+E",
                "sea+desert+E+S",
                "sea+desert+E+S+W",
                "sea+desert+E+W",
                "sea+desert+N",
                "sea+desert+N+E",
                "sea+desert+N+E+S",
                "sea+desert+N+E+S+W",
                "sea+desert+N+E+W",
                "sea+desert+N+S",
                "sea+desert+N+S+W",
                "sea+desert+N+W",
                "sea+desert+S",
                "sea+desert+S+W",
                "sea+desert+W",
                "sea+waste+E",
                "sea+waste+E+S",
                "sea+waste+E+S+W",
                "sea+waste+E+W",
                "sea+waste+N",
                "sea+waste+N+E",
                "sea+waste+N+E+S",
                "sea+waste+N+E+S+W",
                "sea+waste+N+E+W",
                "sea+waste+N+S",
                "sea+waste+N+S+W",
                "sea+waste+N+W",
                "sea+waste+S",
                "sea+waste+S+W",
                "sea+waste+W",
                "sea+NE+overlay",
                "sea+NW+overlay",
                "sea+SE+overlay",
                "sea+SW+overlay",
                "beach+overlay+E",
                "beach+overlay+N",
                "beach+overlay+W",
                "beach+overlay+S",
                ];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var BEACH = new Constructor();
