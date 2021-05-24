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
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("chaperon", false);
        unit.loadSpriteV2("chaperon+mask", GameEnums.Recoloring_Table);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        animation.loadSpriteV2("chaperon+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("chaperon+walk", false, 1.5);
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
    this.startOfTurn = function(unit)
    {
        var variables = unit.getVariables();
        var unitDefendedVariable = variables.createVariable("UNITDEFENDED");
        unitDefendedVariable.writeDataInt32(1);
    };
    this.doSupportDamageReduction = function(unit, attacker, attackerPosition, defender, defenderPosition)
    {
        // gets called after the damage reduction has been applied
        var ret = false;
        var variables = unit.getVariables();
        var unitDefendedVariable = variables.getVariable("UNITDEFENDED");
        var canDefend = 1;
        if (unitDefendedVariable !== null)
        {
            canDefend = unitDefendedVariable.readDataInt32();
        }
        var ammo1 = unit.getAmmo1()
        var pos = unit.getPosition();
        if (canDefend > 0 &&
            globals.getDistance(defenderPosition, pos) < unit.getMaxRange(pos) &&
            globals.getDistance(defenderPosition, attackerPosition) > 1 &&
            ammo1 > 1)
        {
            unitDefendedVariable = variables.createVariable("UNITDEFENDED");
            unitDefendedVariable.writeDataInt32(0);
            unit.setAmmo1(unit.getAmmo1() - 1);
            ret = true;
        }
        return ret;
    };
    this.predictSupportDamageReduction = function(unit, damage, attacker, attackerPosition, attackerBaseHp,
                                                  defenderPosition, defender, luckMode)
    {
        var variables = unit.getVariables();
        var unitDefendedVariable = variables.getVariable("UNITDEFENDED");
        var canDefend = 1;
        if (unitDefendedVariable !== null)
        {
            canDefend = unitDefendedVariable.readDataInt32();
        }
        var ret = 0;
        var ammo1 = unit.getAmmo1();
        var pos = unit.getPosition();
        if (canDefend > 0 &&
            globals.getDistance(defenderPosition, pos) <= unit.getMaxRange(pos) &&
            globals.getDistance(defenderPosition, attackerPosition) > 1 &&
            ammo1 > 0)
        {
            ret = damage * unit.getHpRounded() / 10;
        }
        return ret;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_CHAPERON = new Constructor();
