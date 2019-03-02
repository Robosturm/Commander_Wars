var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        if (map.getCurrentPlayer().getCO(0).canUseSuperpower())
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
        return qsTr("Superpower of CO 1");
    };
    this.getIcon = function()
    {
        return "superpower";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().getCO(0).activateSuperpower();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_SUPERPOWER_CO_0 = new Constructor();
