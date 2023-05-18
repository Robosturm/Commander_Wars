CO_SOPHIE.superPowerSecondAttackOffMalus = 30;
CO_SOPHIE.superPowerDefenderDamageReduction = 0.2;
CO_SOPHIE.powerOffBonus = 0;
CO_SOPHIE.powerDefBonus = 10;
CO_SOPHIE.d2dDefenderDamageReduction = 0.2;
CO_SOPHIE.d2dCoZoneDefenderDamageReduction = 0.0;
CO_SOPHIE.d2dCoZoneDefBonus = 0;
CO_SOPHIE.d2dCoZoneOffBonus = 0;

CO_SOPHIE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SOPHIE.activateSuperpower = function(co, powerMode, map)
{
    CO_SOPHIE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SOPHIE.getSuperPowerDescription = function()
{
    return CO_SOPHIE.getPowerDescription();
};
CO_SOPHIE.getSuperPowerName = function()
{
    return CO_SOPHIE.getPowerName();
};
