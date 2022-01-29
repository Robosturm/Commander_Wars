CO_XAVIER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_XAVIER.activateSuperpower = function(co, powerMode, map)
{
	CO_XAVIER.activatePower(co, powerMode);
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
                             defender, defPosX, defPosY, isDefender, action, map)
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
                return 20 + hpRounded / 2;
            }
            return 20;
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
                return -20 + -hpRounded;
            }
            return -20;
        default:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return -hpRounded;
            }
            break;
    }
    return 0;
};
