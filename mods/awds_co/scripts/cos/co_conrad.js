CO_CONRAD.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (isDefender)
        {
            // return weak counter damage of conrad
            return -20;
        }
        var units = co.getOwner().getUnits();
        var visionCount = 0;
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var x = unit.getX();
            var y = unit.getY();
            var distance = Math.abs(x - defPosX) + Math.abs(y - defPosY);
            if (distance <= unit.getVision(Qt.point(x, y)))
            {
                visionCount += 1;
            }
        }

        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return visionCount * 2 + 10;
        case GameEnums.PowerMode_Power:
            return visionCount * 5 + 10;
        default:
            return visionCount * 2;
        }
    }
    return 0;
};

CO_CONRAD.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        // reduce luck to zero
        return -999;
    }
    return 0;
};

CO_CONRAD.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_CONRAD.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                   defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (isDefender === false)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var variables = co.getVariables();
                var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
                var bonusDamage = dmgModVar.readDataFloat();
                return bonusDamage;
            case GameEnums.PowerMode_Power:
            default:
                break;
            }
        }
        // return average luck as true damage
        return attackerBaseHp / 4;
    }
    return 0;
};

CO_CONRAD.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var variables = co.getVariables();
                var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
                var defenderDamage = defender.getHp() * 10;
                // overkill case
                if (defenderDamage < 0)
                {
                    dmgModVar.writeDataFloat(-defenderDamage);
                }
                else
                {
                    dmgModVar.writeDataFloat(0);
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
        }
    }
};

CO_CONRAD.getVisionrangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            return 1;
        default:
            return 0;
        }
    }
    return 0;
};
