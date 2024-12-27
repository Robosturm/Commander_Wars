var CO_PERK =
{
    isActive : function(co)
    {
        var map = co.getMap();
        if (map !== null)
        {
            if (map.getGameRules().getParallelCos())
            {
                return true;
            }
            else if (co.getIsCO0() === true)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
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
    onCOUnitLost : null,

    onUnitDeath : function(co, unit, map)
    {
    },
    onUnitDeath : null,

    getMovementpointModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getMovementpointModifier : null,

    getOtherMovementpointModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getOtherMovementpointModifier : null,

    buildedUnit : function(co, unit, map)
    {
        // called when someone builded a unit -> ACTION_BUILD_UNITS was performed
    },
    buildedUnit : null,

    getFirerangeModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getFirerangeModifier : null,

    getMinFirerangeModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getMinFirerangeModifier : null,

    getCaptureBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getCaptureBonus : null,

    getAttackHpBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getAttackHpBonus : null,

    getVisionrangeModifier: function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getVisionrangeModifier : null,

    getTerrainDefenseModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getTerrainDefenseModifier : null,

    getFirstStrike : function(co, unit, posX, posY, attacker, isDefender, map, atkPosX, atkPosY)
    {
        return false;
    },
    getFirstStrike : null,

    getEnemyTerrainDefenseModifier : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getEnemyTerrainDefenseModifier : null,

    getDamageReduction : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, luckMode, action, map)
    {
        return 0;
    },
    getDamageReduction : null,

    getTrueDamage : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    },
    getTrueDamage : null,

    getPowerChargeBonus : function(co, map)
    {
        return 0;
    },
    getPowerChargeBonus : null,

    canCounterAttack : function(co, attacker, atkPosX, atkPosY,
                                defender, defPosX, defPosY, luckMode, map)
    {
        return GameEnums.CounterAttackMode_Undefined;
    },
    canCounterAttack : null,

    getPerfectVision : function(co, map)
    {
        return false;
    },
    getPerfectVision : null,

    getWeatherImmune : function(co, map)
    {
        // return true if a weather has no effect for the co
        return false;
    },
    getWeatherImmune : null,

    getHpHidden : function(co, unit, posX, posY, map)
    {
        // are the hp hidden of this unit?
        return false;
    },
    getHpHidden : null,

    getRankInfoHidden : function(co, unit, posX, posY, map)
    {
        // are the hp hidden of this unit?
        return false;
    },
    getRankInfoHidden : null,

    getPerfectHpView : function(co, unit, posX, posY, map)
    {
        // are the hp hidden of this unit?
        return false;
    },
    getPerfectHpView : null,

    getRepairBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getRepairBonus : null,

    getOtherRepairBonus : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getOtherRepairBonus : null,

    getBonusLuck : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getBonusLuck : null,

    getBonusMisfortune : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getBonusMisfortune : null,

    getEnemyBonusLuck : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getEnemyBonusLuck : null,

    getEnemyBonusMisfortune : function(co, unit, posX, posY, map)
    {
        return 0;
    },
    getEnemyBonusMisfortune : null,

    getActionModifierList : function(co, unit, map)
    {
        // return empty list as default
        return [];
    },
    getActionModifierList : null,

    getEnemyActionModifierList  : function(co, unit, map)
    {
        // return empty list as default
        return [];
    },
    getEnemyActionModifierList : null,

    activatePower : function(co, map)
    {
    },
    activatePower : null,

    activateSuperpower : function(co, map)
    {
    },
    activateSuperpower : null,

    getFuelCostModifier : function(co, unit, posX, posY, costs, map)
    {
        // modifies the fuel cost at the start of a turn
        return 0;
    },
    getFuelCostModifier : null,

    getOffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    },
    getOffensiveBonus : null,

    getOffensiveReduction : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    },
    getOffensiveReduction : null,

    getDeffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        return 0;
    },
    getDeffensiveBonus : null,

    getDeffensiveReduction : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        return 0;
    },
    getDeffensiveReduction : null,

    canBeRepaired : function(co, unit, posX, posY, map)
    {
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        return true;
    },

    getCostModifier : function(co, id, baseCost, posX, posY, map)
    {
        return 0;
    },
    getCostModifier : null,

    getMovementcostModifier : function(co, unit, posX, posY, map)
    {
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        return 0;
    },
    getMovementcostModifier : null,

    getMovementFuelCostModifier : function(co, unit, fuelCost, map)
    {
        // modifies the fuel cost when moving
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        // fuelCost are the costs needed for the current movement
        return 0;
    },
    getMovementFuelCostModifier : null,

    getCanMoveAndFire : function(co, unit, posX, posY, map)
    {
        return false;
    },
    getCanMoveAndFire : null,

    gainPowerstar : function(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
    {
    },
    gainPowerstar : null,

    getCOUnitRange : function(co, map)
    {
        return 0;
    },
    getCOUnitRange : null,

    getCOUnits : function(co, building, map)
    {
        return [];
    },
    getCOUnits : null,

    getEnemyCOUnits : function(co, building, map)
    {
        return [];
    },
    getEnemyCOUnits : null,

    getAdditionalBuildingActions : function(co, building, map)
    {
        // called from all co's for a building -> so you can boni from own co and mali from enemy co's here
        // - before an action id will disable the action -> see Mary
        return "";
    },
    getAdditionalBuildingActions : null,

    getTransportUnits : function(co, unit, map)
    {
        // called to check for additional loading units for a transporter
        // - before an unit id will remove the unit from the loading list
        return [];
    },
    getTransportUnits : null,

    getBonusIncome : function(co, building, income, map)
    {
        return 0;
    },
    getBonusIncome : null,

    getIncomeReduction : function(co, building, income, map)
    {
        return 0;
    },
    getIncomeReduction : null,

    getEnemyCostModifier : function(co, id, baseCost, posX, posY, map)
    {
        return 0;
    },
    getEnemyCostModifier : null,

    getBonusLoadingPlace : function (co, unit, x, y, map)
    {
        return 0;
    },
    getBonusLoadingPlace : null,

    getEnemyVisionBonus : function (co, unit, x, y, map)
    {
        return 0;
    },
    getEnemyVisionBonus : null,

    getEnemyMinFirerangeModifier : function (co, unit, x, y, map)
    {
        return 0;
    },
    getEnemyMinFirerangeModifier : null,

    getEnemyFirerangeModifier : function (co, unit, x, y, map)
    {
        return 0;
    },
    getEnemyFirerangeModifier : null,

    postBattleActions : function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        // called after damage was dealt to the defender unit.
        // the damage given is the damage was dealt to the unit.
        // gotAttacked means we own the unit which got damage dealt.
    },
    postBattleActions : null,

    endOfTurn : function(co, map)
    {
        // called at the end of the turn use it to do cool co stuff like caulder's healing :)
    },
    endOfTurn : null,

    startOfTurn : function(co, map)
    {
        // called at the start of the turn use it to do cool co stuff like caulder's healing :)
    },
    startOfTurn : null,

    postAction: function(co, action, map)
    {
        // called after the action was performed
    },
    postAction : null,

    getEnemyRepairCostModifier : function(co, unit, map)
    {
        return 0.0;  // percentage modifier of the unit cost e.g. 0.2 means 20% more expensive
    },
    getEnemyRepairCostModifier : null,

    getRepairCostModifier : function(co, unit, map)
    {
        return 0.0;  // percentage modifier of the unit cost e.g. 0.2 means 20% more expensive
    },
    getRepairCostModifier : null,

    // Perk - Intel
    getDescription : function()
    {
        return "";
    },
    getDescription : null,
    getIcon : function()
    {
        return "";
    },
    getIcon : null,
    getName : function()
    {
        return "";
    },
    getName : null,
    getGroup : function()
    {
        return qsTr("General");
    },

    getCosts : function()
    {
        return 1;
    },
}
