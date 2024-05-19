var COABILITY_MINFIRERANGEMODIFIER =
{

    getAbilityName : function()
    {
        return qsTr("All minimum firerange  bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getMinFirerangeModifier";
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
