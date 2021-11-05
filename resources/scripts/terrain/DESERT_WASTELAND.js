var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_WASTELAND.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Wasteland");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getOffensiveFieldBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        return -20;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("desert_wasteland");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_wasteland";
    };
    this.getDescription = function()
    {
        return qsTr("Infantry and Mech units aside, mobility is impaired on rough terrain. It reduces the firepower of units by 20%.");
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_desertwasteland";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_WASTELAND = new Constructor();
