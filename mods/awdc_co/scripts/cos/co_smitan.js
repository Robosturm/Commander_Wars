CO_SMITAN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SMITAN.activateSuperpower = function(co, powerMode)
{
	CO_SMITAN.activatePower(co, powerMode);
};
CO_SMITAN.getSuperPowerDescription = function()
{
    return CO_SMITAN.getPowerDescription();
};
CO_SMITAN.getSuperPowerName = function()
{
    return CO_SMITAN.getPowerName();
};
CO_SMITAN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    var inRangeCount = 0;
    if (attacker.getBaseMaxRange() === 1)
    {
        var units = co.getOwner().getUnits();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var x = unit.getX();
            var y = unit.getY();
            var distance = Math.abs(x - defPosX) + Math.abs(y - defPosY);
            if (unit.getBaseMaxRange() > 1)
            {
                if (unit.getMinRange(Qt.point(x, y)) <= distance && distance <= unit.getMaxRange(unit.getPosition()))
                {
                    inRangeCount += 1;
                }
            }
        }
        units.remove();
    }

    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1)
            {
                return inRangeCount * 10 + 10;
            }
            else
            {
                return 10;
            }
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getBaseMaxRange() === 1)
                {
                    return inRangeCount * 10 + 10;
                }
                return 10;
            }
            break;
    }
    return 0;
};
CO_SMITAN.getFirerangeModifier = function(co, unit, posX, posY)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            break;
        default:
            break;
    }
    return 0;
};
