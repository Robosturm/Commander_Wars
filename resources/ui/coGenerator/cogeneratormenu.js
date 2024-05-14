var COGENERATORMENU =
{
    coAbilities : null,
    coAbilityIds : ["OFFENSIVEBONUS"],
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
               "            var powerMode = co.getPowerMode();\n"
        ;
    },

    getOffensiveBonusWriteFuncEpilogue : function(coGeneratorMenu)
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
            ret = "            if (powerMode === " + condition.toString() + ")\n" +
                  "            {\n";
        }

        return ret;
    }
};


