CO_SONJA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SONJA.activateSuperpower = function(co, powerMode)
{
	CO_SONJA.activatePower(co, powerMode);
};
CO_SONJA.getSuperPowerDescription = function()
{
    return CO_SONJA.getPowerDescription();
};
CO_SONJA.getSuperPowerName = function()
{
    return CO_SONJA.getPowerName();
};
CO_SONJA.getEnemyTerrainDefenseModifier = function(co, unit, posX, posY)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return -2;
        default:
            return -1;
    }
};
CO_SONJA.getFirstStrike = function(co, unit, posX, posY, attacker)
{
    return false;
};
