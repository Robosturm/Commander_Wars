var COABILITY_REPAIRBONUSUNIT =
{

    getAbilityName : function()
    {
        return qsTr("Unit repair bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getRepairBonus";
    },

    writeCoAbilityContent : function(coAbility, coGeneratorMenu)
    {
        var variables = coAbility.getVariables();
        var bonus = variables.createVariable("bonus");
        var unitId = variables.createVariable("unitID").readDataString();
        if (unitId === "")
        {
            unitId = "INFANTRY";
        }
        return COGENERATORMENU.getActiveConditionPrologue(coAbility) +
               "                if (unitId === \"" + unitId + "\")\n" +
               "                {\n" +
               "                    result = result + " + bonus.readDataInt32().toString() + ";\n" +
               "                }\n" +
               "            }\n"
    }
};
