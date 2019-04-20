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
                    var defTerrain = map.getTerrain(x, y);
                    var defBuilding = defTerrain.getBuilding();
                    var defUnit = defTerrain.getUnit();
                    if (defUnit !== null)
                    {
                        if (ACTION_FIRE.isAttackableUnit(action, defUnit))
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
                    if (((defBuilding !== null) && (defBuilding.getHp() > 0) &&
                         (defBuilding.getIsAttackable(x, y) && unit.getOwner().isEnemy(defBuilding.getOwner()))) ||
                         (defTerrain.getHp() > 0))
                    {
                        if (unit.hasAmmo1())
                        {
                            if (Global[unit.getWeapon1ID()].getEnviromentDamage(defTerrain.getID()) > 0)
                            {
                                return true;
                            }
                        }
                        if (unit.hasAmmo2())
                        {
                            if (Global[unit.getWeapon2ID()].getEnviromentDamage(defTerrain.getID()) > 0)
                            {
                                return true;
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
    this.getStepCursor = function(action, cursorData)
    {
        cursorData.setCursor("cursor+attack");
        cursorData.setXOffset(- map.getImageSize() / 3);
        cursorData.setYOffset(- map.getImageSize() / 3);
        cursorData.setScale(1.5);
    };
    this.calcEnviromentDamage = function(attacker, attackerWeapon, attackerPosition, targetField, enviroment)
    {
        var baseDamage = Global[attackerWeapon].getEnviromentDamage(enviroment);
        var offensive = 100 + attacker.getBonusOffensive(attackerPosition, null, targetField, false);
        var attackerHp = attacker.getHpRounded() + attacker.getAttackHpBonus(attackerPosition);
        var damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, 100);
        return damage;
    };
    this.calcAttackerDamage = function(attacker, attackerWeapon, attackerPosition, defender, luckMode)
    {
        return ACTION_FIRE.calcDamage(attacker, attackerWeapon, attackerPosition, attacker.getHpRounded(),
                          defender, defender.getPosition(), false,
                          luckMode)
    };
    this.calcDefenderDamage = function(attacker, attackerPosition, defender, defenderWeapon, takenDamage, luckMode)
    {
        // only direct units can deal counter damage
        if (defender.getMinRange() === 1)
        {
            var health = defender.getHp() - takenDamage / 10.0;
            if (defender.getFirstStrike(defender.getPosition()))
            {
                health = defender.getHp();
            }
            health = globals.roundUp(health);
            var damage = -1;
            if (health > 0)
            {
                damage = ACTION_FIRE.calcDamage(defender, defenderWeapon, defender.getPosition(), health,
                                                attacker, attackerPosition, true,
                                                luckMode);
            }
        }
        return damage;
    };
    this.calcDamage = function(attacker, attackerWeapon, attackerPosition, attackerBaseHp,
                               defender, defenderPosition, isDefender,
                               luckMode)
    {
        var baseDamage = Global[attackerWeapon].getBaseDamage(defender);
        var damage = baseDamage;
        if (baseDamage > 0)
        {
            var offensive = 100 + attacker.getBonusOffensive(attackerPosition, defender, defender.getPosition(), isDefender);
            var defensive = 100 + defender.getBonusDefensive(defenderPosition, attacker, attackerPosition);
            var attackerHp = attackerBaseHp + attacker.getAttackHpBonus(attackerPosition);
            var damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, defensive);
            if (luckMode !== GameEnums.LuckDamageMode_Off)
            {
                var luck = attackerBaseHp / 2 + attacker.getBonusLuck(attackerPosition);
                var misfortune = attacker.getBonusMisfortune(attackerPosition);
                if (luckMode === GameEnums.LuckDamageMode_On)
                {
                    damage += globals.randInt(-misfortune, luck);
                }
                else if (luckMode === GameEnums.LuckDamageMode_Average)
                {
                    damage += (-misfortune + luck) / 2;
                }
            }
            damage -= defender.getDamageReduction(damage, attacker, attackerPosition, attackerBaseHp,
                                                  defenderPosition, isDefender);
            // avoid healing through negativ damage caused by misfortune
            if (baseDamage >= 0 && damage < 0)
            {
                damage = 0;
            }
        }
        return damage;
    };
    this.isAttackableUnit = function(action, defUnit)
    {
        var unit = action.getTargetUnit();
        if (unit.getOwner().getFieldVisible(defUnit.getX(), defUnit.getY()))
        {
            if (!defUnit.isStealthed(unit.getOwner()))
            {
                if (!defUnit.getHidden() ||
                    (defUnit.getHidden && unit.getMovementType() === defUnit.getMovementType()))
                {
                    return true;
                }
            }
        }
        return false;
    };

    this.calcBattleDamage = function(action, x, y, luckMode)
    {
        var result = Qt.rect(-1, -1, -1, -1);
        if (map.onMap(x, y))
        {
            var unit = action.getTargetUnit();
            var actionTargetField = action.getActionTarget();
            var targetField = action.getTarget();
            var defTerrain = map.getTerrain(x, y);
            var defBuilding = defTerrain.getBuilding();
            var defUnit = defTerrain.getUnit();
            var dmg1 = -1;
            var dmg2 = -1;
            if (defUnit !== null)
            {
                if (ACTION_FIRE.isAttackableUnit(action, defUnit))
                {
                    if (unit.getOwner().isEnemyUnit(defUnit) === true)
                    {
                        if (unit.hasAmmo1())
                        {
                            dmg1 = ACTION_FIRE.calcAttackerDamage(unit, unit.getWeapon1ID(), actionTargetField ,defUnit, luckMode);
                        }
                        if (unit.hasAmmo2())
                        {
                            dmg2 = ACTION_FIRE.calcAttackerDamage(unit, unit.getWeapon2ID(), actionTargetField ,defUnit, luckMode);
                        }
                        if ((dmg1 > 0) || (dmg2 > 0))
                        {
                            if (dmg1 >= dmg2)
                            {
                                result.x = dmg1;
                                result.y = 0;
                            }
                            else
                            {
                                result.x = dmg2;
                                result.y = 1;
                            }
                        }
                        if (Math.abs(actionTargetField.x - x) + Math.abs(actionTargetField.y - y) === 1)
                        {
                            var defDamage = -1;
                            var defWeapon = 0;
                            if (defUnit.hasAmmo1())
                            {
                                defDamage = ACTION_FIRE.calcDefenderDamage(unit, actionTargetField, defUnit, defUnit.getWeapon1ID(), result.x, luckMode);
                            }
                            if (defUnit.hasAmmo2())
                            {
                                var defDamage2 = ACTION_FIRE.calcDefenderDamage(unit, actionTargetField, defUnit, defUnit.getWeapon2ID(), result.x, luckMode);
                                if (defDamage2 > defDamage)
                                {
                                    defDamage = defDamage2;
                                    defWeapon = 1;
                                }
                            }
                            if (defDamage >= 0)
                            {
                                result.width = defDamage
                                result.height = defWeapon
                            }
                        }
                    }
                }
            }
            else
            {
                if (((defBuilding !== null) && (defBuilding.getHp() > 0) &&
                     (defBuilding.getIsAttackable(x, y) && unit.getOwner().isEnemy(defBuilding.getOwner()))) ||
                     (defTerrain.getHp() > 0))
                {
                    if (unit.hasAmmo1())
                    {
                        dmg1 = ACTION_FIRE.calcEnviromentDamage(unit, unit.getWeapon1ID(), actionTargetField, Qt.point(x, y), defTerrain.getID());
                    }
                    if (unit.hasAmmo2())
                    {
                        dmg2 = ACTION_FIRE.calcEnviromentDamage(unit, unit.getWeapon2ID(), actionTargetField, Qt.point(x, y), defTerrain.getID());
                    }
                    if ((dmg1 > 0) || (dmg2 > 0))
                    {
                        if (dmg1 >= dmg2)
                        {
                            result.x = dmg1;
                            result.y = 1;
                        }
                        else
                        {
                            result.x = dmg2;
                            result.y = 2;
                        }
                    }
                }
            }
        }
        return result;
    };

    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var targetField = action.getTarget();
        var actionTargetField = action.getActionTarget();
        var fields = globals.getCircle(unit.getMinRange(), unit.getMaxRange(targetField));
        data.setColor("#C8FF0000");
        data.setZLabelColor("#ff4500");
        data.setZLabelText(qsTr("Damage"))
        data.setShowZData(true);
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + actionTargetField.x;
            var y = fields.at(i).y + actionTargetField.y;
            var result = ACTION_FIRE.calcBattleDamage(action, x, y, GameEnums.LuckDamageMode_Off);
            if (result.x >= 0)
            {
                data.addPoint(Qt.point(x, y));
                data.addZInformation(result.x);
            }
        }
        fields.remove();
    };
    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 1)
        {
            // get the target unit
            action.startReading();
            var targetX = action.readDataInt32();
            var targetY = action.readDataInt32();
            var result = ACTION_FIRE.calcBattleDamage(action, targetX, targetY, GameEnums.LuckDamageMode_On);
            action.writeDataInt32(result.x);
            action.writeDataInt32(result.y);
            action.writeDataInt32(result.width);
            action.writeDataInt32(result.height);
            return true;
        }
        else
        {
            return false;
        }
    };

    this.postAnimationUnit = null;
    this.postAnimationAttackerDamage = -1;
    this.postAnimationAttackerWeapon = -1;
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.postAnimationDefenderDamage = -1;
    this.postAnimationDefenderWeapon = -1;

    this.perform = function(action)
    {
        // we need to move the unit to the target position
        ACTION_FIRE.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_FIRE.postAnimationUnit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_FIRE", "performPostAnimation");
        // move unit to target position
        ACTION_FIRE.postAnimationUnit.moveUnitAction(action);
        // disable unit commandments for this turn
        ACTION_FIRE.postAnimationUnit.setHasMoved(true);
        action.startReading();
        // read action data
        ACTION_FIRE.postAnimationTargetX = action.readDataInt32();
        ACTION_FIRE.postAnimationTargetY = action.readDataInt32();
        ACTION_FIRE.postAnimationAttackerDamage = action.readDataInt32();
        ACTION_FIRE.postAnimationAttackerWeapon = action.readDataInt32();
        ACTION_FIRE.postAnimationDefenderDamage = action.readDataInt32();
        ACTION_FIRE.postAnimationDefenderWeapon = action.readDataInt32();
    };
    this.performPostAnimation = function()
    {
        var defTerrain = map.getTerrain(ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY);
        var defBuilding = defTerrain.getBuilding();
        var defUnit = defTerrain.getUnit();
        if (defUnit !== null)
        {
            var defOwner = defUnit.getOwner();
            var defUnitX = defUnit.getX();
            var defUnitY = defUnit.getY();
            var costs = defUnit.getCosts();
            var damage = ACTION_FIRE.postAnimationAttackerDamage / 10.0;
            var power = 0;
            // gain power based
            if (damage > defUnit.getHp())
            {
                power = costs * defUnit.getHp() / 10;
                damage = defUnit.getHp();
                defUnit.getOwner().gainPowerstar(power, Qt.point(defUnit.getX(), defUnit.getY()));
                ACTION_FIRE.postAnimationUnit.getOwner().gainPowerstar(power / 4,
                                                                       Qt.point(ACTION_FIRE.postAnimationUnit.getX(), ACTION_FIRE.postAnimationUnit.getY()));
            }
            else
            {
                power = costs * damage / 10;
                defUnit.getOwner().gainPowerstar(power, Qt.point(defUnit.getX(), defUnit.getY()));
                ACTION_FIRE.postAnimationUnit.getOwner().gainPowerstar(power / 4,
                                                                       Qt.point(ACTION_FIRE.postAnimationUnit.getX(), ACTION_FIRE.postAnimationUnit.getY()));
            }
            // set attacker damage
            defUnit.setHp(defUnit.getHp() - damage);
            // post battle action of the attacking unit
            ACTION_FIRE.postAnimationUnit.getOwner().postBattleActions(ACTION_FIRE.postAnimationUnit, damage, defUnit, false);
            defOwner.postBattleActions(ACTION_FIRE.postAnimationUnit, damage, defUnit, true);
            if (defUnit.getHp() <= 0)
            {
                defUnit.killUnit();
                defUnit = null;
                if (ACTION_FIRE.postAnimationUnit.getUnitRank() < GameEnums.UnitRank_Veteran)
                {
                    ACTION_FIRE.postAnimationUnit.setUnitRank(ACTION_FIRE.postAnimationUnit.getUnitRank() + 1);
                }
            }
            if (ACTION_FIRE.postAnimationAttackerWeapon === 0)
            {
                ACTION_FIRE.postAnimationUnit.reduceAmmo1(1);
            }
            else
            {
                ACTION_FIRE.postAnimationUnit.reduceAmmo2(1);
            }

            // set counter damage
            if (ACTION_FIRE.postAnimationDefenderDamage > 0)
            {
                ACTION_FIRE.postAnimationUnit.setHp(ACTION_FIRE.postAnimationUnit.getHp() - ACTION_FIRE.postAnimationDefenderDamage / 10.0);

                costs = ACTION_FIRE.postAnimationUnit.getCosts();
                damage = ACTION_FIRE.postAnimationDefenderDamage / 10.0;
                // gain power based
                if (damage > ACTION_FIRE.postAnimationUnit.getHp())
                {
                    power = costs * ACTION_FIRE.postAnimationUnit.getHp() / 10;
                    damage = ACTION_FIRE.postAnimationUnit.getHp();
                    ACTION_FIRE.postAnimationUnit.getOwner().gainPowerstar(power,
                                                                           Qt.point(ACTION_FIRE.postAnimationUnit.getX(), ACTION_FIRE.postAnimationUnit.getY()));
                    defOwner.gainPowerstar(power / 4, Qt.point(defUnitX, defUnitY));
                }
                else
                {
                    power = costs * damage / 10;
                    ACTION_FIRE.postAnimationUnit.getOwner().gainPowerstar(power,
                                                                           Qt.point(ACTION_FIRE.postAnimationUnit.getX(), ACTION_FIRE.postAnimationUnit.getY()));
                    defOwner.gainPowerstar(power / 4, Qt.point(defUnitX, defUnitY));
                }
                // post battle action of the defending unit
                defOwner.postBattleActions(defUnit, damage, ACTION_FIRE.postAnimationUnit, false);
                ACTION_FIRE.postAnimationUnit.getOwner().postBattleActions(defUnit, damage, ACTION_FIRE.postAnimationUnit, true);
                if (ACTION_FIRE.postAnimationUnit.getHp() <= 0)
                {
                    ACTION_FIRE.postAnimationUnit.killUnit();
                    ACTION_FIRE.postAnimationUnit = null;
                    if (defUnit !== null)
                    {
                        if (defUnit.getUnitRank() < GameEnums.UnitRank_Veteran)
                        {
                            defUnit.setUnitRank(defUnit.getUnitRank() + 1);
                        }
                    }
                }
                if (defUnit !== null)
                {
                    if (ACTION_FIRE.postAnimationDefenderWeapon === 0)
                    {
                        defUnit.reduceAmmo1(1);
                    }
                    else
                    {
                        defUnit.reduceAmmo2(1);
                    }
                }
            }
        }
        else
        {
            // we attacked a building or terrain ;)
            // not implemented yet.
            if ((defBuilding !== null) && (defBuilding.getHp() > 0))
            {
                defBuilding.setHp(defBuilding.getHp() - ACTION_FIRE.postAnimationAttackerDamage);
                if (ACTION_FIRE.postAnimationAttackerWeapon === 0)
                {
                    ACTION_FIRE.postAnimationUnit.reduceAmmo1(1);
                }
                else
                {
                    ACTION_FIRE.postAnimationUnit.reduceAmmo2(1);
                }
                if (defBuilding.getHp() <= 0)
                {
                    Global[defBuilding.getBuildingID()].onDestroyed(defBuilding);
                }
            }
            // check for damage against terrain
            else if (defTerrain.getHp() > 0)
            {
                defTerrain.setHp(defTerrain.getHp() - ACTION_FIRE.postAnimationAttackerDamage);
                if (ACTION_FIRE.postAnimationAttackerWeapon === 0)
                {
                    ACTION_FIRE.postAnimationUnit.reduceAmmo1(1);
                }
                else
                {
                    ACTION_FIRE.postAnimationUnit.reduceAmmo2(1);
                }
                if (defTerrain.getHp() <= 0)
                {
                    Global[defTerrain.getID()].onDestroyed(defTerrain);
                }
            }
        }

        // reset stuff
        ACTION_FIRE.postAnimationUnit = null;
        ACTION_FIRE.postAnimationAttackerDamage = -1;
        ACTION_FIRE.postAnimationAttackerWeapon = -1;
        ACTION_FIRE.postAnimationTargetX = -1;
        ACTION_FIRE.postAnimationTargetY = -1;
        ACTION_FIRE.postAnimationDefenderDamage = -1;
        ACTION_FIRE.postAnimationDefenderWeapon = -1;
    };

}


Constructor.prototype = ACTION;
var ACTION_FIRE = new Constructor();
