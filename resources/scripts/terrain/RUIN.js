var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(RUIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Ruin");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite("ruin+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_ruin";
    };
    this.getDescription = function()
    {
        return qsTr("In Fog of War conditions, the ruins provide ground unit <div c='ffffff'>hiding places<div c='00ff00'>.");
    };
};
Constructor.prototype = TERRAIN;
var RUIN = new Constructor();
