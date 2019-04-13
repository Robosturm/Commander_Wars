var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(1);
        co.setSuperpowerStars(4);
    };

    this.getDirectUnitIDS = function()
    {
        return ["BOMBER", "CANNONBOAT", "CRUISER", "DESTROYER", "DUSTER",
                "FIGHTER", "FLAK", "FLARE", "HEAVY_HOVERCRAFT", "HEAVY_TANK",
                "HOVERCRAFT", "HOVERFLAK", "K_HELI", "LIGHT_TANK", "MEGATANK",
                "NEOTANK", "RECON", "STEALTHBOMBER", "SUBMARINE", "WATERPLANE",
                "INFANTRY", "MECH", "MOTORBIKE"];
    };
    this.getIndirectUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "ANTITANKCANNON", "ARTILLERY", "BATTLESHIP",
                "MISSILE", "PIPERUNNER", "ROCKETTHROWER", "SNIPER"];
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_EPOCH.loadCOMusic(co);
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
        CO_EPOCH.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/epoch.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "MA";
    };
    this.getHpHidden = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                break;
            case GameEnums.PowerMode_Power:
                // are the hp hidden of this unit?
                return true;
            default:
                break;
        }
        return false;

    };

    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 15;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(posX, posY)))
                {
                    return 10;
                }
                break;
        }
        return 5;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 40;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                break;
        }
        return 0;
    };
    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 5;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        var indirectUnits = CO_EPOCH.getIndirectUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
                {
                    return 1;
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
        }
        return 0;
    };
    this.getMovementPointModifier = function(co, unit)
    {
        var directUnits = CO_EPOCH.getDirectUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            if (directUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 1;
            }
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_EPOCH = new Constructor();
