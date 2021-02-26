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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("You all right, kid?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, I...I am now. Thank you. My name's Will. I'm a cadet at the academy. ...I mean, I was. Before the meteors."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("A cadet huh? Thought I recognized that jacket. So what happened?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("The academy is gone. One minute I was eating lunch, and then... Everyone died. Instructors and friends and... everybody. I was buried in the mess hall. I've been digging myself out for... I don't know. Weeks, I guess. I looked for survivors, but I only found rubble and bodies. ...I thought I was the only one left."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("You did well to stay alive. I'm impressed."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I didn't want to die."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm Captain Brenner. I command the 12th Battalion, pride of the Rubinelle Army. And this is my second in command..."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("First Lieutenant Lin. Welcome to Brenner's Wolves."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("We've made it our mission to seek out and aid survivors of the disaster. The world is a dangerous place now. I suggest you come with us."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir! That would be fantastic! You won't be sorry about..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you all right?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sorry, I haven't eaten in a long time."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("That was your stomach rumbling? I thought we were under attack. Come on. We'll get you some spare rations. Just don't get too excited. All we have is canned bread."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
            dialog9.queueAnimation(dialog10);
            dialog10.queueAnimation(dialog11);
            dialog11.queueAnimation(dialog12);
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Our world is destroyed."), "CO_RANDOM", GameEnums.COMood_Normal, "#5c5663"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/intro_world.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Fire rained from the sky. The land burned... The earth shook... The oceans raged... The devastation was total."), "CO_RANDOM", GameEnums.COMood_Normal, "#5c5663"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/intro_meteors.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dust covered the earth, blotting out all traces of the sun. It seems impossible that anything could survive."), "CO_RANDOM", GameEnums.COMood_Normal, "#5c5663"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/intro_ruins.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please... Please let me find someone... There's no one here. The world is dead. HELLOOOOOO!! Help me, please! I don't want to be alone..."), "CO_WILL", GameEnums.COMood_Sad, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/intro_will.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait! There's someone there!Where's my radio?! Maybe I can call them and tell them I'm here... Help me! My name is Will! I'm a cadet at the Rubinelle Military Academy! Um, do you read? Over."), "CO_WILL", GameEnums.COMood_Happy, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har!"), "CO_BEAST", GameEnums.COMood_Happy, "#6038a0"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What's so funny?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Look sharp, roaches! We got us a live one! And he's just a punk kid. You know the drill! I want his food, and I want his weapons! Leave his corpse where it falls! Ain't no one going to complain! Gwar har har!"), "CO_BEAST", GameEnums.COMood_Happy, "#6038a0"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Don't do this!"), "CO_WILL", GameEnums.COMood_Sad, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            map.spawnUnit(3, 4, "LIGHT_TANK", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(3, 6, "RECON", map.getPlayer(0), 0); // 0 Spawn Unit
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Give me tactical, Lin."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've got a single soldier being pursued by a pack of raiders. The soldier appears to be one of ours. Shall we intervene?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do it."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir, Captain."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who are you guys?"), "CO_WILL", GameEnums.COMood_Normal, "#f00008"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Skip the introductions. Fall back, and let us handle this."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um...ok."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's the big idea, dogface? This got nothin' to do with you!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's got everything to do with me. I'm a soldier, it's my duty. You remember what duty is, don't you? I think you used to be soldiers."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Duty? Gwar har har! Don't make me laugh! This world's dead, soldier boy, and so is your precious duty!! We stopped being soldiers when the meteors hit. This is our time, and there is no law! We ob! We kill! We're kings!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not anymore."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("And who's gonna stop us? You?"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's right. Your days of preying on survivors are over. We're going to put you down like the rabid dogs you are. You may have forgotten your duty, but we have not."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har! Keep talking, loser! I need a place to aim!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Move out Lin. Let's end this quickly."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
            dialog9.queueAnimation(dialog10);
            dialog10.queueAnimation(dialog11);
            dialog11.queueAnimation(dialog12);
            dialog12.queueAnimation(dialog13);
            dialog13.queueAnimation(dialog14);
            dialog14.queueAnimation(dialog15);
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
        var variable2 = variables.createVariable("variable2");
        var variable2Value = variable2.readDataInt32();
        if (variable2Value === 0){variable2.writeDataInt32(map.getTerrain(11, 5).getUnit().getUniqueID());}
        variable2Value = variable2.readDataInt32();
    // preconditionend
        if (map.getUnit(variable2Value) === null && variable1.readDataBool() === false) {// 11 5 0 Unit Destroyed
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("This... this isn't like training at all! This is the real thing!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
            variable1.writeDataBool(true);
        } // Unit Destroyed End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
