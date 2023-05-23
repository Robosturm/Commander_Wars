var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
        }
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
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power5_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power5_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
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
    };
    
    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };

    this.superPowerTerrainBonus = 10;
    this.superPowerOffBonus = 20;
    this.superPowerHpBonus = 4;
    this.superPowerIndirectDefBonus = 70;
    this.superPowerDefBonus = 50;

    this.powerDefReduction = 0.5;
    this.powerOffBonus = 20;
    this.powerIndirectDefBonus = 60;
    this.powerDefBonus = 40;

    this.d2dIndirectDefBonus = 0;
    this.d2dDefBonus = 0;
    this.d2dOffBonus = 0;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneIndirectDefBonus = 60;
    this.d2dCoZoneDefBonus = 40;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
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
                return terrainDefense * CO_NAPOLEON.superPowerTerrainBonus + CO_NAPOLEON.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_NAPOLEON.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_NAPOLEON.d2dCoZoneOffBonus;
                }
                return CO_NAPOLEON.d2dOffBonus;
            }
        }
        return 0;
    };

    this.getAttackHpBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_NAPOLEON.superPowerHpBonus;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
            }
        }
        return 0;
    };

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                var defHp = defender.getHp() * 10;
                if (damage > defHp * CO_NAPOLEON.powerDefReduction && defHp > 1.0 && damage >= 0)
                {
                    return damage - defHp * CO_NAPOLEON.powerDefReduction;
                }
                return 0;
            default:
                break;
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return CO_NAPOLEON.superPowerIndirectDefBonus;
                }
                else
                {
                    return CO_NAPOLEON.superPowerDefBonus;
                }
            case GameEnums.PowerMode_Power:
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return CO_NAPOLEON.powerIndirectDefBonus;
                }
                else
                {
                    return CO_NAPOLEON.powerDefBonus;
                }
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                    {
                        return CO_NAPOLEON.d2dCoZoneIndirectDefBonus;
                    }
                    else
                    {
                        return CO_NAPOLEON.d2dCoZoneDefBonus;
                    }
                }
                if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return CO_NAPOLEON.d2dIndirectDefBonus;
                }
                else
                {
                    return CO_NAPOLEON.d2dDefBonus;
                }
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "MA";
    };

    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                    buildingId === "TOWN" ||
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_IRON_SHIELD_GENERATOR"];
            }
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
        var text = qsTr("\nSpecial Unit:\nIron Shield Generator\n\n" +
                    "Global Effect: \nDefense is increased by %0% and by %1% against indirect units and offense by %2%." +
                    "\n\nCO Zone Effect: \nDefense is increased by %3% and by %4% against indirect units and offense by %5%.");
        text = replaceTextArgs(text, [CO_NAPOLEON.d2dDefBonus, CO_NAPOLEON.d2dIndirectDefBonus, CO_NAPOLEON.d2dOffBonus,
                                      CO_NAPOLEON.d2dCoZoneDefBonus, CO_NAPOLEON.d2dCoZoneIndirectDefBonus, CO_NAPOLEON.d2dCoZoneOffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Units cannot take damage that is more than %0% of their current HP in a single attack.");
        text = replaceTextArgs(text, [CO_NAPOLEON.powerDefReduction * 100]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Blast Shield");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Units fight as though they were %0 HP stronger and inflict extra damage by %1% based on their terrain cover. Defense is increased by %2% and by %3% against indirect units");
        text = replaceTextArgs(text, [CO_NAPOLEON.superPowerHpBonus, CO_NAPOLEON.superPowerTerrainBonus, CO_NAPOLEON.superPowerDefBonus, CO_NAPOLEON.superPowerIndirectDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Last Stand");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Me and what army, you ask? I have many more soldiers than you think."),
                qsTr("Size does not matter in a battle of wills."),
                qsTr("I can take any attack. You would be wrong to test that statement."),
                qsTr("You can give up now and avoid further bloodshed.  Either way, though, my troops simply won't die."),
                qsTr("My casualties are not dead, but simply waiting to fight again."),
                qsTr("I need not worry about attrition.  On the other hand, you do.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("The survival of the Metal Army is assured."),
                qsTr("My casualties are recovering. Yours are not so lucky."),
                qsTr("Why were you so confident? Success is earned, not granted.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Our losses are unacceptable. I will fix that."),
                qsTr("Stand aside; I will deal with them.")];
    };
    this.getName = function()
    {
        return qsTr("Napoleon");
    };
}

Constructor.prototype = CO;
var CO_NAPOLEON = new Constructor();
