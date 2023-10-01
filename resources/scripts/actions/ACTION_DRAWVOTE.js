var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return map.getIsMultiplayerMatch();
    };
    this.getActionText = function(map)
    {
        return qsTr("Start draw voting");
    };
    this.getIcon = function(map)
    {
        return "surrender";
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
    this.perform = function(action, map)
    {
        map.startDrawVoting();
    };
    this.getName = function()
    {
        return qsTr("Start draw voting");
    };
    this.getDescription = function()
    {
        return qsTr("Starts a voting for a draw for the match if all human players agree for a draw the match will end in a draw.");
    };
}

Constructor.prototype = ACTION;
var ACTION_DRAWVOTE = new Constructor();
