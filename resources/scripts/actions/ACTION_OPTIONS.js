var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Options");
    };
    this.getIcon = function(map)
    {
        return "options";
    };
    this.perform = function(action, map)
    {
        action.startReading();
        var id = action.readDataString();
        var itemInfos = ACTION_OPTIONS.getItemInfos();
        if (id === itemInfos[0][1])
        {
            map.showXmlFileDialog("ui/options/optiongameplaymenu.xml", true);
        }
        else if (id === itemInfos[1][1])
        {
            map.showXmlFileDialog("ui/options/optionaudiomenu.xml", true);
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
        var itemInfos = ACTION_OPTIONS.getItemInfos();
        var length = itemInfos.length;
        for (var i = 0; i < length; ++i)
        {
            data.addData(itemInfos[i][0],
                         itemInfos[i][1],
                         itemInfos[i][2]);
        }
    };
    this.getName = function(map)
    {
        return qsTr("Options");
    };
    this.getDescription = function()
    {
        return qsTr("Allows you to modify several options of the game.");
    };

    this.getItemInfos = function()
    {
        return [[qsTr("Game Options"), "ACTION_OPTIONS_OPTIONS", "options", qsTr("Allows you to modify the game settings.")],
                [qsTr("Audio"), "ACTION_OPTIONS_SOUND", "music", qsTr("Allows you to modify the audio settings.")]]
    };
    this.getSubWikiInfoCount = function()
    {
        return ACTION_OPTIONS.getItemInfos().length;
    };
    this.getSubWikiInfoIcon = function(item)
    {
        return ACTION_OPTIONS.getItemInfos()[item][2];
    };
    this.getSubWikiInfoName = function(item)
    {
        return ACTION_OPTIONS.getItemInfos()[item][0];
    };
    this.getSubWikiInfoDescription = function(item)
    {
        return ACTION_OPTIONS.getItemInfos()[item][3];
    };
    this.getSubWikiInfoId = function(item)
    {
        return ACTION_OPTIONS.getItemInfos()[item][1];
    };
}


Constructor.prototype = ACTION;
var ACTION_OPTIONS = new Constructor();
