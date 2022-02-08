var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("crystall", false);
        building.loadSpriteV2("crystall+mask", GameEnums.Recoloring_Matrix);
        building.loadSprite("crystall+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.actionList = ["ACTION_CRYSTALL_HEAL"];
    this.getDamage = function(building, unit)
    {
        return -2;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_Own;
    };
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(1);
    };
    this.getName = function()
    {
        return qsTr("Crystall");
    };
    this.getActionTargetFields = function(building)
    {
        return globals.getCircle(1, 2);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.onDestroyed = function(building, map)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        map.replaceTerrainOnly("PLAINS_DESTROYED", x, y);
        map.getTerrain(x, y).loadSprites();
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 2, 100);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.addSound("crystal_explode_2.wav");
        animation.addSound("crystal_explode_2.wav", 1, 100);
        animation.addSound("crystal_explode.wav", 1 , 200);
    };

    this.getHealSound = function()
    {
        return "crystal_heal.wav";
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Crystal that can heal all own units for 2 HP in a 13 square area.");
    };
}

Constructor.prototype = BUILDING;
var ZCRYSTALL = new Constructor();
