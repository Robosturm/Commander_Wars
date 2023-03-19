CO_VARLOT.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_VARLOT.activateSuperpower = function(co, powerMode, map)
{
    CO_VARLOT.activatePower(co, map);
};
CO_VARLOT.getSuperPowerDescription = function()
{
    return CO_VARLOT.getPowerDescription();
};
CO_VARLOT.getSuperPowerName = function()
{
    return CO_VARLOT.getPowerName();
};
CO_VARLOT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};
CO_VARLOT.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_VARLOT.getCaptureBonus = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 5;
        }
    }
    return 0;
};

CO_VARLOT.getIncomeReduction = function(co, building, income, map)
{
    if (co.getIsCO0() === true)
    {
        var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
        // set income to 0 during scop
        if (unit !== null && unit.getOwner() === co.getOwner())
        {
            return income * unit.getCapturePoints() / 20;
        }
    }
    return 0;
};
