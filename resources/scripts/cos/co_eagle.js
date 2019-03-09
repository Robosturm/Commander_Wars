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
            if (unitInfantryIDs.indexOf(unit.getUnitID())  < 0)
            {
                unit.setHasMoved(false);
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                if (animations.length < 5)
                {
                    animation.addSprite("power1", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power1", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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
            if (unitInfantryIDs.indexOf(unit.getUnitID())  < 0)
            {
                unit.setHasMoved(false);
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
        audio.addMusic("resources/music/cos/eagle.mp3")
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

    this.getFuelCostModifier = function(co, unit, costs)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        if (airUnits.indexOf(unit.getUnitID()) >= 0)
        {
            return -2;
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        var seaUnits = CO_EAGLE.getSeaUnitIDS();

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 30;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return -30;
                }
                else if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return -40;
                }
                else
                {
                    return -45;
                }
            default:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 20;
                    }
                    return 10;
                }
                break;
        }
        if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_EAGLE = new Constructor();
