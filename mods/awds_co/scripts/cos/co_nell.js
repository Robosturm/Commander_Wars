CO_NELL.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return 100;
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            return 15;
        }
    }
    return 0;
};
