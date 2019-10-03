var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(1);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
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
                powerNameAnimation.queueAnimation(animation);
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

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
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
                powerNameAnimation.queueAnimation(animation);
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
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/epoch.mp3", 100, 79210)
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
            case GameEnums.PowerMode_Tagpower:
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
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 15;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 30;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 30;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        if (unit.getBaseMaxRange() > 1)
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 1;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
            }
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (unit.getBaseMaxRange() === 1)
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
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
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect:\nNo effects") +
               qsTr("\nCO Zone Effect:\nUnits have 10% increased firepower and 10% Luck.");
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
