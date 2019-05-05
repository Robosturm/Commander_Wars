var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "BLACK_BOAT", "LANDER", "DESTROYER", "SUBMARINE",
                "K_HELI", "T_HELI", "WATERPLANE"];
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        var seaUnitIDs = CO_GREYFIELD.getSeaUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.refill();
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

            if (animations.length < 5)
            {
                animation.addSprite("power8", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power8", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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
        CO_GREYFIELD.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        var seaUnitIDs = CO_GREYFIELD.getSeaUnitIDS();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.refill();
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
        units.remove();

        audio.clearPlayList();
        CO_GREYFIELD.loadCOMusic(co);
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
            default:
                audio.addMusic("resources/music/cos/greyfield.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var seaUnits = CO_GREYFIELD.getSeaUnitIDS();

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
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
                if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 10;
                    }
                }
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var seaUnits = CO_GREYFIELD.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (seaUnits.indexOf(defender.getUnitID()) >= 0)
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (seaUnits.indexOf(defender.getUnitID()) >= 0)
                {
                    return 40;
                }
                else
                {
                    return 0;
                }
            default:
                if (seaUnits.indexOf(defender.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                    {
                        return 40;
                    }
                    return 15;
                }
                break;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var seaUnits = CO_GREYFIELD.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (seaUnits.indexOf(unit.getUnitID()) >= 0)
                {
                    return 1;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
        }
        return 0;
    };



    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Wants to rule as the supreme global leader. Ambitious and power-hungry.");
    };
    this.getHits = function()
    {
        return qsTr("Power");
    };
    this.getMiss = function()
    {
        return qsTr("Insubordinates");
    };
    this.getCODescription = function()
    {
        return qsTr("Copter, seaplanes and sea units have higher firepower and defense.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("All units get resupplied and his copter, seaplanes and sea units have higher firepower and defense.");
    };
    this.getPowerName = function()
    {
        return qsTr("Supply Chain");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All units get resupplied and his copter, seaplanes and sea units have higher firepower and defense and increase movement range by 1.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Naval War");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I will brook no opposition. The enemy will be eleminated."),
                qsTr("Overwhelming forces need overwhelming supplies."),
                qsTr("Supplies are here!"),
                qsTr("Admiral Greyfield here. Attack and eleminate them.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Disloyalty will be punished with death."),
                qsTr("Anyone who opposes me will be crushed!"),
                qsTr("The Teal Isles Army will prevail.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Nooo... this is not the end of Teal Isles!"),
                qsTr("What? My army has been crushed.")];
    };
    this.getName = function()
    {
        return qsTr("Greyfield");
    };
}

Constructor.prototype = CO;
var CO_GREYFIELD = new Constructor();
