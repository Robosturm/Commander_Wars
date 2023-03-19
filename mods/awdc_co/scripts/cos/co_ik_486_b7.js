CO_IK_486_B7.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_IK_486_B7.activateSuperpower = function(co, powerMode, map)
{
    CO_IK_486_B7.activatePower(co, map);
};
CO_IK_486_B7.getSuperPowerDescription = function()
{
    return CO_IK_486_B7.getPowerDescription();
};
CO_IK_486_B7.getSuperPowerName = function()
{
    return CO_IK_486_B7.getPowerName();
};
CO_IK_486_B7.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
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
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            if (attacker.getBaseMaxRange() === 1 &&
                    atkPosX === attacker.getX() &&
                    atkPosY === attacker.getY() &&
                    Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return -10;
            }
            return 10;
        }
        break;
    }
    return 0;
};

CO_IK_486_B7.getFirerangeModifier = function(co, unit, posX, posY, map)
{
    if (unit.getBaseMaxRange() === 1 &&
            posX === unit.getX() &&
            posY === unit.getY())
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 2;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                return 1;
            }
        }
    }
    return 0;
};

CO_IK_486_B7.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        if (defender.getBaseMaxRange() === 1)
        {
            return 30;
        }
        break;
    default:
        if (co.inCORange(Qt.point(defPosX, defPosY), defender))
        {
            return 10;
        }
        break;
    }
    return 0;
};
