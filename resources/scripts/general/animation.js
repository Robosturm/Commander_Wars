// this object provides some general call backs for post animation action
var ANIMATION =
{
    postAnimationHeal : function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var heal = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                unit.setHp(unit.getHpRounded() + heal);
            }
        }
    },

    postAnimationDamage : function(postAnimation, map)
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
                var hp = unit.getHpRounded();
                if (hp <= damage)
                {
                    // set hp to very very low
                    unit.setHp(0.1);
                }
                else
                {
                    unit.setHp(hp - damage);
                }
            }
        }
    },

    postAnimationDamageKill : function(postAnimation, map)
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
                unit.setHp(unit.getHpRounded() - damage);
                if (unit.getHp() <= 0)
                {
                    unit.killUnit();
                }
            }
        }
    },

    postAnimationSpawnUnit : function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var unitId = postAnimation.readDataString();
        var owner = postAnimation.readDataInt32();
        var hp = postAnimation.readDataInt32();
        map.spawnUnit(x, y, unitId, map.getPlayer(owner));
        map.getTerrain(x, y).getUnit().setHp(hp);
    },

    preOnAnimationChangedAnimation : function(animation, map)
    {
        map.getGameRules().onWeatherChanged();
    },
};
