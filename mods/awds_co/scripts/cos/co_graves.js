CO_GRAVES.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false)
        {
            var stunLevel = 0;
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                stunLevel = 4;
                break;
            case GameEnums.PowerMode_Power:
                stunLevel = 3;
                break;
            default:
                stunLevel = 2;
                break;
            }
            if (defender.getHpRounded() <= stunLevel)
            {
                defender.setHasMoved(true);
            }
        }
    }
};
