var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
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
    this.getDamage = function(building, unit)
    {
        return 5;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_All;
    };
    this.getName = function()
    {
        return qsTr("Volcan");
    };
    this.canBuildingBePlaced = function(terrain, building)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZVOLCAN.getBuildingWidth(), ZVOLCAN.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_volcan";
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
        if (building.getFireCount() === 0)
        {
            var targetOffset = building.getActionTargetOffset();
            var x = building.getX() + targetOffset.x;
            var y = building.getY() + targetOffset.y;
            // we get called at the start of a new day
            // for now we fire at random :)
            var shotCount = 10;

            var targets = [];
            var targetFields = building.getActionTargetFields();
            for (var i = 0; i < shotCount; i++)
            {
                var targetPos = globals.randInt(0, targetFields.size() - 1);
                var target = Qt.point(targetFields.at(targetPos).x + x, targetFields.at(targetPos).y + y);
                if (map.onMap(target.x, target.y))
                {
                    targets.push(target);
                }
            }
            targetFields.remove();
            ZVOLCAN.volcanFire(building, targets);
        }
    };

    this.volcanFire = function(building, targets)
    {
        var animationCount = GameAnimationFactory.getAnimationCount();
        var targetOffset = building.getActionTargetOffset();
        var x = building.getX() + targetOffset.x;
        var y = building.getY() + targetOffset.y;
        var animation = GameAnimationFactory.createAnimation(x, y - 4);
        animation.addSprite("volcan_eruption", 0, 0, 0, 1.5);
        animation.setSound("volcan_eruption.wav");
        if (animationCount > 0)
        {
            GameAnimationFactory.getAnimation(animationCount - 1).queueAnimation(animation);
        }
        var animation2 = null;
        var animation3 = null;
        for (var i = 0; i < targets.length; i++)
        {
            var target = targets[i];
            if (map.onMap(target.x, target.y))
            {
                animation2 = GameAnimationFactory.createAnimation(target.x, target.y - 3);
                animation2.addSprite("volcan_fireball", 0, -map.getImageSize() * 1, 400, 1.5);
                animation2.addTweenPosition(Qt.point(target.x * map.getImageSize(), target.y * map.getImageSize()), 400);
                if (animation3 === null)
                {
                    animation.queueAnimation(animation2);
                }
                else
                {
                    animation3.queueAnimation(animation2);
                }
                animation3 = GameAnimationFactory.createAnimation(target.x, target.y);
                animation3.addSprite("volcan_hit", -map.getImageSize() / 2, -map.getImageSize() * 1.5, 0, 1.5);
                animation3.setSound("volcan_hit.wav");
                animation2.queueAnimation(animation3);
                animation3.writeDataInt32(target.x);
                animation3.writeDataInt32(target.y);
                animation3.writeDataInt32(5);
                animation3.setEndOfAnimationCall("ANIMATION", "postAnimationDamageKill");
            }
        }
    };

    this.getDescription = function()
    {
        return qsTr("Volcan that fires fireballs each day dealing 5 HP of damage at all units they hit.");
    };
}

Constructor.prototype = BUILDING;
var ZVOLCAN = new Constructor();
