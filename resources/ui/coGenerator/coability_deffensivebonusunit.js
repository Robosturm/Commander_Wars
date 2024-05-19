var COABILITY_DEFFENSIVEBONUSUNIT =
{

    getAbilityName : function()
    {
        return qsTr("Unit defensive bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getDeffensiveBonus";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var defBonus = variables.createVariable("defBonus");
        var unitId = variables.createVariable("unitID").readDataString();
        if (unitId === "")
        {
            unitId = "INFANTRY";
        }
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                if (unitId === \"" + unitId + "\")\n" +
               "                {\n" +
               "                    result = result + " + defBonus.readDataInt32().toString() + ";\n" +
               "                }\n" +
               "            }\n"
    }
};
