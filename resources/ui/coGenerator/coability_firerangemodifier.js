var COABILITY_FIRERANGEMODIFIER =
{

    getAbilityName : function()
    {
        return qsTr("All firerange bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getFirerangeModifier";
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
