var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "MECH";
    };
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_MECH_MG");

        unit.setAmmo2(3);
        unit.setMaxAmmo2(3);
        unit.setWeapon2ID("WEAPON_DESIGNATOR");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
        unit.setMinRange(2);
        unit.setMaxRange(3);
        unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 3000;
    };

    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("ranger+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_MECH";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        animation.loadSpriteV2("ranger+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveboots.wav", -2);
        return animation;
    };
    this.getName = function()
    {
        return qsTr("Ranger");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("An indirect support unit. Uses the attack of the best available indirect unit which is in range of the ranger to attack an enemy unit in range of the ranger. This either extends the range of the indirect unit or boost it's damage output.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Direct;
    };
    this.getTypeOfWeapon2 = function(unit)
    {
        return GameEnums.WeaponType_Indirect;
    };

    this.getBonusOffensive = function(attacker, atkX, atkY, defender, defX, defY, isDefender, action, map)
    {
        var bonus = 0;
        if (globals.getDistance(atkX, atkY, defX, defY) > 1 && !isDefender)
        {
            var indirectToUse = ZCOUNIT_RANGER.getIndirectToUse(attacker, atkX, atkY, defX, defY, map);
            var baseDamage = Global["WEAPON_DESIGNATOR"].getBaseDamage(defender);
            var targetDamage = -1;
            var weaponId = indirectToUse.getWeapon1ID();
            if (weaponId !== "" && indirectToUse.hasAmmo1())
            {
                targetDamage = Global[weaponId].getBaseDamage(defender);
            }
            var targetDamage2 = -1;
            weaponId = indirectToUse.getWeapon2ID();
            if (weaponId !== "" && indirectToUse.hasAmmo2())
            {
                targetDamage2 = Global[weaponId].getBaseDamage(defender);
            }
            if  (targetDamage2 > targetDamage)
            {
                targetDamage = targetDamage2;
            }
            if (baseDamage > 0 && targetDamage > 0)
            {
                bonus = targetDamage / baseDamage * 100 - 100;
            }
            var pos = indirectToUse.getPosition();
            var distance = globals.getDistance(pos, Qt.point(defX, defY))
            if (distance >= indirectToUse.getMinRange(pos) &&
                distance <= indirectToUse.getMaxRange(pos))
            {
                bonus += 50;
            }
            var ownHp = attacker.getHpRounded();
            var unitToUseHp = indirectToUse.getHpRounded();

            var baseBonus = 100;
            var hpBonus = (unitToUseHp / ownHp) * (baseBonus + bonus) - baseBonus - bonus;
            bonus += hpBonus;
        }
        return bonus;
    };

    this.postBattleActions = function(unit, damage, otherUnit, gotAttacked, weapon, action, map)
    {
        if (weapon === 1 && gotAttacked === false)
        {
            var unitPos = unit.getPosition();
            var enemyPos = otherUnit.getPosition();
            var indirectToUse = ZCOUNIT_RANGER.getIndirectToUse(unit, unitPos.x, unitPos.y, enemyPos.x, enemyPos.y, map);
            if (indirectToUse !== null)
            {
                indirectToUse.setHasMoved(true);
                var targetDamage = Global[indirectToUse.getWeapon1ID()].getBaseDamage(defender);
                var targetDamage2 = Global[indirectToUse.getWeapon2ID()].getBaseDamage(defender);
                if  (targetDamage2 > targetDamage)
                {
                    indirectToUse.reduceAmmo1(1);
                }
                else
                {
                    indirectToUse.reduceAmmo2(1);

                }
            }
        }
    };

    this.getIndirectToUse = function(unit, unitX, unitY, targetX, targetY, map)
    {
        if (map.onMap(targetX, targetY))
        {
            var enemy = map.getTerrain(targetX, targetY).getUnit();
            if (enemy !== null)
            {
                var pos = Qt.point(unitX, unitY);
                var fields = globals.getCircle(1, unit.getMaxRange(pos));
                var owner = unit.getOwner();
                var indirectToUse = null;
                var damage = -1;
                var result = Qt.rect(-1, -1, -1, -1);
                for (var i = 0; i < fields.size(); i++)
                {
                    var field = fields.at(i);
                    var x = field.x + unitX;
                    var y = field.y + unitY;
                    if (map.onMap(x, y))
                    {
                        var indirectUnit = map.getTerrain(x, y).getUnit();
                        if (indirectUnit !== null &&
                            indirectUnit.getUnitID() !== "ZCOUNIT_RANGER" &&
                            indirectUnit.getOwner() === owner &&
                            !indirectUnit.getHasMoved())
                        {
                            result = ACTION_FIRE.calcAttackerWeaponDamage(null, indirectUnit, 0, indirectUnit.getPosition(),
                                                                          enemy, targetX, targetY, 0, GameEnums.LuckDamageMode_Average, result,
                                                                          GameEnums.AttackRangeCheck_None);
                            var newDamage = result.x * indirectUnit.getHpRounded() / 10;
                            if (newDamage > damage)
                            {
                                if ((result.y === 0 && indirectUnit.hasAmmo1()) ||
                                     indirectUnit.hasAmmo2())
                                {
                                    indirectToUse = indirectUnit;
                                    damage = newDamage;
                                }
                            }
                        }
                    }
                }
            }
        }
        return indirectToUse;
    };

    this.canUseWeapon = function(unit, weaponIndex, unitX, unitY, targetX, targetY, rangeCheck)
    {
        var pos = unit.getPosition();
        if (weaponIndex === 0 &&
            globals.getDistance(unitX, unitY, targetX, targetY) === 1)
        {
            return true;
        }
        else if (weaponIndex === 1 &&
                 pos.x === unitX &&
                 pos.y === unitY &&
                 ZCOUNIT_RANGER.getIndirectToUse(unit, unitX, unitY, targetX, targetY, unit.getMap()) !== null)
        {
            return true;
        }
        return false;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_RANGER = new Constructor();
