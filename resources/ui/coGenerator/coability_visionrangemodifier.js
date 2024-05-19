var COABILITY_VISIONRANGEMODIFIER =
{

    getAbilityName : function()
    {
        return qsTr("All vision range bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getVisionrangeModifier";
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
