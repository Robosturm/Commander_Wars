var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Options");
    };
    this.getIcon = function()
    {
        return "options";
    };
    this.perform = function(action)
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
        data.addData(qsTr("Game Options"), "OPTIONS", "options");
        data.addData(qsTr("Music"), "SOUND", "music");;
    };
}


Constructor.prototype = ACTION;
var ACTION_OPTIONS = new Constructor();
