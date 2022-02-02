CO_MINA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MINA.activateSuperpower = function(co, powerMode, map)
{
	CO_MINA.activatePower(co, powerMode);
};
CO_MINA.getSuperPowerDescription = function()
{
    return CO_MINA.getPowerDescription();
};
CO_MINA.getSuperPowerName = function()
{
    return CO_MINA.getPowerName();
};
CO_MINA.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                              defender, defPosX, defPosY, isDefender, luckMode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (luckMode === GameEnums.LuckDamageMode_On)
            {
                return globals.randInt(0, 50);
            }
            else if (luckMode === GameEnums.LuckDamageMode_Average)
            {
                return 25;
            }
            else if (luckMode === GameEnums.LuckDamageMode_Min)
            {
                return 0;
            }
            else if (luckMode === GameEnums.LuckDamageMode_Max)
            {
                return 50;
            }
            return 0;
        default:
            break;
    }
    return 0;
};
