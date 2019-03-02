var CO =
{
    init : function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    },

    getMovementPointModifier : function(unit)
    {
        return 0;
    },

    getFirerangeModifier : function(unit, posX, posY)
    {
        return 0;
    },

    getAttackHpBonus : function(unit, posX, posY)
    {
        return 0;
    },

    getTerrainDefenseModifier : function(unit, posX, posY)
    {
        return 0;
    },

    getRepairBonus : function(unit, posX, posY)
    {
        return 0;
    },

    getBonusLuck : function(unit, posX, posY)
    {
        return 0;
    },

    getBonusMisfortune : function(unit, posX, posY)
    {
        return 0;
    },

    activatePower : function(co)
    {
    },

    activateSuperpower : function(co)
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

    getCostModifier : function(id, baseCost)
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

