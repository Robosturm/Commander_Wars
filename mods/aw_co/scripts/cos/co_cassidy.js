CO_CASSIDY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_CASSIDY.activateSuperpower = function(co, powerMode, map)
{
    CO_CASSIDY.activatePower(co, map);
};
CO_CASSIDY.getSuperPowerDescription = function()
{
    return CO_CASSIDY.getPowerDescription();
};
CO_CASSIDY.getSuperPowerName = function()
{
    return CO_CASSIDY.getPowerName();
};
CO_CASSIDY.coZoneBonus = 0;
CO_CASSIDY.defaultPowerBonus = 20;
CO_CASSIDY.powerBonus = 50;
CO_CASSIDY.superPowerBonus = 50;
CO_CASSIDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (defender !== null)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (attacker.getHp() >= defender.getHp())
                {
                    return CO_CASSIDY.powerBonus;
                }
                return CO_CASSIDY.defaultPowerBonus;
            default:
                if (attacker.getHp() >= defender.getHp())
                {
                    return CO_CASSIDY.defaultPowerBonus;
                }
            }
        }
    }
    return 0;
};

CO_CASSIDY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
