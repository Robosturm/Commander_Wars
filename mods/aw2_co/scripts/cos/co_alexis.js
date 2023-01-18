CO_ALEXIS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        var nearBuildings = false;
        var fields = globals.getCircle(0, 2);
        if (map !== null)
        {
            var size = fields.size();
            for (var i = 0; i < size; i++)
            {
                var x = fields.at(i).x + atkPosX;
                var y = fields.at(i).y + atkPosY;
                if (map.onMap(x, y))
                {
                    var building = map.getTerrain(x, y).getBuilding();
                    if (building !== null && building.getOwner() === co.getOwner())
                    {
                        nearBuildings = true;
                        break;
                    }
                }
            }
        }
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (nearBuildings)
            {
                return 40;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            if (nearBuildings)
            {
                return 20;
            }
            return 0;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 10;
            }
        }
    }
};

CO_ALEXIS.startOfTurn = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        var player = co.getOwner();
        if (!player.getIsDefeated())
        {
            var animations = [];
            var counter = 0;
            var buildings = co.getOwner().getBuildings();
            var fields = globals.getCircle(1, 1);
            var viewplayer = map.getCurrentViewPlayer();
            var size1 = buildings.size();
            var size2 = fields.size();
            for (var i2 = 0; i2 < size1; i2++)
            {
                var building = buildings.at(i2);
                var x = building.getX();
                var y = building.getY();
                var animation = null;
                for (var i = 0; i < size2; i++)
                {
                    var point = fields.at(i);
                    var unitX = x + point.x;
                    var unitY = y + point.y;
                    if (map.onMap(unitX, unitY))
                    {
                        var unit = map.getTerrain(unitX, unitY).getUnit();
                        if ((unit !== null) &&
                                (unit.getOwner() === co.getOwner()))
                        {
                            UNIT.repairUnit(unit, 1, map);
                            animation = GameAnimationFactory.createAnimation(map, unitX, unitY);
                            var delay = globals.randInt(135, 265);
                            if (animations.length < 5)
                            {
                                delay *= i;
                            }
                            animation.setSound("power0.wav", 1, delay);
                            if (animations.length < 5)
                            {
                                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                animations.push(animation);
                            }
                            else
                            {
                                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                animations[counter].queueAnimation(animation);
                                animations[counter] = animation;
                                counter++;
                                if (counter >= animations.length)
                                {
                                    counter = 0;
                                }
                            }
                            if (!viewplayer.getFieldVisible(unitX, unitY))
                            {
                                animation.setVisible(false);
                            }
                        }
                    }
                }
            }
        }
    }
};
CO_ALEXIS.coZoneBonus = 0;
CO_ALEXIS.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};

CO_ALEXIS.getRepairBonus = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        return -1;
    }
    return 0;
};
