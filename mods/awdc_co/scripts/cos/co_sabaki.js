CO_SABAKI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SABAKI.activateSuperpower = function(co, powerMode, map)
{
	CO_SABAKI.activatePower(co, powerMode, map);
};
CO_SABAKI.getSuperPowerDescription = function()
{
    return CO_SABAKI.getPowerDescription();
};
CO_SABAKI.getSuperPowerName = function()
{
    return CO_SABAKI.getPowerName();
};
CO_SABAKI.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
{
    if (gotAttacked === false && attacker.getOwner() === co.getOwner())
    {
        var healPercent = 0.0;
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                healPercent = 0.5;
                break;
            default:
                if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                {
                    healPercent = 0.4;
                }
                break;

        }
        // damage can be negativ if we can't do a counter attack the damge is -1
        // avoid loosing hp cause of our passive or power
        if (atkDamage > 0)
        {
            attacker.setHp(attacker.getHp() + atkDamage * healPercent);
        }
    }
};
