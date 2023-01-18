var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var co = map.getCurrentPlayer().getCO(0);
        if ((co !== null) && co.canUsePower())
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
            text = replaceTextArgs(qsTr("Power of %0"), [map.getCurrentPlayer().getCO(0).getCOName()]);
        }
        else
        {
            text = qsTr("Power of CO 0");
        }
        return text;
    };
    this.getIcon = function(map)
    {
        return "power";
    };
    this.perform = function(action, map)
    {
        var player = map.getCurrentPlayer();
        var co = player.getCO(0);
        co.activatePower();
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_Power);
        // achievements

        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_POWERS.powerUsed("POWER");
        }
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };

    this.getName = function()
    {
        return qsTr("Power of CO 1");
    };

    this.getDescription = function()
    {
        return qsTr("Activates the Power of your first CO. Read the CO's power for more information.");
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_POWER_CO_0 = new Constructor();
