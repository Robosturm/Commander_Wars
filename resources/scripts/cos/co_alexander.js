var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
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

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
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

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
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
                audio.addMusic("resources/music/cos/alexander.mp3")
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
        return "BM";
    };

    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerCaptureBonus = 50;
    this.d2dRevoltPoints = 1;
    this.d2dCoZoneBonus = 15;

    this.startOfTurn = function(co, map)
    {
        if (CO.isActive(co))
        {
            var player = co.getOwner();
            if (!player.getIsDefeated())
            {
                var buildings = player.getBuildings();
                var size = buildings.size();
                for (var i = 0; i < size; i++)
                {
                    var building = buildings.at(i);
                    var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
                    if (unit !== null)
                    {
                        var points = unit.getCapturePoints();
                        // apply revolt bonus
                        if (points > 0)
                        {
                            for (var i2 = 0; i2 < CO_ALEXANDER.d2dRevoltPoints; ++i2)
                            {
                                points--;
                            }
                            if (points < 0)
                            {
                                points = 0;
                            }

                            unit.setCapturePoints(points);
                        }
                    }
                }
            }
        }
    };

    this.getIncomeReduction = function(co, building, income, map)
    {
        if (CO.isActive(co))
        {
            // set income to 0 during scop
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return income;
            }
        }
        return 0;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_ALEXANDER.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_ALEXANDER.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_ALEXANDER.d2dCoZoneBonus;
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
                return CO_ALEXANDER.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_ALEXANDER.d2dCoZoneBonus;
            }
        }
        return 0;
    };

    this.getCaptureBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                var hp = unit.getHpRounded();
                return hp * CO_ALEXANDER.powerCaptureBonus / 100;
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
                return ["ZCOUNIT_PARTISAN"];
            }
        }
        return [];
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
        return qsTr("Alexander is loved by his citizens so much that they have a tendency to resist being captured.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nPartisan\n") +
                   qsTr("\nGlobal Effect: \nBuildings that Alexander owns put up a fight when being captured by enemy soldiers. Each building recovers %0 capture point each day while under siege.") +
                   qsTr("\n\nCO Zone Effect: \nUnits gain %1% firepower and defence.");
        text = replaceTextArgs(text, [CO_ALEXANDER.d2dRevoltPoints, CO_ALEXANDER.d2dCoZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Alexander's charisma and brilliant speech wins over the people. Alexander's troops capture at a %0% faster rate, while enemy captures are forced to restart entirely. They also gain %1% firepower and %2% defence.");
        text = replaceTextArgs(text, [CO_ALEXANDER.powerCaptureBonus, CO_ALEXANDER.powerOffBonus, CO_ALEXANDER.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Peace Riots");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Enemies receive no income for this turn, and half of their current funds are diverted to Alexander. They also gain %0% firepower and %1% defence.");
        text = replaceTextArgs(text, [CO_ALEXANDER.powerOffBonus, CO_ALEXANDER.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Inevitable Revolution");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Comrades! I beg of you to come to your senses!"),
                qsTr("There is more to war than bullets and explosions."),
                qsTr("The people will rally under my cause!"),
                qsTr("Now we shall see where their loyalties truly belong."),
                qsTr("Perhaps this will get you to consider a peaceful resolution!"),
                qsTr("You can only win a war if your people support you, allow me to demonstrate!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Your shortcomings stemmed from within your own ranks."),
                qsTr("The citizens were on my side all along."),
                qsTr("Only through unrivaled support did I achieve success.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I failed my people, but I will free them soon."),
                qsTr("You have the support of your people....for now.")];
    };
    this.getName = function()
    {
        return qsTr("Alexander");
    };
}

Constructor.prototype = CO;
var CO_ALEXANDER = new Constructor();
