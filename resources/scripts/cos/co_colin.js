var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co)
    {
        var player = co.getPlayer();
        player.addFonds(player.getFonds() * 0.5);
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
        audio.addMusic("resources/music/cos/colin.mp3")
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };
    this.getCostModifier = function(co, id, baseCost)
    {
        return -baseCost * 0.2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                var bonus = attacker.getOwner().getFonds() / 1000 * 3.33;
                return bonus;
            case GameEnums.PowerMode_Power:
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 0;
                }
                break;
        }
        return -10;
    };
}

Constructor.prototype = CO;
var CO_COLIN = new Constructor();
