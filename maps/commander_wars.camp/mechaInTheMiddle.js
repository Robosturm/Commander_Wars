var Constructor = function() { // scriptStart
    this.immediateStart = function(map) { // immediateStart
        return true;
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
        if (team === 0) { // 0 Victory
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We did it!"), "CO_NANA", GameEnums.COMood_Happy, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The bodies. The piles and piles of bodies. And those are the ones that are still mostly intact."), "co_officier_bd", GameEnums.COMood_Sad, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("They shouldn't have been so mean to me."), "CO_NANA", GameEnums.COMood_Happy, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            // Dialog
        } // Victory End
    }; // victory
    this.turnStart = function(turn, player, map) { // turnStart
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Alright team, we're at the border with Green Earth. Everyone stay careful. Tanks, remember to shadow your indirects; they'll keep you safe."), "CO_NANA", GameEnums.COMood_Happy, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Attention intruders. This fortress is guarded by Napoleon. You cannot defeat my iron defense. Retreat."), "CO_NAPOLEON", GameEnums.COMood_Normal, "#17a195"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hi there Mr. Robot. We're not here to attack you, we just want to free the people of Green Earth."), "CO_NANA", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We too mean you no harm. We're just protecting our borders against this ridiculous invasion."), "CO_JESS", GameEnums.COMood_Normal, "#00c010"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hey, that's mean! I'm not ridiculous."), "CO_NANA", GameEnums.COMood_Normal, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Passage denied. Our resources are enough to defeat all intruders."), "CO_NAPOLEON", GameEnums.COMood_Normal, "#17a195"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So be it. I'll steamroll over both you and the child."), "CO_JESS", GameEnums.COMood_Normal, "#00c010"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hey! I'm the legal age of consent wherever this is taking place. I'm sick of this. LOAD THE CLUSTER MUNITIONS. WE'RE BLOWING EVERYONE UP!"), "CO_NANA", GameEnums.COMood_Normal, "#ff33cc"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            // Dialog
        } // Start Of Turn End
    }; // turnStart
    this.actionDone = function(action, map) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
        var variable1 = variables.createVariable("variable1");
        var variable2 = variables.createVariable("variable2");
    // preconditionend
        if (map.getPlayer(2).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Loss imminent. All remaining forces, retreat to point Elba. We have failed in our primary directive of protecting the Diaspora files."), "CO_NAPOLEON", GameEnums.COMood_Sad, "#20918b"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
            variable1.writeDataBool(true);
        } // Player Defeated End
        if (map.getPlayer(0).getIsDefeated() === true && variable2.readDataBool() === false) {// 0 Player Defeated
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We lost. They're so mean!"), "CO_NANA", GameEnums.COMood_Sad, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("There, there. You're outmatched, but I believe in you. The Metal Army base might have something interesting in it, but you can avoid it if you want. Let Jess expend her forces while you stick to the sides. Take your time, and think carefully and you can win this."), "CO_AMY", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            // Dialog
            variable2.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
