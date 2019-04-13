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
                return true;
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
                return 30;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 30;
            case GameEnums.PowerMode_Power:
                return 20;
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

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("The first prototype for the Robot Commanding Officer Series invented by Lash.");
    };
    this.getHits = function()
    {
        return qsTr("Tests");
    };
    this.getMiss = function()
    {
        return qsTr("Upgrades");
    };
    this.getCODescription = function()
    {
        return qsTr("Epoch is blessed with simple programming so it has only basic command abilities. Powers charge at an unmatched speed.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Firepower is increased and unit HP is hidden from opponents.");
    };
    this.getPowerName = function()
    {
        return qsTr("Short Circuit");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Direct combat units gain one movement and indirect combat units recieve one extra range. Hides HP from opponents. Firepower is increased.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Shocker");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Error: Overcharged. initiate saftey mode."),
                qsTr("Power surge detected."),
                qsTr("Overload! Release Energy!"),
                qsTr("System Over. Overload equals very yes."),
                qsTr("Blackout!"),
                qsTr("Register your firmware for continued use!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Epoch Succesful. Upgrade irrelevant"),
                qsTr("Test Completed. Anaylsing battle log..."),
                qsTr("Victory recorded in database and filed under category 'insulting'")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Noooo... No upgrade please.")];
    };
    this.getName = function()
    {
        return qsTr("Epoch");
    };
}

Constructor.prototype = CO;
var CO_EPOCH = new Constructor();
