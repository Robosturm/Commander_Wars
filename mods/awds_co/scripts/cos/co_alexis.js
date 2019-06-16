CO_ALEXIS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
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
            return 10;
        case GameEnums.PowerMode_Power:
            if (nearBuildings)
            {
                return 30;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 10;
            }
        }
    };


};

CO_ALEXIS.startOfTurn = function(co)
{
    if (co.getIsCO0() === true)
    {
        var buildings = co.getOwner().getBuildings();
        var fields = globals.getCircle(1, 1);
        for (var i2 = 0; i2 < buildings.size(); i2++)
        {
            var building = buildings.at(i2);
            var x = building.getX();
            var y = building.getY();
            var animation = null;
            for (var i = 0; i < fields.size(); i++)
            {
                var point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                    if ((unit !== null) &&
                            (unit.getOwner() === co.getOwner()))
                    {
                        UNIT.repairUnit(unit, 1);
                        animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                        animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                    }
                }
            }
        }
        fields.remove();
        buildings.remove();
    }
};

CO_ALEXIS.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};

CO_ALEXIS.getRepairBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        return -1;
    }
    return 0;
};
