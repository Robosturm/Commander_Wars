CO_NANA.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (!gotAttacked && attacker.getOwner() === co.getOwner())
        {
            var owner = co.getOwner();
            var powerMode = co.getPowerMode();
            var damage = 0;
            var explosionRange = 1;
            var friendlyFire = true;
            var i = 0;
            var defX = defender.getX();
            var defY = defender.getY();
            var unit = null;
            var point = Qt.point(0, 0);
            var hp = 0;
            switch (powerMode)
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                damage = 2;
                explosionRange = 2;
                friendlyFire = false;
                break;
            case GameEnums.PowerMode_Power:
                friendlyFire = false;
                damage = 1;
                if (attacker.getBaseMaxRange() === 1 && damage > 0)
                {
                    damage = 2;
                    var width = map.getMapWidth();
                    var heigth = map.getMapHeight();
                    var maxRange = width;
                    if (heigth > width)
                    {
                        maxRange = heigth;
                    }
                    var found = false;
                    // deal direct chain damage to nearest enemy
                    for (var curRange = 1; curRange < maxRange; curRange++)
                    {
                        var directCircle = globals.getCircle(curRange, curRange);
                        for (i = 0; i < directCircle.size(); i++)
                        {
                            point = directCircle.at(i);
                            if (map.onMap(defX + point.x, defY + point.y))
                            {
                                unit = map.getTerrain(defX + point.x, defY + point.y).getUnit();
                                if (unit !== null &&
                                        owner.isEnemyUnit(unit))
                                {
                                    hp = unit.getHpRounded();
                                    if (hp <= damage)
                                    {
                                        // set hp to very very low
                                        unit.setHp(0.001);
                                    }
                                    else
                                    {
                                        unit.setHp(hp - damage);
                                    }
                                    found = true;
                                    break;
                                }
                            }
                        }
                        directCircle.remove()
                        if (found === true)
                        {
                            break;
                        }
                    }
                }
                break;
            default:
                if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                {
                    damage = 1;
                }
                break;
            }

            // deal indirect unit splash damage
            if (attacker.getBaseMaxRange() > 1 && damage > 0)
            {
                var circle = globals.getCircle(1, explosionRange);
                for (i = 0; i < circle.size(); i++)
                {
                    point = circle.at(i);
                    if (map.onMap(defX + point.x, defY + point.y))
                    {
                        unit = map.getTerrain(defX + point.x, defY + point.y).getUnit();
                        if (unit !== null)
                        {
                            if (owner !== unit.getOwner() || friendlyFire === true)
                            {
                                hp = unit.getHpRounded();
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
                        }
                    }
                }
                circle.remove()
            }
        }
    }
};

CO_NANA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (attacker.getBaseMaxRange() > 1)
        {
            return -20;
        }
        return 0;
    }
};
CO_NANA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
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
