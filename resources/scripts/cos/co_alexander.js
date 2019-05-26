var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
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

        audio.clearPlayList();
        CO_ALEXANDER.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

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
                var fonds = enemyPlayer.getFonds();
                enemyPlayer.setFonds(fonds / 2);
                player.addFonds(fonds / 2);
            }
        }

        audio.clearPlayList();
        CO_ALEXANDER.loadCOMusic(co);
        audio.playRandom();
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
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
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
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/alexander.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 10;
                }
                break;
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

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A diligent politician who works only for the benefit of the citizens and the promotion of peace. Olaf's son and future ruler of Blue Moon.");
    };
    this.getHits = function()
    {
        return qsTr("Peace");
    };
    this.getMiss = function()
    {
        return qsTr("Cruelty");
    };
    this.getCODescription = function()
    {
        return qsTr("Alexander captures revolt against capturing each day which increases the resistance 1.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Alexander's troops capture at a faster rate, while enemy captures are forced to restart.");
    };
    this.getPowerName = function()
    {
        return qsTr("Peace Riots");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Enemies receive no income, and half of their current funs are diverted to Alexander.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Inevitable Revolution");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Comrades, I beg of you to see from my eyes..."),
                qsTr("There is more to war than simply bullets and explosions."),
                qsTr("Express yourselves, my dear people, through actions rather than words!"),
                qsTr("Now we shall see where their sympathies really belong."),
                qsTr("Perhaps this will get you to negotiate instead..."),
                qsTr("You can only win a war if your people support you, and they prefer me!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Ironically, your shortcomings stemmed from within your own ranks."),
                qsTr("Ah, I see now! This is why the citizens were on my side all along."),
                qsTr("Only through unrivaled determination did I acheive success.")];
    };
    this.getDefeatSentences = function()
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
