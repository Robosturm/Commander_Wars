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
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/sasha.mp3", 15916, 105733);
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation2.setSound("power_sasha.wav");
        powerNameAnimation.queueAnimation(animation2);

        var player = co.getOwner();
        // i prefer this version you could change it to 10% per funds easily
        var reduction = co.getOwner().getFunds() / 5000.0;
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                var enemyCo = enemyPlayer.getCO(0);
                if (enemyCo !== null)
                {
                    enemyCo.setPowerFilled(enemyCo.getPowerFilled() - reduction);
                }
                enemyCo = enemyPlayer.getCO(1);
                if (enemyCo !== null)
                {
                    enemyCo.setPowerFilled(enemyCo.getPowerFilled() - reduction);
                }
            }
        }
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

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };

    this.getBonusIncome = function(co, building, income)
    {
        return income * 0.1;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 20;
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
    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    // damage can be negativ if we can't do a counter attack the damge is -1
                    // avoid loosing money cause of our super power
                    if (atkDamage > 0)
                    {
                        co.getOwner().addFunds(atkDamage / 10.0 * defender.getUnitCosts() * 0.5);
                    }
                    break;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
            }
        }
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Colin's older sister. Normally ladylike, but becomes daring when she gets angry. Dominatrix");
    };
    this.getHits = function(co)
    {
        return qsTr("Truffles");
    };
    this.getMiss = function(co)
    {
        return qsTr("Pork rinds");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Being the heir to a vast fortune, she gets an additional 10% funds from allied properties.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nShe gets additional funds from allied properties.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain additional firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("The more funds she has, the more she can decrease the enemy's CO power gauge.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Market Crash");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Earns funds when she inflicts damage on a foe. The greater the damage, the more she earns.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("War Bonds");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Wealth is power."),
                qsTr("I won't go easy on you."),
                qsTr("I'll keep fighting until peace is restored."),
                qsTr("I will not back down."),
                qsTr("I'll show you my true strength."),
                qsTr("You've made me very, very...upset.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Even kittens have claws."),
                qsTr("Money is power."),
                qsTr("Bravo!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is like some horrible dream."),
                qsTr("This is such a nightmare.")];
    };
    this.getName = function()
    {
        return qsTr("Sasha");
    };
}

Constructor.prototype = CO;
var CO_SASHA = new Constructor();
