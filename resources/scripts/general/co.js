var CO =
{
    init : function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    },

    loadCOMusic : function(co)
    {
        // put the co music in here.
    },

    getMovementPointModifier : function(co, unit)
    {
        return 0;
    },

    buildedUnit : function(co, unit)
    {
        // called when someone builded a unit -> ACTION_BUILD_UNITS was performed
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

    getVisionrangeModifier: function(co, unit, posX, posY)
    {
        return 0;
    },

    getTerrainDefenseModifier : function(co, unit, posX, posY)
    {
        return 0;
    },

    getFirstStrike : function(co, unit, posX, posY)
    {
        return false;
    },

    getEnemyTerrainDefenseModifier : function(co, unit, posX, posY)
    {
        return 0;
    },

    getDamageReduction : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender)
    {
        return 0;
    },

    getTrueDamage : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                             defender, defPosX, defPosY, isDefender)
    {
        return 0;
    },

    getPerfectVision : function(co)
    {
        // return true if forest and reafs etc are disabled in fog of war see sonja
        return false;
    },

    getHpHidden : function(co, unit, posX, posY)
    {
        // are the hp hidden of this unit?
        return false;
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

    getActionModifierList : function(co, unit)
    {
        // return empty list as default
        return [];
    },

    activatePower : function(co)
    {
    },

    activateSuperpower : function(co)
    {
    },

    getFuelCostModifier : function(co, unit, posX, posY, costs)
    {
        return 0;
    },

    getOffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        return 0;
    },

    getDeffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender)
    {
        return 0;
    },

    getMovementpointModifier : function(co, unit, posX, posY)
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

    getAdditionalBuildingActions : function(co, building)
    {
        return "";
    },

    getBonusIncome : function(co, building, income)
    {
        return 0;
    },

    getIncomeReduction : function(co, building, income)
    {
        return 0;
    },

    postBattleActions : function(co, attacker, atkDamage, defender, gotAttacked)
    {
        // called after damage was dealt to the defender unit.
        // the damage given is the damage was dealt to the unit.
        // gotAttacked means we own the unit which got damage dealt.
    },

    startOfTurn : function(co)
    {
        // called at the start of the turn use it to do cool co stuff like caulder's healing :)
    },

    // CO - Intel
    getBio : function()
    {
        return "";
    },
    getHits : function()
    {
        return "";
    },
    getMiss : function()
    {
        return "";
    },
    getCODescription : function()
    {
        return "";
    },
    getPowerDescription : function()
    {
        return "";
    },
    getPowerName : function()
    {
        return "";
    },
    getSuperPowerDescription : function()
    {
        return "";
    },
    getSuperPowerName : function()
    {
        return "";
    },
    getPowerSentences : function()
    {
        return [];
    },
    getVictorySentences : function()
    {
        return [];
    },
    getDefeatSentences : function()
    {
        return [];
    },
    getName : function()
    {
        return "";
    },
}

