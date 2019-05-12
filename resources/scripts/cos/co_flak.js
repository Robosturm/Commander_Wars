var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
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
        CO_FLAK.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
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
        CO_FLAK.loadCOMusic(co);
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
                audio.addMusic("resources/music/cos/flak.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 90;
            case GameEnums.PowerMode_Power:
                return 55;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 25;
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
                return 40;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 10;
                }
                break;
        }
        return 5;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("The strongman of the Black Hole army. Promoted form private by Hawke, who was impressed by his natural ability.");
    };
    this.getHits = function()
    {
        return qsTr("Meat");
    };
    this.getMiss = function()
    {
        return qsTr("Vegetables");
    };
    this.getCODescription = function()
    {
        return qsTr("High firepower, but he relies solely on strength. His shoddy technique sometimes reduces the damage his units deal.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Firepower rises, but so does his chances of reduced firepower.");
    };
    this.getPowerName = function()
    {
        return qsTr("Brute Force");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Firepower rises dramatically, but so does his chances of reduced power.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Barbaric Blow");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Stay outta my way!"),
                qsTr("Grrrrrraaaaaaa! I'm outta control!"),
                qsTr("You can't hide from me! I'll hunt you down like a dog!"),
                qsTr("Nnnggrr! Braaaawrr! Smaarrrrgh!"),
                qsTr("Grr! Now you've really made me angry!"),
                qsTr("You're gonna get pummeled!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("What was that? A waste of my time!"),
                qsTr("Graaa! Blaarrgggg! Smaaarrgggghhh!!"),
                qsTr("Enough pussyfooting around!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("I-I don't believe it... They were... stronger than me?"),
                qsTr("Grrr... I wanna smash your face in!!!")];
    };
    this.getName = function()
    {
        return qsTr("Flak");
    };
}

Constructor.prototype = CO;
var CO_FLAK = new Constructor();
