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
                                (terrainID === "DESERT_FOREST") ||
                                (terrainID === "SNOW_FOREST");
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
        return "Sets movement costs over woods to 1.";
    };
    this.getIcon = function()
    {
        return "ranger";
    };
    this.getName = function()
    {
        return "Pathfinder";
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PATHFINDER = new Constructor();
