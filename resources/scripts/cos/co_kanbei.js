var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3", "+alt4"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return baseCost * CO_KANBEI.d2dCostIncrease / 100;
            }
        }
        return 0;
    };

    this.d2dcoZoneBonus = 50;

    this.d2dBonus = 20;
    this.d2dCostIncrease = 20;

    this.powerOffBonus = 60;
    this.powerDefBonus = 60;

    this.superPowerOffBonus = 70;
    this.superPowerCounterBonus = 140;
    this.superPowerDefBonus = 70;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender)
                {
                    return CO_KANBEI.superPowerCounterBonus;
                }
                return CO_KANBEI.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_KANBEI.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_KANBEI.d2dcoZoneBonus;
                }
                break;
            }
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_KANBEI.d2dBonus;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_KANBEI.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                return CO_KANBEI.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_KANBEI.d2dcoZoneBonus;
                }
                break;
            }
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_KANBEI.d2dBonus;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
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
                return ["ZCOUNIT_ROYAL_GUARD"];
            }
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
        return qsTr("All of his units have high offensive and defensive capabilities, but are expensive to deploy.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_KANBEI.d2dBonus, CO_KANBEI.d2dCostIncrease];
        }
        var text = qsTr("\nSpecial Unit:\nRoyal Guard\n") +
                qsTr("\nGlobal Effect: \nKanbei's units gain +%0% firepower, +%0% defence, and have a +%1% increase in deployment costs.") +
                qsTr("\n\nCO Zone Effect: \nKanbei's units gain +%2% firepower and +%2% defence.");
        text = replaceTextArgs(text, [values[0], values[1], CO_KANBEI.d2dcoZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Kanbei's units gain +%0% firepower and +%1% defence.");
        text = replaceTextArgs(text, [CO_KANBEI.powerOffBonus, CO_KANBEI.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Morale Boost");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text =  qsTr("Kanbei's units gain +%0% firepower and +%1% defence. Their counterattacks gain a ridiculous +%2% firepower.");
        text = replaceTextArgs(text, [CO_KANBEI.superPowerOffBonus, CO_KANBEI.superPowerDefBonus, CO_KANBEI.superPowerCounterBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Samurai Spirit");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Who shall taste my wrath?"),
                qsTr("Observe the heart of a true samurai."),
                qsTr("Your numbers mean nothing! Nothing can stop me!"),
                qsTr("My mind is honed for battle. That is the essence of a samurai!"),
                qsTr("True samurai don't know the meaning of retreat."),
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
