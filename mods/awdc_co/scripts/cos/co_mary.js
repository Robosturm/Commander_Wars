CO_MARY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MARY.activateSuperpower = function(co, powerMode, map)
{
	CO_MARY.activatePower(co, map);
};
CO_MARY.getSuperPowerDescription = function()
{
    return CO_MARY.getPowerDescription();
};
CO_MARY.getSuperPowerName = function()
{
    return CO_MARY.getPowerName();
};
CO_MARY.canBeRepaired = function(co, unit, posX, posY, map)
{
    // disable enemy repairs
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return false;
        default:
            var variables = co.getVariables();
            var repairVariable = variables.createVariable("UNITREPAIR");
            var repairs = repairVariable.readDataListInt32();
            var unitId = unit.getUniqueID();
            if (repairs.indexOf(unitId) >= 0)
            {
                return false;
            }
    }
    return true;
};

CO_MARY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    // get cop and scop offensive bonus
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 20;
            }
            return 0;
    }
};

CO_MARY.getDeffensiveReduction = function(co, attacker, atkPosX, atkPosY,
                              defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    return 0;
};

CO_MARY.getAdditionalBuildingActions = function(co, building, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            // disable enemy production line
            if (co.getOwner().isEnemy(building.getOwner()))
            {
                return "-ACTION_BUILD_UNITS";
            }
            break;
        default:
            break;
    }
    return "";
};

CO_MARY.getCaptureBonus = function(co, unit, posX, posY, map)
{
    var applyBonus = false;
    // store capture bonus
    var variables = co.getVariables();
    var xVariable = variables.createVariable("POSXBUILDINGS");
    var yVariable = variables.createVariable("POSYBUILDINGS");
    var pointsX = xVariable.readDataListInt32();
    var pointsY = yVariable.readDataListInt32();
    for (var i = 0; i < pointsX.length; i++)
    {
        if (pointsX[i] === posX &&
            pointsX[i] === posX)
        {
            // apply capture bonus and remove it from the list
            applyBonus = true;
            pointsX.splice(i, 1);
            pointsY.splice(i, 1);
            xVariable.writeDataListInt32(pointsX);
            yVariable.writeDataListInt32(pointsY);
            break;
        }
    }
    if (applyBonus === true)
    {
        return 5;
    }
    return 0;
};
