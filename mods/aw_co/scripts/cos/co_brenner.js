CO_BRENNER.powerBonus = 20;
CO_BRENNER.superPowerBonus = 20;
CO_BRENNER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_BRENNER.activateSuperpower = function(co, powerMode, map)
{
    CO_BRENNER.activatePower(co, powerMode, map);
};
CO_BRENNER.getSuperPowerDescription = function()
{
    return CO_BRENNER.getPowerDescription();
};
CO_BRENNER.getSuperPowerName = function()
{
    return CO_BRENNER.getPowerName();
};
CO_BRENNER.coZoneBonus = 0;
CO_BRENNER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return CO_BRENNER.powerBonus;
        default:
            return 0;
        }
    }
    return 0;
};

CO_BRENNER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};
