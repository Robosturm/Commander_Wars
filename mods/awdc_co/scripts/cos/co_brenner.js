CO_BRENNER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_BRENNER.activateSuperpower = function(co, powerMode)
{
	CO_BRENNER.activatePower(co, powerMode);
};
CO_BRENNER.getSuperPowerDescription = function()
{
    return CO_BRENNER.getPowerDescription();
};
CO_BRENNER.getSuperPowerName = function()
{
    return CO_BRENNER.getPowerName();
};
CO_BRENNER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 30;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 30;
            }
            break;
    }
    return 0;
};
