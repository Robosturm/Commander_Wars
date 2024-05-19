var COABILITY_OFFENSIVEBONUS =
{

    getAbilityName : function()
    {
        return qsTr("All offensive bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getOffensiveBonus";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var offBonus = variables.createVariable("offBonus");
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                result = result + " + offBonus.readDataInt32().toString() + ";\n" +
               "            }\n"
    }
};
