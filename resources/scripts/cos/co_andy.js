var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.setHp(unit.getHpRounded() + 2);

            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        CO_ANDY.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.setHp(unit.getHpRounded() + 5);

            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
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
        CO_ANDY.loadCOMusic(co);
        audio.playRandom();
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
                audio.addMusic("resources/music/cos/andy.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 20;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getMovementPointModifier = function(co, unit)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            return 1;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A whiz with a wrench, this mechanical boy wonder earned fame as the hero who saved Macro Land in the last great war.");
    };
    this.getHits = function()
    {
        return qsTr("Mechanics");
    };
    this.getMiss = function()
    {
        return qsTr("Waking up too early");
    };
    this.getCODescription = function()
    {
        return qsTr("No real weaknesses. Proficient with air, sea and land units. Ready to fight wherever and whenever.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Restores two HP to all units.");
    };
    this.getPowerName = function()
    {
        return qsTr("Hyper Repair");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Restores five HP to all units. Firepower rises, and unit movement increases by one space.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Hyper Upgrade");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I've got parts to spare!"),
                qsTr("I'm not giving up!"),
                qsTr("Time to roll up my sleeves!"),
                qsTr("I haven't even cranked the engine yet!"),
                qsTr("Pass me my wrench!!"),
                qsTr("It's time for a tune-up!"),
                qsTr("Never give up, and never lose! I'm on my way!"),
                qsTr("I'm not worried! I can fix anything!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("We won! Wooooooohooo!"),
                qsTr("I can fix anything!"),
                qsTr("I did it! Did you see that!?")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Oh, come on!"),
                qsTr("Next time I see you, you're in trouble!")];
    };
    this.getName = function()
    {
        return qsTr("Andy");
    };
}

Constructor.prototype = CO;
var CO_ANDY = new Constructor();
