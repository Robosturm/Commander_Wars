var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2", "+alt3", "+alt4"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
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
            audio.addMusic("resources/music/cos/kanbei.mp3", 4783, 60483);
            break;
        }
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
            if (i % 2 === 0)
            {
                animation.setSound("power2_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power2_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        units.remove();
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };
    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        return baseCost * CO_KANBEI.costIncrease / 100;
    };
    this.coZoneBonus = 50;
    this.globalBonus = 15;
    this.costIncrease = 20;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender)
            {
                return 140;
            }
            return 70;
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_KANBEI.coZoneBonus;
            }
            break;
        }
        return CO_KANBEI.globalBonus;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 70;
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_KANBEI.coZoneBonus;
            }
            break;
        }
        return CO_KANBEI.globalBonus;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
                buildingId === "TOWN" ||
                buildingId === "HQ")
        {
            return ["ZCOUNIT_ROYAL_GUARD"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The emperor of Yellow Comet. A skilled commander who has a soft spot for his daughter.");
    };
    this.getHits = function(co)
    {
        return qsTr("Sonja");
    };
    this.getMiss = function(co)
    {
        return qsTr("Computers");
    };
    this.getCODescription = function(co)
    {
        return qsTr("All units have high offensive and defensive capabilities, but are expensive to deploy.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nRoyal Guard\n") +
                qsTr("\nGlobal Effect: \nUnits have %0% stronger firepower and defense, but are %1% more expensive.") +
                qsTr("\n\nCO Zone Effect: \nUnits have %2% stronger firepower and defense.");
        text = replaceTextArgs(text, [CO_KANBEI.globalBonus, CO_KANBEI.costIncrease , CO_KANBEI.coZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Increases firepower and defence of all units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Morale Boost");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Greatly strengthens offensive and defensive abilities of all units. Counterattacks are stronger.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Samurai Spirit");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Who shall taste my wrath?"),
                qsTr("Observe the heart of a true Samurai."),
                qsTr("Your numbers mean nothing!  Nothing can stop me!"),
                qsTr("My mind is honed for battle.  That is the essence of a samurai!"),
                qsTr("True Samurai don't know the meaning of retreat."),
                qsTr("I hope you thoroughly enjoy the taste of my sword!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Kanbei is victorious! We shall meet again!"),
                qsTr("Raise your voices in victory!"),
                qsTr("No enemy can stand before Kanbei!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Preposterous! So many of Kanbei's forces defeated in such a short time? Withdraw!"),
                qsTr("Regardless of the odds, Kanbei should never lose this many units! Withdraw!")];
    };
    this.getName = function()
    {
        return qsTr("Kanbei");
    };
}

Constructor.prototype = CO;
var CO_KANBEI = new Constructor();
