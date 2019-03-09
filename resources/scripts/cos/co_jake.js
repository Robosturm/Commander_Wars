var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getVehicleUnitIDS = function()
    {
        return ["FLAK", "FLARE", "HEAVY_HOVERCRAFT", "HEAVY_TANK",
                "HOVERCRAFT", "HOVERFLAK", "LIGHT_TANK", "MEGATANK",
                "NEOTANK", "RECON", "ANTITANKCANNON", "ARTILLERY", "MISSILE",
                "PIPERUNNER", "ROCKETTHROWER"];
    };
    this.getGroundIndirectUnitIDS = function()
    {
        return ["ANTITANKCANNON", "ARTILLERY", "MISSILE",
                "PIPERUNNER", "ROCKETTHROWER"];
    };

    this.activatePower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
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
        audio.addMusic("resources/music/cos/jake.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (terrainID === "PLAINS")
                {
                    return 40;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (terrainID === "PLAINS")
                {
                    return 20;
                }
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (terrainID === "PLAINS")
                    {
                        return 20;
                    }
                }
                break;
        }
        if (terrainID === "PLAINS")
        {
            return 10;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        var indirectUnits = CO_JAKE.getGroundIndirectUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
                {
                    return 1;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
                {
                    return 1;
                }
                break;
            default:
                    return 0;
        }
        return 0;
    };
    this.getMovementPointModifier = function(co, unit)
    {
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            if (vehicleUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 2;
            }
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_JAKE = new Constructor();
