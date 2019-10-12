var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_PIPELINE.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Pipeline");
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,WELD,DESTROYEDWELD,PIPESTATION,SNOW_PIPELINE,SNOW_DESTROYEDWELD,SNOW_WELD,DESERT_PIPELINE,DESERT_DESTROYEDWELD,DESERT_WELD", false, false, GameEnums.Directions_Direct, true, true);
        var x = terrain.getX();
        var y = terrain.getY();
        if (typeof map !== 'undefined')
        {
            if (map.onMap(x, y + 1))
            {
                var building = map.getTerrain(x, y + 1).getBuilding();
                if (building !== null &&
                        building.getBuildingID() === "ZBLACKHOLE_FACTORY" &&
                        building.getX() - 1 === x && building.getY() - 4 === y)
                {
                    if (surroundings.indexOf("+W") >= 0)
                    {
                        surroundings = surroundings.replace("+W", "+S+W");
                    }
                    else
                    {
                        surroundings += "+S";
                    }
                }
            }
        }

        if (surroundings === "")
        {
            terrain.loadBaseSprite("desert_pipeline+E+W");
        }
        else
        {
            terrain.loadBaseSprite("desert_pipeline" + surroundings);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_pipeline";
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Pipeline which can't be crossed by most units. It reduces the firerange of indirect units by 1.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert_pipeline+E",
                "desert_pipeline+E+S",
                "desert_pipeline+E+S+W",
                "desert_pipeline+E+W",
                "desert_pipeline+N",
                "desert_pipeline+N+E",
                "desert_pipeline+N+E+S",
                "desert_pipeline+N+E+S+W",
                "desert_pipeline+N+E+W",
                "desert_pipeline+N+S",
                "desert_pipeline+N+S+W",
                "desert_pipeline+N+W",
                "desert_pipeline+S",
                "desert_pipeline+S+W",
                "desert_pipeline+W"];
    };
};
Constructor.prototype = TERRAIN;
var DESERT_PIPELINE = new Constructor();
