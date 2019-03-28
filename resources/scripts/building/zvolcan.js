var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("volcan", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 4;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 4;
    };
    this.canBuildingBePlaced = function(terrain)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, ZVOLCAN.getBuildingWidth(), ZVOLCAN.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getActionTargetFields = function(building)
    {
        return globals.getCircle(0, 20);
    };
    this.getActionTargetOffset = function(building)
    {
        // offset for large buildings since there reference point is bound to the lower right corner.
        return Qt.point(-2, -2);
    };
    this.startOfTurn = function(building)
    {
        // do some fire action here
        var targetOffset = building.getActionTargetOffset();
        var x = building.getX() + targetOffset.x;
        var y = building.getY() + targetOffset.y;
        var animation = GameAnimationFactory.createAnimation(x, y - 4);
        animation.addSprite("volcan_eruption", 0, 0, 0, 1.5);
        // we get called at the start of a new day
        // for now we fire at random :)
        var shotCount = 10;
        ZVOLCAN.targetFields = [];
        var animation2 = null;
        var animation3 = null;
        var targetFields = building.getActionTargetFields();
        for (var i = 0; i < shotCount; i++)
        {
            var targetPos = globals.randInt(0, targetFields.size() - 1);
            var target = Qt.point(targetFields.at(targetPos).x + x, targetFields.at(targetPos).y + y);
            if (map.onMap(target.x, target.y))
            {
                ZVOLCAN.targetFields.push(target);
                animation2 = GameAnimationFactory.createAnimation(target.x, target.y - 3);
                animation2.addSprite("volcan_fireball", 0, -map.getImageSize() * 1, 400, 1.5);
                animation2.addTweenPosition(Qt.point(target.x * map.getImageSize(), target.y * map.getImageSize()), 400);
                animation.queueAnimation(animation2);
                animation3 = GameAnimationFactory.createAnimation(target.x, target.y);
                animation3.addSprite("volcan_hit", -map.getImageSize() / 2, -map.getImageSize() * 1.5, 0, 1.5);
                animation2.queueAnimation(animation3);
            }
        }
        if (animation3 !== null)
        {
            animation3.setEndOfAnimationCall("ZVOLCAN", "postAnimationDamage");
        }
        targetFields.remove();
    };
    this.targetFields = [];
    this.postAnimationDamage = function()
    {
        for (var i = 0; i < ZVOLCAN.targetFields.length; i++)
        {
            var unit = map.getTerrain(ZVOLCAN.targetFields[i].x, ZVOLCAN.targetFields[i].y).getUnit();
            if (unit !== null)
            {
                unit.setHp(unit.getHpRounded() - 5);
                if (unit.getHp() <= 0)
                {
                    unit.killUnit();
                }
            }
        }
        ZVOLCAN.targetFields = [];
    };
}

Constructor.prototype = BUILDING;
var ZVOLCAN = new Constructor();
