var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        // are we allowed to attack from this field?
        if (((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y)) ||
                ((action.getMovementTarget() === null) && unit.canMoveAndFire(targetField)))
        {
            var fields = globals.getCircle(unit.getMinRange(actionTargetField), unit.getMaxRange(actionTargetField));
            // check all fields we can attack
            for (var i = 0; i < fields.size(); i++)
            {
                var x = fields.at(i).x + actionTargetField.x;
                var y = fields.at(i).y + actionTargetField.y;
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
                                return true
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
                                    return true;
                                }
                            }
                            if (unit.hasAmmo2() && unit.getWeapon2ID() !== "" &&
                                unit.canAttackWithWeapon(1, actionTargetField.x, actionTargetField.y, x, y))
                            {
                                if (Global[unit.getWeapon2ID()].getEnviromentDamage(defTerrain.getID()) > 0)
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
        var damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, 100, 0);
        return damage;
    };
    this.calcAttackerDamage = function(attacker, attackerWeapon, takenDamage, attackerPosition, defender, luckMode)
    {
        return ACTION_FIRE.calcDamage(attacker, attackerWeapon, attackerPosition, globals.roundUp(attacker.getHp() - takenDamage / 10.0),
                                      defender, defender.getPosition(), false,
                                      luckMode)
    };
    this.calcDefenderDamage = function(attacker, attackerPosition, defender, defenderWeapon, takenDamage, luckMode)
    {
        var damage = -1;
        // only direct units can deal counter damage
        if (Math.abs(attackerPosition.x - defender.getX()) + Math.abs(attackerPosition.y - defender.getY()) === 1)
        {
            if (defender.getMinRange(Qt.point(defender.getX(), defender.getY())) === 1 && defenderWeapon !== "")
            {
                var health = defender.getHp() - takenDamage / 10.0;
                if (defender.getFirstStrike(defender.getPosition(), attacker))
                {
                    health = defender.getHp();
                }
                health = globals.roundUp(health);
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
        if (attackerBaseHp <= 0)
        {
            return -1;
        }
        var baseDamage = Global[attackerWeapon].getBaseDamage(defender);
        var damage = baseDamage;
        if (baseDamage > 0.0)
        {
            var hp = attacker.getHp();
            attacker.setHp(attackerBaseHp);
            var offensive = 100 + attacker.getBonusOffensive(attackerPosition, defender, defender.getPosition(), isDefender);
            var defensive = 100 + defender.getBonusDefensive(defenderPosition, attacker, attackerPosition, isDefender);
            attacker.setHp(hp);
            var attackerHp = attackerBaseHp + attacker.getAttackHpBonus(attackerPosition);
            var luckDamage = 0;
            if (luckMode !== GameEnums.LuckDamageMode_Off)
            {
                var luck = 10 + attacker.getBonusLuck(attackerPosition);
                var misfortune = attacker.getBonusMisfortune(attackerPosition);
                if (luck > -misfortune)
                {
                    if (luckMode === GameEnums.LuckDamageMode_On)
                    {
                        var luckValue = globals.randInt(0, luck);
                        var misfortuneValue = globals.randInt(-misfortune, 0);
                        luckDamage += (luckValue + misfortuneValue);
                    }
                    else if (luckMode === GameEnums.LuckDamageMode_Average)
                    {
                        luckDamage += (-misfortune + luck) / 2;
                    }
                }
            }
            damage = Global[attackerWeapon].calculateDamage(attackerHp, baseDamage, offensive, defensive, luckDamage);
            damage += attacker.getTrueDamage(damage, attackerPosition, attackerBaseHp,
                                             defender, defenderPosition, isDefender);

            damage -= defender.getDamageReduction(damage, attacker, attackerPosition, attackerBaseHp,
                                                  defenderPosition, isDefender, luckMode);
            // avoid healing through negativ damage caused by misfortune or other stuff
            if (damage <= 0.0)
            {
                damage = 0.0;
            }
        }
        return damage;
    };


    this.calcBattleDamage = function(action, x, y, luckMode)
    {
        return ACTION_FIRE.calcBattleDamage2(action.getTargetUnit(),action.getActionTarget(),
                                             x, y, luckMode);
    };

    this.calcBattleDamage2 = function(attacker, atkPos, x, y, luckMode)
    {
        return ACTION_FIRE.calcBattleDamage3(attacker, 0, atkPos.x, atkPos.y, null, x, y, 0, luckMode);
    };

    this.calcBattleDamage3 = function(attacker, attackerTakenDamage, atkPosX, atkPosY, defender, x, y, defenderTakenDamage, luckMode, ignoreOutOfVisionRange = false)
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

            var dmg1 = -1;
            var dmg2 = -1;
            var baseDamage1 = -1;
            var baseDamage2 = -1;
            if (defUnit !== null)
            {
                if (unit.isAttackable(defUnit, ignoreOutOfVisionRange, Qt.point(atkPosX, atkPosY)))
                {
                    var weaponID = unit.getWeapon1ID();
                    if (unit.hasAmmo1() && weaponID !== "" &&
                        unit.canAttackWithWeapon(0, atkPosX, atkPosY,x, y))
                    {
                        baseDamage1 = Global[weaponID].getBaseDamage(defUnit);
                        dmg1 = ACTION_FIRE.calcAttackerDamage(unit, weaponID, attackerTakenDamage, actionTargetField ,defUnit, luckMode);
                    }
                    weaponID = unit.getWeapon2ID();
                    if (unit.hasAmmo2() && weaponID &&
                        unit.canAttackWithWeapon(1, atkPosX, atkPosY, x, y))
                    {
                        baseDamage2 = Global[weaponID].getBaseDamage(defUnit);
                        dmg2 = ACTION_FIRE.calcAttackerDamage(unit, weaponID, attackerTakenDamage, actionTargetField ,defUnit, luckMode);
                    }

                    if ((baseDamage1 >= 0.0) || (baseDamage2 >= 0.0))
                    {
                        if (baseDamage1 >= baseDamage2)
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
                    if (Math.abs(actionTargetField.x - x) + Math.abs(actionTargetField.y - y) === 1 &&
                            defUnit.isAttackable(unit, true, actionTargetField))
                    {
                        var defDamage = -1;
                        var defWeapon = -1;
                        baseDamage1 = -1;
                        baseDamage2 = -1;
                        weaponID = defUnit.getWeapon1ID();
                        if (defUnit.hasAmmo1() && weaponID !== "" &&
                            defUnit.canAttackWithWeapon(0, x, y, atkPosX, atkPosY))
                        {
                            baseDamage1 = Global[weaponID].getBaseDamage(unit);
                            defDamage = ACTION_FIRE.calcDefenderDamage(unit, actionTargetField, defUnit, defUnit.getWeapon1ID(), result.x + defenderTakenDamage, luckMode);
                            defWeapon = 0;
                        }
                        weaponID = defUnit.getWeapon2ID();
                        if (defUnit.hasAmmo2() && weaponID !== "" &&
                            defUnit.canAttackWithWeapon(1, x, y, atkPosX, atkPosY))
                        {
                            baseDamage2 = Global[weaponID].getBaseDamage(unit);
                            var defDamage2 = ACTION_FIRE.calcDefenderDamage(unit, actionTargetField, defUnit, defUnit.getWeapon2ID(), result.x + defenderTakenDamage, luckMode);
                            if (baseDamage2 > baseDamage1)
                            {
                                defDamage = defDamage2;
                                defWeapon = 1;
                            }
                        }
                        if (defDamage >= 0.0)
                        {
                            result.width = defDamage
                            result.height = defWeapon
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
                    if (unit.hasAmmo1() && unit.getWeapon1ID() !== "" &&
                        unit.canAttackWithWeapon(0, atkPosX, atkPosY, x, y))
                    {
                        dmg1 = ACTION_FIRE.calcEnviromentDamage(unit, unit.getWeapon1ID(), actionTargetField, Qt.point(x, y), defTerrain.getID());
                    }
                    if (unit.hasAmmo2() && unit.getWeapon2ID() !== "" &&
                        unit.canAttackWithWeapon(1, atkPosX, atkPosY, x, y))
                    {
                        dmg2 = ACTION_FIRE.calcEnviromentDamage(unit, unit.getWeapon2ID(), actionTargetField, Qt.point(x, y), defTerrain.getID());
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

    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var targetField = action.getTarget();
        var actionTargetField = action.getActionTarget();
        var fields = globals.getCircle(unit.getMinRange(actionTargetField), unit.getMaxRange(actionTargetField));
        data.setColor("#FFFF0000");
        data.setZLabelColor("#ff4500");
        data.setZLabelText(qsTr("Damage"))
        data.setShowZData(true);
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + actionTargetField.x;
            var y = fields.at(i).y + actionTargetField.y;
            // generally attacks on shrouded fields are forbidden
            if (unit.getOwner().getFieldVisibleType(x, y) !== GameEnums.VisionType_Shrouded)
            {
                var result = ACTION_FIRE.calcBattleDamage(action, x, y, GameEnums.LuckDamageMode_Off);
                if (result.x >= 0.0)
                {
                    data.addPoint(Qt.point(x, y));
                    data.addZInformation(result.x);
                }
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
        action.startReading();
        // read action data
        ACTION_FIRE.postAnimationTargetX = action.readDataInt32();
        ACTION_FIRE.postAnimationTargetY = action.readDataInt32();
        ACTION_FIRE.postAnimationAttackerDamage = action.readDataInt32();
        ACTION_FIRE.postAnimationAttackerWeapon = action.readDataInt32();
        ACTION_FIRE.postAnimationDefenderDamage = action.readDataInt32();
        ACTION_FIRE.postAnimationDefenderWeapon = action.readDataInt32();

        // we need to move the unit to the target position
        ACTION_FIRE.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_FIRE.postAnimationUnit.getUnitID()].doWalkingAnimation(action);

        var currentPlayer = map.getCurrentPlayer();
        var currentViewPlayer = map.getCurrentViewPlayer();
        if (currentViewPlayer.getFieldVisible(ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY) &&
                currentPlayer.getBaseGameInput().getAiType() !== GameEnums.AiTypes_Human)
        {
            var animation2 = GameAnimationFactory.createAnimation(ACTION_FIRE.postAnimationTargetX, ACTION_FIRE.postAnimationTargetY, 70);
            animation2.addSprite("cursor+attack", -map.getImageSize() / 3, -map.getImageSize() / 3, 0, 1.5, 0, 2);
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

    this.performPostAnimation = function(postAnimation)
    {
        ACTION_FIRE.battle(ACTION_FIRE.postAnimationUnit, ACTION_FIRE.postAnimationAttackerDamage, ACTION_FIRE.postAnimationAttackerWeapon,
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
    this.battle = function(attacker, attackerDamage, attackerWeapon,
                           defenderX, defenderY, defenderDamage, defenderWeapon,
                           dontKillUnits)
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
            map.getGameRecorder().attacked(attacker.getOwner().getPlayerID(), attackerDamage);

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
            if (attackerWeapon === 0)
            {
                attacker.reduceAmmo1(1);
            }
            else
            {
                attacker.reduceAmmo2(1);
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
                if (defenderWeapon === 0)
                {
                    defUnit.reduceAmmo1(1);
                }
                else
                {
                    defUnit.reduceAmmo2(1);
                }
            }

            // post battle action of the attacking unit
            attacker.getOwner().postBattleActions(attacker, damage, defUnit, false);
            defOwner.postBattleActions(attacker, damage, defUnit, true);
            attacker.postBattleActions(damage, defUnit, false);
            defUnit.postBattleActions(damage, attacker, true);

            // post battle action of the defending unit
            defOwner.postBattleActions(defUnit, counterdamage, attacker, true);
            attacker.getOwner().postBattleActions(defUnit, counterdamage, attacker, false);
            defUnit.postBattleActions(counterdamage, attacker, true);
            attacker.postBattleActions(counterdamage, defUnit, false);

            // only kill units if we should else we stop here
            if (dontKillUnits  === false)
            {
                var unitBattleAnimation = GameAnimationFactory.createBattleAnimation(attacker.getTerrain(), attacker, atkStartHp, attacker.getHp(), attackerWeapon,
                                                                                     defUnit.getTerrain(), defUnit, defStartHp, defUnit.getHp(), defenderWeapon, defenderDamage);
                ACTION_FIRE.postUnitAnimationAttacker = attacker;
                ACTION_FIRE.postUnitAnimationDefender = defUnit;
                unitBattleAnimation.setEndOfAnimationCall("ACTION_FIRE", "performPostUnitAnimation");
            }
        }
        else
        {
            // we attacked a building or terrain ;)
            var buildBattleAnimation = GameAnimationFactory.createBattleAnimation(attacker.getTerrain(), attacker, -1, -1, attackerWeapon,
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
    this.performPostUnitAnimation = function()
    {
        var player = map.getCurrentPlayer();
        var attacker = ACTION_FIRE.postUnitAnimationAttacker;
        var defUnit = ACTION_FIRE.postUnitAnimationDefender;
        // level up and defender destruction
        if (attacker.getHp() <= 0)
        {
            // achievements
            if (defUnit.getOwner().getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
            {
                ACHIEVEMENT_KILL_UNIT.unitKilled(attacker.getUnitID());
            }
            // we destroyed a unit
            map.getGameRecorder().destroyedUnit(defUnit.getOwner().getPlayerID(), attacker.getUnitID());
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
            map.getGameRecorder().destroyedUnit(attacker.getOwner().getPlayerID(), defUnit.getUnitID());
            defUnit.killUnit();
            UNITRANKINGSYSTEM.increaseRang(attacker);
        }
        ACTION_FIRE.postUnitAnimationAttacker = null;
        ACTION_FIRE.postUnitAnimationDefender = null;
    }

    this.postBuildingAnimationTerrain = null;
    this.performPostBuildingAnimation = function()
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
                Global[defBuilding.getBuildingID()].onDestroyed(defBuilding);
            }
        }
        else if (ACTION_FIRE.postBuildingAnimationTerrain.getHp() <= 0)
        {
            // achievements
            if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
            {
                ACHIEVEMENT_KILL_NEUTRAL_BUILDING.killed(ACTION_FIRE.postBuildingAnimationTerrain.getID());
            }
            Global[ACTION_FIRE.postBuildingAnimationTerrain.getID()].onDestroyed(ACTION_FIRE.postBuildingAnimationTerrain);
        }
        ACTION_FIRE.postBuildingAnimationTerrain = null;
    };
    this.getDescription = function()
    {
        return qsTr("Orders a unit to attack another one. If the attack is a direct attack the enemy unit will counter with an attack of its own.");
    };

    // you may implement the following function in a mod to further tweak when a unit can attack with a certain weapon.
    // extendedCanAttackCheck(unit, weaponIndex, weaponType, unitX, unitY, targetX, targetY)
}


Constructor.prototype = ACTION;
var ACTION_FIRE = new Constructor();
