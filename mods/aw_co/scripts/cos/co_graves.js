CO_GRAVES.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_GRAVES.activateSuperpower = function(co, powerMode)
{
    CO_GRAVES.activatePower(co, powerMode);
};
CO_GRAVES.getSuperPowerDescription = function()
{
    return CO_GRAVES.getPowerDescription();
};
CO_GRAVES.getSuperPowerName = function()
{
    return CO_GRAVES.getPowerName();
};
CO_GRAVES.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            var stunLevel = 0;
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                stunLevel = 3;
                break;
            default:
                stunLevel = 2;
                break;
            }
            if (defender.getHpRounded() <= stunLevel)
            {
                defender.setHasMoved(true);
            }
        }
    }
};
CO_GRAVES.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_GRAVES.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
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
