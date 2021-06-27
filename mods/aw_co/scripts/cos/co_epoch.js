CO_EPOCH.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(1);
};
CO_EPOCH.activateSuperpower = function(co, powerMode)
{
    CO_EPOCH.activatePower(co, powerMode);
};
CO_EPOCH.getSuperPowerDescription = function()
{
    return CO_EPOCH.getPowerDescription();
};
CO_EPOCH.getSuperPowerName = function()
{
    return CO_EPOCH.getPowerName();
};
CO_EPOCH.getHpHidden = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            // are the hp hidden of this unit?
            return true;
        default:
            break;
        }
    }
    return false;

};

CO_EPOCH.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
        default:
            return 10;
        }
    }
    return 0;
};
CO_EPOCH.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};
CO_EPOCH.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};
CO_EPOCH.getFirerangeModifier = function(co, unit, posX, posY)
{
    return 0;
};
CO_EPOCH.getMovementpointModifier = function(co, unit, posX, posY)
{
    return 0;
};
