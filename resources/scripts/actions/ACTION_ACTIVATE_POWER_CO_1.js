var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        if (map.getCurrentPlayer().getCO(1).canUsePower())
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        return qsTr("Power of CO 2");
    };
    this.getIcon = function()
    {
        return "power";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().getCO(1).activatePower();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_POWER_CO_1 = new Constructor();
