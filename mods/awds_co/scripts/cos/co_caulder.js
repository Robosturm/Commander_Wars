CO_CAULDER.coZoneBonus = 0;
CO_CAULDER.coGlobalBonus = 20;
CO_CAULDER.powerBonus = 30;
CO_CAULDER.superPowerBonus = 50;
CO_CAULDER.coHealing = 1;
CO_CAULDER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return CO_CAULDER.superPowerBonus;
        case GameEnums.PowerMode_Power:
            return CO_CAULDER.powerBonus;
        default:
            return CO_CAULDER.coGlobalBonus;
        }
    }
};

CO_CAULDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return CO_CAULDER.superPowerBonus;
        case GameEnums.PowerMode_Power:
            return CO_CAULDER.powerBonus;
        default:
            return CO_CAULDER.coGlobalBonus;
        }
    }
};

CO_CAULDER.startOfTurn = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        var player = co.getOwner();
        if (!player.getIsDefeated())
        {
            var animations = [];
            var counter = 0;
            var units = co.getOwner().getUnits();
            units.randomize();
            var viewplayer = map.getCurrentViewPlayer();
            var size = units.size();
            for (var i = 0; i < size; i++)
            {
                var unit = units.at(i);
                UNIT.repairUnit(unit, 1, map);
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                var animation = GameAnimationFactory.createAnimation(map, unitX, unitY);
                animation.setSound("power0.wav", 1, delay);
                if (animations.length < 5)
                {
                    animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    animations[counter].queueAnimation(animation);
                    animations[counter] = animation;
                    counter++;
                    if (counter >= animations.length)
                    {
                        counter = 0;
                    }
                }
                if (!viewplayer.getFieldVisible(unitX, unitY))
                {
                    animation.setVisible(false);
                }
            }
            units.remove();
        }
    }
};

