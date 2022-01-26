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
                        if (terrainID === "PLAINS")
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
        return qsTr("Sets movement costs over plains to 1.");
    };
    this.getIcon = function()
    {
        return "prairiedog";
    };
    this.getName = function()
    {
        return qsTr("Prairie Dog");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PRAIRIE_DOG = new Constructor();
