var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("crystall", false);
		building.loadSprite("crystall+mask", true);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_CRYSTALL_HEAL";
    };
    this.getDamage = function(building, unit)
    {
        return -2;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_Own;
    };
    this.startOfTurn = function(building)
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
    this.onDestroyed = function(building)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        map.replaceTerrain("PLAINS_DESTROYED", x, y);
        map.getTerrain(x, y).loadSprites();
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+land.wav");
    };
}

Constructor.prototype = BUILDING;
var ZCRYSTALL = new Constructor();
