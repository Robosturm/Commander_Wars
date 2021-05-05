var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        if ((co0 !== null) && co0.canUseSuperpower() &&
            (co1 !== null) && co1.canUseSuperpower())
        {
            return true;
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Tagpower");
    };
    this.getIcon = function()
    {
        return "tagpower";
    };
    this.perform = function(action)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        co0.activateSuperpower(GameEnums.PowerMode_Tagpower);
        co1.activateSuperpower(GameEnums.PowerMode_Tagpower);
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_SuperPower);
        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_POWERS.powerUsed("TAGPOWER");
        }
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.getDescription = function()
    {
        return qsTr("Activates the superpower of both CO's. Read the CO's superpower for more information. This will also apply the Tagpower-Synergy. ") +
               qsTr("The synergy gives a hidden firepower boost and a luck boost based on the affinity of the co's.");
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_TAGPOWER = new Constructor();
