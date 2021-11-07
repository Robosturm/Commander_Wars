var Constructor = function() { // scriptStart
    this.immediateStart = function() { // immediateStart
        return true;
    }; // immediateStart
    this.getVictoryInfo = function() // victoryInfo
    {
        var variables = map.getGameScript().getVariables();
        var textData = variables.createVariable("victory_info");
        return textData.readDataString();
    }; // victoryInfo
    this.victory = function(team) { // victory
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (team === 0) { // 0 Victory
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ifrit-4 calling the Brown Desert Command, target has been sabotaged. We're ready for withdrawal."), "CO_EMPTY_BD", GameEnums.COMood_Normal, "#bc8248"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Excellent. Now retreat to the rendezvous coordinate as planed and await further orders."), "CO_FORSYTHE", GameEnums.COMood_Happy, "#bc8248"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dammit! Contact the Eastern Command! Orange Star is under attack!"), "CO_EMPTY_OS", GameEnums.COMood_Sad, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            // Dialog
        } // Victory End
    }; // victory
    this.turnStart = function(turn, player) { // turnStart
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ifrit-4 calling High Command. We're in position."), "CO_EMPTY_BD", GameEnums.COMood_Normal, "#bc8248"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Very well. Remember our objectives today:"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#bc8248"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Capture the pipe station with the commando in the North-west. The task force in South-east will provide distraction. Keep in mind that there is a Orange Star garrison nearby that will surely respond to our attack. Complete the objectives before they have a chance to reinforce the station."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#bc8248"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's all. \"Operation Quicksand\" has begun, good luck, Ifrit-4."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#bc8248"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Roger that, General. Ifrit-4 off."), "CO_EMPTY_BD", GameEnums.COMood_Normal, "#bc8248"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Commander! These sand people are attacking the pipe station! We can't hold them for long!"), "co_officier_os", GameEnums.COMood_Sad, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("So they have come. Scramble the entire garrison! The pipelines are the lifeline to Orange Star economy!"), "CO_EMPTY_OS", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            // Dialog
        } // Start Of Turn End
    }; // turnStart
    this.actionDone = function(action) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
        var variable1 = variables.createVariable("variable1");
    // preconditionend
        if (map.getTerrain(16, 6).getBuilding().getOwner() !== null && map.getTerrain(16, 6).getBuilding().getOwner().getPlayerID() === 0 && variable1.readDataBool() === false) {// 0 Building Captured            
            map.getPlayer(1).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Building Captured End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
