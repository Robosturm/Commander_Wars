CO_NELL.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NELL.activateSuperpower = function(co, powerMode, map)
{
	CO_NELL.activatePower(co, map);
};
CO_NELL.getSuperPowerDescription = function()
{
    return CO_NELL.getPowerDescription();
};
CO_NELL.getSuperPowerName = function()
{
    return CO_NELL.getPowerName();
};
CO_NELL.getBonusLuck = function(co, unit, posX, posY, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                return 15;
            }
            break;
    }
};
