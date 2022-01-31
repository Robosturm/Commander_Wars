CO_ALEXANDER.startOfTurn = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        var player = co.getOwner();
        if (!player.getIsDefeated())
        {
            var buildings = player.getBuildings();
            for (var i = 0; i < buildings.size(); i++)
            {
                var building = buildings.at(i);
                var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
                if (unit !== null)
                {
                    var points = unit.getCapturePoints();
                    // apply revolt bonus
                    if (points > 0)
                    {
                        points--;
                        unit.setCapturePoints(points);
                    }
                }
            }
            buildings.remove();
        }
    }
};

CO_ALEXANDER.getIncomeReduction = function(co, building, income, map)
{
    // set income to 0 during scop
    if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
    {
        return income;
    }
    return 0;
};

CO_ALEXANDER.getCaptureBonus = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            var hp = unit.getHpRounded();
            return hp / 2;
        }
    }
    return 0;
};

CO_ALEXANDER.coZoneBonus = 0;
CO_ALEXANDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};

CO_ALEXANDER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
