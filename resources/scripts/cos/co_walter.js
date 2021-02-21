var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(5);
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
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_WALTER.power(co, 1, powerNameAnimation);
    };

    this.power = function(co, value, powerNameAnimation)
    {
        var player = co.getOwner();
        var units = player.getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("CO_WALTER", "postAnimationDamage");

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
                units.remove();
            }
        }
    };

    this.postAnimationDamage = function(postAnimation)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var damage = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                if (damage > 0)
                {
                    var hp = unit.getHpRounded();
                    if (hp <= damage)
                    {
                        // set hp to very very low
                        unit.setHp(0.001);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
                // reduce ammo
                if (unit.getMaxAmmo2() > 0)
                {
                    unit.reduceAmmo2(unit.getMaxAmmo2() * 0.8);
                }
                if (unit.getMaxAmmo1() > 0)
                {
                    unit.reduceAmmo1(unit.getMaxAmmo1() * 0.8);
                }
            }

        }
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
        default:
            audio.addMusic("resources/music/cos/walter.mp3");
            break;
        }
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 10;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 20;
            }
            break;
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 10;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 20;
            }
            break;
        }
        return 0;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (co.getOwner() !== attacker.getOwner())
            {
                if (atkDamage > 0)
                {
                    var attackerHp = attacker.getHp();
                    attacker.setHp(attackerHp - atkDamage);
                }
            }
            break;
        default:
            break;
        }
    };

    this.getHpHidden = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return true;
        case GameEnums.PowerMode_Power:
        default:
            return false;
        }
    };

    this.getPerfectHpView = function(co, unit, posX, posY)
    {
        // are the hp hidden of this unit?
        return true;
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Commander of the Capitol Guard of Teal Islands. He excels in survival and sheer tenacity. The Long Standing Guardian");
    };
    this.getHits = function(co)
    {
        return qsTr("Tongue Twisters");
    };
    this.getMiss = function(co)
    {
        return qsTr("Colloquialisms");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Using Teal Isles intelligence division, Walter can view units' HP more accurately than other COs.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo bonus.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain additional firepower and defence.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Enemies take additional damage when they attack.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Memento");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Massively reduces the ammo from enemy units and deals minor damage to them. Hides his units HP from enemies.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Magnum Opus");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Greater opponents have burnt out against me."),
                qsTr("For the Isles sake, I will endure you!"),
                qsTr("I am the most stalwart foe that you will ever face!"),
                qsTr("Morale is the key to victory, but you seem rather....ill equipped."),
                qsTr("A battle does not determine who is right. It determines who is left. Me, of course."),
                qsTr("The spirit of Teal Isles will carry us for eternity!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Another battle has been won. Teal Isles is safe for now."),
                qsTr("You have been outwitted, outmatched and outmaneuvered."),
                qsTr("Hah, yet again I have stood the test of time!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("That wasn't the best idea to take fight."),
                qsTr("I got detected too early..")];
    };
    this.getName = function()
    {
        return qsTr("Walter");
    };
}

Constructor.prototype = CO;
var CO_WALTER = new Constructor();
