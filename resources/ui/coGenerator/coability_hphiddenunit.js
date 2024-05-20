var COABILITY_HPHIDDENUNIT =
{

    getAbilityName : function()
    {
        return qsTr("Unit HP hidden");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getHpHidden";
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
