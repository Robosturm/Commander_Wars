var Constructor = function()
{
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
    {
        if (CO_PERK.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                    var isMountain = terrainID === "MOUNTAIN" ||
                                     terrainID === "DESERT_ROCK" ||
                                     terrainID === "SNOW_MOUNTAIN" ||
                                     terrainID === "WASTE_MOUNTAIN";
                    if (isMountain)
                    {
                        return 10;
                    }
                }
            }
        }
        return 0;
    };
    // Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the attack from mountains by 10%.");
    };
    this.getIcon = function()
    {
        return "mountaineer";
    };
    this.getName = function()
    {
        return qsTr("Mountaineer");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_MOUNTAINEER = new Constructor();
