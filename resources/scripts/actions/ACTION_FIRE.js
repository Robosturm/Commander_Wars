var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true))
        {
            return false;
        }
        // are we allowed to attack from this field?
        if (((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y)) ||
            ((action.getMovementTarget() === null) && unit.canMoveAndFire(targetField)))
        {
            var fields = globals.getCircle(unit.getMinRange(), unit.getMaxRange(targetField));
            // check all fields we can attack
            for (var i = 0; i < fields.size(); i++)
            {
                var x = fields.at(i).x + actionTargetField.x;
                var y = fields.at(i).y + actionTargetField.y;
                // check with which weapon we can attack and if we could deal damage with this weapon
                if (map.onMap(x, y))
                {
                    var defUnit = map.getTerrain(x, y).getUnit();
                    if (defUnit !== null)
                    {
                        if (unit.getOwner().isEnemyUnit(defUnit) === true)
                        {
                            if (unit.hasAmmo1())
                            {
                                if (Global[unit.getWeapon1ID()].getBaseDamage(defUnit) > 0)
                                {
                                    return true;
                                }
                            }
                            if (unit.hasAmmo2())
                            {
                                if (Global[unit.getWeapon2ID()].getBaseDamage(defUnit) > 0)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            fields.remove();
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Fire");
    };
    this.getIcon = function()
    {
        return "fire";
    };
    this.getStepInputType = function(action)
    {
        return "FIELD";
    };
    this.getStepCursor = function(action)
    {
        return "cursor+attack";
    };
    this.calcAttackerDamage = function(attacker, attackerWeapon, attackerPosition, defender, useLuck)
    {
        return ACTION_FIRE.calcDamage(attacker, attackerWeapon, attackerPosition, attacker.getHpRounded(),
                          defender, useLuck)
    };
    this.calcDeffenderDamage = function(attacker, attackerPosition, defender, defenderWeapon, takenDamage, useLuck)
    {

    };
    this.calcDamage = function(attacker, attackerWeapon, attackerPosition, attackerBaseHp,
                               defender, useLuck)
    {
        var baseDamage = Global[attackerWeapon].getBaseDamage(defender);
        var offensive = 100 + attacker.getBonusOffensive(attackerPosition, defender, defender.getPosition());
        var defensive = 100 + defender.getBonusDefensive(defender.getPosition(), attacker, attackerPosition);
        var attackerHp = attackerBaseHp + attacker.getAttackHpBonus(attackerPosition);
        var damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, defensive);
        if (useLuck)
        {
            var luck = attackerBaseHp / 2 + attacker.getBonusLuck(attackerPosition);
            var misfortune = attacker.getBonusMisfortune(attackerPosition);
            damage += globals.randInt(misfortune, luck);
        }
        return damage;
    };
    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var fields = globals.getCircle(unit.getMinRange(), unit.getMaxRange(targetField));
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + actionTargetField.x;
            var y = fields.at(i).y + actionTargetField.y;
            if (map.onMap(x, y))
            {
                var defUnit = map.getTerrain(x, y).getUnit();
                if (defUnit !== null)
                {
                    if (unit.getOwner().isEnemyUnit(defUnit) === true)
                    {
                        var dmg1 = -1;
                        if (unit.hasAmmo1())
                        {
                            dmg1 = ACTION_FIRE.calcAttackerDamage(unit, unit.getWeapon1ID(), actionTargetField ,defUnit, false);
                        }
                        var dmg2 = -1;
                        if (unit.hasAmmo2())
                        {
                            dmg2 = ACTION_FIRE.calcAttackerDamage(unit, unit.getWeapon2ID(), actionTargetField ,defUnit, false);
                        }
                        if ((dmg1 > 0) || (dmg2 > 0))
                        {
                            if (dmg1 >= dmg2)
                            {
                                data.addPoint(Qt.point(x, y));
                                data.addZInformation(dmg1);
                            }
                            else
                            {
                                data.addPoint(Qt.point(x, y));
                                data.addZInformation(dmg2);
                            }
                        }
                    }
                }
            }
        }
        fields.remove();
        data.setColor("#FF0000");
        data.setZLabelColor("#ff4500");
        data.setZLabelText(qsTr("Damage"))
        data.setShowZData(true);
    };
    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        ACTION_UNLOAD.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_UNLOAD.postAnimationUnit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_FIRE", "performPostAnimation");
        // move unit to target position
        ACTION_UNLOAD.postAnimationUnit.moveUnitAction(action);
        // disable unit commandments for this turn
        ACTION_UNLOAD.postAnimationUnit.setHasMoved(true);
        action.startReading();

        //
    };
    this.performPostAnimation = function()
    {

    };

}


Constructor.prototype = ACTION;
var ACTION_FIRE = new Constructor();
