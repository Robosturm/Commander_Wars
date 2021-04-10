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
        building.loadSprite("minicannon+E", false);
        building.loadSpriteV2("minicannon+E+mask", GameEnums.Recoloring_Table);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_CANNON_FIRE";
    };
    this.startOfTurn = function(building)
    {
        building.setFireCount(1);
    };
    this.getActionTargetFields = function(building)
    {
        return globals.getShotFields(1, 4, 1);
    };
    this.getName = function()
    {
        return qsTr("Mini Cannon");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getDamage = function(building, unit)
    {
        return 3;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_Enemy;
    };
    this.onDestroyed = function(building)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        map.replaceTerrainOnly("PLAINS_DESTROYED", x, y);
        map.getTerrain(x, y).loadSprites();
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.setSound("explosion+land.wav");
    };
    this.getShotAnimation = function(building)
    {
        var animation = GameAnimationFactory.createAnimation(building.getX(), building.getY(), 70);
        animation.addSprite("minicanon_east", map.getImageSize() * 0.5, -map.getImageSize() * 0.5, 0, 1.5);
        animation.setSound("minicanon_shot.wav");
        return animation;
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Minicannon that can deal 3 HP of damage to a single unit in a small range.");
    };
}

Constructor.prototype = BUILDING;
var ZMINICANNON_E = new Constructor();
