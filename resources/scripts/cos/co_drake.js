var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
    {
        CO_DRAKE.drakeDamage(co, 1, null);
        audio.clearPlayList();
        CO_DRAKE.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        map.getGameRules().changeWeather("WEATHER_RAIN", map.getPlayerCount() * 2);
        CO_DRAKE.drakeDamage(co, 2, animation2);
        audio.clearPlayList();
        CO_DRAKE.loadCOMusic(co);
        audio.playRandom();
    };

    this.drakeDamage = function(co, value, animation2)
    {
        var player = co.getPlayer();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
        var animation = null;
        var animations = [];
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    var unit = units.at(i);

                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        if (animation2 !== null)
                        {
                            animation2.queueAnimation(animation);
                        }
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                    var hp = unit.getHpRounded();
                    if (hp <= value)
                    {
                        // set hp to very very low
                        unit.setHp(0.001);
                    }
                    else
                    {
                        unit.setHp(hp - value);
                    }
                    // reduce fuel
                    unit.setFuel(unit.getFuel() / 2);
                }
                units.remove();
            }
        }
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
            default:
                audio.addMusic("resources/music/cos/drake.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };
    this.getAirUnitIDS = function()
    {
        return ["BOMBER", "FIGHTER", "DUSTER", "K_HELI", "T_HELI", "STEALTHBOMBER", "TRANSPORTPLANE", "WATERPLANE"];
    };
    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "DESTROYER", "SUBMARINE"];
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        var seaUnits = CO_EAGLE.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 20;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 20;
                }
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
                    {
                        return 20;
                    }
                }
                break;
        }
        if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return 10;
        }
        if (airUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -15;
        }
        return 0;
    };
    this.getMovementPointModifier = function(co, unit)
    {
        var seaUnits = CO_EAGLE.getSeaUnitIDS();
        if (seaUnits.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A bighearted former pirate who hates fighting. Also a great surfer.");
    };
    this.getHits = function()
    {
        return qsTr("The sea");
    };
    this.getMiss = function()
    {
        return qsTr("High places");
    };
    this.getCODescription = function()
    {
        return qsTr("Naval units have superior firepower, but air units have reduced firepower.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Causes a tidal wave that does one HP of damage to all enemy units.");
    };
    this.getPowerName = function()
    {
        return qsTr("Tsunami");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Causes a giant tidal wave that does two HP of damage to all enemy units and weather changes to rain.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Typhoon");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Panic is for landlubbers!"),
                qsTr("Prepare to be washed away!"),
                qsTr("Time to show you how we do things at sea!"),
                qsTr("Wave good-bye to your troops!"),
                qsTr("Can't you just feel the riptide washing you out to open sea?"),
                qsTr("Drake is taking the helm!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("I'm just glad I survived that..."),
                qsTr("That was some rough sailing!|Blow me down... we finally won.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("A job well done! You've beaten me."),
                qsTr("Curses! I've been defeated. Time to hoist sail and flee!")];
    };
    this.getName = function()
    {
        return qsTr("Drake");
    };
}

Constructor.prototype = CO;
var CO_DRAKE = new Constructor();
