var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getDirectUnitIDS = function()
    {
        return ["BOMBER", "CANNONBOAT", "CRUISER", "DESTROYER", "DUSTER",
                "FIGHTER", "FLAK", "FLARE", "HEAVY_HOVERCRAFT", "HEAVY_TANK",
                "HOVERCRAFT", "HOVERFLAK", "K_HELI", "LIGHT_TANK", "MEGATANK",
                "NEOTANK", "RECON", "STEALTHBOMBER", "SUBMARINE", "WATERPLANE"];
    };
    this.getIndirectUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "ANTITANKCANNON", "ARTILLERY", "BATTLESHIP",
                "MISSILE", "PIPERUNNER", "ROCKETTHROWER"];
    };

    this.activatePower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        var tankUnits = CO_MAX.getDirectUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (tankUnits.indexOf(unit.getUnitID()) >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                animation.addSprite("power6", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                if (animations.length < 5)
                {
                    animations.push(animation);
                }
                else
                {
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
        var tankUnits = CO_MAX.getDirectUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (tankUnits.indexOf(unit.getUnitID())  >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                if (animations.length < 5)
                {
                    animations.push(animation);
                }
                else
                {
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
        audio.addMusic("resources/music/cos/max.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        var tankUnits = CO_MAX.getDirectUnitIDS();
        var indirectUnits = CO_MAX.getIndirectUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (tankUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (tankUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 45;
                }
                break;
            default:
                if (tankUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 30;
                    }
                    return 15;
                }
                break;
        }
        if (indirectUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        return -1;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getMovementPointModifier = function(co, unit)
    {
        var tankUnits = CO_MAX.getDirectUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if (tankUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {

            if (tankUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 2;
            }
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_MAX = new Constructor();
