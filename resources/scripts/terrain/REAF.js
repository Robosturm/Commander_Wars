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
            terrain.setPalette(REAF.getDefaultPalette());
        }
        terrain.setVisionHigh(1);
        terrain.setTerrainName(REAF.getName());
    };
    this.getName = function()
    {
        return qsTr("Reaf");
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
            terrain.loadBaseTerrain("SEA", currentPalette);
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        var surroundingsPlainsDiagonal = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Diagnonal, false);
        var surroundingsPlainsDirect = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
        var fullName = "reaf" + surroundingsPlainsDiagonal + surroundingsPlainsDirect + "+mask";
        if (terrain.existsResAnim(fullName))
        {
            terrain.loadBaseSprite(fullName);
        }
        else if (surroundingsPlainsDirect !== "" &&
                 terrain.existsResAnim("reaf" + surroundingsPlainsDirect + "+mask"))
        {
            terrain.loadBaseSprite("reaf" + surroundingsPlainsDirect + "+mask");
        }
        else if (surroundingsPlainsDiagonal !== "")
        {
            if (surroundingsPlainsDiagonal.includes("+NE") &&
                surroundingsPlainsDiagonal.includes("+SE") &&
                surroundingsPlainsDiagonal.includes("+SW") ||
                surroundingsPlainsDiagonal === "+SE")
            {
                terrain.loadBaseSprite("reaf+E+S+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+SE") &&
                     surroundingsPlainsDiagonal.includes("+SW") &&
                     surroundingsPlainsDiagonal.includes("+NW") ||
                     surroundingsPlainsDiagonal === "+SW")
            {
                terrain.loadBaseSprite("reaf+S+W+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+SW") &&
                     surroundingsPlainsDiagonal.includes("+NW") &&
                     surroundingsPlainsDiagonal.includes("+NE") ||
                     surroundingsPlainsDiagonal === "+NW")
            {
                terrain.loadBaseSprite("reaf+N+W+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+NW") &&
                     surroundingsPlainsDiagonal.includes("+NE") &&
                     surroundingsPlainsDiagonal.includes("+SE") ||
                     surroundingsPlainsDiagonal === "+NE")
            {
                terrain.loadBaseSprite("reaf+N+E+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+SE") &&
                surroundingsPlainsDiagonal.includes("+SW"))
            {
                terrain.loadBaseSprite("reaf+S+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+NE") &&
                     surroundingsPlainsDiagonal.includes("+NW"))
            {
                terrain.loadBaseSprite("reaf+N+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+NE") &&
                     surroundingsPlainsDiagonal.includes("+SE"))
            {
                terrain.loadBaseSprite("reaf+E+mask");
            }
            else if (surroundingsPlainsDiagonal.includes("+NW") &&
                     surroundingsPlainsDiagonal.includes("+SW"))
            {
                terrain.loadBaseSprite("reaf+W+mask");
            }
        }
        else
        {
            terrain.loadBaseSprite("reaf+N+E+S+W+mask");
        }
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "SEA")
        {
            SEA.loadOverlaySprite(terrain, map);
        }
        else
        {
            LAKE.loadOverlaySprite(terrain, map);
        }
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.canBePlaced = function(x, y, map)
    {
        var terrain = map.getTerrain(x, y);
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "SEA" ||
            baseId === "LAKE")
        {
            var diagonalCount = 0;
            var surroundingsPlainsDiagonal = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Diagnonal, false);
            var surroundingsPlainsDirect = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
            var fullName = "reaf" + surroundingsPlainsDiagonal + surroundingsPlainsDirect;
            if (terrain.existsResAnim(fullName))
            {
                return true;
            }
            if (surroundingsPlainsDiagonal.includes("+SE"))
            {
                diagonalCount++;
            }
            if (surroundingsPlainsDiagonal.includes("+SW"))
            {
                diagonalCount++;
            }
            if (surroundingsPlainsDiagonal.includes("+NE"))
            {
                diagonalCount++;
            }
            if (surroundingsPlainsDiagonal.includes("+NW"))
            {
                diagonalCount++;
            }
            var directCount = 0;
            if (surroundingsPlainsDirect.includes("+N"))
            {
                directCount++;
            }
            if (surroundingsPlainsDirect.includes("+S"))
            {
                directCount++;
            }
            if (surroundingsPlainsDirect.includes("+E"))
            {
                directCount++;
            }
            if (surroundingsPlainsDirect.includes("+W"))
            {
                directCount++;
            }
            if ((directCount <= 2 && diagonalCount <= 2) ||
                (directCount == 0 && diagonalCount <= 3))
            {
                return true;
            }
        }
        return false;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_reaf";
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("<r>This spiky reaf provides ideal </r><div c='#00ff00'>hiding places</div><r> for naval units in Fog of War.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["reaf+0+N+E+S+W+mask",
                "reaf+1+N+E+S+W+mask",
                "reaf+2+N+E+S+W+mask",
                "reaf+3+N+E+S+W+mask",
                "reaf+E+mask",
                "reaf+N+mask",
                "reaf+S+mask",
                "reaf+W+mask",
                "reaf+E+S+mask",
                "reaf+N+E+mask",
                "reaf+N+W+mask",
                "reaf+S+W+mask",
                "reaf+E+S+W+mask",
                "reaf+E+W+mask",
                "reaf+N+E+S+mask",
                "reaf+N+E+W+mask",
                "reaf+N+S+W+mask",
                "reaf+N+S+mask",
                "reaf+NE+NW+S+mask",
                "reaf+NE+NW+mask",
                "reaf+NE+S+W+mask",
                "reaf+NE+S+mask",
                "reaf+NE+W+mask",
                "reaf+NE+mask",
                "reaf+NW+E+mask",
                "reaf+NW+S+E+mask",
                "reaf+NW+S+mask",
                "reaf+NW+mask",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var REAF = new Constructor();
