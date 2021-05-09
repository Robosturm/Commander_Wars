var Constructor = function()
{
    this.getUnitDamageID = function()
    {
        return "HEAVY_TANK";
    };

    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_HEAVY_TANK_GUN");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_HEAVY_TANK_MG");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("royal_guard", false);
        unit.loadSpriteV2("royal_guard+mask", GameEnums.Recoloring_Table);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        animation.loadSpriteV2("royal_guard+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("royal_guard+walk", false, 1.5);
        animation.setSound("moveheavytank.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.getBaseCost = function()
    {
        return 14000;
    };
    this.getName = function()
    {
        return qsTr("Royal Guard");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Reinforced Tank that buffs allied units at the start of each turn. Attack and defence capabilities superior to standard tank.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.startOfTurn = function(unit)
    {
        var owner = unit.getOwner();
        var playerId  = owner.getPlayerID();
        ZCOUNIT_ROYAL_GUARD.addBuff(unit, playerId);
        var fields = globals.getCircle(1, 1);
        var unitX = unit.getX();
        var unitY = unit.getY();
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + unitX;
            var y = fields.at(i).y + unitY;
            if (map.onMap(x, y))
            {
                var targetUnit = map.getTerrain(x, y).getUnit();
                if (targetUnit !== null && targetUnit.getOwner() === owner)
                {
                    ZCOUNIT_ROYAL_GUARD.addBuff(targetUnit, playerId);
                }
            }
        }
        fields.remove();
    };
    this.offBonus = 15;
    this.defBonus = 10;
    this.addBuff = function(unit, playerId, duration = 1)
    {
        unit.addDefensiveBonus(ZCOUNIT_ROYAL_GUARD.defBonus, duration);
        unit.addOffensiveBonus(ZCOUNIT_ROYAL_GUARD.offBonus, duration);
        unit.loadIcon("arrow_up", map.getImageSize() / 2, map.getImageSize() / 2, 1, playerId);
    }

    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_ROYAL_GUARD = new Constructor();
