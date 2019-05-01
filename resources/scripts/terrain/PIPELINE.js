var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Pipeline"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,WELD,DESTROYEDWELD,PIPESTATION", false, false, GameEnums.Directions_Direct, true, true);
        var x = terrain.getX();
        var y = terrain.getY();
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
};
Constructor.prototype = TERRAIN;
var PIPELINE = new Constructor();
