var CO =
{
    init : function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    },

    getMovementPointModifier : function(co, unit)
    {
        return 0;
    },

    getFirerangeModifier : function(co, unit, posX, posY)
    {
        return 0;
    },

    getCaptureBonus : function(co, unit, posX, posY)
    {
        return 0;
    },

    getAttackHpBonus : function(co, unit, posX, posY)
    {
        return 0;
    },

    getTerrainDefenseModifier : function(co, unit, posX, posY)
    {
        return 0;
    },

    getRepairBonus : function(co, unit, posX, posY)
    {
        return 0;
    },

    getBonusLuck : function(co, unit, posX, posY)
    {
        return 0;
    },

    getBonusMisfortune : function(co, unit, posX, posY)
    {
        return 0;
    },

    activatePower : function(co)
    {
    },

    activateSuperpower : function(co)
    {
    },

    getOffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY)
    {
        return 0;
    },

    getDeffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY)
    {
        return 0;
    },

    getCanMoveAndFire : function(co, unit, posX, posY)
    {
        return false;
    },

    getCostModifier : function(co, id, baseCost)
    {
        return 0;
    },

    getCOArmy : function()
    {
        return "OS";
    },

    gainPowerstar : function(co, fondsDamage, x, y)
    {
        co.setPowerFilled(co.getPowerFilled() + fondsDamage / 10000)
    },

    getCOUnitRange : function(co)
    {
        return 0;
    },

    startOfTurn : function(co)
    {
        // called at the start of the turn use it to do cool co stuff like caulder's healing :)
    },
}

