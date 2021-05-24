CO_NAPOLEON.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var terrainDefense = 0;
                if (map.onMap(atkPosX, atkPosY))
                {
                    terrainDefense = map.getTerrain(atkPosX, atkPosY).getDefense(attacker);
                }
                return terrainDefense * 10 + 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                return 0;
            }
    }
    return 0;
};

CO_NAPOLEON.getAttackHpBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 4;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
    }
    return 0;
};

CO_NAPOLEON.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                          defender, defPosX, defPosY, isDefender, luckMode)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            var defHp = defender.getHp() * 10;
            if (damage  > defHp / 2)
            {
                return damage - defHp / 2;
            }
            return 0;
        default:
            break;
        }
    }
    return 0;
};

CO_NAPOLEON.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 60
            }
            else
            {
                return 40;
            }
        case GameEnums.PowerMode_Power:
            if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 30
            }
            else
            {
                return 20;
            }
        default:
            if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 30
            }
            else
            {
                return 15;
            }
        }
    }
    return 0;
};
