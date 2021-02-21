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
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co)
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
                audio.addMusic("resources/music/cos/kindle.mp3", 1995, 63956);
                break;
        }
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
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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

        CO_KINDLE.kindleDamage(co, 3, animations, powerNameAnimation);
    };

    this.kindleDamage = function(co, value, animations, powerNameAnimation)
    {
        var player = co.getOwner();
        var units = player.getUnits();
        var counter = 0;
        var unit = null;
        var animation = null;
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    unit = units.at(i);
                    if (unit.getTerrain().getBuilding() !== null)
                    {
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


                        animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                        if (animations.length < 5)
                        {
                            animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                            powerNameAnimation.queueAnimation(animation);
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
                    }
                }
                units.remove();
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
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (typeof map !== 'undefined')
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                var building = map.getTerrain(atkPosX, atkPosY).getBuilding();
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var ret = 0;
                    if (building !== null)
                    {
                        ret = 130;
                    }
                    ret += co.getOwner().getBuildingListCount(["TEMPORARY_AIRPORT", "TEMPORARY_HARBOUR"], false) * 3 + 10;
                    return ret;
                case GameEnums.PowerMode_Power:
                    if (building !== null)
                    {
                        return 80;
                    }
                    return 10;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        if (building !== null)
                        {
                            return 70;
                        }
                        return 10;
                    }
                    break;
                }
            }
        }
        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return 10;
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

    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Jugger and Koal's commanding officer. Has a blunt, queen-like personality.");
    };
    this.getHits = function(co)
    {
        return qsTr("Anything chic");
    };
    this.getMiss = function(co)
    {
        return qsTr("Anything passe");
    };
    this.getCODescription = function(co)
    {
        return qsTr("An upper-crust CO who excels at urban warfare. Firepower of all units increased on properties.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits on buildings have increased firepower.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Inflicts three HP of damage to enemy units on properties and increases firepower of all units on a property.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Urban Blight");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("The more properties she controls, the more firepower she gains and greatly increases firepower of all units on a property.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("High Society");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("So, you want to see me get serious? Very well! You asked for it!"),
                qsTr("Even the prettiest rose has thorns!"),
                qsTr("I am Kindle, Gorgeous AND Clever!"),
                qsTr("Let us play a more dangerous game, shall we? I grow weary of this."),
                qsTr("Aha ha ha! Beg for mercy, rat!"),
                qsTr("Still putting up a fight? Unbelievable!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Winning is almost as wonderful as I am. Aha ha ha!"),
                qsTr("I won again? This is getting so boring."),
                qsTr("Aha ha ha! Pathetic.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Losing to you corn-fed country folk is enough to give me frown lines."),
                qsTr("That was a bit of a disappointment.")];
    };
    this.getName = function()
    {
        return qsTr("Kindle");
    };
}

Constructor.prototype = CO;
var CO_KINDLE = new Constructor();
