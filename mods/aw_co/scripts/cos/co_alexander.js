CO_ALEXANDER.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_ALEXANDER.activateSuperpower = function(co, powerMode)
{
    CO_ALEXANDER.activatePower(co, powerMode);
};
CO_ALEXANDER.getSuperPowerDescription = function()
{
    return CO_ALEXANDER.getPowerDescription();
};
CO_ALEXANDER.getSuperPowerName = function()
{
    return CO_ALEXANDER.getPowerName();
};
CO_ALEXANDER.startOfTurn = function(co)
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
        }
    }
};

CO_ALEXANDER.getIncomeReduction = function(co, building, income)
{
    return 0;
};

CO_ALEXANDER.getCaptureBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            var hp = unit.getHpRounded();
            return hp / 2;
        }
    }
    return 0;
};

CO_ALEXANDER.coZoneBonus = 0;
CO_ALEXANDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_ALEXANDER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
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
