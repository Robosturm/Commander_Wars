var Constructor = function()
{
    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
        if (CO_PERK.isActive(co))
        {
            if (unit.getOwner() === co.getOwner())
            {
                if (typeof map !== 'undefined')
                {
                    if (map.onMap(posX, posY))
                    {
                        var terrainID = map.getTerrain(posX, posY).getID();
                        var isForest = 	(terrainID === "FOREST") ||
                                        (terrainID === "FOREST1") ||
                                        (terrainID === "FOREST2") ||
                                        (terrainID === "FOREST3") ||
                                        (terrainID === "DESERT_FOREST") ||
                                        (terrainID === "DESERT_FOREST1") ||
                                        (terrainID === "SNOW_FOREST") ||
                                        (terrainID === "SNOW_FOREST1") ||
                                        (terrainID === "SNOW_FOREST2");
                        if (isForest)
                        {
                            return -999;
                        }
                    }
                }
            }
        }
        return 0;
    };
    // Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Sets movement costs over woods to 1.");
    };
    this.getIcon = function()
    {
        return "ranger";
    };
    this.getName = function()
    {
        return qsTr("Pathfinder");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PATHFINDER = new Constructor();
