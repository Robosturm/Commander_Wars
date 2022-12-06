var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var co = map.getCurrentPlayer().getCO(0);
        if ((co !== null) && co.canUseSuperpower())
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function(map)
    {
        var text = "";
        if (map &&
            map !== null &&
            map.getCurrentPlayer() !== null &&
            map.getCurrentPlayer().getCO(0) !== null)
        {
            text = replaceTextArgs(qsTr("Superpower of %0"), [map.getCurrentPlayer().getCO(0).getCOName()]);
        }
        else
        {
            text = qsTr("Superpower of CO 0");
        }
        return text;
    };
    this.getIcon = function(map)
    {
        return "superpower";
    };
    this.perform = function(action, map)
    {
        var player = map.getCurrentPlayer();
        var co = player.getCO(0);
        co.activateSuperpower(GameEnums.PowerMode_Superpower);
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_SuperPower);
        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_POWERS.powerUsed("SUPERPOWER");
        }
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };

    this.getName = function()
    {
        return qsTr("Superpower of CO 1");
    };

    this.getDescription = function()
    {
        return qsTr("Activates the superpower of your first CO. Read the CO's superpower for more information.");
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_SUPERPOWER_CO_0 = new Constructor();
