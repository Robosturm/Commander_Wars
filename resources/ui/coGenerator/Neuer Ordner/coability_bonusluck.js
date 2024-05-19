var COABILITY_BONUSLUCK =
{

    getAbilityName : function()
    {
        return qsTr("All luck bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getBonusLuck";
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
