var COABILITY_OFFENSIVEBONUSUNIT =
{

    getAbilityName : function()
    {
        return qsTr("Unit offensive bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getOffensiveBonus";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var offBonus = variables.createVariable("offBonus");
        var unitId = variables.createVariable("unitID").readDataString();
        if (unitId === "")
        {
            unitId = "INFANTRY";
        }
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                if (unitId === \"" + unitId + "\")\n" +
               "                {\n" +
               "                    result = result + " + offBonus.readDataInt32().toString() + ";\n" +
               "                }\n" +
               "            }\n"
    }
};
