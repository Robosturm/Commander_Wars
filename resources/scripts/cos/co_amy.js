var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(5);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
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
                audio.addMusic("resources/music/cos/amy.mp3", 3444, 604297)
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };

    this.superPowerDeffensiveBonus = 110;
    this.powerHoverCraftBoost = 80;
    this.powerCostReduction = 0.3;
    this.powerMovementBonus = 2;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.d2dCoZoneHoverCraftBoost = 80;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dHoverCraftBoost = 0;
    this.d2dMovementBonus = 1;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
                {
                    return CO_AMY.powerHoverCraftBoost;
                }
                return CO_AMY.powerOffBonus;
            default:                
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
                    {
                        return CO_AMY.d2dCoZoneHoverCraftBoost;
                    }
                    return CO_AMY.d2dCoZoneOffBonus;
                }
                else if (CO.getGlobalZone(co, map))
                {
                    if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
                    {
                        return CO_AMY.d2dHoverCraftBoost;
                    }
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
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_AMY.superPowerDeffensiveBonus;
            case GameEnums.PowerMode_Power:
                return CO_AMY.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_AMY.d2dCoZoneDefBonus;
                }
                break;
            }
        }
        return 0;
    };
    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender === true)
                {
                    return damage;
                }
            }
        }
        return 0;
    }

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getMovementType() === "MOVE_HOVERCRAFT")
            {
                if (co.getPowerMode() === GameEnums.PowerMode_Power)
                {

                    return CO_AMY.powerMovementBonus;
                }
                else
                {
                    return CO_AMY.d2dMovementBonus;
                }
            }
        }
        return 0;
    };    
    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                if (Global[id].getMovementType() === "MOVE_HOVERCRAFT")
                {
                    return -baseCost * CO_AMY.powerCostReduction;
                }
            }
        }
        return 0;

    };

    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (map.getTerrain(posX, posY).getTerrainID() === "REAF")
            {
                return -999;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getMovementType() === "MOVE_HOVERCRAFT")
        {
            return 3;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The Commander-in-Chief of Pink Frontier. A former physicist who was thrust into war. She is now a hardened naval commander.");
    };
    this.getHits = function(co)
    {
        return qsTr("Quantum Theory");
    };
    this.getMiss = function(co)
    {
        return qsTr("Hawke");
    };
    this.getCODescription = function(co)
    {
        return qsTr("She can command hovercraft units at optimal level. Reefs pose no problem for Amy's units.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nReef movement costs are equal to 1 for all of Amy's units and Hovercrafts have %1 extra movement and gain %2% firepower.") +
                   qsTr("\n\nCO Zone Effect: \nHovercrafts gain %3% firepower.");
        text = replaceTextArgs(text, [CO_AMY.d2dMovementBonus, CO_AMY.d2dHoverCraftBoost, CO_AMY.hoverCraftBoost]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All hovercraft units gain %0% firepower along with one extra movement and %1% defence. Hovercrafts also receive a %2% reduction in deployment costs. All other units gain %1% firepower and %3% defence");
        text = replaceTextArgs(text, [CO_AMY.powerHoverCraftBoost, CO_AMY.powerDefBonus, CO_AMY.powerCostReduction * 100, CO_AMY.powerOffBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Fluid Dynamics");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All hovercraft units gain %0% firepower. Her defense rises to an astonishing %1%. Additionally, she takes no counter-attack damage when attacking an enemy. All other units gain %2% firepower");
        text = replaceTextArgs(text, [CO_AMY.powerHoverCraftBoost, CO_AMY.superPowerDeffensiveBonus, CO_AMY.powerOffBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Photon Discharge");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Try all you might, my magnetic shields will prove to be impenetrable!"),
                qsTr("Quantum physics... allow me to show you its true potential."),
                qsTr("I will show you exactly what splitting an atom feels like."),
                qsTr("Allow me to demonstrate the power of my hovercraft!"),
                qsTr("Perhaps if you valued science more, you would not be in this predicament!"),
                qsTr("Science has gotten me this far, let's see just how far it will take me.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That one was for you, Lara."),
                qsTr("Theoretically speaking, you never stood a chance."),
                qsTr("For lack of a better word, you were fully atomized!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("How can this be?! All of my calculations were flawless!"),
                qsTr("It seems I need to do more experimentations.")];
    };
    this.getName = function()
    {
        return qsTr("Amy");
    };
}

Constructor.prototype = CO;
var CO_AMY = new Constructor();
