var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(7);
        co.setSuperpowerStars(5);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(5);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");

            if (animations.length < 5)
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_CAULDER.loadCOMusic(co);
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
            animation.writeDataInt32(10);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
            if (animations.length < 5)
            {
                animation.addSprite("power11", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power11", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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
        CO_CAULDER.loadCOMusic(co);
        audio.playRandom();
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
                audio.addMusic("resources/music/cos/caulder.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 60;
                case GameEnums.PowerMode_Power:
                    return 40;
                default:
                    if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                    {
                        return 60;
                    }
                    else
                    {
                        return -10;
                    }
            }
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 60;
            case GameEnums.PowerMode_Power:
                return 40;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 60;
                }
                else
                {
                    return -20;
                }
        }
    };

    this.startOfTurn = function(co)
    {
        var counit = co.getCOUnit();
        var coRange = co.getCORange();
        if (counit !== null)
        {
            UNIT.repairUnit(counit, 5);
            var fields = globals.getCircle(1, coRange);
            var x = counit.getX();
            var y = counit.getY();
            var animation = null;
            for (var i = 0; i < fields.size(); i++)
            {
                var point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                    if ((unit !== null) &&
                            (unit.getOwner() === counit.getOwner()))
                    {
                        UNIT.repairUnit(unit, 5);
                        animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                        animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                    }
                }
            }
            fields.remove();
        }
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Head of IDS the research department of Dark Matter. Conducts in inhuman experiments. All he wants is to be free to satisfy his curiosity.");
    };
    this.getHits = function()
    {
        return qsTr("Unrestricted experiments");
    };
    this.getMiss = function()
    {
        return qsTr("Ethics");
    };
    this.getCODescription = function()
    {
        return qsTr("His units are superior in his CO-Zone but weaker outside. On top his troops heal 5 Hp each turn inside his CO-Zone.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("All his units gain five Hp and get a offense and defense buff.");
    };
    this.getPowerName = function()
    {
        return qsTr("Mass Regeneration");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All his units heal to full and gain a massive offense and defense buff.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Perfect Healing");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Ahhh watch this experiment. I wonder what it does..."),
                qsTr("March my clones march and kill them all."),
                qsTr("You and your ethnics make you weak. Watch the power of science..."),
                qsTr("I am simply curious.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Interesting. Very Interesting"),
                qsTr("Quite satisfactory."),
                qsTr("I am simply curious.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Only a failed experiment nothing to worry about!"),
                qsTr("Argh i'm useless as well? Impossible!")];
    };
    this.getName = function()
    {
        return qsTr("Caulder");
    };
}

Constructor.prototype = CO;
var CO_CAULDER = new Constructor();
