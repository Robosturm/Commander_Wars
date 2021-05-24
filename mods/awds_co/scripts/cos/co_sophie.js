CO_SOPHIE.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                        defender, defPosX, defPosY, isDefender, luckMode)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender === true)
            {
                return damage * 0.2;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (isDefender === true)
            {
                return damage;
            }
            break;
        default:
            if (isDefender === true)
            {
                return damage * 0.2;
            }
            break;
        }
    }
    return 0;
};

CO_SOPHIE.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (!gotAttacked && defender.getHp() > 0 && attacker.getHp() > 0)
                {
                    // we're the attacker
                    var variable = attacker.getVariables().createVariable("SOPHIE_SCOP");
                    if (variable.readDataBool() === false)
                    {
                        variable.writeDataBool(true);
                        var damageResult = ACTION_FIRE.calcBattleDamage2(attacker, Qt.point(attacker.getX(), attacker.getY()),
                                                                         defender.getX(), defender.getY(), GameEnums.LuckDamageMode_On);
                        // do another attack
                        ACTION_FIRE.battle(attacker, damageResult.x, damageResult.y,
                                           defender.getX(), defender.getY(), damageResult.width, damageResult.height,
                                           true);
                    }
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

CO_SOPHIE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender === false)
            {
                var variable = attacker.getVariables().createVariable("SOPHIE_SCOP");
                if (variable.readDataBool() === true)
                {
                    // reduce damage during the second attack
                    return -30;
                }
            }
            return 10;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};

CO_SOPHIE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
