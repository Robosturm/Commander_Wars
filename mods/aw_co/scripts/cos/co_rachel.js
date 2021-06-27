CO_RACHEL.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(6);
};
CO_RACHEL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};
CO_RACHEL.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_RACHEL.getBonusLuck = function(co, unit, posX, posY)
{
    return 0;
};

CO_RACHEL.getRepairBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        return 1;
    }
    return 0;
};
