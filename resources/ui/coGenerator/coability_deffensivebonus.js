var COABILITY_DEFFENSIVEBONUS =
{

    getAbilityName : function()
    {
        return qsTr("All defensive bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getDeffensiveBonus";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var defBonus = variables.createVariable("defBonus");
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                result = result + " + defBonus.readDataInt32().toString() + ";\n" +
               "            }\n"
    }
};
