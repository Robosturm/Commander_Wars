var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Info");
    };
    this.getIcon = function(map)
    {
        return "help";
    };
    this.perform = function(action, map)
    {
        action.startReading();
        var id = action.readDataString();
        if (id === "VICTORY")
        {
            map.victoryInfo();
        }
        else if (id === "CO")
        {
            map.showCOInfo();
        }
        else if (id === "GAME")
        {
            map.showGameInfo(action.getPlayer());
        }
        else if (id === "ATTACK")
        {
            map.showAttackLog(action.getPlayer());
        }
        else if (id === "UNIT")
        {
            map.showUnitInfo(action.getPlayer());
        }
        else if (id === "WIKI")
        {
            map.showWiki();
        }
        else if (id === "RULES")
        {
            map.showRules();
        }
        else if (id === "STATISTICS")
        {
            map.showUnitStatistics(action.getPlayer());
        }
        else if (id === "CALCULATOR")
        {
            map.showDamageCalculator();
        }
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        var step = action.getInputStep();
        if (step === 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    };
    this.getStepInputType = function(action, map)
    {
        return "MENU";
    };
    this.getStepData = function(action, data, map)
    {
        var step = action.getInputStep();
        data.addData(qsTr("Victory Info"), "VICTORY", "help");
        data.addData(qsTr("CO Info"), "CO", "co_info");
        data.addData(qsTr("Unit Info"), "UNIT", "unit_info");
        data.addData(qsTr("Unit Statistics"), "STATISTICS", "statistics");
        data.addData(qsTr("Game Info"), "GAME", "intel");
        data.addData(qsTr("Attack Log"), "ATTACK", "icon_fire");
        data.addData(qsTr("Rules"), "RULES", "rules");
        data.addData(qsTr("Wiki"), "WIKI", "wiki");
        data.addData(qsTr("Calculator"), "CALCULATOR", "calculator");

    };
}


Constructor.prototype = ACTION;
var ACTION_INFO = new Constructor();
