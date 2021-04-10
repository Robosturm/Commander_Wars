var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/napoleon.mp3", 40388, 83068);
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
            if (globals.randInt(0, 1) === 0)
            {
                animation.setSound("power5_1.wav");
            }
            else
            {
                animation.setSound("power5_2.wav");
            }
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
    };

    
    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var terrainDefense = 0;
            if (map.onMap(atkPosX, atkPosY))
            {
                terrainDefense = map.getTerrain(atkPosX, atkPosY).getDefense(attacker);
            }
            return terrainDefense * 10 + 20;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 20;
            }
            break;
        }
        return 0;
    };

    this.getAttackHpBonus = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 4;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
        }
        return 0;
    };

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, luckMode)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                var defHp = defender.getHp() * 10;
                if (damage  > defHp / 2)
                {
                    return damage - defHp / 2;
                }
                return 0;
            default:
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return 65;
                }
                else
                {
                    return 45;
                }
            case GameEnums.PowerMode_Power:
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return 45;
                }
                else
                {
                    return 25;
                }
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                    {
                        return 60;
                    }
                    else
                    {
                        return 40;
                    }
                }
                break;
        }
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "MA";
    };

    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_IRON_SHIELD_GENERATOR"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The current second commander of the Metal Army. Believes Metal Army's ingenuity gives it a right to control the world.");
    };
    this.getHits = function(co)
    {
        return qsTr("Sudoku");
    };
    this.getMiss = function(co)
    {
        return qsTr("Horror Movies");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Units have high defensive capabilities.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nIron Shield Generator\n\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nDefense is increased, even more against indirect units.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Units cannot take damage that is more than half of their starting HP in a single attack.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Blast Shield");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Units fight as though they were four HP stronger and inflict extra damage relative to their terrain cover.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Last Stand");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Me and what army, you ask?  I have many more soldiers than you think."),
                qsTr("Size does not matter in a battle of wills."),
                qsTr("I can take any attack.  You would be wrong to test that statement."),
                qsTr("You can give up now and avoid further bloodshed.  Either way, though, my troops simply won't die."),
                qsTr("My casualties are not dead, but simply waiting to fight again."),
                qsTr("I need not worry about attrition.  On the other hand, you do.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("The survival of the Metal Army is assured."),
                qsTr("My casualties are recovering.  Yours are not so lucky."),
                qsTr("Why were you so confident?  Success is earned, not granted.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Our losses are unacceptable.  I will fix that."),
                qsTr("Stand aside; I will deal with them.")];
    };
    this.getName = function()
    {
        return qsTr("Napoleon");
    };
}

Constructor.prototype = CO;
var CO_NAPOLEON = new Constructor();
