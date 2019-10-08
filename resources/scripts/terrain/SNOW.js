var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(SNOW.getName());
    };
    this.getName = function()
    {
        return qsTr("Snow");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain)
    {
		terrain.loadBaseSprite("snow");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow";
    };
    this.loadOverlaySprite = function(terrain)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS,WASTELAND,SEA", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlains.includes("+N"))
        {
            terrain.loadOverlaySprite("plains+N");
        }
        if (surroundingsPlains.includes("+E"))
        {
            terrain.loadOverlaySprite("plains+E");
        }
        if (surroundingsPlains.includes("+S"))
        {
            terrain.loadOverlaySprite("plains+S");
        }
        if (surroundingsPlains.includes("+W"))
        {
            terrain.loadOverlaySprite("plains+W");
        }
    };
    this.getDescription = function()
    {
        return qsTr("Snowy terrain reduces rough to cross.");
    };
};
Constructor.prototype = TERRAIN;
var SNOW = new Constructor();
