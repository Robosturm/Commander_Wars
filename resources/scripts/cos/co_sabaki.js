var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
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
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

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
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/sabaki.mp3", 42287, 86073);
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
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            var healPercent = 0.0;
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    healPercent = 1.0;
                    break;
                case GameEnums.PowerMode_Power:
                    healPercent = 0.5;
                    break;
                default:
                    if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                    {
                        healPercent = 0.4;
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
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_CRYSTAL_TANK"];
        }
        return [];
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A former Yellow Comet CO that started a rebellion, when she saw the state of Amber Corona. An expert at salvaging parts and field repairs.");
    };
    this.getHits = function(co)
    {
        return qsTr("Spare parts");
    };
    this.getMiss = function(co)
    {
        return qsTr("Poverty");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Highly skilled at salvaging parts, Sabaki's units can drain HP from enemies.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nCrystal Tanks\n\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits have lifesteal.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Units can drain even more HP.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Speed Salvage");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Units heal HP equal amount of damage they inflict.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Rejuvination Raid");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Grab those parts! There's a tank that needs fixing!"),
				qsTr("Get those machines up and running! We need to push forward!"),
				qsTr("Every machine has a weakness, you just have to know where to look."),
				qsTr("You may think I'm nuts, but you're about to get screwed..."),
				qsTr("Time to throw a wrench in their plans."),
				qsTr("Let's see how you react when the hammer falls...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("My army acts like a well-oiled machine."),
				qsTr("Nothing's more satisfying than killing an enemy with their own gun."),
				qsTr("Your army just fell apart out there.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("The revolution isn't over."),
				qsTr("Even by healing my troops.. I couldn't win.")];
    };
    this.getName = function()
    {
        return qsTr("Sabaki");
    };
}

Constructor.prototype = CO;
var CO_SABAKI = new Constructor();
