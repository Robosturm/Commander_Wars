CO_CASSIDY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_CASSIDY.activateSuperpower = function(co, powerMode, map)
{
	CO_CASSIDY.activatePower(co, powerMode);
};
CO_CASSIDY.getSuperPowerDescription = function()
{
    return CO_CASSIDY.getPowerDescription();
};
CO_CASSIDY.getSuperPowerName = function()
{
    return CO_CASSIDY.getPowerName();
};
CO_CASSIDY.coZoneBonus = 60;
CO_CASSIDY.defaultPowerBonus = 30;
CO_CASSIDY.powerBonus = 60;
CO_CASSIDY.superPowerBonus = 60;
CO_CASSIDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action, map)
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
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getHp() >= defender.getHp())
                {
                    return CO_CASSIDY.coZoneBonus;
                }
                return 10;
            }
            break;
        }
    }
    return 0;
};
