var COGENERATORMENU =
{
    coAbilities : [],
    getCoAbilities : function()
    {
        return COGENERATORMENU.coAbilities;
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

    writeCoEpilogue : function(coGeneratorMenu)
    {
        var coName = coGeneratorMenu.getCoName()
        var coId = "CO_" + coName.toUpperCase();
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
            data  = data + "qsTr(\"" + powerSentences[i] + "\"),";
        }
        data  = data + "        ];\n" +
                       "};\n"
        ;
        data  = data + "    this.getVictorySentences = function(co)\n" +
                       "    {\n" +
                       "        return [\n"
        ;
        for (var i = 0; i < victorySentences.length; ++i)
        {
            data  = data + "qsTr(\"" + victorySentences[i] + "\"),";
        }
        data  = data + "        ];\n" +
                       "};\n"
        ;
        data  = data + "    this.getDefeatSentences = function(co)\n" +
                       "    {\n" +
                       "        return [\n"
        ;
        for (var i = 0; i < defeatSentences.length; ++i)
        {
            data  = data + "qsTr(\"" + defeatSentences[i] + "\"),";
        }
        data  = data + "        ];\n" +
                       "};\n"
        ;
        data = data + "}\n" +
                      "Constructor.prototype = CO;\n" +
                      "var CO_ADDER = new Constructor();\n";
        return data;
    },
};


