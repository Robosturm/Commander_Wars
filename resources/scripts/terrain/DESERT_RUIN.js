var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(DESERT_RUIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Ruin");
    };
    this.getDefense = function()
    {
        return 1;
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
    this.getVisionHide = function()
    {
        return true;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite("desert_ruin+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_ruin";
    };
    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div><r> It reduces the firepower of units by 20%.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert_ruin+0",
                "desert_ruin+1",
                "desert_ruin+2"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_desertruin";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_RUIN = new Constructor();
