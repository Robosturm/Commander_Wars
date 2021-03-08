var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Info");
    };
    this.getIcon = function()
    {
        return "help";
    };
    this.perform = function(action)
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
    };
    this.isFinalStep = function(action)
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
    this.getStepInputType = function(action)
    {
        return "MENU";
    };
    this.getStepData = function(action, data)
    {
        var step = action.getInputStep();
        data.addData(qsTr("Victory Info"), "VICTORY", "help");
        data.addData(qsTr("CO Info"), "CO", "co_info");
        data.addData(qsTr("Unit Info"), "UNIT", "unit_info");
        data.addData(qsTr("Game Info"), "GAME", "intel");
        data.addData(qsTr("Attack Log"), "ATTACK", "fire");
        data.addData(qsTr("Rules"), "RULES", "wiki");
        data.addData(qsTr("Wiki"), "WIKI", "wiki");
    };
}


Constructor.prototype = ACTION;
var ACTION_INFO = new Constructor();
