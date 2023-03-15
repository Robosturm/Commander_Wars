CO_XAVIER.firepowerBonus = 20;
CO_XAVIER.luckDamage = 40;

CO_XAVIER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_XAVIER.activateSuperpower = function(co, powerMode, map)
{
	CO_XAVIER.activatePower(co, map);
};
CO_XAVIER.getSuperPowerDescription = function()
{
    return CO_XAVIER.getPowerDescription();
};
CO_XAVIER.getSuperPowerName = function()
{
    return CO_XAVIER.getPowerName();
};
CO_XAVIER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return CO_XAVIER.firepowerBonus;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_XAVIER.firepowerBonus;
            }
            break;
    }
    return 0;
};
CO_XAVIER.getBonusLuck = function(co, unit, posX, posY, map)
{
     var hpRounded = unit.getHpRounded();
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return CO_XAVIER.luckDamage + hpRounded / 2;
            }
            return CO_XAVIER.luckDamage;
        default:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return hpRounded / 2;
            }
    }
    return 0;
};

CO_XAVIER.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    var hpRounded = unit.getHpRounded();
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return -CO_XAVIER.luckDamage + -hpRounded;
            }
            return -CO_XAVIER.luckDamage;
        default:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return -hpRounded;
            }
            break;
    }
    return 0;
};
