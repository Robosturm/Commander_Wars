CO_GRAVES.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRAVES.activateSuperpower = function(co, powerMode, map)
{
	CO_GRAVES.activatePower(co, powerMode, map);
};
CO_GRAVES.getSuperPowerDescription = function()
{
    return CO_GRAVES.getPowerDescription();
};
CO_GRAVES.getSuperPowerName = function()
{
    return CO_GRAVES.getPowerName();
};
CO_GRAVES.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
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
};
