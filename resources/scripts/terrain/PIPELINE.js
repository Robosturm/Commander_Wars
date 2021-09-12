var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(PIPELINE.getName());
    };
    this.getName = function()
    {
        return qsTr("Pipeline");
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
            terrain.loadBaseSprite("pipeline+E+W");
        }
        else
        {
            terrain.loadBaseSprite("pipeline" + surroundings);
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
        return qsTr("Black Hole Pipeline which can't be crossed by most units.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["pipeline+E",
                "pipeline+E+S",
                "pipeline+E+S+W",
                "pipeline+E+W",
                "pipeline+N",
                "pipeline+N+E",
                "pipeline+N+E+S",
                "pipeline+N+E+S+W",
                "pipeline+N+E+W",
                "pipeline+N+S",
                "pipeline+N+S+W",
                "pipeline+N+W",
                "pipeline+S",
                "pipeline+S+W",
                "pipeline+W"];
    };
};
Constructor.prototype = TERRAIN;
var PIPELINE = new Constructor();
