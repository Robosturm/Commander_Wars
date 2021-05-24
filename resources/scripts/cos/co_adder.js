var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(3);
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
            animation.setSound("power0.wav");
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
            if (globals.randInt(0, 1) === 0)
            {
                animation.setSound("power12_1.wav");
            }
            else
            {
                animation.setSound("power12_2.wav");
            }
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
            audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
            break;
        case GameEnums.PowerMode_Superpower:
            audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
            break;
        case GameEnums.PowerMode_Tagpower:
            audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
            break;
        default:
            audio.addMusic("resources/music/cos/adder.mp3", 2456, 62475);
            break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    this.coZoneBonus = 20;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 10;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_ADDER.coZoneBonus;
            }
            break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return CO_ADDER.coZoneBonus;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
        return 0;
    };

    this.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
    {
        // adder gains more power than other co's
        var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
        co.setPowerFilled(co.getPowerFilled() + powerGain * 1.2);
    };
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_HOT_TANK"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A self-absorbed CO who believes his skills are matchless. Second to Hawke in rank.");
    };
    this.getHits = function(co)
    {
        return qsTr("His own Face");
    };
    this.getMiss = function(co)
    {
        return qsTr("Dirty Things");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Adept at making quick command decisions. He stores up energy for his CO Power more rapidly than other CO's.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nHot Tank\n") +
                   qsTr("\nGlobal Effect: \nNo bonus.") +
                   qsTr("\n\nCO Zone Effect: \nUnits gain %0% firepower and defence.");
        text = replaceTextArgs(text, [CO_ADDER.coZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Movement range for all units is increased by one space.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sideslip");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Movement range for all units is increased by two spaces.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Sidewinder");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("The look of terror on your face... It's absolutely delicious!"),
                qsTr("Heh heh heh... I'm going to enjoy breaking you!"),
                qsTr("Heh heh heh... Can't a guy have a little fun!?"),
                qsTr("Heh heh heh... It only hurts for a moment."),
                qsTr("A battle with me is a greater honor than you deserve!"),
                qsTr("Heh heh heh... Kneel before Adder!"),
                qsTr("Hee hee. Your skills are laughable!"),
                qsTr("It's already too late for you!"),
                qsTr("Let me hear your last words!"),
                qsTr("Check out my new and improved powers!"),
                qsTr("Wriggle worm... wriggle!"),
                qsTr("My venom courses through your veins!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Heh heh heh... What did you expect?"),
                qsTr("My apologies. Should I have gone easier on you?"),
                qsTr("Don't forget me now. That would be a shame.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Hssss! Today was... unlucky. A bad day. Nothing more."),
                qsTr("It's the blasted weather! That was the problem! Hssss!")];
    };
    this.getName = function()
    {
        return qsTr("Adder");
    };
}

Constructor.prototype = CO;
var CO_ADDER = new Constructor();
