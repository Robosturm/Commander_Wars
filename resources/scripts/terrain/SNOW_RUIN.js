var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(SNOW_RUIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Ruin");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        terrain.loadBaseTerrain("SNOW");
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite("snow_ruin+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_ruin";
    };
    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div><r> Snowy terrain rough to cross.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow_ruin+0",
                "snow_ruin+1",
                "snow_ruin+2"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_snowruin";
    };
};
Constructor.prototype = TERRAIN;
var SNOW_RUIN = new Constructor();
