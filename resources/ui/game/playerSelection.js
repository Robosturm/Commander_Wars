var PlayerSelection =
{
    storeCurrentPlayer : function(player)
    {
        var variables = currentMenu.getVariables();
        var playerVariable = variables.createVariable("player");
        playerVariable.writeDataInt32(player)
    },
    readCurrentPlayer : function()
    {
        var variables = currentMenu.getUiParent().getVariables();
        var playerVariable = variables.createVariable("player");
        return playerVariable.readDataInt32();
    },
    getSelectedArmy : function()
    {
        var playerIdx = PlayerSelection.readCurrentPlayer();
        var player = currentMenu.getUiParent().getMap().getPlayer(playerIdx);
        if (player.getPlayerArmySelected())
        {
            return player.getPlayerArmy();
        }
        else
        {
            return 0;
        }
    },
    getArmySelectable : function()
    {
        var playerIdx = PlayerSelection.readCurrentPlayer();
        var playerSelection = currentMenu.getUiParent();
        return !playerSelection.getIsServerNetworkInterface() ||
                playerSelection.getSaveGame() ||
                playerSelection.getIsObserverNetworkInterface() ||
                (playerSelection.getPlayerAiType(playerIdx) > GameEnums.AiTypes_Human && !playerSelection.getIsArmyCustomizationAllowed());

    },
    getIsServerOrSavegameOrCampaignEnabled : function(menu)
    {
        return !((!menu.getIsServerNetworkInterface() && menu.hasNetworkInterface()) ||
                 menu.getSaveGame() ||
                 menu.getIsObserverNetworkInterface() ||
                 menu.getIsCampaign());
    },
    getIsServerOrSavegameOrCampaignAiEnabled : function(menu)
    {
        return !((!menu.getIsServerNetworkInterface() && menu.hasNetworkInterface()) ||
                   menu.getSaveGame() ||
                   menu.getIsObserverNetworkInterface() ||
                  (menu.getMap().getPlayer(loopIdx).getControlType() > GameEnums.AiTypes_Human && menu.getIsCampaign()));
    },
    getIsServerOrSavegameOrCampaignAiOrSingleCoEnabled : function(menu)
    {
        var map = menu.getMap();
        return !((!menu.getIsServerNetworkInterface() && menu.hasNetworkInterface()) ||
                   menu.getSaveGame() ||
                   menu.getIsObserverNetworkInterface() ||
                  map.getGameRules().getSingleCo() ||
                 (map.getPlayer(loopIdx).getControlType() > GameEnums.AiTypes_Human && menu.getIsCampaign()));
    },
    getIsServerOrSavegameOrArmyCustomizationEnabled : function(menu)
    {
        return !((!menu.getIsServerNetworkInterface() && menu.hasNetworkInterface()) ||
                   menu.getSaveGame() ||
                   menu.getIsObserverNetworkInterface() ||
                  !menu.getIsArmyCustomizationAllowed());
    },
    getIsArmyOrPerkSelectionEnabled : function(menu)
    {
        var playerIdx = PlayerSelection.readCurrentPlayer();
        var player = menu.getMap().getPlayer(playerIdx);
        var ai = player.getBaseGameInput();
        var aiType = GameEnums.AiTypes_Open;
        if (ai !== null)
        {
            aiType = ai.getAiType();
        }
        var isServer = menu.getIsServerNetworkInterface();
        return !(menu.getSaveGame() ||                 
                 menu.getIsObserverNetworkInterface() ||
                (player.getControlType() > GameEnums.AiTypes_Human && menu.getIsCampaign()) ||
                (isServer && !menu.isNotServerChangeable(player)) ||
                (!isServer && aiType !== GameEnums.AiTypes_Human))

    }
};
