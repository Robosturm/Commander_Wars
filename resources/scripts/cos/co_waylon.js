var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getAirUnitIDS = function()
    {
        return ["BOMBER", "FIGHTER", "BLACK_BOMB", "DUSTER", "K_HELI", "T_HELI", "STEALTHBOMBER", "TRANSPORTPLANE", "WATERPLANE"];
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        var airUnitIDs = CO_WAYLON.getAirUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (airUnitIDs.indexOf(unit.getUnitID())  >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                if (animations.length < 5)
                {
                    animation.addSprite("power2", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power2", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                    animations[counter].queueAnimation(animation);
                    animations[counter] = animation;
                    counter++;
                    if (counter >= animations.length)
                    {
                        counter = 0;
                    }
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        CO_WAYLON.loadCOMusic(co);
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
        var airUnitIDs = CO_WAYLON.getAirUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (airUnitIDs.indexOf(unit.getUnitID())  >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

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
        }
        units.remove();

        audio.clearPlayList();
        CO_WAYLON.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3");
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/waylon.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_WAYLON.getAirUnitIDS();

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 60;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 20;
                }
                else
                {
                    return 0;
                }
            default:
                if (airUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 20;
                    }
                }
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_WAYLON.getAirUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (airUnits.indexOf(defender.getUnitID()) >= 0)
                {
                    return 270;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (airUnits.indexOf(defender.getUnitID()) >= 0)
                {
                    return 200;
                }
                else
                {
                    return 0;
                }
            default:
                if (airUnits.indexOf(defender.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                    {
                        return 30;
                    }
                }
                break;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A Teal Isle Commander. Destructive, egotistical and extremely vain.");
    };
    this.getHits = function()
    {
        return qsTr("Living the good life");
    };
    this.getMiss = function()
    {
        return qsTr("Responsibility");
    };
    this.getCODescription = function()
    {
        return qsTr("Air units have higher firepower and defense.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Air units increase firepower and highly increases defense.");
    };
    this.getPowerName = function()
    {
        return qsTr("Bad Company");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Air units increase firepower and highly increases defense.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Wingman");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I'm over here! C'mon give me your best shot."),
                qsTr("You think you can get the better of me? You've got a lot to learn."),
                qsTr("Woo-hoo!"),
                qsTr("All i want is total air supremacy! Then i'll pick off your forces.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("The end of the world? Fine by me..."),
                qsTr("Woo-hoo!"),
                qsTr("How'd you like that?")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("This is not the freedom i want!"),
                qsTr("The eagle has landed.")];
    };
    this.getName = function()
    {
        return qsTr("Waylon");
    };
}

Constructor.prototype = CO;
var CO_WAYLON = new Constructor();
