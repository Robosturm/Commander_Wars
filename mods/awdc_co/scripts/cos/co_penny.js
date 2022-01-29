CO_PENNY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_PENNY.activateSuperpower = function(co, powerMode, map)
{
    CO_PENNY.activatePower(co, powerMode);
};
CO_PENNY.getSuperPowerDescription = function()
{
    return CO_PENNY.getPowerDescription();
};
CO_PENNY.getSuperPowerName = function()
{
    return CO_PENNY.getPowerName();
};
CO_PENNY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
{
    if (map !== null)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                    co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return 10;
            }
            break;
        }
    }
    else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
    {
        return 10;
    }
    return 0;
};
CO_PENNY.getFirerangeModifier = function(co, unit, posX, posY, map)
{
    return 0;
};

CO_PENNY.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    return 0;
};

CO_PENNY.getVisionrangeModifier = function(co, unit, posX, posY, map)
{
    return 0;
};

CO_PENNY.getPerfectVision = function(co, map)
{
    return false;
};
