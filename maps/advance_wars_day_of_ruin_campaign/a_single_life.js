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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAAAA! NOTAGAIN! How did those dirtballs survive? This ain't over, ladies! You ain't seen the last of The Beast!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("You OK, Will?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I'm fine, sir. But the girl we found..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who is she?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I don't know. I fond her unconscious near some ruined buildings."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Good work bringing her in."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr(" ..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's your name, sweetie?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Umm... I don't know."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("You don't know? Listen, kid I--"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr(" Captain Brenner! Let me explain..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr(" She lost her memory? Is that what you're saying? It's not surprising, with all that's happened to this world. Sometimes I wish mine was gone too."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's more, sir. It seems the girl has detailed knowledge of our army. I would be interested to know where she obtained her information... Very interested, sir."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Easy, Lin. She's a survivor who needs our help, not an enemy combatant. Will saved her, and we're going to see that she gets someplace safe. Clear?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("She's your responsibility now, Will. Stay with her, and see that she's taken care of."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Understood!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
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
            dialog15.queueAnimation(dialog16);
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
        var variable1 = variables.createVariable("variable1");
        var variable2 = variables.createVariable("variable2");
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("You've got permission to leave camp, Will. Don't wander too far away."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sure thing, Captain Brenner."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm glad you've recovered enough to be up and about."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've got you to thank for it, sir."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, don't push yourself too hard. And be careful. This area's not safe."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, I... I know."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's wrong?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("The world really has changed, has it? This ash is everywhere. The sky is dark. It's like a never-ending twilight has fallen. Even the ground is coated in it. I've never seen so much gray."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ashes to ashes..."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("When the meteors struck, huge clouds of dust and ash filled the sky and blocked out the sun. We haven't seen a single ray since."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("When will the sky return to normal?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Years? Decades? Who can say?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Decades?"), "CO_WILL", GameEnums.COMood_Sad, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't make that face, Will. We'll be OK. As as there's life, there's hope."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("As long as there's life..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Right. The world we know is gone, but we can't give in to despair. Someday things will be set to right. We'll have peace, and the life we had will return. That's what I believe. That's why we must help everyone we can."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Keep it simple--never give up. If you can do that, anything is possible. Got it?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Got it!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's the spirit! Now let's get to work. Lin, I want recon units canvassing the area."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain! I want to help too!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr(" No, I don't think so, Will. It's too dangerous."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, c'mon! Look, I know I'm still just a cadet, but that doesn't matter! I can drive or search or wash dishes or whatever. I just want to help."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("...You've got spirit, I'll give you that. OK, you're in. But if anything happens, get your tail back here. You're too weak to fight yet."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey! Are you all right? Can you hear me?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hang it up kid. We're too late. He's dead. They're all dead."), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("No..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("All these towns are the same. No survivors anywhere."), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("That can't be true! There have to be people out there who need our help. What if Captain Brenner had given up on me? We have to keep looking."), "CO_WILL", GameEnums.COMood_Normal, "#f00008"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hello? Is anyone there?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Move it, kid! Mission's over! We got raiders coming. Let Captain Brenner step in and take care of the hostiles. we stay out here any longer, and they'll be sizing us for body bags."), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Just a little more time. I've got a feeling about this. Captain Brenner always says to never give up."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("All nonmilitary personnel, fall back! This is now a combat zone! Get that tank up there and block the road! Keep that artillery safe!"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog34.loadBackground(""); // 1 ScriptEventDialogItem
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
            dialog15.queueAnimation(dialog16);
            dialog16.queueAnimation(dialog17);
            dialog17.queueAnimation(dialog18);
            dialog18.queueAnimation(dialog19);
            dialog19.queueAnimation(dialog20);
            dialog20.queueAnimation(dialog21);
            dialog21.queueAnimation(dialog22);
            dialog22.queueAnimation(dialog23);
            dialog23.queueAnimation(dialog24);
            dialog24.queueAnimation(dialog25);
            dialog25.queueAnimation(dialog26);
            dialog26.queueAnimation(dialog27);
            dialog27.queueAnimation(dialog28);
            dialog28.queueAnimation(dialog29);
            dialog29.queueAnimation(dialog30);
            dialog30.queueAnimation(dialog31);
            dialog31.queueAnimation(dialog32);
            dialog32.queueAnimation(dialog33);
            dialog33.queueAnimation(dialog34);
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har! You ain't getting away this time! You hear me, soldier boy? You and that punk kid are gonna pay!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey! There's someone over there! Hold on! I'm coming!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (player === 0 && variable1.readDataBool() === true && variable2.readDataBool() === false) {
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's happening?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't worry, you're safe. You're with the Rubinelle Army now."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Rubinelle... Rubinelle is one of the two main countries on this continent. At the last known date, the army employed 1,296,973 men."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr(" What the... How do you know that?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr(" Rubinelle and its neighbor have been at war for a century. Despite several armistices, the fighting has never truly ended. Ten percent of Rubinelle's GDP is expended on weaponry and research. The last recorded assessment of its military strength was 'Excellent'"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Where did you learn that?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I...I don't know. I don't understand."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, OK... Let's talk about it later. Right now we have to get back to camp in one piece."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("All right."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            variable2.writeDataBool(true);
        }
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
        if (map.isPlayersUnitInArea(Qt.rect(11, 6, 1, 1), [0]) && variable1.readDataBool() === false) {// 1 Player in Area
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Can you hear me? Hold on!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is she alive?"), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I... I think so. Her breathing is really faint. Come on! We've got toget her back to camp!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("It ain't worth the trouble, kid. I doubt she's long for this world."), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm not giving up! If you won't help, I'll carry her myself!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you serious?"), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey! Can you hear me? Give me a sign, will you?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's it! I ain't sticking around to rescue a corpse!"), "co_officier_ac", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Open your eyes... I know there's not much to see, but you can't give up. You're alive, you know? And where there's life, there's hope."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("... Who... Who are you?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes! You heard me! You're alive!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("P-please tell me... Who are you?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm Will. I'm with Brenner's Wolves. What's your name?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I don't know."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you mean?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't remember. I don't remember anything."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK, OK, don't worry. You've been through a lot. Everything's going to be fine. Just come with me."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("...All right."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/a_single_life_isabell.png"); // 1 ScriptEventDialogItem
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
            dialog15.queueAnimation(dialog16);
            dialog16.queueAnimation(dialog17);
            // Dialog
            variable1.writeDataBool(true);
        } // Player in Area End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
