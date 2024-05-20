var COABILITY_HPHIDDEN =
{

    getAbilityName : function()
    {
        return qsTr("All HP hidden");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getHpHidden";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                result = true;\n" +
               "            }\n"
    }
};
