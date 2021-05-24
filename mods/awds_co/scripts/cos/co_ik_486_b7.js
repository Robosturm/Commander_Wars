CO_IK_486_B7.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() === 1 &&
                    atkPosX === attacker.getX() &&
                    atkPosY === attacker.getY() &&
                    Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 0;
            }
            else
            {
                return 10;
            }
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1 &&
                    atkPosX === attacker.getX() &&
                    atkPosY === attacker.getY() &&
                    Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 0;
            }
            else
            {
                return 10;
            }
        default:
            if (attacker.getBaseMaxRange() === 1 &&
                    atkPosX === attacker.getX() &&
                    atkPosY === attacker.getY() &&
                    Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return -10;
            }
            break;
        }
    }
    return 0;
};

CO_IK_486_B7.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getBaseMaxRange() === 1 &&
                posX === unit.getX() &&
                posY === unit.getY())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 3;
            case GameEnums.PowerMode_Power:
                return 2;
            default:
                return 1;
            }
        }
    }
    return 0;
};

CO_IK_486_B7.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (defender.getBaseMaxRange() === 1)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 40;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                break;
            }
        }
    }
    return 0;
};
