CO_SONJA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_SONJA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_SONJA.getBonusMisfortune = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        return 5;
    }
    return 0;
};

CO_SONJA.getVisionrangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 2;
        default:
            return 1;
        }
    }
    return 0;
};
CO_SONJA.getEnemyTerrainDefenseModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return -3;
        case GameEnums.PowerMode_Power:
            return -2;
        default:
            return -1;
        }
    }
    return 0;
};

CO_SONJA.getPerfectVision = function(co)
{
    if (co.getIsCO0() === true)
    {
        // return true if forest and reafs etc are disabled in fog of war see sonja
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return true;
        case GameEnums.PowerMode_Power:
            return true;
        default:
            return false;
        }
    }
    return false;
};

CO_SONJA.getFirstStrike = function(co, unit, posX, posY, attacker)
{
    if (co.getIsCO0() === true)
    {
        // sonja's troops get first strike during counter attacks
        // meaning her troops attack before they loose their hp
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return true;
        case GameEnums.PowerMode_Power:
            return false;
        default:
            return false;
        }
    }
    return false;
};

CO_SONJA.getHpHidden = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        // are the hp hidden of this unit?
        return true;
    }
    return false;
};
