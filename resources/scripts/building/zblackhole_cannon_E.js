var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building)
    {
        building.loadSprite("black_cannon+E", false);
        building.loadSpriteV2("black_cannon+E+mask", GameEnums.Recoloring_Matrix);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.actionList = ["ACTION_CANNON_FIRE"];
    this.getName = function()
    {
        return qsTr("Black Cannon");
    };
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(1);
    };
    this.getActionTargetFields = function(building)
    {
        return globals.getShotFields(1, 10, 1, 0);
    };
    this.getActionTargetOffset = function(building)
    {
        // offset for large buildings since there reference point is bound to the lower right corner.
        return Qt.point(0, -1);
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 3;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZBLACKHOLE_CANNON_E.getBuildingWidth(), ZBLACKHOLE_CANNON_E.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getIsAttackable = function(building, x, y)
    {
        var buildX = building.getX();
        var buildY = building.getY();
        if (y === buildY && buildX - 1 === x)
        {
            return true;
        }
        return false;
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
        animation2.setSound("black_canon_destroyed.wav");
        map.getTerrain(x, y).loadBuilding("ZBLACK_BUILDING_DESTROYED");
    };
    this.getDamage = function(building, unit)
    {
        return 5;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_Enemy;
    };
    this.getShotAnimation = function(building, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, building.getX(), building.getY(), 70);
        animation.addSprite("blackhole_shot_east", -map.getImageSize() * 2.0, -map.getImageSize() * 3.0, 0, 2);
        animation.setSound("blackcanon_shot.wav");
        return animation;
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Cannon that can deal 5 damage to a single unit in a large range.");
    };
}

Constructor.prototype = BUILDING;
var ZBLACKHOLE_CANNON_E = new Constructor();
