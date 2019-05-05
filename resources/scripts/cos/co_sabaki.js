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
                animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_SABAKI.loadCOMusic(co);
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
        CO_SABAKI.loadCOMusic(co);
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
                audio.addMusic("resources/music/cos/sabaki.mp3")
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
    
	this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        if (gotAttacked === false)
        {
            var healPercent = 0.0;
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Superpower:
                    healPercent = 1.0;
                    break;
                case GameEnums.PowerMode_Power:
                    healPercent = 0.5;
                    break;
                default:
                    if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                    {
                        healPercent = 0.2;
                    }
                    else
                    {
                        healPercent = 0.1;
                    }
                    break;

            }
            // damage can be negativ if we can't do a counter attack the damge is -1
            // avoid loosing hp cause of our passive or power
            if (atkDamage > 0)
            {
                attacker.setHp(attacker.getHp() + atkDamage * healPercent);
            }
        }
    };
    
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A former Yellow Comet CO that started a rebellion when she saw the state of Amber Corona. An expert at salvaging parts and field repairs.");
    };
    this.getHits = function()
    {
        return qsTr("Spare parts");
    };
    this.getMiss = function()
    {
        return qsTr("Poverty");
    };
    this.getCODescription = function()
    {
        return qsTr("Highly skilled at salvaging parts, Sabaki's units can drain HP from enemies.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Units can drain even more HP.");
    };
    this.getPowerName = function()
    {
        return qsTr("Speed Salvage");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Units heal HP equal to the amount of damage they inflict.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Rejuvination Raid");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Grab those parts! There's a tank that needs fixing!"),
				qsTr("Get those machines up and running! We need to push forward!"),
				qsTr("Every machine has a weakness, you just have to know where to look."),
				qsTr("You may think I'm nuts, but you're about to get screwed..."),
				qsTr("Time to throw a wrench in their plans."),
				qsTr("Let's see how you react when the hammer falls...")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("My army acts like a well-oiled machine."),
				qsTr("Nothing's more satisfying than killing an enemy with their own gun."),
				qsTr("Your army just fell apart out there.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("The revolution isn't over."),
				qsTr("Even by healing my troops. I couldn't win.")];
    };
    this.getName = function()
    {
        return qsTr("Sabaki");
    };
}

Constructor.prototype = CO;
var CO_SABAKI = new Constructor();
