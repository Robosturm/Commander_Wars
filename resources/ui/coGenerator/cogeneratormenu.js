var COGENERATORMENU =
{
    coAbilities : null,
    coAbilityIds : ["COABILITY_OFFENSIVEBONUS",
                    "COABILITY_OFFENSIVEBONUSUNIT",
                    "COABILITY_DEFFENSIVEBONUS",
                    "COABILITY_DEFFENSIVEBONUSUNIT",
                    "COABILITY_FIRERANGEMODIFIER",
                    "COABILITY_FIRERANGEMODIFIERUNIT",
                    "COABILITY_MOVEMENTPOINTMODIFIER",
                    "COABILITY_MOVEMENTPOINTMODIFIERUNIT",
                    "COABILITY_ATTACKHPBONUS",
                    "COABILITY_ATTACKHPBONUSUNIT",
                    "COABILITY_CAPTUREBONUS",
                    "COABILITY_CAPTUREBONUSUNIT",
                    "COABILITY_MINFIRERANGEMODIFIER",
                    "COABILITY_MINFIRERANGEMODIFIERUNIT",
                    "COABILITY_VISIONRANGEMODIFIER",
                    "COABILITY_VISIONRANGEMODIFIERUNIT",
                    "COABILITY_TERRAINDEFENSEMODIFIER",
                    "COABILITY_TERRAINDEFENSEMODIFIERUNIT",
                    "COABILITY_DAMAGEREDUCTION",
                    "COABILITY_DAMAGEREDUCTIONUNIT",
                    "COABILITY_TRUEDAMAGE",
                    "COABILITY_TRUEDAMAGEUNIT",
                    "COABILITY_REPAIRBONUS",
                    "COABILITY_REPAIRBONUSUNIT",
                    "COABILITY_BONUSLUCK",
                    "COABILITY_BONUSLUCKUNIT",
    ],

    //getFirstStrike
    //getHpHidden
    //getPerfectHpView

    //getBonusMisfortune

    //getCostModifier

    coPowermodes : [qsTr("Day to day"),
                    qsTr("Power"),
                    qsTr("Superpower"),],
    getCoAbilitiyIDs : function()
    {
        return COGENERATORMENU.coAbilityIds;
    },
    getCoAbilities : function()
    {
        if (COGENERATORMENU.coAbilities === null)
        {
            COGENERATORMENU.coAbilities = [];
            for (var i = 0; i < COGENERATORMENU.coAbilityIds.length; ++i)
            {
                COGENERATORMENU.coAbilities.push(Global[COGENERATORMENU.coAbilityIds[i]].getAbilityName())
            }
        }
        return COGENERATORMENU.coAbilities;
    },

    getPowermodes : function()
    {
        return COGENERATORMENU.coPowermodes;
    },

    getOffensiveBonusWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,\n" +
               "                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(atkPosX, atkPosY), attacker);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = attacker.getUnitID();\n"
        ;
    },

    getOffensiveBonusWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getDeffensiveBonusWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,\n" +
               "                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(defPosX, defPosY), defender);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = defender.getUnitID();\n"
        ;
    },

    getDeffensiveBonusWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getMovementpointModifierWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getMovementpointModifier = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getMovementpointModifierWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getFirerangeModifierWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getFirerangeModifier = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getFirerangeModifierWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getMinFirerangeModifierWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getMinFirerangeModifier = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getMinFirerangeModifierWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getCaptureBonusWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getCaptureBonus = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getCaptureBonusWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getAttackHpBonusWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getAttackHpBonus = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getAttackHpBonusWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getVisionrangeModifierWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getVisionrangeModifier = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getVisionrangeModifierWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getTerrainDefenseModifierWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getTerrainDefenseModifier = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getTerrainDefenseModifierWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getFirstStrikeWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getFirstStrike = function(co, unit, posX, posY, attacker, isDefender, map, atkPosX, atkPosY)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getFirstStrikeWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getDamageReductionWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getDamageReduction = function(co, attacker, atkPosX, atkPosY,\n" +
               "                                      defender, defPosX, defPosY, isDefender, luckMode, action, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(defPosX, defPosY), defender);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = defender.getUnitID();\n"
        ;
    },

    getDamageReductionWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getTrueDamageWriteFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getTrueDamage = function(co, attacker, atkPosX, atkPosY,\n" +
               "                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(atkPosX, atkPosY), attacker);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = attacker.getUnitID();\n"
        ;
    },

    getTrueDamageWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getHpHiddenFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getHpHidden = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getHpHiddenWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getRankInfoHiddenFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getRankInfoHidden = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getRankInfoHiddenWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getPerfectHpViewFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getPerfectHpView = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getPerfectHpViewWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getRepairBonusFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getRepairBonus = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getRepairBonusWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getBonusLuckFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getBonusLuck = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getBonusLuckWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getBonusMisfortuneFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getBonusMisfortune = function(co, unit, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n" +
               "            var unitId = unit.getUnitID();\n"
        ;
    },

    getBonusMisfortuneWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getCostModifierFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getCostModifier = function(co, id, baseCost, posX, posY, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var inCoRange = co.inCORange(Qt.point(posX, posY), unit);\n" +
               "            var powerMode = co.getPowerMode();\n"
        ;
    },

    getCostModifierWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    getBonusIncomeFuncPrologue : function(coGeneratorMenu)
    {
        return "    this.getBonusIncome = function(co, building, income, map)\n" +
               "    {\n" +
               "        var result = 0;\n" +
               "        if (CO.isActive(co))\n" +
               "        {\n" +
               "            var powerMode = co.getPowerMode();\n"
        ;
    },

    getBonusIncomeWriteFuncEpilogue : function(coGeneratorMenu)
    {
        return "        }\n" +
               "        return result;\n" +
               "    };\n"
    },

    writeCoPrologue : function(coGeneratorMenu)
    {
        var coName = coGeneratorMenu.getCoName()
        var coId = "CO_" + coName.toUpperCase();

        var data = "var Constructor = function()\n" +
                   "{\n" +
                   "    this.init = function(co, map)\n" +
                   "    {\n" +
                   "        co.setPowerStars(" + coGeneratorMenu.getPowerStars().toString() +");\n" +
                   "        co.setSuperpowerStars(" + coGeneratorMenu.getSuperpowerStars().toString() + ");\n" +
                   "    };\n" +
                   "    this.getCOUnitRange = function(co, map)\n" +
                   "    {\n" +
                   "        return " + coGeneratorMenu.getCoUnitRange().toString() + ";\n" +
                   "    };\n" +
                   "    this.getCOArmy = function()\n" +
                   "    {\n" +
                   "        return \"" + coGeneratorMenu.getCoArmy() + "\";\n" +
                   "    };\n"
        ;
        return data;
    },

    getCoId : function(coGeneratorMenu)
    {
        return "CO_" + coGeneratorMenu.getCoId().toUpperCase();
    },

    writeCoEpilogue : function(coGeneratorMenu)
    {
        var coName = coGeneratorMenu.getCoName()

        var powerSentences = coGeneratorMenu.getCoPowerSentences();
        var victorySentences = coGeneratorMenu.getCoVictorySentences();
        var defeatSentences = coGeneratorMenu.getCoDefeatSentences();
        var data = "    // CO - Intel\n" +
                   "    this.getName = function()\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoName() + "\");\n" +
                   "    };\n" +
                   "    this.getBio = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoBio() + "\");\n" +
                   "    };\n" +
                   "    this.getHits = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoHits() + "\");\n" +
                   "    };\n" +
                   "    this.getMiss = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoMiss() + "\");\n" +
                   "    };\n" +
                   "    this.getCODescription = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoDescription() + "\");\n" +
                   "    };\n" +
                   "    this.getLongCODescription = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoLongDescription() + "\");\n" +
                   "    };\n" +
                   "    this.getPowerDescription = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoPowerDescription() + "\");\n" +
                   "    };\n" +
                   "    this.getPowerName = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoPowerName() + "\");\n" +
                   "    };\n" +
                   "    this.getSuperPowerDescription = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoSuperpowerDescription() + "\");\n" +
                   "    };\n" +
                   "    this.getSuperPowerName = function(co)\n" +
                   "    {\n" +
                   "        return qsTr(\"" + coGeneratorMenu.getCoSuperpowerName() + "\");\n" +
                   "    };\n"
        ;
        data  = data + "    this.getPowerSentences = function(co)\n" +
                       "    {\n" +
                       "        return [\n"
        ;
        for (var i = 0; i < powerSentences.length; ++i)
        {
            data  = data + "qsTr(\"" + powerSentences[i] + "\"),\n";
        }
        data  = data + "        ];\n" +
                       "    };\n"
        ;
        data  = data + "    this.getVictorySentences = function(co)\n" +
                       "    {\n" +
                       "        return [\n"
        ;
        for (var i = 0; i < victorySentences.length; ++i)
        {
            data  = data + "qsTr(\"" + victorySentences[i] + "\"),\n";
        }
        data  = data + "        ];\n" +
                       "    };\n"
        ;
        data  = data + "    this.getDefeatSentences = function(co)\n" +
                       "    {\n" +
                       "        return [\n"
        ;
        for (var i = 0; i < defeatSentences.length; ++i)
        {
            data  = data + "qsTr(\"" + defeatSentences[i] + "\"),\n";
        }
        data  = data + "        ];\n" +
                       "    };\n"
        ;
        data = data + "}\n" +
                      "Constructor.prototype = CO;\n" +
                      "var " + COGENERATORMENU.getCoId(coGeneratorMenu) + " = new Constructor();\n";
        return data;
    },

    loadCoDataCallback : function(menu, file)
    {
        menu.loadCoData(file);
    },

    storeCoDataCallback : function(menu, folder)
    {
        menu.storeCoData(folder, COGENERATORMENU.getCoId(menu));
    },

    generateCoCallback : function(menu, folder)
    {
        menu.generateCo(folder, COGENERATORMENU.getCoId(menu));
    },

    getActiveConditionPrologue : function(coAbility)
    {
        var inCoRange = coAbility.getIncoRange();
        var condition = coAbility.getActiveCondition();
        var ret = "";
        if (inCoRange)
        {
            ret = "            if (powerMode === " + condition.toString() + " && inCoRange)\n" +
                  "            {\n";
        }
        else
        {
            ret = "            if (powerMode === " + condition.toString() + " && (map === null || (map !== null && map.getGameRules().getCoGlobalD2D())))\n" +
                  "            {\n";
        }

        return ret;
    }
};


