var COABILITY_ATTACKHPBONUSUNIT =
{

    getAbilityName : function()
    {
        return qsTr("Unit attack HP bonus");
    },

    getFunctionNameContext : function(coAbility, coGeneratorMenu)
    {
        return "getAttackHpBonus";
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
