var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
		if (unit === null)
		{
			return true;
		}
    };
    this.getActionText = function()
    {
        return qsTr("Build");
    };
    this.getIcon = function()
    {
        return "build";
    };
    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    };


    this.perform = function(action)
    {
        
    };

    this.getStepInputType = function(action)
    {
        // supported types are MENU and FIELD
        if (action.getInputStep() === 0)
        {
            return "MENU";
        }
        return "";
    };

    this.getStepData = function(action, data)
    {
        // type of the data object depends on the StepInputType
        data.addData("Infantry 1000", "INFANTRY", 1000, true);
    };
}

Constructor.prototype = ACTION;
var ACTION_BUILD_UNITS = new Constructor();
