var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var ret = false;
        // are we allowed to attack from this field?
        if (unit.getHasMoved() === false &&
           ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y)) ||
           ((action.getMovementTarget() === null) && unit.canMoveAndFire(targetField) && (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) > 0)))
        {
            var minRange = unit.getMinRange(actionTargetField);
            var fields = globals.getCircle(minRange, unit.getMaxRange(actionTargetField));
            if (unit.hasDirectWeapon() && minRange > 1)
            {
                var minFields = globals.getCircle(1, 1);
                var size = minFields.size();
                for (var i = 0; i < size; ++i)
                {
                    fields.append(minFields.at(i))
                }
            }

            // check all fields we can attack
            var size = fields.size();
            for (var i = 0; i < size; i++)
            {
                var field = fields.at(i);
                var x = field.x + actionTargetField.x;
                var y = field.y + actionTargetField.y;
                if (unit.getOwner().getFieldVisibleType(x, y) !== GameEnums.VisionType_Shrouded)
                {
                    // check with which weapon we can attack and if we could deal damage with this weapon
                    if (map.onMap(x, y))
                    {
                        var defTerrain = map.getTerrain(x, y);
                        var defBuilding = defTerrain.getBuilding();
                        var defUnit = defTerrain.getUnit();
                        if (defUnit !== null)
                        {
                            if (unit.isAttackableFromPosition(defUnit, actionTargetField))
                            {
                                ret = true
                                break;
                            }
                        }
                        if (((defBuilding !== null) && (defBuilding.getHp() > 0) &&
                             (defBuilding.getIsAttackable(x, y) && unit.getOwner().isEnemy(defBuilding.getOwner()))) ||
                                (defTerrain.getHp() > 0))
                        {
                            if (unit.hasAmmo1() && unit.getWeapon1ID() !== "" &&
                                unit.canAttackWithWeapon(0, actionTargetField.x, actionTargetField.y, x, y))
                            {
                                if (Global[unit.getWeapon1ID()].getEnviromentDamage(defTerrain.getID()) > 0)
                                {
                                    ret = true
                                    break;
                                }
                            }
                            if (unit.hasAmmo2() && unit.getWeapon2ID() !== "" &&
                                unit.canAttackWithWeapon(1, actionTargetField.x, actionTargetField.y, x, y))
                            {
                                if (Global[unit.getWeapon2ID()].getEnviromentDamage(defTerrain.getID()) > 0)
                                {
                                    ret = true
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        return ret;
    };
    this.getActionText = function(map)
    {
        return qsTr("Fire");
    };
    this.getIcon = function(map)
    {
        return "icon_fire";
    };
    this.getStepInputType = function(action, map)
    {
        return "FIELD";
    };
    this.getStepCursor = function(action, cursorData, map)
    {
        cursorData.setCursor("cursor+attack");
        cursorData.setXOffset(- map.getImageSize() / 3);
        cursorData.setYOffset(- map.getImageSize() / 3);
        cursorData.setScale(2);
    };
    this.calcEnviromentDamage = function(action, attacker, attackerWeapon, attackerPosition, targetField, enviroment, luckMode)
    {
        var baseDamage = Global[attackerWeapon].getEnviromentDamage(enviroment);
        var offensive = 100 + attacker.getBonusOffensive(action, attackerPosition, null, targetField, false, luckMode);
        var attackerHp = attacker.getHpRounded() + attacker.getAttackHpBonus(attackerPosition);
        var damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, 100, 0);
        return damage;
    };

    this.calcAttackerWeaponDamage = function(action, unit, attackerTakenDamage, actionTargetField,
                                             defUnit, x, y, defenderTakenDamage, luckMode, result, rangeCheck = 0, fast = false, fastInaccurate = false)
    {
        var baseDamage1 = -1;
        var baseDamage2 = -1;
        var weaponID1 = unit.getWeapon1ID();
        if (unit.hasAmmo1() && weaponID1 !== "" &&
            unit.canAttackWithWeapon(0, actionTargetField.x, actionTargetField.y, x, y, rangeCheck))
        {
            baseDamage1 = Global[weaponID1].getBaseDamage(defUnit);
        }
        var weaponID2 = unit.getWeapon2ID();
        if (unit.hasAmmo2() && weaponID2 !== "" &&
            unit.canAttackWithWeapon(1, actionTargetField.x, actionTargetField.y, x, y, rangeCheck))
        {
            baseDamage2 = Global[weaponID2].getBaseDamage(defUnit);
        }

        if ((baseDamage1 >= 0.0) || (baseDamage2 >= 0.0))
        {
            if (baseDamage1 >= baseDamage2)
            {
                if (fast)
                {
                    result.x = baseDamage1;
                }
                else
                {
                    result.x = ACTION_FIRE.calcAttackerDamage(action, unit, weaponID1, attackerTakenDamage, actionTargetField, defUnit, luckMode, fastInaccurate);
                }
                result.y = 0;
            }
            else
            {
                if (fast)
                {
                    result.x = baseDamage2;
                }
                else
                {
                    result.x = ACTION_FIRE.calcAttackerDamage(action, unit, weaponID2, attackerTakenDamage, actionTargetField, defUnit, luckMode, fastInaccurate);
                }
                result.y = 1;
            }
        }
        return result;
    };
    this.calcAttackerDamage = function(action, attacker, attackerWeapon, takenDamage, attackerPosition, defender, luckMode, fastInaccurate = false)
    {
        var hp = globals.roundUp(attacker.getVirtualHp() - takenDamage / 10.0);
        if (hp < 0)
        {
            return 0;
        }
        else
        {
            return ACTION_FIRE.calcDamage(action, attacker, attackerWeapon, attackerPosition, hp,
                                          defender, defender.getPosition(), false,
                                          luckMode, fastInaccurate);
        }
    };

    this.calcDefenderWeaponDamage = function(action, unit, attackerTakenDamage, actionTargetField, defUnit, x, y, defenderTakenDamage, luckModeDefender, result, rangeCheck = 0, fastInaccurate = false)
    {
        var baseDamage1 = -1;
        var baseDamage2 = -1;
        var weaponID1 = defUnit.getWeapon1ID();
        if (defUnit.hasAmmo1() && weaponID1 !== "" &&
            defUnit.canAttackWithWeapon(0, x, y, actionTargetField.x, actionTargetField.y, rangeCheck))
        {
            baseDamage1 = Global[weaponID1].getBaseDamage(unit);
        }
        var weaponID2 = defUnit.getWeapon2ID();
        if (defUnit.hasAmmo2() && weaponID2 !== "" &&
            defUnit.canAttackWithWeapon(1, x, y, actionTargetField.x, actionTargetField.y, rangeCheck))
        {
            baseDamage2 = Global[weaponID2].getBaseDamage(unit);
        }
        if ((baseDamage1 >= 0.0) || (baseDamage2 >= 0.0))
        {
            if (baseDamage1 >= baseDamage2)
            {
                result.width = ACTION_FIRE.calcDefenderDamage(action, unit, actionTargetField, defUnit, weaponID1, defenderTakenDamage, luckModeDefender, x, y, fastInaccurate);
                result.height = 0;
            }
            else
            {
                result.width = ACTION_FIRE.calcDefenderDamage(action, unit, actionTargetField, defUnit, weaponID2, defenderTakenDamage, luckModeDefender, x, y, fastInaccurate);
                result.height = 1;
            }
        }
        return result;
    };
    this.calcDefenderDamage = function(action, attacker, attackerPosition, defender, defenderWeapon, takenDamage, luckMode, defenderX, defenderY, fastInaccurate = false)
    {
        var damage = -1;
        // only direct units can deal counter damage
        var defPos = Qt.point(defenderX, defenderY);
        var health = defender.getVirtualHp() - takenDamage / 10.0;
        damage = ACTION_FIRE.calcDamage(action, defender, defenderWeapon, defPos, health,
                                        attacker, attackerPosition, true,
                                        luckMode, fastInaccurate);
        return damage;
    };
    this.calcDamage = function(action, attacker, attackerWeapon, attackerPosition, attackerBaseHp,
                               defender, defenderPosition, isDefender,
                               luckMode, fastInaccurate = false)
    {
        if (attackerBaseHp <= 0)
        {
            return -1;
        }
        var baseDamage = Global[attackerWeapon].getBaseDamage(defender);
        var damage = baseDamage;
        if (baseDamage > 0.0)
        {
            var virtualHp = attacker.getVirtualHpValue();
            attacker.setVirtualHpValue(attackerBaseHp);
            var offensive = 100;
            var defensive = 100;
            var attackerHp = attackerBaseHp;
            var luckDamage = 0;
            if (fastInaccurate === false)
            {
                offensive += attacker.getBonusOffensive(action, attackerPosition, defender, defenderPosition, isDefender, luckMode);
                defensive += defender.getBonusDefensive(action, defenderPosition, attacker, attackerPosition, isDefender, luckMode);
                attackerHp += attacker.getAttackHpBonus(attackerPosition);
                if (luckMode !== GameEnums.LuckDamageMode_Off)
                {
                    var luck = 10 + attacker.getBonusLuck(attackerPosition);
                    var misfortune = attacker.getBonusMisfortune(attackerPosition);
                    // only roll if we have valid luck misfortune pair
                    if (luck > -misfortune)
                    {
                        if (luckMode === GameEnums.LuckDamageMode_On)
                        {
                            var luckValue = 0;
                            // roll luck?
                            if (luck > 0)
                            {
                                // roll against zero or against negative misfortune?
                                if (misfortune < 0)
                                {
                                    luckValue = globals.randInt(-misfortune, luck);
                                }
                                else
                                {
                                    luckValue = globals.randInt(0, luck);
                                }
                            }
                            var misfortuneValue = 0;
                            // roll misfortune?
                            if (misfortune > 0)
                            {
                                // roll against zero or against luck?
                                if (luck < 0)
                                {
                                    misfortuneValue = globals.randInt(-misfortune, luck);
                                }
                                else
                                {
                                    misfortuneValue = globals.randInt(-misfortune, 0);
                                }
                            }
                            luckDamage += (luckValue + misfortuneValue);
                        }
                        else if (luckMode === GameEnums.LuckDamageMode_Average)
                        {
                            luckDamage += (-misfortune + luck) / 2;
                        }
                        else if (luckMode === GameEnums.LuckDamageMode_Min)
                        {
                            luckDamage -= misfortune;
                        }
                        else if (luckMode === GameEnums.LuckDamageMode_Max)
                        {
                            luckDamage += luck;
                        }
                    }
                }
            }
            else
            {
                luckDamage = 5;
            }
            damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, defensive, luckDamage);
            if (fastInaccurate === false)
            {
                damage += attacker.getTrueDamage(action, damage, attackerPosition, attackerBaseHp,
                                                 defender, defenderPosition, isDefender, luckMode);
                damage -= defender.getDamageReduction(action, damage, attacker, attackerPosition, attackerBaseHp,
                                                      defenderPosition, isDefender, luckMode);
                damage -= ACTION_FIRE.predictSupportDamageReduction(damage, attacker, attackerPosition, attackerBaseHp,
                                                                    defenderPosition, defender, luckMode);
            }
            // avoid healing through negativ damage caused by misfortune or other stuff
            if (damage <= 0.0)
            {
                damage = 0.0;
            }
            attacker.setVirtualHpValue(virtualHp);
        }
        return damage;
    };

    this.predictSupportDamageReduction = function(damage, attacker, attackerPosition, attackerBaseHp,
                                                  defenderPosition, defender, luckMode)
    {
        var ret = 0;
        var owner = defender.getOwner();
        var units = owner.getUnits();
        var size = units.size();
        for (var i = 0; i < size; ++i)
        {
            var unit = units.at(i);
            var unitId = unit.getUnitID();
            if (typeof Global[unitId].predictSupportDamageReduction !== 'undefined')
            {
                var retInfo = Global[unitId].predictSupportDamageReduction(unit, damage, attacker, attackerPosition, attackerBaseHp,
                                                                           defenderPosition, defender, luckMode);
                if (retInfo[0] === true)
                {
                    ret = retInfo[1];
                    break;
                }
            }
        }
        return ret;
    };
    this.doSupportDamageReduction = function(damage, attacker, attackerPosition, defender, defenderPosition)
    {
        var reduction = 0;
        var owner = defender.getOwner();
        var units = owner.getUnits();
        var size = units.size();
        for (var i = 0; i < size; ++i)
        {
            var unit = units.at(i);
            var unitId = unit.getUnitID();
            if (typeof Global[unitId].doSupportDamageReduction !== 'undefined')
            {
                var ret = Global[unitId].doSupportDamageReduction(unit, damage, attacker, attackerPosition, defender, defenderPosition);
                if (ret[0] === true)
                {
                    reduction = ret[1];
                    break;
                }
            }
        }
        return reduction;
    };

    this.calcBattleDamage = function(map, action, x, y, luckMode)
    {
        return ACTION_FIRE.calcBattleDamage2(map, action, action.getTargetUnit(), action.getActionTarget(),
                                             x, y, luckMode);
    };

    this.calcBattleDamage2 = function(map, action, attacker, atkPos, x, y, luckMode)
    {
        return ACTION_FIRE.calcBattleDamage3(map, action, attacker, 0, atkPos.x, atkPos.y, null, x, y, 0, luckMode, luckMode);
    };

    this.calcBattleDamage3 = function(map, action, attacker, attackerTakenDamage, atkPosX, atkPosY, defender, x, y, defenderTakenDamage, luckModeAtk, luckModeDef, ignoreOutOfVisionRange = false, fastInaccurate = false)
    {
        return ACTION_FIRE.calcBattleDamage4(map, action, attacker, 0, atkPosX, atkPosY, defender, x, y, 0, luckModeAtk, luckModeDef, ignoreOutOfVisionRange, false, fastInaccurate);
    }

    this.calcBattleDamage4 = function(map, action, attacker, attackerTakenDamage, atkPosX, atkPosY, defender, x, y, defenderTakenDamage, luckMode, luckModeDefender, ignoreOutOfVisionRange = false, fast = false, fastInaccurate = false)
    {
        var result = Qt.rect(-1, -1, -1, -1);
        if (map.onMap(x, y))
        {
            var unit = attacker;
            var actionTargetField = Qt.point(atkPosX, atkPosY);
            var defTerrain = map.getTerrain(x, y);
            var defBuilding = defTerrain.getBuilding();
            var defUnit = defTerrain.getUnit();
            if (defender !== null)
            {
                defUnit = defender;
            }
            if (defUnit !== null)
            {
                if (action === null ||
                    fastInaccurate ||
                    unit.isAttackable(defUnit, ignoreOutOfVisionRange, Qt.point(atkPosX, atkPosY)))
                {
                    if (fastInaccurate || (defUnit.isAttackable(unit, true, actionTargetField, true) &&
                        defUnit.canCounterAttack(action, Qt.point(x, y), attacker, actionTargetField, luckMode)))
                    {
                        var defPos = defUnit.getPosition();
                        var defFirststrike = false;
                        var atkFirststrike = false;
                        if (fastInaccurate === false)
                        {
                            defFirststrike = defUnit.getFirstStrike(defPos, attacker, true, actionTargetField);
                            atkFirststrike = attacker.getFirstStrike(actionTargetField, defUnit, false, defPos);
                        }
                        if (defFirststrike && !atkFirststrike)
                        {
                            if (!fast)
                            {
                                result = ACTION_FIRE.calcDefenderWeaponDamage(action, unit, attackerTakenDamage, actionTargetField, defUnit, x, y, defenderTakenDamage, luckModeDefender, result, 0, fastInaccurate);
                            }
                            result = ACTION_FIRE.calcAttackerWeaponDamage(action, unit, result.width + attackerTakenDamage, actionTargetField, defUnit, x, y, defenderTakenDamage, luckMode, result, 0, fast, fastInaccurate);
                        }
                        else
                        {
                            result = ACTION_FIRE.calcAttackerWeaponDamage(action, unit, attackerTakenDamage, actionTargetField, defUnit, x, y, defenderTakenDamage, luckMode, result, 0, fast, fastInaccurate);
                            if (!fast)
                            {
                                result = ACTION_FIRE.calcDefenderWeaponDamage(action, unit, attackerTakenDamage, actionTargetField, defUnit, x, y, result.x + defenderTakenDamage, luckModeDefender, result, 0, fastInaccurate);
                            }
                        }
                    }
                    else
                    {
                        result = ACTION_FIRE.calcAttackerWeaponDamage(action, unit, attackerTakenDamage, actionTargetField, defUnit, x, y, defenderTakenDamage, luckMode, result, 0, fast, fastInaccurate);
                    }
                }
            }
            else
            {
                var dmg1 = -1;
                var dmg2 = -1;
                if (((defBuilding !== null) && (defBuilding.getHp() > 0) &&
                     (defBuilding.getIsAttackable(x, y) && unit.getOwner().isEnemy(defBuilding.getOwner()))) ||
                        (defTerrain.getHp() > 0))
                {
                    if (unit.hasAmmo1() && unit.getWeapon1ID() !== "" &&
                        unit.canAttackWithWeapon(0, atkPosX, atkPosY, x, y))
                    {
                        dmg1 = ACTION_FIRE.calcEnviromentDamage(action, unit, unit.getWeapon1ID(), actionTargetField, Qt.point(x, y), defTerrain.getID(), luckMode);
                    }
                    if (unit.hasAmmo2() && unit.getWeapon2ID() !== "" &&
                        unit.canAttackWithWeapon(1, atkPosX, atkPosY, x, y))
                    {
                        dmg2 = ACTION_FIRE.calcEnviromentDamage(action, unit, unit.getWeapon2ID(), actionTargetField, Qt.point(x, y), defTerrain.getID(), luckMode);
                    }
                    if ((dmg1 > 0.0) || (dmg2 > 0.0))
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
                }
            }
        }
        return result;
    };

    this.getStepData = function(action, data, map)
    {
        var unit = action.getTargetUnit();
        var targetField = action.getTarget();
        var actionTargetField = action.getActionTarget();
        var minRange = unit.getMinRange(actionTargetField);
        var fields = globals.getCircle(minRange, unit.getMaxRange(actionTargetField));
        if (unit.hasDirectWeapon() && minRange > 1)
        {
            var minFields = globals.getCircle(1, 1);
            var size = minFields.size();
            for (var i = 0; i < size; ++i)
            {
                fields.append(minFields.at(i))
            }
        }

        data.setColor("#FFFF0000");
        data.setZLabelColor("#ff4500");
        data.setZLabelText(qsTr("Damage"))
        data.setShowZData(true);
        var aiType = map.getCurrentPlayer().getBaseGameInput().getAiType();
        var detailedForecast = settings.getShowDetailedBattleForcast();
        var result = Qt.rect(-1, -1, -1, -1);
        var owner = unit.getOwner();
        // check all fields we can attack
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var field = fields.at(i)
            var x = field.x + actionTargetField.x;
            var y = field.y + actionTargetField.y;
            // generally attacks on shrouded fields are forbidden
            if (map.onMap(x, y) && owner.getFieldVisibleType(x, y) !== GameEnums.VisionType_Shrouded)
            {
                if ((aiType === GameEnums.AiTypes_Human || aiType === GameEnums.AiTypes_MovePlanner) && detailedForecast)
                {
                    var defUnit = map.getTerrain(x, y).getUnit();
                    if (defUnit !== null)
                    {
                        var resultNoLuckDmg = ACTION_FIRE.calcBattleDamage4(map, action, unit, 0,
                                                                            actionTargetField.x, actionTargetField.y, null, x, y, 0,
                                                                            GameEnums.LuckDamageMode_Off, GameEnums.LuckDamageMode_Off);
                        if (resultNoLuckDmg.x >= 0.0)
                        {
                            var resultMaxDmg = ACTION_FIRE.calcBattleDamage4(map, action, unit, 0,
                                                                             actionTargetField.x, actionTargetField.y, null, x, y, 0,
                                                                             GameEnums.LuckDamageMode_Max, GameEnums.LuckDamageMode_Min);
                            var resultMinDmg = ACTION_FIRE.calcBattleDamage4(map, action, unit, 0,
                                                                             actionTargetField.x, actionTargetField.y, null, x, y, 0,
                                                                             GameEnums.LuckDamageMode_Min, GameEnums.LuckDamageMode_Max);

                            var names       = [qsTr("Dmg"),             qsTr("Min"),        qsTr("Max")];
                            var ownData     = [resultNoLuckDmg.x,       resultMinDmg.x,     resultMaxDmg.x];
                            var enemyData   = [resultNoLuckDmg.width,   resultMaxDmg.width, resultMinDmg.width];
                            data.addPoint(Qt.point(x, y));
                            data.addComplexZInformation(names, ownData, enemyData, defUnit.getOwner().getColor());
                        }
                    }
                    else
                    {
                        result = ACTION_FIRE.calcBattleDamage(map, action, x, y, GameEnums.LuckDamageMode_Off);
                        if (result.x >= 0.0)
                        {
                            data.addPoint(Qt.point(x, y));
                            data.addZInformation(result.x);
                        }
                    }
                }
                else
                {
                    if ((aiType === GameEnums.AiTypes_Human ||
                         aiType === GameEnums.AiTypes_MovePlanner))
                    {
                        result = ACTION_FIRE.calcBattleDamage4(map, action, unit, 0,
                                                                   actionTargetField.x, actionTargetField.y, null, x, y, 0,
                                                                   GameEnums.LuckDamageMode_Off, GameEnums.LuckDamageMode_Off);
                    }
                    else
                    {
                        result = ACTION_FIRE.calcBattleDamage4(map, action, unit, 0,
                                                               actionTargetField.x, actionTargetField.y, null, x, y, 0,
                                                               GameEnums.LuckDamageMode_Off, GameEnums.LuckDamageMode_Off,
                                                               false, true);
                    }
                    if (result.x >= 0.0)
                    {
                        data.addPoint(Qt.point(x, y));
                        data.addZInformation(result.x);
                    }
                }
            }
        }
    };
    this.isFinalStep = function(action, map)
    {
        if (action.getInputStep() === 1)
        {
            // get the target unit
            action.startReading();
            var targetX = action.readDataInt32();
            var targetY = action.readDataInt32();
            var result = ACTION_FIRE.calcBattleDamage(map, action, targetX, targetY, GameEnums.LuckDamageMode_On);
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

    this.postAnimationAction = null;
    this.postAnimationUnit = null;
    this.postAnimationAttackerDamage = -1;
    this.postAnimationAttackerWeapon = -1;
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.postAnimationDefenderDamage = -1;
    this.postAnimationDefenderWeapon = -1;

    this.perform = function(action, map)
    {
        action.startReading();
        // read action data
        ACTION_FIRE.postAnimationAction = action;
        ACTION_FIRE.postAnimationTargetX = action.readDataInt32();
        ACTION_FIRE.postAnimationTargetY = action.readDataInt32();
        ACTION_FIRE.postAnimationAttackerDamage = action.readDataInt32();
        ACTION_FIRE.postAnimationAttackerWeapon = action.readDataInt32();
        ACTION_FIRE.postAnimationDefenderDamage = action.readDataInt32();
        ACTION_FIRE.postAnimationDefenderWeapon = action.readDataInt32();        
        // we need to move the unit to the target position
        ACTION_FIRE.postAnimationUnit = action.getTargetUnit();
        ACTION_FIRE.applyPostDamageReduction(map);
        var animation = Global[ACTION_FIRE.postAnimationUnit.getUnitID()].doWalkingAnimation(action, map);
        var currentPlayer = map.getCurrentPlayer();
        var currentViewPlayer = map.getCurrentViewPlayer();
        if (currentViewPlayer.getFieldVisible(ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY) &&
            currentPlayer.getBaseGameInput().getAiType() !== GameEnums.AiTypes_Human)
        {
            var animation2 = GameAnimationFactory.createAnimation(map, ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY, 70);
            animation2.addSprite("cursor+attack", -map.getImageSize() / 3, -map.getImageSize() / 3, 0, 2, 0, 2);
            animation2.setEndOfAnimationCall("ACTION_FIRE", "performPostAnimation");
            animation.queueAnimation(animation2);
        }
        else
        {
            animation.setEndOfAnimationCall("ACTION_FIRE", "performPostAnimation");
        }

        // move unit to target position
        ACTION_FIRE.postAnimationUnit.moveUnitAction(action);
        // disable unit commandments for this turn
        ACTION_FIRE.postAnimationUnit.setHasMoved(true);

    };

    this.applyPostDamageReduction = function(map)
    {
        var defTerrain = map.getTerrain(ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY);
        var defBuilding = defTerrain.getBuilding();
        var defUnit = defTerrain.getUnit();
        if (defUnit !== null)
        {
            var damageReduction = ACTION_FIRE.doSupportDamageReduction(ACTION_FIRE.postAnimationAttackerDamage, ACTION_FIRE.postAnimationUnit, ACTION_FIRE.postAnimationUnit.getPosition(),
                                                                       defUnit, Qt.point(ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY));
            ACTION_FIRE.postAnimationAttackerDamage  -= damageReduction;
            if (ACTION_FIRE.postAnimationAttackerDamage < 0)
            {
                ACTION_FIRE.postAnimationAttackerDamage = 0;
            }
        }
    };

    this.performPostAnimation = function(postAnimation, map)
    {
        ACTION_FIRE.battle(map, ACTION_FIRE.postAnimationUnit, ACTION_FIRE.postAnimationAttackerDamage, ACTION_FIRE.postAnimationAttackerWeapon,
                           ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY,
                           ACTION_FIRE.postAnimationDefenderDamage, ACTION_FIRE.postAnimationDefenderWeapon,
                           false);
        // reset stuff
        ACTION_FIRE.postAnimationUnit = null;
        ACTION_FIRE.postAnimationAttackerDamage = -1;
        ACTION_FIRE.postAnimationAttackerWeapon = -1;
        ACTION_FIRE.postAnimationTargetX = -1;
        ACTION_FIRE.postAnimationTargetY = -1;
        ACTION_FIRE.postAnimationDefenderDamage = -1;
        ACTION_FIRE.postAnimationDefenderWeapon = -1;
    };
    this.battle = function(map, attacker, attackerDamage, attackerWeapon,
                           defenderX, defenderY, defenderDamage, defenderWeapon,
                           dontKillUnits, simulation = false)
    {
        var defTerrain = map.getTerrain(defenderX, defenderY);
        var defBuilding = defTerrain.getBuilding();
        var defUnit = defTerrain.getUnit();
        if (defUnit !== null)
        {
            var defOwner = defUnit.getOwner();
            var defUnitX = defUnit.getX();
            var defUnitY = defUnit.getY();
            var defStartHp = defUnit.getHp();
            var atkStartHp = attacker.getHp();            
            var costs = defUnit.getCosts();
            var damage = attackerDamage / 10.0;
            if (damage < 0.0)
            {
                damage = 0.0;
            }
            var counterdamage = defenderDamage / 10.0;
            var power = 0;

            // we're attacking do recording
            if (!simulation)
            {
                map.getGameRecorder().attacked(attacker.getOwner().getPlayerID(), attackerDamage);
            }
            // gain power based on damage
            if (damage > defUnit.getHp())
            {
                power = costs * defUnit.getHp() / 10;
                damage = defUnit.getHp();
                defUnit.getOwner().gainPowerstar(power, Qt.point(defUnit.getX(), defUnit.getY()), damage, true, false);
                attacker.getOwner().gainPowerstar(power, Qt.point(attacker.getX(), attacker.getY()), damage, false, false);
            }
            else
            {
                power = costs * damage / 10;
                defUnit.getOwner().gainPowerstar(power, Qt.point(defUnit.getX(), defUnit.getY()), damage, true, false);
                attacker.getOwner().gainPowerstar(power, Qt.point(attacker.getX(), attacker.getY()), damage, false, false);
            }

            // deal damage
            defUnit.setHp(defUnit.getHp() - attackerDamage / 10.0);
            // reduce attacker ammo
            if (!simulation)
            {
                if (attackerWeapon === 0)
                {
                    attacker.reduceAmmo1(1);
                }
                else
                {
                    attacker.reduceAmmo2(1);
                }
            }
            // set counter damage
            if (counterdamage > 0)
            {
                costs = attacker.getCosts();
                // gain power based
                if (counterdamage > attacker.getHp())
                {
                    power = costs * attacker.getHp() / 10;
                    counterdamage = attacker.getHp();
                    attacker.getOwner().gainPowerstar(power, Qt.point(attacker.getX(), attacker.getY()), counterdamage, true, true);
                    defOwner.gainPowerstar(power, Qt.point(defUnitX, defUnitY), counterdamage, false, true);
                }
                else
                {
                    power = costs * counterdamage / 10;
                    attacker.getOwner().gainPowerstar(power, Qt.point(attacker.getX(), attacker.getY()), counterdamage, true, true);
                    defOwner.gainPowerstar(power, Qt.point(defUnitX, defUnitY), counterdamage, false, true);
                }
                // deal damage
                attacker.setHp(attacker.getHp() - defenderDamage / 10.0);
                // reduce ammo
                if (!simulation)
                {
                    if (defenderWeapon === 0)
                    {
                        defUnit.reduceAmmo1(1);
                    }
                    else
                    {
                        defUnit.reduceAmmo2(1);
                    }
                }
            }

            // post battle action of the attacking unit
            attacker.getOwner().postBattleActions(attacker, damage, defUnit, false, attackerWeapon, ACTION_FIRE.postAnimationAction);
            defOwner.postBattleActions(attacker, damage, defUnit, true, attackerWeapon, ACTION_FIRE.postAnimationAction);
            attacker.postBattleActions(damage, defUnit, false, attackerWeapon, ACTION_FIRE.postAnimationAction);
            defUnit.postBattleActions(damage, attacker, true, attackerWeapon, ACTION_FIRE.postAnimationAction);

            // post battle action of the defending unit
            attacker.getOwner().postBattleActions(defUnit, counterdamage, attacker, false, defenderWeapon, ACTION_FIRE.postAnimationAction);
            defOwner.postBattleActions(defUnit, counterdamage, attacker, true, defenderWeapon, ACTION_FIRE.postAnimationAction);
            attacker.postBattleActions(counterdamage, defUnit, true, defenderWeapon, ACTION_FIRE.postAnimationAction);
            defUnit.postBattleActions(counterdamage, attacker, false, defenderWeapon, ACTION_FIRE.postAnimationAction);

            // only kill units if we should else we stop here
            if (dontKillUnits === false && !simulation)
            {
                var atkPos = attacker.getPosition();
                var defPos = defUnit.getPosition();
                var defFirststrike = defUnit.getFirstStrike(defPos, attacker, true, atkPos);
                var atkFirststrike = attacker.getFirstStrike(atkPos, defUnit, false, defPos);
                var unitBattleAnimation = null;
                if (defFirststrike && !atkFirststrike && counterdamage >= 0)
                {
                    unitBattleAnimation = GameAnimationFactory.createBattleAnimation(map, defUnit.getTerrain(), defUnit, defStartHp, defUnit.getHp(), defenderWeapon,
                                                                                     attacker.getTerrain(), attacker, atkStartHp, attacker.getHp(), attackerWeapon, attackerDamage);
                }
                else
                {
                    unitBattleAnimation = GameAnimationFactory.createBattleAnimation(map, attacker.getTerrain(), attacker, atkStartHp, attacker.getHp(), attackerWeapon,
                                                                                     defUnit.getTerrain(), defUnit, defStartHp, defUnit.getHp(), defenderWeapon, defenderDamage);
                }

                ACTION_FIRE.postUnitAnimationAttacker = attacker;
                ACTION_FIRE.postUnitAnimationDefender = defUnit;
                unitBattleAnimation.setEndOfAnimationCall("ACTION_FIRE", "performPostUnitAnimation");
            }
        }
        else
        {
            // we attacked a building or terrain ;)
            var buildBattleAnimation = GameAnimationFactory.createBattleAnimation(map, attacker.getTerrain(), attacker, -1, -1, attackerWeapon,
                                                                                  defTerrain, null, -1, -1, defenderWeapon, -1);
            buildBattleAnimation.setEndOfAnimationCall("ACTION_FIRE", "performPostBuildingAnimation");
            ACTION_FIRE.postBuildingAnimationTerrain = defTerrain;
            if ((defBuilding !== null) && (defBuilding.getHp() > 0))
            {
                defBuilding.setHp(defBuilding.getHp() - attackerDamage);
                if (attackerWeapon === 0)
                {
                    attacker.reduceAmmo1(1);
                }
                else
                {
                    attacker.reduceAmmo2(1);
                }
            }
            // check for damage against terrain
            else if (defTerrain.getHp() > 0)
            {
                defTerrain.setHp(defTerrain.getHp() - attackerDamage);
                if (attackerWeapon === 0)
                {
                    attacker.reduceAmmo1(1);
                }
                else
                {
                    attacker.reduceAmmo2(1);
                }
            }
        }
    };
    this.postUnitAnimationAttacker = null;
    this.postUnitAnimationDefender = null;
    this.performPostUnitAnimation = function(postAnimation, map)
    {
        var player = map.getCurrentPlayer();
        var attacker = ACTION_FIRE.postUnitAnimationAttacker;
        var defUnit = ACTION_FIRE.postUnitAnimationDefender;
        if (attacker !== null && defUnit !== null)
        {
            // level up and defender destruction
            if (attacker.getHp() <= 0)
            {
                // achievements
                if (defUnit.getOwner().getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
                {
                    ACHIEVEMENT_KILL_UNIT.unitKilled(attacker.getUnitID());
                }
                // we destroyed a unit
                map.getGameRecorder().destroyedUnit(defUnit.getOwner().getPlayerID(), attacker.getUnitID(), attacker.getOwner().getPlayerID());
                attacker.killUnit();
                UNITRANKINGSYSTEM.increaseRang(defUnit);
            }
            // level up and attacker destruction
            if (defUnit.getHp() <= 0)
            {
                // achievements
                if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
                {
                    ACHIEVEMENT_KILL_UNIT.unitKilled(defUnit.getUnitID());
                }
                // we destroyed a unit nice
                map.getGameRecorder().destroyedUnit(attacker.getOwner().getPlayerID(), defUnit.getUnitID(), defUnit.getOwner().getPlayerID());
                defUnit.killUnit();
                UNITRANKINGSYSTEM.increaseRang(attacker);
            }
            ACTION_FIRE.postUnitAnimationAttacker = null;
            ACTION_FIRE.postUnitAnimationDefender = null;
            ACTION_FIRE.postAnimationAction = null;
        }
    }

    this.postBuildingAnimationTerrain = null;
    this.performPostBuildingAnimation = function(postAnimation, map)
    {
        var defBuilding = ACTION_FIRE.postBuildingAnimationTerrain.getBuilding();
        var player = map.getCurrentPlayer();
        if (defBuilding !== null)
        {
            if ((defBuilding.getHp() <= 0))
            {
                // achievements
                if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
                {
                    ACHIEVEMENT_KILL_NEUTRAL_BUILDING.killed(defBuilding.getBuildingID());
                }
                Global[defBuilding.getBuildingID()].onDestroyed(defBuilding, map);
            }
        }
        else if (ACTION_FIRE.postBuildingAnimationTerrain !== null &&
                 ACTION_FIRE.postBuildingAnimationTerrain.getHp() <= 0)
        {
            // achievements
            if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
            {
                ACHIEVEMENT_KILL_NEUTRAL_BUILDING.killed(ACTION_FIRE.postBuildingAnimationTerrain.getID());
            }
            Global[ACTION_FIRE.postBuildingAnimationTerrain.getID()].onDestroyed(ACTION_FIRE.postBuildingAnimationTerrain, map);
        }
        ACTION_FIRE.postBuildingAnimationTerrain = null;
        ACTION_FIRE.postAnimationAction = null;
    };
    this.getName = function()
    {
        return qsTr("Fire");
    };
    this.getDescription = function()
    {
        return qsTr("Orders a unit to attack another one. If the attack is a direct attack the enemy unit will counter with an attack of its own.");
    };

    this.createOverworldBattleAnimation = function(pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                             pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage, map)
    {
        var pRet = null;
        // attacking unit
        var atkDamage = globals.roundUp(defStartHp) - globals.roundUp(defEndHp);
        var defDamage = globals.roundUp(atkStartHp) - globals.roundUp(atkEndHp);
        var pAtk = GameAnimationFactory.createAnimation(map, pDefTerrain.getX(), pDefTerrain.getY(), 70);
        var imageSize = map.getImageSize();
        pAtk.addSprite("blackhole_shot", -imageSize * 0.5, -imageSize * 0.5, 0, 2.0);
        pAtk.setSound("talongunhit.wav", 1);
        var pDmgTextAtk = GameAnimationFactory.createAnimation(map, pDefTerrain.getX(), pDefTerrain.getY());
        pDmgTextAtk.addText(atkDamage + " Hp", -8, 0, 2.0, "#FF0000");
        pDmgTextAtk.addTweenPosition(Qt.point(pDefTerrain.getX() * imageSize, (pDefTerrain.getY() - 2) * imageSize), 1000);
        pDmgTextAtk.addTweenWait(1500);
        pAtk.queueAnimation(pDmgTextAtk);
        if (defenderDamage >= 0)
        {
            // counter damage
            pRet = GameAnimationFactory.createAnimation(map, pAtkTerrain.getX(), pAtkTerrain.getY(), 70);
            pRet.addSprite("blackhole_shot", -imageSize * 0.5, -imageSize * 0.5, 0, 2.0);
            pRet.setSound("talongunhit.wav", 1);
            pDmgTextAtk.queueAnimation(pRet);
            var pDmgTextDef = GameAnimationFactory.createAnimation(map, pAtkTerrain.getX(), pAtkTerrain.getY());
            pDmgTextDef.addText(defDamage + " Hp", -8, 0, 2.0, "#FF0000");
            pDmgTextDef.addTweenPosition(Qt.point(pAtkTerrain.getX() * imageSize, (pAtkTerrain.getY() - 2) * imageSize), 1000);
            pDmgTextDef.addTweenWait(1500);
            pRet.queueAnimation(pDmgTextDef);
        }
        pRet = pAtk;
        return pRet;
    }

    // you may implement the following function in a mod to further tweak when a unit can attack with a certain weapon.
    // extendedCanAttackCheck(unit, weaponIndex, weaponType, unitX, unitY, targetX, targetY, rangeCheck)
}


Constructor.prototype = ACTION;
var ACTION_FIRE = new Constructor();
