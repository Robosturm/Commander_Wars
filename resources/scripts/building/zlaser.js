var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 3;
    };
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("laser", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        building.loadSpriteV2("laser+mask", GameEnums.Recoloring_Matrix);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.actionList = ["ACTION_LASER_FIRE"];
    this.getDamage = function(building, unit)
    {
        return 5;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_All;
    };
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(1);
    };
    this.getName = function()
    {
        return qsTr("Laser");
    };
    this.getActionTargetFields = function(building)
    {
        var targets = globals.getEmptyPointArray();
        targets.append(Qt.point(0, 0));
        // laser do not fire infinitly but the range is still fucking huge :)
        for (var i = 1; i < 40; i++)
        {
            targets.append(Qt.point(0, i));
            targets.append(Qt.point(0, -i));
            targets.append(Qt.point(i, 0));
            targets.append(Qt.point(-i, 0));
        }
        return targets;
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
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.setSound("explosion+land.wav");
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Laser that can deal 5 HP of damage to all units in 4 rays from the laser.");
    };
}

Constructor.prototype = BUILDING;
var ZLASER = new Constructor();
