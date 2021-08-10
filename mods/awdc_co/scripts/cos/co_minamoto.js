CO_MINAMOTO.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MINAMOTO.activateSuperpower = function(co, powerMode)
{
	CO_MINAMOTO.activatePower(co, powerMode);
};
CO_MINAMOTO.getSuperPowerDescription = function()
{
    return CO_MINAMOTO.getPowerDescription();
};
CO_MINAMOTO.getSuperPowerName = function()
{
    return CO_MINAMOTO.getPowerName();
};
CO_MINAMOTO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action)
{
    var nearMountains = false;
    if (typeof map !== 'undefined')
    {
        var fields = globals.getCircle(0, 2);
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + atkPosX;
            var y = fields.at(i).y + atkPosY;
            if (map.onMap(x, y))
            {
                if (map.getTerrain(x, y).getID() === "MOUNTAIN")
                {
                    nearMountains = true;
                    break;
                }
            }
        }
        fields.remove();
    }
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        if (nearMountains === true)
        {
            return 70;
        }
        else
        {
            return 10;
        }
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            if (nearMountains === true)
            {
                return 70;
            }
            return 10;
        }
        break;
    }
    return 0;
};
CO_MINAMOTO.getMovementpointModifier = function(co, unit, posX, posY)
{
    return 0;
};
CO_MINAMOTO.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (gotAttacked === false && attacker.getOwner() === co.getOwner())
    {
        // here begins the fun :D
        var blowRange = 0;
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (atkDamage >= 4.5)
            {
                blowRange = 2;
            }
            break;
        default:
            break;
        }
        var distX = defender.getX() - attacker.getX();
        var distY = defender.getY() - attacker.getY();
        var distance = Math.abs(distX) + Math.abs(distY);
        if (defender.getHp() > 0 && blowRange > 0 && distance === 1)
        {
            var newPosition = Qt.point(defender.getX(), defender.getY());
            // find blow away position
            for (var i = 1; i <= blowRange; i++)
            {
                var testPosition = Qt.point(defender.getX() + distX * i, defender.getY() + distY * i);
                if (map.onMap(testPosition.x, testPosition.y))
                {
                    var terrain = map.getTerrain(testPosition.x, testPosition.y);
                    if (terrain.getUnit() === null &&
                            defender.canMoveOver(testPosition.x, testPosition.y) === true)
                    {
                        newPosition = testPosition;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            // blow unit away
            defender.moveUnitToField(newPosition.x, newPosition.y);
        }
    }
};
