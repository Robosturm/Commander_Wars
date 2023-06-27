var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power3.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);
        CO_WALTER.power(co, CO_WALTER.superPowerDamage, powerNameAnimation, map);
    };

    this.power = function(co, value, powerNameAnimation, map)
    {
        var player = co.getOwner();
        var units = player.getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                    (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                units = enemyPlayer.getUnits();
                units.randomize();
                var size = units.size();
                for (i = 0; i < size; i++)
                {
                    unit = units.at(i);
                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());

                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("CO_WALTER", "postAnimationDamage");
                    animation.setSound("power4.wav", 1, delay);
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        powerNameAnimation.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        }
    };

    this.postAnimationDamage = function(postAnimation, map)
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
                    if (hp - damage <= 0.1)
                    {
                        // set hp to very very low
                        unit.setHp(0.1);
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

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
        }
    };

    this.superPowerDamage = 1;

    this.powerOffBonus = 20;
    this.powerDefBonus = 20;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 20;

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_WALTER.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_WALTER.d2dCoZoneDefBonus;
                }
                break;
            }
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_WALTER.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_WALTER.d2dCoZoneOffBonus;
                }
                break;
            }
        }
        return 0;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                break;
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
        }
    };

    this.getHpHidden = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
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
        }
    };

    this.getPerfectHpView = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            return true;
        }
        return false;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                buildingId === "TOWN" ||
                BUILDING.isHq(building))
            {
                return ["ZCOUNIT_INTEL_TRUCK"];
            }
        }
        return [];
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
        var text = qsTr("\nSpecial Unit:\nIntel truck\n") +
            qsTr("\nGlobal Effect: \nNo bonus.") +
            qsTr("\n\nCO Zone Effect: \nUnits gain additional %0% firepower and %1% defence.");
        text = replaceTextArgs(text, [CO_WALTER.d2dCoZoneOffBonus, CO_WALTER.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Enemies take additional damage when they attack and units gain  additional %0% firepower and %1% defence.");
        text = replaceTextArgs(text, [CO_WALTER.powerOffBonus, CO_WALTER.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Memento");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Massively reduces the ammo from enemy units and deals %0 HP damage to them. Hides his units HP from enemies. Units gain  additional %0% firepower and %1% defence.");
        text = replaceTextArgs(text, [CO_WALTER.superPowerDamage, CO_WALTER.powerOffBonus, CO_WALTER.powerDefBonus]);
        return text;
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
