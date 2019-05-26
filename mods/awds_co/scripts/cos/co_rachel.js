CO_RACHEL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    return 0;
};
CO_RACHEL.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            break;
        case GameEnums.PowerMode_Power:
            return 40;
        default:
            break;
        }
    }
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
