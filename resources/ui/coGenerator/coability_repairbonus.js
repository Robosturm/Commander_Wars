var COABILITY_REPAIRBONUS =
{

    getAbilityName : function()
    {
        return qsTr("All repair bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getRepairBonus";
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
