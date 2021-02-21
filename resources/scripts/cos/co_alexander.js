var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        var width = map.getMapWidth();
        var heigth = map.getMapHeight();
        var player = co.getOwner();
        for (var x = 0; x < width; x++)
        {
            for (var y = 0; y < heigth; y++)
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null &&
                        player.isEnemyUnit(unit))
                {
                    unit.setCapturePoints(0);
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        var player = co.getOwner();
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                var funds = enemyPlayer.getFunds();
                enemyPlayer.setFunds(funds / 2);
                player.addFunds(funds / 2);
            }
        }
    };

    this.startOfTurn = function(co)
    {
        var player = co.getOwner();
        var buildings = player.getBuildings();
        for (var i = 0; i < buildings.size(); i++)
        {
            var building = buildings.at(i);
            var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
            if (unit !== null)
            {
                var points = unit.getCapturePoints();
                // apply revolt bonus
                if (points > 0)
                {
                    points--;
                    unit.setCapturePoints(points);
                }
            }
        }
    };

    this.getIncomeReduction = function(co, building, income)
    {
        // set income to 0 during scop
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return income;
        }
        return 0;
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
                audio.addMusic("resources/music/cos/alexander.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };
    this.coZoneBonus = 15;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
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
                    return CO_ALEXANDER.coZoneBonus;
                }
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
            co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return CO_ALEXANDER.coZoneBonus;
        }
        return 0;
    };

    this.getCaptureBonus = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            var hp = unit.getHpRounded();
            return hp / 2;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A diligent politician who works only for the benefit of the citizens and the promotion of peace. Olaf's son and future ruler of Blue Moon.");
    };
    this.getHits = function(co)
    {
        return qsTr("Peace");
    };
    this.getMiss = function(co)
    {
        return qsTr("Cruelty");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Alexander captures revolt against capturing each day which increases the resistance 1.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nHis Buildings that get captured revolt and increase the resistance 1 by one each day.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain %0% firepower and defence.");
        text = replaceTextArgs(text, [CO_ALEXANDER.coZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Alexander's troops capture at a faster rate, while enemy captures are forced to restart.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Peace Riots");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Enemies receive no income, and half of their current funs are diverted to Alexander.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Inevitable Revolution");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Comrades, I beg of you to see from my eyes..."),
                qsTr("There is more to war than simply bullets and explosions."),
                qsTr("Express yourselves, my dear people, through actions rather than words!"),
                qsTr("Now we shall see where their sympathies really belong."),
                qsTr("Perhaps this will get you to negotiate instead..."),
                qsTr("You can only win a war if your people support you, and they prefer me!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Ironically, your shortcomings stemmed from within your own ranks."),
                qsTr("Ah, I see now! This is why the citizens were on my side all along."),
                qsTr("Only through unrivaled determination did I achieve success.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Hssss! Today was... unlucky. A bad day. Nothing more."),
                qsTr("It's the blasted weather! That was the problem! Hssss!")];
    };
    this.getName = function()
    {
        return qsTr("Alexander");
    };
}

Constructor.prototype = CO;
var CO_ALEXANDER = new Constructor();
