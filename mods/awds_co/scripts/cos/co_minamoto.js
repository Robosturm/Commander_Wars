CO_MINAMOTO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
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
        }
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (nearMountains === true)
            {
                return 50;
            }
            else
            {
                return 10;
            }
        case GameEnums.PowerMode_Power:
            if (nearMountains === true)
            {
                return 50;
            }
            else
            {
                return 10;
            }
        default:
            if (nearMountains === true)
            {
                return 20;
            }
            break;
        }
    }
    return 0;
};
CO_MINAMOTO.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
    }
    return 0;
};
CO_MINAMOTO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};
CO_MINAMOTO.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            // here begins the fun :D
            var blowRange = 0;
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (atkDamage >= 3.5)
                {
                    blowRange = 4;
                }
                break;
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
    }
};
