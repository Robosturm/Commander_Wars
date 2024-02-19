var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "INFANTRY";
    };


    this.init = function(unit)
    {
        unit.setAmmo1(1);
        unit.setMaxAmmo1(1);
        unit.setWeapon1ID("WEAPON_FIREBOMB");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_INFANTRY_MG");

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
        unit.loadSpriteV2("partisan+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_FEET";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, INFANTRY.armyData);
        animation.loadSpriteV2("partisan+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movewalk.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 2500;
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
    this.startOfTurn = function(unit, map)
    {
        var fields = globals.getCircle(1, 3);
        var x = unit.getX();
        var y = unit.getY();
        var owner = unit.getOwner();
        var size = fields.size();
        for (var i = 0; i < size; i++)
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
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.getEditorPlacementSound = function()
    {
        return "movewalk.wav";
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_PARTISAN = new Constructor();
