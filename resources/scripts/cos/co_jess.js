var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;

        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.refill();

            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
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
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.refill();

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
        units.remove();

        audio.clearPlayList();
        audio.addMusic("resources/music/cos/superpower.mp3");
        audio.playRandom();
    };
    this.startOfTurn = function(co)
    {
        audio.addMusic("resources/music/cos/jess.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };

    this.getAirUnitIDS = function()
    {
        return ["BOMBER", "FIGHTER", "DUSTER", "K_HELI", "T_HELI", "STEALTHBOMBER", "TRANSPORTPLANE", "WATERPLANE"];
    };
    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "DESTROYER", "SUBMARINE"];
    };
    this.getInfatryUnitIDS = function()
    {
        return ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_JESS.getAirUnitIDS();
        var seaUnits = CO_JESS.getSeaUnitIDS();
        var infantryUnits = CO_JESS.getInfatryUnitIDS();

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
                {
                    return 50;
                }
                break;
            default:
                if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 30;
                    }
                    return 15;
                }
                break;
        }
        if ((airUnits.indexOf(attacker.getUnitID()) >= 0) ||
            (seaUnits.indexOf(attacker.getUnitID()) >= 0))
        {
            return -10;
        }
        return 0;
    };

    this.getMovementPointModifier = function(co, unit)
    {
        var airUnits = CO_JESS.getAirUnitIDS();
        var seaUnits = CO_JESS.getSeaUnitIDS();
        var infantryUnits = CO_JESS.getInfatryUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if ((airUnits.indexOf(unit.getUnitID()) < 0) &&
                (seaUnits.indexOf(unit.getUnitID()) < 0) &&
                (infantryUnits.indexOf(unit.getUnitID()) < 0))
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            if ((airUnits.indexOf(unit.getUnitID()) < 0) &&
                (seaUnits.indexOf(unit.getUnitID()) < 0) &&
                (infantryUnits.indexOf(unit.getUnitID()) < 0))
            {
                return 2;
            }
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_JESS = new Constructor();
