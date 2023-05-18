CO_BRENNER.superPowerHeal = 5;
CO_BRENNER.superPowerDefBonus = 50;

CO_BRENNER.powerHeal = 3;
CO_BRENNER.powerDefBonus = 30;
CO_BRENNER.powerOffBonus = 0;

CO_BRENNER.d2dCoZoneDefBonus = 0;
CO_BRENNER.d2dCoZoneOffBonus = 0;

CO_BRENNER.d2dDefBonus = 15;


CO_BRENNER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_BRENNER.activateSuperpower = function(co, powerMode, map)
{
    CO_BRENNER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_BRENNER.getSuperPowerDescription = function()
{
    return CO_BRENNER.getPowerDescription();
};
CO_BRENNER.getSuperPowerName = function()
{
    return CO_BRENNER.getPowerName();
};
