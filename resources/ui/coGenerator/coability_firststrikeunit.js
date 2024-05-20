var COABILITY_FIRSTSTRIKEUNIT =
{

    getAbilityName : function()
    {
        return qsTr("Unit first strike");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getFirstStrike";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var unitId = variables.createVariable("unitID").readDataString();
        if (unitId === "")
        {
            unitId = "INFANTRY";
        }
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                if (unitId === \"" + unitId + "\")\n" +
               "                {\n" +
               "                    result = true;\n" +
               "                }\n" +
               "            }\n"
    }
};
