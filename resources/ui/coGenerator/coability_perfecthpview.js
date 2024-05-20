var COABILITY_PERFECTHPVIEW =
{

    getAbilityName : function()
    {
        return qsTr("All perfect HP view");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getPerfectHpView";
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
