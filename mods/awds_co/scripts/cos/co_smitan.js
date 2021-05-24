CO_SMITAN.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() > 1)
                {
                    var variables = attacker.getVariables();
                    var variable = variables.createVariable("SMITAN_ATTACK_COUNT");
                    var counter = variable.readDataInt32();
                    counter--;
                    variable.writeDataInt32(counter);
                    // enable unit for another move
                    attacker.setHasMoved(false);
                }
                break;
            default:
                break;
            }
        }
    }
};

CO_SMITAN.getActionModifierList = function(co, unit)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                var variables = unit.getVariables();
                var variable = variables.createVariable("SMITAN_ATTACK_COUNT");
                var counter = variable.readDataInt32();
                if (counter <= 0)
                {
                    // disable firing with this unit
                    return ["-ACTION_FIRE"];
                }
            }
            break;
        default:
            break;
        }
    }
    return [];
};

CO_SMITAN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
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
            if (attacker.getBaseMaxRange() === 1)
            {
                return inRangeCount * 20 + 10;
            }
            else
            {
                return 10;
            }
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
            if (attacker.getBaseMaxRange() === 1)
            {
                return inRangeCount * 5;
            }
            break;
        }
    }
    return 0;
};
CO_SMITAN.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (unit.getBaseMaxRange() > 1)
            {
                return 2;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            break;
        default:
            break;
        }
    }
    return 0;
};
