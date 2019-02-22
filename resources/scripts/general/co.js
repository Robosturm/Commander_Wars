var CO =
{
    getMovementPointModifier : function(unit)
    {
        return 0;
    },

    getFirerangeModifier : function(unit, posX, posY)
    {
        return 0;
    },

    getTerrainDefenseModifier : function(unit, posX, posY)
    {
        return 0;
    },

    activatePower : function()
    {
    },

    activateSuperpower : function()
    {
    },

    getOffensiveBonus : function(attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        return 0;
    },

    getDeffensiveBonus : function(attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY)
    {
        return 0;
    },

    getCanMoveAndFire : function(unit, posX, posY)
    {
        return false;
    },
}

