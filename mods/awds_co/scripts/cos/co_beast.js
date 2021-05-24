CO_BEAST.coZoneBonus = 30;
CO_BEAST.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (!isDefender)
            {
                return CO_BEAST.coZoneBonus;
            }
            break;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) && !isDefender)
            {
                return CO_BEAST.coZoneBonus;
            }
            break;
        }
    }
    return 0;
};
CO_BEAST.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_BEAST.getMovementpointModifier = function(co, unit, posX, posY)
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
CO_BEAST.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
        {
            attacker.setHp(attacker.getHp() - 1);
        }
    }
};
