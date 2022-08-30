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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Everyone, pull back. Faster! If the enemy doesn’t get you, I will, so move it!"), "CO_SMITAN", GameEnums.COMood_Normal, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Sir? High command’s not going to be happy about us abandoning our post."), "co_officier_ge", GameEnums.COMood_Normal, "#00c010"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We’ll bring them the scans we got of their new tech; that’ll save our hides. If the hippies don’t get us first. So move!"), "CO_SMITAN", GameEnums.COMood_Normal, "#00c010"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("*meanwhile*"), "CO_EMPTY_PF", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("People of Green Earth, you're free. Your dictators can't hurt you any more!"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The only one oppressing us is you!"), "co_officier_yc", GameEnums.COMood_Normal, "#00c010"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Green Earth won't just let this slide. You'll pay!"), "co_civilian1", GameEnums.COMood_Normal, "#00c010"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What's going on here? Why aren't they celebrating their freedom? I need to do more research."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Sir, Pink Frontier soldiers are-"), "co_officier_ge", GameEnums.COMood_Normal, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I see ‘em. These pinko hippies are probably just here to rebalance their chakras and have a few bong water enemas."), "CO_SMITAN", GameEnums.COMood_Normal, "#00c010"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We’re not here for bong water enemas; we’re here for justice!"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I tried to say, they’re live on comms now."), "co_officier_ge", GameEnums.COMood_Normal, "#00c010"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I see… Well, I don’t know about justice. Looks like an invasion to me."), "CO_SMITAN", GameEnums.COMood_Normal, "#00c010"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We’re liberating the innocent people of Green Earth by putting a stop to your dictatorship."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You had too many hits of the janky juice? You’ve got no ships or aircraft. How are you planning on getting over here?"), "CO_SMITAN", GameEnums.COMood_Normal, "#00c010"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("With science!"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (map.getTerrain(18, 2).getBuilding().getOwner() !== null && map.getTerrain(18, 2).getBuilding().getOwner().getPlayerID() === 0 && variable1.readDataBool() === false) {// 0 Building Captured
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why are you attacking us?"), "co_civilian6", GameEnums.COMood_Normal, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You leave the mayor alone!"), "co_civilian7", GameEnums.COMood_Normal, "#00c010"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We're fighting to liberate them and they're still against us? How did the Green Earth dictatorship get them this brainwashed?"), "CO_AMY", GameEnums.COMood_Sad, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            // Dialog
            variable1.writeDataBool(true);
        } // Building Captured End
        if (map.getPlayer(0).getIsDefeated() === true && variable2.readDataBool() === false) {// 0 Player Defeated
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You mess with Smitan, you get a smitin'."), "CO_SMITAN", GameEnums.COMood_Normal, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't get it. My plan was solid: use the hovercraft to overwhelm the enemy before they can deploy a response. We can't give up on freeing the people of Green Earth. I'll regather my forces and try this again, hopping from island to island and taking them as I have the forces."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            // Dialog
            variable2.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
