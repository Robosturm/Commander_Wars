CO_DAVIS.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var unitCount = co.getOwner().getUnitCount();
        var maxLuck = 0;
        var luckPerUnit = 0;
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            maxLuck = 150;
            luckPerUnit = 6;
            break;
        case GameEnums.PowerMode_Power:
            luckPerUnit = 4;
            maxLuck = 100;
            break;
        default:
                luckPerUnit = 1.2;
                maxLuck = 30
            break;
        }
        var luck = luckPerUnit * unitCount;
        if (luck > maxLuck)
        {
            luck = maxLuck;
        }
        return luck;
    }
    return 0;
};

CO_DAVIS.getBonusMisfortune = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var owner = co.getOwner();
        var playerCounter = map.getPlayerCount();
        var unitCount = 0;
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== owner) &&
                    (owner.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                unitCount += enemyPlayer.getUnitCount();
            }
        }
        var maxMissFortune = 0;
        var luckMissFortune = 0;
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            maxMissFortune = 100;
            luckMissFortune = 4;
            break;
        case GameEnums.PowerMode_Power:
            maxMissFortune = 60;
            luckMissFortune = 3;
            break;
        default:
                maxMissFortune = 20;
                luckMissFortune = 1;
            break;
        }
        var missFortune = luckMissFortune * unitCount;
        if (missFortune > maxMissFortune)
        {
            missFortune = maxMissFortune;
        }
        return missFortune;
    }
    return 0;
};

CO_DAVIS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_DAVIS.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
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
