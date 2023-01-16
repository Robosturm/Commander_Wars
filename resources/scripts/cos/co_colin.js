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
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation = GameAnimationFactory.createAnimation(map, 0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("power_colin.wav");
        powerNameAnimation.queueAnimation(animation);

        var player = co.getOwner();
        var funds = player.getFunds();
        if (funds > 0)
        {
            player.addFunds(player.getFunds() * 0.5);
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
            audio.addMusic("resources/music/cos/colin.mp3", 6900, 75513)
            break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };
    this.costModifier = 20;
    this.coZoneBoost = 5;
    this.globalBoost = 15;
    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        return -baseCost * CO_COLIN.costModifier / 100;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var bonus = attacker.getOwner().getFunds() / 1000 * 3.33 - 5;
            return bonus;
        case GameEnums.PowerMode_Power:
            return -5;
        default:
            break;
        }
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return -CO_COLIN.coZoneBoost;
        }
        return -CO_COLIN.globalBoost;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
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
            return ["ZCOUNIT_LOGIC_TRUCK"];
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Blue Moon's rich boy CO and Sasha's little brother. A gifted CO with a sharp, if insecure, mind.");
    };
    this.getHits = function(co)
    {
        return qsTr("Olaf and Grit");
    };
    this.getMiss = function(co)
    {
        return qsTr("Black Hole");
    };
    this.getCODescription = function(co)
    {
        return qsTr("The heir to a vast fortune who can purchase units at bargain-basement prices. Troops are low firepower stems from his lack of confidence.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nLogistic Truck\n") +
                qsTr("\nGlobal Effect: \nUnits are %0% cheaper and have %1% less firepower.") +
                qsTr("\n\nCO Zone Effect: \nUnits have only %2% weaker firepower.");
        text = replaceTextArgs(text, [CO_COLIN.costModifier, CO_COLIN.globalBoost, CO_COLIN.coZoneBoost]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Increases current funds by 50 percent.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Gold Rush");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Uses wealth to increase the strength of units. The more funds available, the more firepower his units receive.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Power of Money");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Let me show you the power of money!"),
                qsTr("This is not a drill... I won't give up so easily!"),
                qsTr("People of Blue Moon!  I need your help!"),
                qsTr("I'll give it everything I've got!"),
                qsTr("You're not getting away with this!"),
                qsTr("I'll show you what I can do!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I w-won! Whew!"),
                qsTr("I'll win if I try my best!"),
                qsTr("Whew... I won! I really won!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("If only Commander Olaf were here..."),
                qsTr("Wait till I tell my sis! Then you'll be sorry!")];
    };
    this.getName = function()
    {
        return qsTr("Colin");
    };
    this.getAiUsePower = function(co, powerSurplus, turnMode)
    {
        // cop spam
        if (co.canUseSuperpower())
        {
            return GameEnums.PowerMode_Superpower;
        }
        else if (co.canUsePower())
        {
            return GameEnums.PowerMode_Power;
        }
        return GameEnums.PowerMode_Off;
    };
}

Constructor.prototype = CO;
var CO_COLIN = new Constructor();
