CO_CASSIDY.coZoneBonus = 0;
CO_CASSIDY.defaultPowerBonus = 30;
CO_CASSIDY.powerBonus = 60;
CO_CASSIDY.superPowerBonus = 80;
CO_CASSIDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        if (defender !== null)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getHp() >= defender.getHp())
                {
                    return CO_CASSIDY.superPowerBonus;
                }
                return CO_CASSIDY.defaultPowerBonus;
            case GameEnums.PowerMode_Power:
                if (attacker.getHp() >= defender.getHp())
                {
                    return CO_CASSIDY.powerBonus;
                }
                return CO_CASSIDY.defaultPowerBonus;
            default:
                if (attacker.getHp() >= defender.getHp())
                {
                    return 20;
                }
            }
        }
    }
    return 0;
};

CO_CASSIDY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
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
