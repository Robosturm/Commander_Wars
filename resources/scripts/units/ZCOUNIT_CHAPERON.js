var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "FLAK";
    };

    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_GATLING");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(60);
        unit.setMaxFuel(60);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(3);
        unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("chaperon+mask", GameEnums.Recoloring_Matrix);
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        animation.loadSpriteV2("chaperon+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.getBaseCost = function()
    {
        return 8000;
    };
    this.getName = function()
    {
        return qsTr("Chaperon");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Direct;
    };
    this.getDescription = function()
    {
        return qsTr("Specialised anti-air unit. Can negate one indirect attack per turn on own units. Damage negation is based on current units alive.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.startOfTurn = function(unit, map)
    {
        var variables = unit.getVariables();
        var unitDefendedVariable = variables.createVariable("UNITDEFENDED");
        unitDefendedVariable.writeDataInt32(1);
    };
    this.doSupportDamageReduction = function(unit, damage, attacker, attackerPosition, defender, defenderPosition)
    {
        // gets called after the damage reduction has been applied
        var ret = [false, 0];
        var apply = ZCOUNIT_CHAPERON.getApplyReduction(unit, attackerPosition, defenderPosition);
        if (apply)
        {
            ret[0] = true;
            var variables = unit.getVariables();
            var unitDefendedVariable = variables.createVariable("UNITDEFENDED");
            unitDefendedVariable.writeDataInt32(0);
            unit.setAmmo1(unit.getAmmo1() - 1);
            if (unit.isStealthed(attacker.getOwner()))
            {
                ret[1] = ZCOUNIT_CHAPERON.getReductionDamage(damage, unit);
            }
        }
        return ret;
    };
    this.predictSupportDamageReduction = function(unit, damage, attacker, attackerPosition, attackerBaseHp,
                                                  defenderPosition, defender, luckMode)
    {
        var ret = [false, 0];
        var apply = ZCOUNIT_CHAPERON.getApplyReduction(unit, attackerPosition, defenderPosition);
        if (apply)
        {
            ret[0] = true;
            if (!unit.isStealthed(attacker.getOwner()))
            {
                ret[1] = ZCOUNIT_CHAPERON.getReductionDamage(damage, unit);
            }
        }
        return ret;
    };

    this.getReductionDamage = function(damage, unit)
    {
        return damage * unit.getHpRounded() / 10;
    };

    this.getApplyReduction = function(unit, attackerPosition, defenderPosition)
    {
        var variables = unit.getVariables();
        var unitDefendedVariable = variables.getVariable("UNITDEFENDED");
        var canDefend = 1;
        if (unitDefendedVariable !== null)
        {
            canDefend = unitDefendedVariable.readDataInt32();
        }
        var ret = false;
        var ammo1 = unit.getAmmo1();
        var pos = unit.getPosition();
        var unitDistance = globals.getDistance(defenderPosition, pos);
        if (canDefend > 0 &&
            unitDistance <= unit.getMaxRange(pos) &&
            globals.getDistance(defenderPosition, attackerPosition) > 1 &&
            ammo1 > 0)
        {
            ret = true;
        }
        return ret;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.getEditorPlacementSound = function()
    {
        return "movetank.wav";
    };
};

Constructor.prototype = UNIT;
var ZCOUNIT_CHAPERON = new Constructor();
