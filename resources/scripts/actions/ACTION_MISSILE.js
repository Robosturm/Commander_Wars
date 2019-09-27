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
        var missileBuildings = ACTION_MISSILE.getMissileBuildings();
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
            (action.getMovementTarget() === null))
        {
            var building = action.getMovementBuilding();
            if ((building !== null) &&
                (missileBuildings.indexOf(building.getBuildingID()) >= 0))
            {
                return true;
            }
        }
        return false;
    };

    this.getMissileBuildings = function()
    {
        return ["SILO_ROCKET"];
    };

    this.getActionText = function()
    {
        return qsTr("Missile");
    };
    this.getIcon = function()
    {
        return "missile_icon";
    };
    this.getStepCursor = function(action, cursorData)
    {
        cursorData.setCursor("cursor+missile");
        cursorData.setXOffset(- map.getImageSize() * 2);
        cursorData.setYOffset(- map.getImageSize() * 2);
        cursorData.setScale(1.5);
    };
    this.getStepData = function(action, data)
    {
        data.setAllFields(true);
        data.setShowZData(false);
    };
    this.getStepInputType = function(action)
    {
        return "FIELD";
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
    this.postAnimationUnit = null;
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_MISSILE", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);

        action.startReading();
        // read action data
        ACTION_MISSILE.postAnimationTargetX = action.readDataInt32();
        ACTION_MISSILE.postAnimationTargetY = action.readDataInt32();
        ACTION_MISSILE.postAnimationUnit = unit;
    };
    this.performPostAnimation = function(postAnimation)
    {
        var radius = 2;
        var damage = 3;
        var fields = globals.getCircle(0, radius);
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + ACTION_MISSILE.postAnimationTargetX;
            var y = fields.at(i).y + ACTION_MISSILE.postAnimationTargetY;
            // check with which weapon we can attack and if we could deal damage with this weapon
            if (map.onMap(x, y))
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null)
                {
                    var hp = unit.getHpRounded();
                    if (hp <= damage)
                    {
                        // set hp to very very low
                        unit.setHp(0.001);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
            }
        }

        var animation = GameAnimationFactory.createAnimation(ACTION_MISSILE.postAnimationTargetX - radius, ACTION_MISSILE.postAnimationTargetY - radius - 1);
        animation.addSprite("explosion+silo", -map.getImageSize() / 2, 0, 0, 1.5, 0);
        audio.playSound("missle_explosion.wav");
        // replace silo with rocket with
        ACTION_MISSILE.postAnimationUnit.getTerrain().loadBuilding("SILO");

        ACTION_MISSILE.postAnimationTargetX = -1;
        ACTION_MISSILE.postAnimationTargetY = -1;
        ACTION_MISSILE.postAnimationUnit = null;
    }
}

Constructor.prototype = ACTION;
var ACTION_MISSILE = new Constructor();
