CO_ALEXIS.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ALEXIS.activateSuperpower = function(co, powerMode)
{
    CO_ALEXIS.activatePower(co, powerMode);
};
CO_ALEXIS.getSuperPowerDescription = function()
{
    return CO_ALEXIS.getPowerDescription();
};
CO_ALEXIS.getSuperPowerName = function()
{
    return CO_ALEXIS.getPowerName();
};
CO_ALEXIS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    var nearBuildings = false;
    var fields = globals.getCircle(0, 2);
    if (map !== null)
    {
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + atkPosX;
            var y = fields.at(i).y + atkPosY;
            if (map.onMap(x, y))
            {
                var building = map.getTerrain(x, y).getBuilding();
                if (building !== null && building.getOwner() === co.getOwner())
                {
                    nearBuildings = true;
                    break;
                }
            }
        }
    }
    fields.remove();
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        if (nearBuildings)
        {
            return 30;
        }
        return 10;
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return CO_ALEXIS.coZoneBonus;
        }
    }
    return 0;
};
