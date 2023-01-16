var Constructor = function()
{
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
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
    this.getIcon = function(map)
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
    this.getCosts = function()
    {
        return 2;
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PRAIRIE_DOG = new Constructor();
