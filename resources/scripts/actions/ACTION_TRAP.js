var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return false;
    };
    // this action can't be performed by the player it's performed when moving over a stealthed unit by the game itself
    // it replaces the actual action that should have been performed
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action, map);
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
        animation.setEndOfAnimationCall("ACTION_TRAP", "performPostAnimation");
        action.startReading();
        ACTION_TRAP.postAnimationTrapSignX = action.readDataInt32();
        ACTION_TRAP.postAnimationTrapSignY = action.readDataInt32();
        ACTION_TRAP.postAnimationUnit = unit;
    };
    this.postAnimationTrapSignX = -1;
    this.postAnimationTrapSignY = -1;
    this.postAnimationUnit = null;
    this.performPostAnimation = function(postAnimation, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, ACTION_TRAP.postAnimationTrapSignX, ACTION_TRAP.postAnimationTrapSignY);
        var width = animation.addText(qsTr("TRAP!"), map.getImageSize() / 2 + 25, -2, 1);
        animation.addBox("info", map.getImageSize() / 2, 0, width + 36, map.getImageSize(), 400);
        animation.addSprite("trap", map.getImageSize() / 2 + 4, 4, 400, 2);
        audio.playSound("trap.wav");
        ACTION_TRAP.seaMineExplosion(animation,
                                     ACTION_TRAP.postAnimationUnit,
                                     ACTION_TRAP.postAnimationTrapSignX,
                                     ACTION_TRAP.postAnimationTrapSignY,
                                     map);
        ACTION_TRAP.postAnimationTrapSignX = -1;
        ACTION_TRAP.postAnimationTrapSignY = -1;
        ACTION_TRAP.postAnimationUnit = null;
    };

    this.seaMineExplosion = function(queueAnimation, targetUnit, trapX, trapY, map)
    {

        // explode mine?
        var unit = map.getTerrain(trapX, trapY).getUnit();
        if (unit !== null)
        {
            var unitId = unit.getUnitID();
            if (ACTION_DISABLE_MINE.mines.includes(unitId))
            {
                var owner = unit.getOwner();
                var explode = false;
                if (owner.isEnemyUnit(targetUnit) &&
                    targetUnit.getUnitType() !== GameEnums.UnitType_Air)
                {
                    explode = true;
                }
                if (explode)
                {
                    if (!Global[targetUnit.getUnitID()].actionList.includes("ACTION_DISABLE_MINE"))
                    {
                        var xPos = targetUnit.getX();
                        var yPos = targetUnit.getY();
                        var animation = GameAnimationFactory.createAnimation(map, xPos, yPos);
                        animation.writeDataInt32(xPos);
                        animation.writeDataInt32(yPos);
                        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
                        animation.setEndOfAnimationCall("ACTION_TRAP", "postAnimationMineDamge")
                        if (queueAnimation !== null)
                        {
                            queueAnimation.queueAnimation(animation);
                        }
                    }
                    // we destroyed a unit
                    animation = GameAnimationFactory.createAnimation(map, trapX, trapY);
                    animation.writeDataInt32(trapX);
                    animation.writeDataInt32(trapY);
                    animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
                    animation.setSound("explosion+water.wav");
                    animation.setEndOfAnimationCall("ACTION_TRAP", "postAnimationSelfKill")
                    if (queueAnimation !== null)
                    {
                        queueAnimation.queueAnimation(animation);
                    }
                }
            }
        }
    };

    this.postAnimationSelfKill = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var xPos = postAnimation.readDataInt32();
        var yPos = postAnimation.readDataInt32();
        var terrain = map.getTerrain(xPos, yPos);
        var unit = terrain.getUnit();
        if (unit !== null)
        {
            var owner = unit.getOwner();
            map.getGameRecorder().destroyedUnit(owner.getPlayerID(), unit.getUnitID(), unit.getOwner().getPlayerID());
            unit.removeUnit();
        }
    }

    this.postAnimationMineDamge = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var xPos = postAnimation.readDataInt32();
        var yPos = postAnimation.readDataInt32();
        var terrain = map.getTerrain(xPos, yPos);
        var targetUnit = terrain.getUnit();
        if (targetUnit !== null &&
            targetUnit.getUnitType() !== GameEnums.UnitType_Air)
        {
            var specialDestruction = map.getGameRules().getSpecialDestruction();
            var newHp = targetUnit.getHpRounded() - 5;
            if (!specialDestruction && newHp <= 0.1)
            {
                newHp = 0.1;
            }
            targetUnit.setHp(newHp);
            if (targetUnit.getHp() <= 0)
            {
                // we destroyed a unit
                map.getGameRecorder().destroyedUnit(targetUnit.getOwner().getPlayerID(), targetUnit.getUnitID(), targetUnit.getOwner().getPlayerID());
                targetUnit.killUnit();
            }
        }
    }

    this.getIcon = function(map)
    {
        return "help";
    };
    this.getName = function(map)
    {
        return qsTr("Trap");
    };
    this.getDescription = function()
    {
        return qsTr("This action can't be disabled and traps a unit if you try to move over a hidden unit.");
    };
    this.isTrap = function(action, moveUnit, targetFieldUnit, targetX, targetY, previousX, previousY, moveCost, map)
    {
        // used to determine if a trap is in the move path.
        // the engine takes care of checking the path in the correct order and cutting the path.
        if ((targetFieldUnit !== null &&
             targetFieldUnit.isStealthed(moveUnit.getOwner()) &&
             !moveUnit.getIgnoreUnitCollision()) ||
             moveCost < 0)
        {
            return true;
        }
        return false;
    };

    this.isStillATrap = function(action, moveUnit, targetFieldUnit, targetX, targetY, previousX, previousY, moveCost, map)
    {
        // this function gets called to find a field at which the unit can actually stop it's movement
        // E.g. don't get trapped on a teleport tile or getting trapped on an allied unit etc.
        if (targetFieldUnit !== null || moveCost <= 0)
        {
            return true;
        }
        return false;
    };

}

Constructor.prototype = ACTION;
var ACTION_TRAP = new Constructor();
