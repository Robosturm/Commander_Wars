var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(2);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(3);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");

            if (animations.length < 5)
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_BRENNER.loadCOMusic(co);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(5);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");

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
        CO_BRENNER.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/brenner.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 50;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY)))
                {
                    return 20;
                }
                break;
        }
        return 10;
    };
	
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("He has faith in the goodness of humanity. Feels strong dutx to help anyone who is in trouble.");
    };
    this.getHits = function()
    {
        return qsTr("Helping Others");
    };
    this.getMiss = function()
    {
        return qsTr("Dishonor");
    };
    this.getCODescription = function()
    {
        return qsTr("His units have a higher defence than normal units.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Heals 3 hp of his units and increases his defence.");
    };
    this.getPowerName = function()
    {
        return qsTr("Reinforce");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Heals 5 hp of his units and increases his defence greatly.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("First Aid");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I want everyone to get out here alive."),
                qsTr("Prepare the troops. It's time to go on the offensive."),
                qsTr("Don't give up. We will survive that."),
                qsTr("You battled brave out there. It's time to get out there.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("The world ends - and yet war goes on..."),
                qsTr("The priority is attending to the casualties...")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Where there's life, there's hope."),
                qsTr("At least my troops got out...")];
    };
    this.getName = function()
    {
        return qsTr("Brenner");
    };
}

Constructor.prototype = CO;
var CO_BRENNER = new Constructor();
