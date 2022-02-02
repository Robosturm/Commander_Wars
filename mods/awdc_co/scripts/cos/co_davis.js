CO_DAVIS.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_DAVIS.activateSuperpower = function(co, powerMode, map)
{
    CO_DAVIS.activatePower(co, powerMode);
};
CO_DAVIS.getSuperPowerDescription = function()
{
    return CO_DAVIS.getPowerDescription();
};
CO_DAVIS.getSuperPowerName = function()
{
    return CO_DAVIS.getPowerName();
};
CO_DAVIS.getBonusLuck = function(co, unit, posX, posY, map)
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
        case GameEnums.PowerMode_Power:
            luckPerUnit = 4;
            maxLuck = 100;
            break;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                luckPerUnit = 2;
                maxLuck = 50
            }
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

CO_DAVIS.getBonusMisfortune = function(co, unit, posX, posY, map)
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
        case GameEnums.PowerMode_Power:
            maxMissFortune = 60;
            luckMissFortune = 3;
            break;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                maxMissFortune = 40;
                luckMissFortune = 2;
            }
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

