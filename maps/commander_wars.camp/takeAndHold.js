var Constructor = function() { // scriptStart
    this.immediateStart = function(map) { // immediateStart
        return false;
    }; // immediateStart
    this.getVictoryInfo = function(map) // victoryInfo
    {
        var variables = map.getGameScript().getVariables();
        var textData = variables.createVariable("victory_info");
        return textData.readDataString();
    }; // victoryInfo
    this.victory = function(team, map) { // victory
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
    }; // victory
    this.turnStart = function(turn, player, map) { // turnStart
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (turn === 3 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 6 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 9 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 12 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 15 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 18 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 21 && player === 0) { // 0 Start Of Turn
            map.getGameRules().changeWeather(3, map.getPlayerCount() * 1); // 0 Change Weather
        } // Start Of Turn End
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander, enemy forces have taken over our oil field. It seems they've also destroyed the surrounding village."), "CO_EMPTY_BD", GameEnums.COMood_Sad, "#bc8248"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Those poor villagers did nothing wrong."), "CO_ZANDRA", GameEnums.COMood_Sad, "#bc8248"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("There is also an army that is fast approaching this area."), "CO_EMPTY_BD", GameEnums.COMood_Sad, "#bc8248"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We need to stop them from destroying other villagers."), "CO_ZANDRA", GameEnums.COMood_Normal, "#bc8248"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We have until turn 11 before the reinforcements come and it will become too much for us."), "CO_EMPTY_BD", GameEnums.COMood_Sad, "#bc8248"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Sandstorms happen every 3 turns in this area, which is my specialty. It was their mistake to attack us."), "CO_ZANDRA", GameEnums.COMood_Normal, "#bc8248"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            // Dialog
        } // Start Of Turn End
        if (turn === 11 && player === 0) {
            if (map.getTerrain(19, 4).getBuilding().getOwner() !== null && map.getTerrain(19, 4).getBuilding().getOwner().getPlayerID() === 0 &&
                map.getTerrain(21, 4).getBuilding().getOwner() !== null && map.getTerrain(21, 4).getBuilding().getOwner().getPlayerID() === 0 &&
                map.getTerrain(19, 14).getBuilding().getOwner() !== null && map.getTerrain(19, 14).getBuilding().getOwner().getPlayerID() === 0 &&
                map.getTerrain(21, 14).getBuilding().getOwner() !== null && map.getTerrain(21, 14).getBuilding().getOwner().getPlayerID() === 0)
            {
                map.spawnUnit(2, 8, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(2, 10, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(1, 8, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(1, 10, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(2, 7, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(2, 9, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(2, 11, "HEAVY_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                if(map.getTerrain(0, 9).getUnit() !== null){map.getTerrain(0, 9).getUnit().setAiMode(GameEnums.GameAi_Offensive);} // 0 Change Unit AI
                map.replaceTerrain("DESERT_PATH", 16, 12, false, true); // 0 Modify Terrain
                map.replaceTerrain("DESERT_PATH", 16, 6, false, true); // 0 Modify Terrain
                map.replaceTerrain("DESERT_PATH", 24, 12, false, true); // 0 Modify Terrain
                map.replaceTerrain("DESERT_PATH", 24, 6, false, true); // 0 Modify Terrain
                map.spawnUnit(0, 8, "K_HELI", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(0, 10, "K_HELI", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(8, 7, "INFANTRY", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(8, 8, "INFANTRY", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(8, 9, "INFANTRY", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(8, 10, "INFANTRY", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(8, 11, "INFANTRY", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(7, 8, "SNIPER", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(7, 9, "SNIPER", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(7, 10, "SNIPER", map.getPlayer(1), 0); // 0 Spawn Unit
                // Dialog
                var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Alright boys. We've chased down our enemy to here. Let's clean them up."), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
                // Dialog
                map.spawnUnit(6, 9, "ANTITANKCANNON", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(6, 8, "LIGHT_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
                map.spawnUnit(6, 10, "LIGHT_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
            }
            else
            {
                map.getPlayer(0).defeatPlayer(null);
            }
        }
    }; // turnStart
    this.actionDone = function(action, map) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
