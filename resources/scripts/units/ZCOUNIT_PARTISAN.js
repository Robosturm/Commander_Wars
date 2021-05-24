var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "INFANTRY";
    };


    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_INFANTRY_MG");

        unit.setAmmo2(1);
        unit.setMaxAmmo2(1);
        unit.setWeapon2ID("WEAPON_FIREBOMB");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
        unit.setCursorInfoRange(3);
    };
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("partisan", false);
        unit.loadSpriteV2("partisan+mask", GameEnums.Recoloring_Table);
    };
    this.getMovementType = function()
    {
        return "MOVE_FEET";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_MISSILE,ACTION_CAPTURE,ACTION_JOIN,ACTION_LOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, INFANTRY.armyData);
        animation.loadSpriteV2("partisan+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("partisan+walk", false, 1.5);
        animation.setSound("movewalk.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 2000;
    };
    this.getName = function()
    {
        return qsTr("Partisan");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Can </r><div c='#00ff00'>capture </div><r> bases. </r><div c='#00ff00'>Vision +3 </div><r> when on mountains. Leads to rebellions in nearby cities (3) making it harder to capture buildings.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.startOfTurn = function(unit)
    {
        var fields = globals.getCircle(1, 3);
        var x = unit.getX();
        var y = unit.getY();
        var owner = unit.getOwner();
        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var terrain = map.getTerrain(x + point.x, y + point.y);
                var enemy = terrain.getUnit();
                if (enemy !== null &&
                    owner.isEnemyUnit(enemy))
                {
                    var points = enemy.getCapturePoints();
                    points = points - 3;
                    if (points < 0)
                    {
                        points = 0;
                    }
                    enemy.setCapturePoints(points);
                }
            }
        }
        fields.remove();
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_PARTISAN = new Constructor();
