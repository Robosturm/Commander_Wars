var CO_PERK =
{
    isActive : function(co)
    {
        return true;
    },

    isSelectable : function()
    {
        // return false if the perk should not be selectable during the player selection
         return true;
    },

    getPerkEnabled : function(co, map)
    {
        // called during the perk selection
        // use the co object to get the current active perks and define if this perk can be selected or not
        return true;
    },

    onCOUnitLost : function(co, map)
    {
    },

    onUnitDeath : function(co, unit, map)
    {
    },

    getMovementpointModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    buildedUnit : function(co, unit, map)
    {
        // called when someone builded a unit -> ACTION_BUILD_UNITS was performed
    },

    getFirerangeModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getMinFirerangeModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getCaptureBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getAttackHpBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getVisionrangeModifier: function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getTerrainDefenseModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getFirstStrike : function(co, unit, posX, posY, attacker, isDefender, map, atkPosX, atkPosY)
    {
        return false;
    },

    getEnemyTerrainDefenseModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getDamageReduction : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, luckMode, map)
    {
        return 0;
    },

    getTrueDamage : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    },

    getPowerChargeBonus : function(co, map)
    {
        return 0;
    },

    canCounterAttack : function(co, attacker, atkPosX, atkPosY,
                                defender, defPosX, defPosY, luckMode, map)
    {
        return GameEnums.CounterAttackMode_Undefined;
    },

    getPerfectVision : function(co, map)
    {
        return false;
    },

    getWeatherImmune : function(co, map)
    {
        // return true if a weather has no effect for the co
        return false;
    },

    getHpHidden : function(co, unit, posX, posY, map)
    {
        // are the hp hidden of this unit?
        return false;
    },

    getRankInfoHidden : function(co, unit, posX, posY, map)
    {
        // are the hp hidden of this unit?
        return false;
    },

    getPerfectHpView : function(co, unit, posX, posY, map)
    {
        // are the hp hidden of this unit?
        return false;
    },

    getRepairBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getBonusLuck : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getBonusMisfortune : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getEnemyBonusLuck : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getEnemyBonusMisfortune : function(co, unit, posX, posY, map)
    {
        return 0;
    },

    getActionModifierList : function(co, unit, map)
    {
        // return empty list as default
        return [];
    },

    activatePower : function(co, map)
    {
    },

    activateSuperpower : function(co, map)
    {
    },

    getFuelCostModifier : function(co, unit, posX, posY, costs, map)
    {
        // modifies the fuel cost at the start of a turn
        return 0;
    },

    getOffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    },

    getOffensiveReduction : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    },

    getDeffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        return 0;
    },

    getDeffensiveReduction : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        return 0;
    },

    canBeRepaired : function(co, unit, posX, posY, map)
    {
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        return true;
    },

    getCostModifier : function(co, id, baseCost, posX, posY, map)
    {
        return 0;
    },

    getMovementcostModifier : function(co, unit, posX, posY, map)
    {
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        return 0;
    },

    getMovementFuelCostModifier : function(co, unit, fuelCost, map)
    {
        // modifies the fuel cost when moving
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        // fuelCost are the costs needed for the current movement
        return 0;
    },

    getCanMoveAndFire : function(co, unit, posX, posY, map)
    {
        return false;
    },

    gainPowerstar : function(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
    {
    },

    getCOUnitRange : function(co, map)
    {
        return 0;
    },

    getCOUnits : function(co, building, map)
    {
        return [];
    },

    getAdditionalBuildingActions : function(co, building, map)
    {
        // called from all co's for a building -> so you can boni from own co and mali from enemy co's here
        // - before an action id will disable the action -> see Mary
        return "";
    },

    getTransportUnits : function(co, unit, map)
    {
        // called to check for additional loading units for a transporter
        // - before an unit id will remove the unit from the loading list
        return [];
    },

    getBonusIncome : function(co, building, income, map)
    {
        return 0;
    },

    getIncomeReduction : function(co, building, income, map)
    {
        return 0;
    },

    getEnemyCostModifier : function(co, id, baseCost, posX, posY, map)
    {
        return 0;
    },

    getBonusLoadingPlace : function (co, unit, x, y, map)
    {
        return 0;
    },

    getEnemyVisionBonus : function (co, unit, x, y, map)
    {
        return 0;
    },

    getEnemyMinFirerangeModifier : function (co, unit, x, y, map)
    {
        return 0;
    },

    getEnemyFirerangeModifier : function (co, unit, x, y, map)
    {
        return 0;
    },

    postBattleActions : function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        // called after damage was dealt to the defender unit.
        // the damage given is the damage was dealt to the unit.
        // gotAttacked means we own the unit which got damage dealt.
    },

    endOfTurn : function(co, map)
    {
        // called at the end of the turn use it to do cool co stuff like caulder's healing :)
    },

    startOfTurn : function(co, map)
    {
        // called at the start of the turn use it to do cool co stuff like caulder's healing :)
    },

    postAction: function(co, action, map)
    {
        // called after the action was performed
    },

    getEnemyRepairCostModifier : function(co, unit, map)
    {
        return 0.0;  // percentage modifier of the unit cost e.g. 0.2 means 20% more expensive
    },

    getRepairCostModifier : function(co, unit, map)
    {
        return 0.0;  // percentage modifier of the unit cost e.g. 0.2 means 20% more expensive
    },

    // Perk - Intel
    getDescription : function()
    {
        return "";
    },
    getIcon : function()
    {
        return "";
    },
    getName : function()
    {
        return "";
    },
    getGroup : function()
    {
        return qsTr("General");
    },
}
