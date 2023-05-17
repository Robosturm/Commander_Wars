CO_YUKIO.mintrueDamage = 10;
CO_YUKIO.trueDamageBonus = 15;
CO_YUKIO.trueDefenseBonus = 15;
CO_YUKIO.bombDamage = 3;
CO_YUKIO.powerOffBonus = 0;
CO_YUKIO.powerDefBonus = 10;
CO_YUKIO.d2dOffBonus = 0;
CO_YUKIO.d2dDefBonus = 0;

CO_YUKIO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(8);
};
CO_YUKIO.activateSuperpower = function(co, powerMode, map)
{
    CO_YUKIO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_YUKIO.getSuperPowerDescription = function()
{
    return CO_YUKIO.getPowerDescription();
};
CO_YUKIO.getSuperPowerName = function()
{
    return CO_YUKIO.getPowerName();
};
