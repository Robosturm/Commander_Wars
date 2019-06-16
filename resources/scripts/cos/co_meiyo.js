var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co)
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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

        audio.clearPlayList();
        CO_MEIYO.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(1);
            animation.setEndOfAnimationCall("CO_MEIYO", "postAnimationRanking");

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

        audio.clearPlayList();
        CO_MEIYO.loadCOMusic(co);
        audio.playRandom();
    };

    this.postAnimationRanking = function(postAnimation)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var upgrade = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                var currentRank = unit.getUnitRank();
                if (currentRank < GameEnums.UnitRank_Veteran)
                {
                    var rank = currentRank + upgrade;
                    if (rank > GameEnums.UnitRank_Veteran)
                    {
                        rank = GameEnums.UnitRank_Veteran;
                    }
                    unit.setUnitRank(rank);
                }
            }
        }
    }

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
                audio.addMusic("resources/music/cos/meiyo.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "GS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                switch (attacker.getUnitRank())
                {
                    case GameEnums.UnitRank_Lieutenant:
                        return 20;
                    case GameEnums.UnitRank_General:
                        return 30;
                    case GameEnums.UnitRank_Veteran:
                    case GameEnums.UnitRank_CO0:
                    case GameEnums.UnitRank_CO1:
                        return 50;
                    default:
                        return 10;
                }
            case GameEnums.PowerMode_Power:
                switch (attacker.getUnitRank())
                {
                    case GameEnums.UnitRank_Lieutenant:
                        return 20;
                    case GameEnums.UnitRank_General:
                        return 30;
                    case GameEnums.UnitRank_Veteran:
                    case GameEnums.UnitRank_CO0:
                    case GameEnums.UnitRank_CO1:
                        return 50;
                    default:
                        return 10;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    switch (attacker.getUnitRank())
                    {
                        case GameEnums.UnitRank_Lieutenant:
                            return 15;
                        case GameEnums.UnitRank_General:
                            return 20;
                        case GameEnums.UnitRank_Veteran:
                        case GameEnums.UnitRank_CO0:
                        case GameEnums.UnitRank_CO1:
                            return 30;
                        default:
                            return 10;
                    }
                }
                else
                {
                    switch (attacker.getUnitRank())
                    {
                        case GameEnums.UnitRank_Lieutenant:
                            return 2.5;
                        case GameEnums.UnitRank_General:
                            return 5;
                        case GameEnums.UnitRank_Veteran:
                        case GameEnums.UnitRank_CO0:
                        case GameEnums.UnitRank_CO1:
                            return 10;
                        default:
                            return -5;
                    }
                }
        }
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                switch (defender.getUnitRank())
                {
                    case GameEnums.UnitRank_Lieutenant:
                        return 20;
                    case GameEnums.UnitRank_General:
                        return 30;
                    case GameEnums.UnitRank_Veteran:
                    case GameEnums.UnitRank_CO0:
                    case GameEnums.UnitRank_CO1:
                        return 50;
                    default:
                        return 10;
                }
            case GameEnums.PowerMode_Power:
                switch (defender.getUnitRank())
                {
                    case GameEnums.UnitRank_Lieutenant:
                        return 20;
                    case GameEnums.UnitRank_General:
                        return 30;
                    case GameEnums.UnitRank_Veteran:
                    case GameEnums.UnitRank_CO0:
                    case GameEnums.UnitRank_CO1:
                        return 50;
                    default:
                        return 10;
                }
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    switch (defender.getUnitRank())
                    {
                        case GameEnums.UnitRank_Lieutenant:
                            return 15;
                        case GameEnums.UnitRank_General:
                            return 20;
                        case GameEnums.UnitRank_Veteran:
                        case GameEnums.UnitRank_CO0:
                        case GameEnums.UnitRank_CO1:
                            return 30;
                        default:
                            return 10;
                    }
                }
                else
                {
                    switch (defender.getUnitRank())
                    {
                        case GameEnums.UnitRank_Lieutenant:
                            return 2.5;
                        case GameEnums.UnitRank_General:
                            return 5;
                        case GameEnums.UnitRank_Veteran:
                        case GameEnums.UnitRank_CO0:
                        case GameEnums.UnitRank_CO1:
                            return 10;
                        default:
                            return -5;
                    }
                }
        }
    };
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("The Supreme Commander of Golden Sun. A veteran of a lot of battles who relies on experienced troops.");
    };
    this.getHits = function()
    {
        return qsTr("Veterans");
    };
    this.getMiss = function()
    {
        return qsTr("Recruits");
    };
    this.getCODescription = function()
    {
        return qsTr("Firepower rises the higher a rank of a unit is. However unexperienced units are weaker.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Units with a higher rank have even higher defence and offense.");
    };
    this.getPowerName = function()
    {
        return qsTr("Training Camp");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Units with a higher rank have even higher defence and offense. All units gain one rank.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("War Training");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Get those recruits out of my way!"),
                qsTr("An army of veterans awaits you..."),
                qsTr("I know how to train soldiers!"),
                qsTr("An old soldier without wounds is the most dangerous.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("A knight needs to be strong and experienced!"),
                qsTr("The better prepared knight wins."),
                qsTr("Experience always wins.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("You getting better recuit."),
                qsTr("Tsss! Some recruits have one! A catastroph.")];
    };
    this.getName = function()
    {
        return qsTr("Meiyo");
    };
}

Constructor.prototype = CO;
var CO_MEIYO = new Constructor();
