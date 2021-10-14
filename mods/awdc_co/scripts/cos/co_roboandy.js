CO_ROBOANDY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ROBOANDY.activateSuperpower = function(co, powerMode)
{
	CO_ROBOANDY.activatePower(co, powerMode);
};
CO_ROBOANDY.getSuperPowerDescription = function()
{
    return CO_ROBOANDY.getPowerDescription();
};
CO_ROBOANDY.getSuperPowerName = function()
{
    return CO_ROBOANDY.getPowerName();
};

CO_ROBOANDY.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (gotAttacked && defender.getOwner() === co.getOwner())
            {
                if (defender.getHp() <= 0)
                {
                    attacker.setHp(attacker.getHp() - 1);
                }
            }
            else if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
            {
                attacker.setHp(attacker.getHp() + 1);
            }
            break;
        default:
            break;
    }
};
