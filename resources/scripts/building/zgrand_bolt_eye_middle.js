var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 4;
    };
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(1);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("grand_bolt+eye+middle+mask", GameEnums.Recoloring_Matrix);
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Grand Bolt");
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 7;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 1;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZGRAND_BOLT_EYE_MIDDLE.getBuildingWidth(), ZGRAND_BOLT_EYE_MIDDLE.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getIsAttackable = function(building, x, y)
    {
        var buildX = building.getX();
        var buildY = building.getY();
        if (y === buildY && buildX - 3 === x)
        {
            return true;
        }
        return false;
    };
    this.actionList = ["ACTION_PRODUCE_OOZIUM_FREE"];
    this.getActionTargetOffset = function(building)
    {
        // offset for large buildings since there reference point is bound to the lower right corner.
        return Qt.point(-3, 1);
    };
    this.onDestroyed = function(building, map)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation2.addScreenshake(45, 0.98, 3000, 200);
        var owner = building.getOwner();
        var terrain = map.getTerrain(x, y);
        terrain.loadBuilding("ZGRAND_BOLT_EYE_MIDDLE_DESTROYED");
        terrain.getBuilding().setOwner(owner);
    };
}

Constructor.prototype = BUILDING;
var ZGRAND_BOLT_EYE_MIDDLE = new Constructor();
