CO_PETER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_PETER.activateSuperpower = function(co, powerMode, map)
{
    CO_PETER.activatePower(co, powerMode);
};
CO_PETER.getSuperPowerDescription = function()
{
    return CO_PETER.getPowerDescription();
};
CO_PETER.getSuperPowerName = function()
{
    return CO_PETER.getPowerName();
};
CO_PETER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (defender === null)
    {
        return 0;
    }
    var bonus = 0;
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        if (defender.getBaseMaxRange() === 1)
        {
            bonus += 50;
        }
        bonus += 10;
        break;
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            if (defender.getBaseMaxRange() === 1)
            {
                bonus = 50;
            }
            bonus += 10;
        }
        else
        {
            if (defender.getBaseMaxRange() === 1)
            {
                bonus = 7;
            }
        }
        break;
    }
    bonus -= defender.getTerrainDefense() * 5;
    return bonus;
};
