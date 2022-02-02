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
        if (id === "OPTIONS")
        {
            map.options();
        }
        else if (id === "SOUND")
        {
            map.changeSound();
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
        data.addData(qsTr("Game Options"), "OPTIONS", "options");
        data.addData(qsTr("Music"), "SOUND", "music");;
    };
}


Constructor.prototype = ACTION;
var ACTION_OPTIONS = new Constructor();
