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
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/napoleon.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
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
        audio.clearPlayList();
        CO_NAPOLEON.loadCOMusic(co);
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
        audio.clearPlayList();
        CO_NAPOLEON.loadCOMusic(co);
        audio.playRandom();
    };

    
    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var terrainDefense = map.getTerrain(atkPosX, atkPosY).getDefense(attacker);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return terrainDefense * 10;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY)))
            {
                return 0;
            }
            break;
        }
        return 0;
    };

    this.getAttackHpBonus = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
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
                                  defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
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
            case GameEnums.PowerMode_Superpower:
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return 60
                }
                else
                {
                    return 40;
                }
            case GameEnums.PowerMode_Power:
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return 30
                }
                else
                {
                    return 20;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                    {
                        return 30
                    }
                    else
                    {
                        return 20;
                    }
                }
                break;
        }
        if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
        {
            return 20
        }
        else
        {
            return 10;
        }
    };

    this.getCOArmy = function()
    {
        return "MA";
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("The current second commander of the Metall Army. Believes Metall Army's ingenuity gives it a right to control the world.");
    };
    this.getHits = function()
    {
        return qsTr("Sudoku");
    };
    this.getMiss = function()
    {
        return qsTr("Horror Movies");
    };
    this.getCODescription = function()
    {
        return qsTr("Unit's have high defensive capabilities.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Units cannot take damage that is more than half of their starting HP in a single attack.");
    };
    this.getPowerName = function()
    {
        return qsTr("Blast Shield");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Units fight as though they were four HP stronger and inflict extra damage relative to their terrain cover.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Last Stand");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Me and what army, you ask?  I have many more soldiers than you think."),
                qsTr("Size does not matter in a battle of wills."),
                qsTr("I can take any attack.  You would be wrong to test that statement."),
                qsTr("You can give up now and avoid further bloodshed.  Either way, though, my troops simply won't die."),
                qsTr("My casualties are not dead, but simply waiting to fight again."),
                qsTr("I need not worry about attrition.  On the other hand, you do.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("The survival of the Metall Army is assured."),
                qsTr("My casualties are recovering.  Yours are not so lucky."),
                qsTr("Why were you so confident?  Success is earned, not granted.")];
    };
    this.getDefeatSentences = function()
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
