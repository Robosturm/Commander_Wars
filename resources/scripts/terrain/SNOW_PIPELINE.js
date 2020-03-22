var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(SNOW_PIPELINE.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Pipeline");
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
            terrain.loadBaseSprite("snow_pipeline+E+W");
        }
        else
        {
            terrain.loadBaseSprite("snow_pipeline" + surroundings);
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
        return qsTr("Black Hole Pipeline which can't be crossed by most units and is also hard to cross for those who can.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow_pipeline+E",
                "snow_pipeline+E+S",
                "snow_pipeline+E+S+W",
                "snow_pipeline+E+W",
                "snow_pipeline+N",
                "snow_pipeline+N+E",
                "snow_pipeline+N+E+S",
                "snow_pipeline+N+E+S+W",
                "snow_pipeline+N+E+W",
                "snow_pipeline+N+S",
                "snow_pipeline+N+S+W",
                "snow_pipeline+N+W",
                "snow_pipeline+S",
                "snow_pipeline+S+W",
                "snow_pipeline+W"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_snowpipe";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_snow";
    };
};
Constructor.prototype = TERRAIN;
var SNOW_PIPELINE = new Constructor();
