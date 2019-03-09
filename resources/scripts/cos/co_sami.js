var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(6);
    };

    this.activatePower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unitInfantryIDs.indexOf(unit.getUnitID()) >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (animations.length < 5)
                {
                    animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                    animations[counter].queueAnimation(animation);
                    animations[counter] = animation;
                    counter++;
                    if (counter >= animations.length)
                    {
                        counter = 0;
                    }
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        audio.addMusic("resources/music/cos/power.mp3");
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unitInfantryIDs.indexOf(unit.getUnitID())  >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                if (animations.length < 5)
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                    animations[counter].queueAnimation(animation);
                    animations[counter] = animation;
                    counter++;
                    if (counter >= animations.length)
                    {
                        counter = 0;
                    }
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        audio.addMusic("resources/music/cos/superpower.mp3");
        audio.playRandom();
    };
    this.startOfTurn = function(co)
    {
        audio.addMusic("resources/music/cos/sami.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 80;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 50;
                }
                break;
            default:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 30;
                    }
                    return 15;
                }
                break;
        }
        return -10;
    };
    this.getCaptureBonus = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            return 20;
        }
        else
        {
            var hp = unit.getHpRounded();
            return hp / 2;
        }
    };

    this.getMovementPointModifier = function(co, unit)
    {
        var unitTransportIDs = ["APC", "LANDER", "T_HELI", "TRANSPORTPLANE"];
        if (unitTransportIDs.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
            if (unitInfantryIDs.indexOf(unit.getUnitID()) >= 0)
            {
                return 2;
            }
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_SAMI = new Constructor();
