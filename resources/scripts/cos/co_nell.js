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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
            if (animations.length < 5)
            {
                animations.push(animation);
            }
            else
            {
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
        audio.addMusic("resources/music/cos/power.mp3");
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
            if (animations.length < 5)
            {
                animations.push(animation);
            }
            else
            {
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
        audio.addMusic("resources/music/cos/superpower.mp3");
        audio.playRandom();
    };
    this.startOfTurn = function(co)
    {
        audio.addMusic("resources/music/cos/nell.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {

        return 0;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 100;
            case GameEnums.PowerMode_Power:
                return 60;
            default:
                if (co.inCORange(Qt.point(posX, posY)))
                {
                    return 25;
                }
                break;
        }
        return 5;
    };
}

Constructor.prototype = CO;
var CO_NELL = new Constructor();
