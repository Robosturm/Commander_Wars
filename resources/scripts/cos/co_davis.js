var Constructor = function()
{
    this.getCOStyles = function()
    {
        return [];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
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
            if (i % 2 === 0)
            {
                animation.setSound("power7_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power7_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/davis.mp3")
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
        return "TI";
    };

    this.getAlliedUnitCount = function(owner, map)
    {
        var playerCounter = map.getPlayerCount();
        var unitCount = owner.getUnitCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var ownPlayer = map.getPlayer(i2);
            if ((ownPlayer !== owner) &&
                (owner.checkAlliance(ownPlayer) === GameEnums.Alliance_Friend))
            {
                unitCount += ownPlayer.getUnitCount();
            }
        }
        return unitCount;
    }

    this.getEnemyUnitCount = function(owner, map)
    {
        var playerCounter = map.getPlayerCount();
        var enemyCount = 0
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var ownPlayer = map.getPlayer(i2);
            if ((ownPlayer !== owner) &&
                (owner.checkAlliance(ownPlayer) === GameEnums.Alliance_Enemy))
            {
                var unitCount = CO_DAVIS.getAlliedUnitCount(ownPlayer, map);
                if (unitCount > enemyCount)
                {
                    enemyCount = unitCount;
                }
            }
        }
        return enemyCount;
    }

    this.superPowerMaxLuck = 150;
    this.superPowerLuckPerUnit = 6;
    this.superPowerMaxMissFortune = 100;
    this.superPowerMissFortunePerUnit = 4;

    this.powerMaxLuck = 100;
    this.powerLuckPerUnit = 4;
    this.powerMaxMissFortune = 60;
    this.powerMissFortunePerUnit = 3;
    this.powerDefBonus = 10;
    this.powerOffBonus = 10;

    this.d2dMaxLuck = 0;
    this.d2dLuckPerUnit = 0;
    this.d2dMaxMissFortune = 0;
    this.d2dMissFortunePerUnit = 0;

    this.d2dCoZoneMaxLuck = 50;
    this.d2dCoZoneLuckPerUnit = 2;
    this.d2dCoZoneMaxMissFortune = 40;
    this.d2dCoZoneMissFortunePerUnit = 2;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneOffBonus = 10;

    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var owner = co.getOwner();
            var unitCount = CO_DAVIS.getAlliedUnitCount(owner, map);
            var enemyCount = CO_DAVIS.getEnemyUnitCount(owner, map);
            var luck = 0;
            if (unitCount > enemyCount)
            {
                var maxLuck = 0;
                var luckPerUnit = 0;
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    maxLuck = CO_DAVIS.superPowerMaxLuck;
                    luckPerUnit = CO_DAVIS.superPowerLuckPerUnit;
                    break;
                case GameEnums.PowerMode_Power:
                    maxLuck = CO_DAVIS.powerMaxLuck;
                    luckPerUnit = CO_DAVIS.powerLuckPerUnit;
                    break;
                default:
                    if (co.inCORange(Qt.point(posX, posY), unit))
                    {
                        maxLuck = CO_DAVIS.d2dCoZoneMaxLuck;
                        luckPerUnit = CO_DAVIS.d2dCoZoneLuckPerUnit;
                    }
                    else
                    {
                        maxLuck = CO_DAVIS.d2dMaxLuck;
                        luckPerUnit = CO_DAVIS.d2dLuckPerUnit;
                    }
                    break;
                }
                luck = luckPerUnit * (unitCount - enemyCount);
                if (luck > maxLuck)
                {
                    luck = maxLuck;
                }
            }
            return luck;
        }
        return 0;
    };
	
    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var owner = co.getOwner();
            var unitCount = CO_DAVIS.getAlliedUnitCount(owner, map);
            var enemyCount = CO_DAVIS.getEnemyUnitCount(owner, map);
            var missFortune = 0;
            if (enemyCount > unitCount)
            {
                var maxMissFortune = 0;
                var luckMissFortune = 0;
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    maxMissFortune = CO_DAVIS.superPowerMaxMissFortune;
                    luckMissFortune = CO_DAVIS.superPowerMissFortunePerUnit;
                    break;
                case GameEnums.PowerMode_Power:
                    maxMissFortune = CO_DAVIS.powerMaxMissFortune;
                    luckMissFortune = CO_DAVIS.powerMissFortunePerUnit;
                    break;
                default:
                    if (co.inCORange(Qt.point(posX, posY), unit))
                    {
                        maxMissFortune = CO_DAVIS.d2dCoZoneMaxMissFortune;
                        luckMissFortune = CO_DAVIS.d2dCoZoneMissFortunePerUnit;
                    }
                    else
                    {
                        maxMissFortune = CO_DAVIS.d2dMaxMissFortune;
                        luckMissFortune = CO_DAVIS.d2dMissFortunePerUnit;
                    }
                    break;
                }
                missFortune = luckMissFortune * (enemyCount - unitCount);
                if (missFortune > maxMissFortune)
                {
                    missFortune = maxMissFortune;
                }
            }
            return missFortune;
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {        
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_DAVIS.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_DAVIS.d2dCoZoneOffBonus;
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
                return CO_DAVIS.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_DAVIS.d2dCoZoneDefBonus;
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
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("He is a coward who always sides with the winning side.");
    };
    this.getHits = function(co)
    {
        return qsTr("Side with the winning side");
    };
    this.getMiss = function(co)
    {
        return qsTr("Side with the losing side");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Gains luck and missfortune based on the amount of troops on the field.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nNone\n") +
               qsTr("\nGlobal Effect: \nIf Davis and his allies controls more units than his enemies. He gains %0% luck per unit up to a maximum of %1%. Else he gains %2% missfortune per unit up to a maximum of %3%.") +
               qsTr("\n\nCO Zone Effect: \nIf Davis and his allies controls more units than his enemies. He gains %4% luck per unit up to a maximum of %5%. Else he gains %6% missfortune per unit up to a maximum of %7%.");
        text = replaceTextArgs(text, [CO_DAVIS.d2dLuckPerUnit, CO_DAVIS.d2dMaxLuck, CO_DAVIS.d2dMissFortunePerUnit, CO_DAVIS.d2dMaxMissFortune,
                                      CO_DAVIS.d2dCoZoneLuckPerUnit, CO_DAVIS.d2dCoZoneMaxLuck, CO_DAVIS.d2dCoZoneMissFortunePerUnit, CO_DAVIS.d2dCoZoneMaxMissFortune]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("If Davis and his allies controls more units than his enemies. He gains %0% luck per unit up to a maximum of %1%. Else he gains %2% missfortune per unit up to a maximum of %3%.");
        text = replaceTextArgs(text, [CO_DAVIS.powerLuckPerUnit, CO_DAVIS.powerMaxLuck, CO_DAVIS.powerMissFortunePerUnit, CO_DAVIS.powerMaxMissFortune]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Survival Plans");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("If Davis and his allies controls more units than his enemies. He gains %0% luck per unit up to a maximum of %1%. Else he gains %2% missfortune per unit up to a maximum of %3%.");
        text = replaceTextArgs(text, [CO_DAVIS.superPowerLuckPerUnit, CO_DAVIS.superPowerMaxLuck, CO_DAVIS.superPowerMissFortunePerUnit, CO_DAVIS.superPowerMaxMissFortune]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("The stronger Side");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("We're going to win now!"),
                qsTr("I-I will win this, I have to!"),
                qsTr("I'll survive this war!"),];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That's why i sided with them!"),
                qsTr("The stronger side always wins!!"),
                qsTr("I joined the correct side!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I-I don't believe it... They were... stronger than me?"),
                qsTr("I-I'll join you and do whatever you want!!!")];
    };
    this.getName = function()
    {
        return qsTr("Davis");
    };
}

Constructor.prototype = CO;
var CO_DAVIS = new Constructor();
