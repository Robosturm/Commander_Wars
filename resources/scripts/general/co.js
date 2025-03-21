var CO =
{
    init : function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    },
    init : null,

    onCOUnitLost : function(co, map)
    {
        // called when a co unit got destroyed
        if (map !== null)
        {
            var gamerules = map.getGameRules();
            co.setPowerFilled(co.getPowerFilled() * (1 - gamerules.getPowerLoose()));
        }
    },

    getCoGroupModifier : function (co, system, unitIds, map)
    {
        var value = 1;
        var length = unitIds.length;
        if (length > 0)
        {
            value = 0;
            for (var i = 0; i < length; ++i)
            {
                value += 1 + Global[co.getCoID()].getAiCoUnitBonus(co, system.getDummyUnit(unitIds[i]), map) * 0.2;
            }
            value /= length;
        }
        if (value <= 0)
        {
            value = 0.1;
        }
        return value;
    },
    getCoGroupModifier : null,

    loadCOMusic : function(co, map)
    {
        // put the co music in here.
    },
    loadCOMusic : null,

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

    canCounterAttack : function(co, attacker, atkPosX, atkPosY,
                                defender, defPosX, defPosY, luckMode, map)
    {
        return GameEnums.CounterAttackMode_Undefined;
    },
    canCounterAttack : null,

    getTrueDamage : function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                             defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },
    getTrueDamage : null,

    getPowerChargeBonus : function(co, map)
    {
        return 0;
    },
    getPowerChargeBonus : null,

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
        // are the co zone and rank hidden?
        return false;
    },
    getPerfectHpView: null,

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
                                 defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },
    getOffensiveBonus : null,

    getOffensiveReduction : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },
    getOffensiveReduction : null,

    getDeffensiveBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckMode, map)
    {
        return 0;
    },
    getDeffensiveBonus : null,

    getDeffensiveReduction : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isAttacker, action, luckMode, map)
    {
        return 0;
    },
    getDeffensiveReduction : null,

    canBeRepaired : function(co, unit, posX, posY, map)
    {
        // called from all co's for a unit -> so you can apply boni from own co and mali from enemy co's here
        return true;
    },
    canBeRepaired : null,

    getCostModifier : function(co, id, baseCost, posX, posY, map)
    {
        return 0;
    },
    getCostModifier : null,

    getEnemyCostModifier : function(co, id, baseCost, posX, posY, map)
    {
        return 0;
    },
    getEnemyCostModifier : null,

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

    getCOArmy : function(map)
    {
        return "OS";
    },

    starFundsCost = 10000,
    starHpCost = 10.0,
    getStarGain : function(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
    {
        var gamerules = map.getGameRules();
        var powerCostIncrease = gamerules.getPowerUsageReduction();
        var multiplier = 1 / (1.0 + co.getPowerUsed() * powerCostIncrease);
        var gainMode = gamerules.getPowerGainMode();
        var gainZone = gamerules.getPowerGainZone();
        var baseValue = 0;
        // select gain value
        if (gainMode === GameEnums.PowerGainMode_Money)
        {
            baseValue = fundsDamage / CO.starFundsCost;
            if (!defender)
            {
                // reduce damage for attacker
                baseValue *= 0.5;
            }
        }
        else if (gainMode === GameEnums.PowerGainMode_Money_OnlyAttacker)
        {
            if (!defender)
            {
                // only charge for attacker
                baseValue = fundsDamage / CO.starFundsCost;
            }
        }
        else if (gainMode === GameEnums.PowerGainMode_Hp)
        {
            baseValue = hpDamage / CO.starHpCost;
            if (!defender)
            {
                // reduce damage for attacker
                baseValue *= 0.5;
            }
        }
        else if (gainMode === GameEnums.PowerGainMode_Hp_OnlyAttacker)
        {
            if (!defender)
            {
                // only charge for attacker
                baseValue = hpDamage / CO.starHpCost;
            }
        }
        var powerGain = baseValue * multiplier;
        if (gainZone === GameEnums.PowerGainZone_Global)
        {
            // do nothing
        }
        else if (gainZone === GameEnums.PowerGainZone_GlobalCoZoneBonus)
        {
            if (!co.inCORange(Qt.point(x, y), null))
            {
                // reduce power meter gain when not in co range
                powerGain *= 0.5;
            }
        }
        else if (gainZone === GameEnums.PowerGainZone_OnlyCoZone)
        {
            if (!co.inCORange(Qt.point(x, y), null))
            {
                // no power gain outside co-zone
                powerGain = 0;
            }
        }
        var chargeBonus = co.getPowerChargeBonus();
        return powerGain * (100 + chargeBonus) / 100;
    },

    getStarCost : function(co, map)
    {
        var startCost = 10000;
        if (map !== null)
        {
            var gamerules = map.getGameRules();
            var powerCostIncrease = gamerules.getPowerUsageReduction();
            var costIncrease = 1.0 + co.getPowerUsed() * powerCostIncrease;
            var gainMode = gamerules.getPowerGainMode();
            if (gainMode === GameEnums.PowerGainMode_Money ||
                    gainMode === GameEnums.PowerGainMode_Money_OnlyAttacker)
            {
                startCost = costIncrease * CO.starFundsCost;
            }
            else if (gainMode === GameEnums.PowerGainMode_Hp ||
                     gainMode === GameEnums.PowerGainMode_Hp_OnlyAttacker)
            {
                startCost = costIncrease * CO.starHpCost;
            }
        }
        return startCost;
    },

    gainPowerstar : function(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
    {
        var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
        co.setPowerFilled(co.getPowerFilled() + powerGain)
    },

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

    getBonusLoadingPlace : function (co, unit, x, y, map)
    {
        return 0;
    },
    getBonusLoadingPlace : null,

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

    onUnitDeath : function(co, unit, map)
    {
    },
    onUnitDeath : null,

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

    // CO - Intel
    getBio : function(co, map)
    {
        return "";
    },
    getBio : null,
    getLongBio : function(co, map)
    {
        return "";
    },
    getLongBio : null,
    getHits : function(co, map)
    {
        return "";
    },
    getHits : null,
    getMiss : function(co, map)
    {
        return "";
    },
    getMiss : null,
    getCODescription : function(co, map)
    {
        return "";
    },
    getCODescription : null,
    getLongCODescription : function(co, map)
    {
        return "";
    },
    getLongCODescription : null,
    getPowerDescription : function(co, map)
    {
        return "";
    },
    getPowerDescription : null,
    getPowerName : function(co, map)
    {
        return "";
    },
    getPowerName : null,
    getSuperPowerDescription : function(co, map)
    {
        return "";
    },
    getSuperPowerDescription : null,
    getSuperPowerName : function(co, map)
    {
        return "";
    },
    getSuperPowerName : null,
    getPowerSentences : function(co, map)
    {
        return [];
    },
    getPowerSentences : null,
    getVictorySentences : function(co, map)
    {
        return [];
    },
    getVictorySentences : null,
    getDefeatSentences : function(co, map)
    {
        return [];
    },
    getDefeatSentences : null,
    getName : function(co, map)
    {
        return "";
    },
    getName : null,

    showDefaultUnitGlobalBoost : function(co, map)
    {
        return true;
    },
    getCustomUnitGlobalBoostCount : function(co, map)
    {
        return 0;
    },
    getCustomUnitGlobalBoostCount : null,
    getCustomUnitGlobalBoost : function(co, index, info, map)
    {
    },
    getCustomUnitGlobalBoost : null,
    showDefaultUnitZoneBoost : function(co, map)
    {
        return true;
    },
    getCustomUnitZoneBoostCount : function(co, map)
    {
        return 0;
    },
    getCustomUnitZoneBoostCount : null,
    getCustomUnitZoneBoost : function(co, index, info, map)
    {
    },
    getCustomUnitZoneBoost : null,

    getCOStyles : function()
    {
        // string array containing the endings of the alternate co style
        
        return [];
    },
    getCOStyles : null,

    // ai hints for using co powers
    /**
      * co              : getting checked
      * powerSurplus    : surplus on the co power e.g 1 if one more star is filled over the normal co power
      * unitCount       : amount of units owned by the player
      * repairUnits     : amount of units that need to be repaired
      * indirectUnits   : amount of indirect units
      * directUnits     : amount of direct units
      * enemyUnits      : amount of enemy units
      * turnMode        : see GameEnums.h AiTurnMode describes the current turn mode on the ai
      * return          : see GameEnums PowerMode return unknown for default fallback -> not recommended
      */
    getAiUsePower(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode, map)
    {
        return CO.getAiUsePowerAtStart(co, powerSurplus, turnMode, map);
    },

    getAiCoUnitBonus : function(co, unit)
    {
        // implement this function for a co to make the ai build more of the co's good units and to increase the chance
        // the ai deploys a co unit in it. The return value is capped at 10 and -10
        return 1;
    },

    getAiCoBuildRatioModifier : function(co, map)
    {
        // multiplier shifting the general indirect to direct unit ratio the ai tries to maintain.
        return 1.0;
    },

    getAiUsePowerAlways : function(co, powerSurplus, map)
    {
        if (co.canUseSuperpower())
        {
            return GameEnums.PowerMode_Superpower;
        }
        else if (powerSurplus <= 0.5 &&
                 co.canUsePower())
        {
            return GameEnums.PowerMode_Power;
        }
        return GameEnums.PowerMode_Off;
    },

    getAiUsePowerAtStart : function(co, powerSurplus, turnMode, map)
    {
        if (turnMode === GameEnums.AiTurnMode_StartOfDay)
        {
            if (co.canUseSuperpower())
            {
                return GameEnums.PowerMode_Superpower;
            }
            else if (powerSurplus <= 0.5 &&
                     co.canUsePower())
            {
                return GameEnums.PowerMode_Power;
            }
        }
        return GameEnums.PowerMode_Off;
    },

    getAiUsePowerAtEnd : function(co, powerSurplus, turnMode, map)
    {
        if (turnMode === GameEnums.AiTurnMode_EndOfDay)
        {
            if (co.canUseSuperpower())
            {
                return GameEnums.PowerMode_Superpower;
            }
            else if (powerSurplus <= 0.5 &&
                     co.canUsePower())
            {
                return GameEnums.PowerMode_Power;
            }
        }
        return GameEnums.PowerMode_Off;
    },

    getAiUsePowerAtUnitCount : function(co, powerSurplus, turnMode, unitCount, count, map)
    {
        if (turnMode === GameEnums.AiTurnMode_StartOfDay &&
            unitCount >= 5)
        {
            if (co.canUseSuperpower())
            {
                return GameEnums.PowerMode_Superpower;
            }
            else if (powerSurplus <= 0.5 &&
                     co.canUsePower())
            {
                return GameEnums.PowerMode_Power;
            }
        }
        return GameEnums.PowerMode_Off;
    },

    getUnitBuildValue : function(co, unitID, map)
    {
        return 0.0;
    },
    getUnitBuildValue : null,

    getAddtionalCoFaces : function()
    {
        return ["co_fanatic",
                "co_cyrus",
                "co_dr_morris",
                "co_major",
                "co_civilian1",
                "co_civilian2",
                "co_civilian3",
                "co_civilian4",
                "co_civilian5",
                "co_civilian6",
                "co_civilian7",
                "co_civilian8",
                "co_officier_os",
                "co_officier_bm",
                "co_officier_ge",
                "co_officier_yc",
                "co_officier_bh",
                "co_officier_ac",
                "co_officier_bd",
                "co_officier_ti",
                "co_officier_dm",];
    },

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
}

