var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 6;
    };
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("waste_weld+E+W", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        building.loadSpriteV2("waste_weld+E+W+mask", GameEnums.Recoloring_Matrix);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Waste Weld");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_weld";
    };
    this.onDestroyed = function(terrain, map)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = terrain.getX();
        var y = terrain.getY();
        map.replaceTerrainOnly("WASTE_DESTROYEDWELD", x, y);
        map.getTerrain(x, y).loadSprites();
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.setSound("pipe_destroyed.wav");
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Pipeline Weld can be destroyed to cross the pipeline.");
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "fore_pipeline";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain, map);
        return TERRAIN.getTerrainBackgroundId(id, "", true);
    };
    this.getShowInEditor = function()
    {
        return false;
    };
    this.getVision = function(building)
    {
        return -1;
    };
}

Constructor.prototype = BUILDING;
var ZWASTEWELD_E_W = new Constructor();
