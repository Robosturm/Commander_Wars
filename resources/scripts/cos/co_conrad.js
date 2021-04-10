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

        var variables = co.getVariables();
        var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
        dmgModVar.writeDataFloat(50);
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
                audio.addMusic("resources/music/cos/conrad.mp3", 4027, 104641)
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

    this.startOfTurn = function(co)
    {
        var variables = co.getVariables();
        // clean up possible variables from scop
        variables.clear();
    }

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (isDefender)
        {
            // return weak counter damage of conrad
            return -20;
        }
        var units = co.getOwner().getUnits();
        var visionCount = 0;
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var x = unit.getX();
            var y = unit.getY();
            var distance = Math.abs(x - defPosX) + Math.abs(y - defPosY);
            if (distance <= unit.getVision(Qt.point(x, y)))
            {
                visionCount += 1;
            }
        }

        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return visionCount * 2 + 10;
            case GameEnums.PowerMode_Power:
                return visionCount * 5 + 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return visionCount * 5 + 10;
                }
                break;
        }
        return visionCount * 1;
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

    this.getBonusLuck = function(co, unit, posX, posY)
    {
        // reduce luck to zero
        return -unit.getHpRounded() / 2;
    };

    this.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender)
    {
        if (isDefender === false)
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var variables = co.getVariables();
                    var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
                    var bonusDamage = dmgModVar.readDataFloat();
                    return bonusDamage;
                case GameEnums.PowerMode_Power:
                default:
                    break;
            }
        }
        // return average luck as true damage
        return attackerBaseHp / 4;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var variables = co.getVariables();
                    var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
                    var defenderDamage = defender.getHp() * 10;
                    // overkill case
                    if (defenderDamage < 0)
                    {
                        dmgModVar.writeDataFloat(-defenderDamage);
                    }
                    else
                    {
                        dmgModVar.writeDataFloat(0);
                    }
                    break;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
            }
        }
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                return 1;
            default:
                return 0;
        }
    };

    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A damage analyst from Teal Isles who favors precise tactics. Very intelligent, but inexperienced.");
    };
    this.getHits = function(co)
    {
        return qsTr("Techno");
    };
    this.getMiss = function(co)
    {
        return qsTr("Blackouts");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Units have no basic luck instead they deal a fix amount of damage. Firepower increases as more units are within vision range but they have weak counters.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nUnits deal no luck damage. Units gain  firepower for own units in vision range. Counter attacks are weaker.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain even more firepower for own units in vision range.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Unit vision is extended. Firepower increases even more as more units are within vision range.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Calculated Strike");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Conrad's first attack deals +50% luck damage. If Conrad would deal over 100%, the damage is rolled over to the next engagement.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Precision Assault");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You might be more experienced, but that doesn't mean I can't teach you a thing or two!"),
                qsTr("I won't go down that easily! This battle isn't even close to finished!"),
                qsTr("The chances of you pulling through are pretty slim. Believe me, I would know!"),
                qsTr("My attack plan is perfect! Even I can't mess it up!"),
                qsTr("Your strategy doesn't add up. Mine practically multiplies!"),
                qsTr("Don't make me angry! ...It makes me screw up my numbers.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Solid tactics get solid results!"),
                qsTr("That was easier than I expected. Were my numbers off?"),
                qsTr("Either I'm getting better, or you guys are getting worse!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("What? I didn't see that."),
                qsTr("How could you hide from me?")];
    };
    this.getName = function()
    {
        return qsTr("Conrad");
    };
}

Constructor.prototype = CO;
var CO_CONRAD = new Constructor();
