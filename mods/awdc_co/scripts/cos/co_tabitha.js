CO_TABITHA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};

CO_TABITHA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 60;
            }
            break;
    }
    return 0;
};
CO_TABITHA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 60;
            }
            break;
    }
};
