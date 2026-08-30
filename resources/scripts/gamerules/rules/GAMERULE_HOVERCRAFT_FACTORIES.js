var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("Enables if hovercrafts can be build in factories.");
        }
        else
        {
            return qsTr("Enables if hovercrafts can be build in harbours.");
        }
    };
    this.getRuleName = function(buggyVar, itemNumber, rule)
    {
        if (itemNumber === 0)
        {
            return qsTr("Hovercraft Facties");
        }
        else
        {
            return qsTr("Hovercraft Harbours");
        }
    };
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return ["checkbox", "checkbox"];
    };
    this.getRuleCategory = function(rule, itemNumber)
    {
        return GAMERULE.ruleCategories[GAMERULE.ruleCategoryGamePlay];
    };
    this.getIsDisabled = function(rule)
    {
        return !rule.getRuleValue(0) && !rule.getRuleValue(1);
    }
};

Constructor.prototype = GAMERULE;
var GAMERULE_HOVERCRAFT_FACTORIES = new Constructor();
