var Constructor = function()
{
    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
        if (CO_PERK.isActive(co))
        {
            if (unit.getOwner() === co.getOwner())
            {
                if (map !== null)
                {
                    if (map.onMap(posX, posY))
                    {
                        var terrainID = map.getTerrain(posX, posY).getID();
                        var isMountain = terrainID === "MOUNTAIN" ||
                                         terrainID === "DESERT_ROCK" ||
                                         terrainID === "SNOW_MOUNTAIN" ||
                                         terrainID === "WASTE_MOUNTAIN";
                        if (isMountain)
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
        return qsTr("Sets movement costs over mountains to 1.");
    };
    this.getIcon = function()
    {
        return "mountaineer";
    };
    this.getName = function()
    {
        return qsTr("Climber");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_CLIMBER = new Constructor();
