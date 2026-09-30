CO_XAVIER.superpowerBonus = 60;
CO_XAVIER.powerFirepowerBonus = 0;
CO_XAVIER.powerSupportedFirepowerBonus = 30;
CO_XAVIER.powerDefBonus = 10;
CO_XAVIER.d2dSupportedFirepowerBonus = 20;
CO_XAVIER.d2dCoZoneFirepowerBonus = 0;
CO_XAVIER.d2dCoZoneSupportedFirepowerBonus = 0;
CO_XAVIER.d2dCoZoneDefBonus = 0;
CO_XAVIER.d2dMinLuckHp = 5;
CO_XAVIER.d2dCoZoneMinLuckHp = 0;

CO_XAVIER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_XAVIER.activateSuperpower = function(co, powerMode, map)
{
    CO_XAVIER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_XAVIER.getSuperPowerDescription = function()
{
    return CO_XAVIER.getPowerDescription();
};
CO_XAVIER.getSuperPowerName = function()
{
    return CO_XAVIER.getPowerName();
};
