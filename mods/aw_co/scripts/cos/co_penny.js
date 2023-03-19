CO_PENNY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_PENNY.activateSuperpower = function(co, powerMode, map)
{
    CO_PENNY.activatePower(co, map);
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
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};
CO_PENNY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
CO_PENNY.getFirerangeModifier = function(co, unit, posX, posY, map)
{
    return 0;
};

CO_PENNY.getWeatherImmune = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        return true;
    }
    return false;
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
