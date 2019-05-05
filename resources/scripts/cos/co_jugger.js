var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/jugger.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

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
        CO_JUGGER.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

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
        CO_JUGGER.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 95;
            case GameEnums.PowerMode_Power:
                return 55;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 30;
                }
                break;
        }
        return 15;
    };
	
    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 45;
            case GameEnums.PowerMode_Power:
                return 25;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 15;
                }
                break;
        }
        return 7;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A robot-like commander with the Bolt Guard army. No one knows his true identity!");
    };
    this.getHits = function()
    {
        return qsTr("Energy");
    };
    this.getMiss = function()
    {
        return qsTr("Static electricity");
    };
    this.getCODescription = function()
    {
        return qsTr("Units may suddenly deal more damage than expected, but units' firepower is inherently low.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("There is a chance he might get a stronger blow, but base firepower is slightly reduced.");
    };
    this.getPowerName = function()
    {
        return qsTr("Overclock");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("There is a chance he might get a devestating blow, but firepower is reduced.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("System Crash");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Enemy: Prepare for mega hurtz."),
                qsTr("Memory: upgraded. Shell: shined. Ready to uh...roll."),
                qsTr("Enemy system purge initiated..."),
                qsTr("Blue screen of death!"),
                qsTr("Crushware loaded..."),
                qsTr("Approaching system meltdown.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Victory; downloading party hat."),
                qsTr("Victory dance initiated."),
                qsTr("Jugger; superior. Enemy; lame.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Critical Error: Does not compute."),
                qsTr("Victory impossible! Units overwhelmed. Jugger must... Control-Alt-Delete.")];
    };
    this.getName = function()
    {
        return qsTr("Jugger");
    };
}

Constructor.prototype = CO;
var CO_JUGGER = new Constructor();
