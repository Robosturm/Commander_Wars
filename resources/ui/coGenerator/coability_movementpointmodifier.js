var COABILITY_MOVEMENTPOINTMODIFIER =
{

    getAbilityName : function()
    {
        return qsTr("All movementpoint bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getMovementpointModifier";
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
