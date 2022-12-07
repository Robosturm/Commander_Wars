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
    this.getItemInfos = function()
    {
        return [[qsTr("Victory Info"), "ACTION_INFO_VICTORY", "help", qsTr("Shows the current victory info.")],
                [qsTr("CO Info"), "ACTION_INFO_CO", "co_info", qsTr("Shows the infos about the co's of the current game.")],
                [qsTr("Unit Info"), "ACTION_INFO_UNIT", "unit_info", qsTr("Shows information about your current units.")],
                [qsTr("Unit Statistics"), "ACTION_INFO_STATISTICS", "statistics", qsTr("Shows which units have been build and destroyed by all players.")],
                [qsTr("Game Info"), "ACTION_INFO_GAME", "intel", qsTr("Shows the information about the current game.")],
                [qsTr("Attack Log"), "ACTION_INFO_ATTACK", "icon_fire", qsTr("Shows which unit where attacked and has been attacked.")],
                [qsTr("Rules"), "ACTION_INFO_RULES", "rules", qsTr("Shows the current active rules for this game.")],
                [qsTr("Wiki"), "ACTION_INFO_WIKI", "wiki", qsTr("Allows you to search in the wiki of the game.")],
                [qsTr("Calculator"), "ACTION_INFO_CALCULATOR", "calculator", qsTr("Allows you to calculate damage between different units.")],];
    };
    this.getSubWikiInfoCount = function()
    {
        return ACTION_INFO.getItemInfos().length;
    };
    this.getSubWikiInfoIcon = function(item)
    {
        return ACTION_INFO.getItemInfos()[item][2];
    };
    this.getSubWikiInfoName = function(item)
    {
        return ACTION_INFO.getItemInfos()[item][0];
    };
    this.getSubWikiInfoDescription = function(item)
    {
        return ACTION_INFO.getItemInfos()[item][3];
    };
    this.getSubWikiInfoId = function(item)
    {
        return ACTION_INFO.getItemInfos()[item][1];
    };

    this.perform = function(action, map)
    {
        action.startReading();
        var id = action.readDataString();
        var itemInfos = ACTION_INFO.getItemInfos();
        if (id === itemInfos[0][1])
        {
            map.victoryInfo();
        }
        else if (id === itemInfos[1][1])
        {
            map.showCOInfo();
        }
        else if (id === itemInfos[2][1])
        {
            map.showUnitInfo(action.getPlayer());
        }
        else if (id === itemInfos[3][1])
        {
            map.showUnitStatistics(action.getPlayer());
        }
        else if (id === itemInfos[4][1])
        {
            map.showGameInfo(action.getPlayer());
        }
        else if (id === itemInfos[5][1])
        {
            map.showAttackLog(action.getPlayer());
        }
        else if (id === itemInfos[6][1])
        {
            map.showRules();
        }
        else if (id === itemInfos[7][1])
        {
            map.showWiki();
        }
        else if (id === itemInfos[8][1])
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
        var itemInfos = ACTION_INFO.getItemInfos();
        var length = itemInfos.length;
        for (var i = 0; i < length; ++i)
        {
            data.addData(itemInfos[i][0],
                         itemInfos[i][1],
                         itemInfos[i][2]);
        }
    };
    this.getName = function()
    {
        return qsTr("Info");
    };
    this.getDescription = function()
    {
        return qsTr("Allows you to get useful information about the current match.");
    };
}


Constructor.prototype = ACTION;
var ACTION_INFO = new Constructor();
