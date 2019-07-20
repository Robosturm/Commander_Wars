var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Ruin"));
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
};
Constructor.prototype = TERRAIN;
var RUIN = new Constructor();
