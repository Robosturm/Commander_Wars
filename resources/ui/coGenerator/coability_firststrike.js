var COABILITY_FIRSTSTRIKE =
{

    getAbilityName : function()
    {
        return qsTr("All first strike");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getFirstStrike";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                result = true;\n" +
               "            }\n"
    }
};
