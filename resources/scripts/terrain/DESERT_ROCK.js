var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Rock"));
    };
    this.getDefense = function()
    {
        return 4;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("DESERT");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("desert_rock");
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_rock";
    };
    this.getBonusVision = function(unit)
    {
        var infantry = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        if (infantry.indexOf(unit.getUnitID()) >= 0)
        {
            return 3;
        }
        else
        {
            return 0;
        }
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_mountain";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_mountain";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_ROCK = new Constructor();
