CO_ALEXIS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    var nearBuildings = false;
    var fields = globals.getCircle(0, 2);
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

    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (nearBuildings)
            {
                return 50;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            if (nearBuildings)
            {
                return 30;
            }
            return 0;
        default:
            return 0;
    }
};
