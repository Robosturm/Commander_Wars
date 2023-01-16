var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
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
                audio.addMusic("resources/music/cos/davis.mp3")
                break;
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

    this.getBonusLuck = function(co, unit, posX, posY, map)
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
                maxLuck = 150;
                luckPerUnit = 6;
                break;
            case GameEnums.PowerMode_Power:
                luckPerUnit = 4;
                maxLuck = 100;
                break;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    luckPerUnit = 2;
                    maxLuck = 50
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
    };
	
    this.getBonusMisfortune = function(co, unit, posX, posY, map)
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
                maxMissFortune = 100;
                luckMissFortune = 4;
                break;
            case GameEnums.PowerMode_Power:
                maxMissFortune = 60;
                luckMissFortune = 3;
                break;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    maxMissFortune = 40;
                    luckMissFortune = 2;
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
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
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
        return qsTr("Gains luck and mussfortune based on the amount of troops on the field.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nNone\n") +
               qsTr("\nGlobal Effect: \nNone") +
               qsTr("\n\nCO Zone Effect: \nUnits gain bonus luck based on the amount of troops Davis controls and gain missfortune based on the amount of troops all enemies control. The bonus/penalty depends of which unit count is higher.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("His luck is increased  based on the amount of troops Davis controls. However he also gains increased missfortune based  on the amount of troops all enemies control. The bonus/penalty depends of which unit count is higher.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Survival Plans");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("His luck raises even more based on the amount of troops Davis controls. However the cap for missfortune raises as well. The bonus/penalty depends of which unit count is higher.");
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
