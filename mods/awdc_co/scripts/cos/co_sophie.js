CO_SOPHIE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SOPHIE.activateSuperpower = function(co, powerMode, map)
{
	CO_SOPHIE.activatePower(co, map);
};
CO_SOPHIE.getSuperPowerDescription = function()
{
    return CO_SOPHIE.getPowerDescription();
};
CO_SOPHIE.getSuperPowerName = function()
{
    return CO_SOPHIE.getPowerName();
};
CO_SOPHIE.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                              defender, defPosX, defPosY, isDefender, luckMode, map)
{
    // reduce counter damage by a flat amount here
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (isDefender === true)
            {
                return damage;
            }
            break;
        default:
            if (isDefender === true)
            {
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return damage * 0.2;
                }
            }
            break;
    }
    return 0;
};

CO_SOPHIE.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
{
};

CO_SOPHIE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
             return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 20;
            }
            break;
    }
    return 0;
};
