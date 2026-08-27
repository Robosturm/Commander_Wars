var VICTORYRULE =
{
    getRuleDescription : function(itemNumber, map)
    {
        return "";
    },
    getRuleDescription : null,

    getRuleProgress : function(rule, player, map, item)
    {
        return 0;
    },
    getRuleProgress : null,

    ruleCategoryIdValue : 0,
    ruleCategoryNameValue : 1,
    ruleCategoryTooltipValue : 2,
    ruleCategoryOrderValue : 3,
    ruleCategoryColorValue : 4,
    ruleCategoryInformation :
    [
        // ID -         name -                  tooltip -                                                                   order - color
        ["HQ",          qsTr("HQ"),             qsTr("Victory rules that are primarly affected by capturing enemies HQ's."),    0, "#FFA500"],
        ["UNITS",       qsTr("Units"),          qsTr("Victory rules that are primarly affected by eliminating enemy units."),   1, "#FFA500"],
        ["TURNS",       qsTr("Turn"),           qsTr("Victory rules that are primarly affected by turn limits."),               2, "#FFA500"],
        ["MAP_CONTROL", qsTr("Map Control"),    qsTr("Victory rules that are primarly affected by map control limits."),        3, "#FFA500"],
    ],

    getRuleCategoryInfo : function(id)
    {
        for (var i = 0; i < VICTORYRULE.ruleCategoryInformation.length; ++i)
        {
            if (VICTORYRULE.ruleCategoryInformation[i][VICTORYRULE.ruleCategoryIdValue] === id)
            {
                return VICTORYRULE.ruleCategoryInformation[i];
            }
        }
        return null;
    },

    // optional victory rule menu metadata
    getRuleCategory : function(rule, map)
    {
        var info = VICTORYRULE.getRuleCategoryInfo(rule.getRuleCategoryId());
        if (info !== null)
        {
            return info[VICTORYRULE.ruleCategoryNameValue];
        }
        return "";
    },
    getRuleCategoryId : function(rule, map)
    {
        return "";
    },
    getRuleCategoryColor : function(rule, map)
    {
        var info = VICTORYRULE.getRuleCategoryInfo(rule.getRuleCategoryId());
        if (info !== null)
        {
            return info[VICTORYRULE.ruleCategoryColorValue];
        }
        return "";
    },
    getRuleCategoryTooltip : function(rule, map)
    {
        var info = VICTORYRULE.getRuleCategoryInfo(rule.getRuleCategoryId());
        if (info !== null)
        {
            return info[VICTORYRULE.ruleCategoryTooltipValue];
        }
        return "";
    },
    getRuleCategoryOrder : function(rule, map)
    {
        var info = VICTORYRULE.getRuleCategoryInfo(rule.getRuleCategoryId());
        if (info !== null)
        {
            return info[VICTORYRULE.ruleCategoryOrderValue];
        }
        return 0;
    },
    getRuleOrder : function(rule, map)
    {
        return 0;
    },

    // shown name in the game
    getRuleName : function(itemNumber, map)
    {
        return "";
    },
    getRuleName : null,

    // the type how the rule will be represented in the map selection ui
    getRuleType : function()
    {
        // for now checkbox or spinbox
        return ["checkbox"];
    },
    // defines the default value during map selection for this rule
    getDefaultRuleValue : function(itemNumber, map)
    {
        return 0;
    },
    getDefaultRuleValue : null,

    getInfiniteValue : function(itemNumber, map)
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    },
    getInfiniteValue : null,

    getMaxValue : function(itemNumber)
    {
        return 99999;
    },
    getRuleTargetValue  : function(rule, map, itemNumber, player)
    {
        var ruleTypes = rule.getRuleType();
        if (ruleTypes[0] === "checkbox")
        {
            return 0;
        }
        else
        {
            return rule.getRuleValue(0);
        }
    },
    getRuleTargetCount : function(rule, map)
    {
        return 1;
    },
    // create and initialize the variables for this rule
    init : function(rule, map)
    {
    },
    init : null,

    // sets the rule value of the spinbox
    setRuleValue : function(rule, value, item = 0, map)
    {
        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        variable.writeDataInt32(value);
    },
    // called from the engine when the player changes the value in the selection screen
    getRuleValue : function(rule, item = 0, map)
    {
        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        return variable.readDataInt32();
    },

    // checks if the selected player is declared defeated by this rule
    checkDefeat : function(rule, player, map)
    {
        return GameEnums.DefeatType_Alive;
    },
};
