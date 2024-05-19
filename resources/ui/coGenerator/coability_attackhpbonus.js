var COABILITY_ATTACKHPBONUS =
{

    getAbilityName : function()
    {
        return qsTr("All attack HP bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getAttackHpBonus";
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
