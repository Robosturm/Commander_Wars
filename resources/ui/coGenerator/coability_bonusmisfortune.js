var COABILITY_BONUSMISFORTUNE =
{

    getAbilityName : function()
    {
        return qsTr("All misfortune bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getBonusMisfortune";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var bonus = variables.createVariable("bonus");
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                result = result + " + bonus.readDataInt32().toString() + ";\n" +
               "            }\n"
    }
};
