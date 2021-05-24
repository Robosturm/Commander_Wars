CO_MARY.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        var destroyed = false;
        var x = -1;
        var y = -1;
        var unitID = 0;
        if (gotAttacked  && defender.getOwner() === co.getOwner())
        {
            if (attacker.getHp() <= 0)
            {
                destroyed = true;
                x = attacker.getX();
                y = attacker.getY();
            }
            else
            {
                unitID = attacker.getUniqueID();
            }
        }
        else if (attacker.getOwner() === co.getOwner())
        {
            if (defender.getHp() <= 0)
            {
                destroyed = true;
                x = defender.getX();
                y = defender.getY();
            }
            else
            {
                unitID = defender.getUniqueID();
            }
        }
        // store variable data

        var variables = co.getVariables();
        if (destroyed)
        {
            var building = map.getTerrain(x, y).getBuilding();
            if (building !== null)
            {
                // store capture bonus
                var xVariable = variables.createVariable("POSXBUILDINGS");
                var yVariable = variables.createVariable("POSYBUILDINGS");
                var pointsX = xVariable.readDataListInt32();
                var pointsY = yVariable.readDataListInt32();
                var found = false;
                for (var i = 0; i < pointsX.length; i++)
                {
                    if (pointsX[i] === x &&
                            pointsY[i] === y)
                    {
                        found = true;
                        break;
                    }
                }
                if (found === false)
                {
                    pointsX.push(x);
                    pointsY.push(y);
                    xVariable.writeDataListInt32(pointsX);
                    yVariable.writeDataListInt32(pointsY);
                }
            }
        }
        else
        {
            // store repair block
            var repairVariable = variables.createVariable("UNITREPAIR");
            var repairs = repairVariable.readDataListInt32();
            repairs.push(unitID);
            repairVariable.writeDataListInt32(repairs);
        }
    }
};

CO_MARY.canBeRepaired = function(co, unit, posX, posY)
{
    // disanle enemy repairs
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Power:
        return false;
    case GameEnums.PowerMode_Superpower:
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
                                     defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        // get cop and scop offensive bonus
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 30;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 0;
        }
    }
    return 0;
};

CO_MARY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
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

CO_MARY.getDeffensiveReduction = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        // reduce enemy defense
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (co.getOwner().isEnemyUnit(defender) &&
                    defender.useTerrainDefense())
            {
                return defender.getTerrainDefense() * 15;
            }
        }
    }
    return 0;
};

CO_MARY.getAdditionalBuildingActions = function(co, building)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Superpower:
        return ""
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

CO_MARY.getCaptureBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
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
        if ((co.getPowerMode() === GameEnums.PowerMode_Superpower ||
             co.getPowerMode() === GameEnums.PowerMode_Tagpower) &&
                applyBonus === true)
        {
            return 15;
        }
        else if (applyBonus === true)
        {
            return 5;
        }
    }
    return 0;
};
