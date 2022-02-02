CO_SASHA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SASHA.activateSuperpower = function(co, powerMode, map)
{
	CO_SASHA.activatePower(co, powerMode);
};
CO_SASHA.getSuperPowerDescription = function()
{
    return CO_SASHA.getPowerDescription();
};
CO_SASHA.getSuperPowerName = function()
{
    return CO_SASHA.getPowerName();
};
CO_SASHA.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
{
};
