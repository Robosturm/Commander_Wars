var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co, map)
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
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power0.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
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
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.ogg", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.ogg", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/adder.ogg", 2456, 62475);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    this.d2dPowerChargeBonus = 20;

    this.d2dCoZoneBonus = 20;

    this.powerOffBonus = 20;
    this.powerDefBonus = 20;
    this.powerMovement = 1;

    this.superPowerMovement = 2;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_ADDER.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_ADDER.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_ADDER.d2dCoZoneBonus;
                }
                break;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_ADDER.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_ADDER.d2dCoZoneBonus;
            }
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                    co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return CO_ADDER.superPowerMovement;
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                return CO_ADDER.powerMovement;
            }
        }
        return 0;
    };

    this.getPowerChargeBonus = function(co, map)
    {
        if (CO.isActive(co))
        {
            return CO_ADDER.d2dPowerChargeBonus;
        }
        return 0;
    };

    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                buildingId === "TOWN" ||
                BUILDING.isHq(building))
            {
                return ["ZCOUNIT_HOT_TANK"];
            }
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
        return qsTr("His own face");
    };
    this.getMiss = function(co)
    {
        return qsTr("Dirty things");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Adept at making quick command decisions. He stores up energy for his CO Power more rapidly than other CO's.");
    };
    this.getLongCODescription = function(co, map)
    {
        var text = qsTr("\nSpecial Unit:\nHot Tank\n") +
                   qsTr("\nGlobal Effect: \nAdder's power charge is increased by %1%.") +
                   qsTr("\n\nCO Zone Effect: \nAdder's units gain +%0% firepower and +%0% defence.");
        text = replaceTextArgs(text, [CO_ADDER.d2dCoZoneBonus, CO_ADDER.d2dPowerChargeBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All of Adder's units receive +%0 movement, +%1% firepower, and +%2% defence.");
        text = replaceTextArgs(text, [CO_ADDER.powerMovement, CO_ADDER.powerOffBonus, CO_ADDER.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sideslip");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All of Adder's units receive +%0 movement, +%1% firepower, and +%2% defence.");
        text = replaceTextArgs(text, [CO_ADDER.superPowerMovement, CO_ADDER.powerOffBonus, CO_ADDER.powerDefBonus]);
        return text;
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
                qsTr("Wiggle worm... wiggle!"),
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
				qsTr("So much for 'Adder time'."),
                qsTr("It's the blasted weather! That was the problem! Hssss!")];
    };
    this.getName = function()
    {
        return qsTr("Adder");
    };
}

Constructor.prototype = CO;
var CO_ADDER = new Constructor();