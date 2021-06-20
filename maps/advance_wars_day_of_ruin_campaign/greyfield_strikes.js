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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Clearly the enemy had a method to their madness. We have no choise we must retreat."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tell Greyfield what happened here..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Admiral Greyfield! Brenner's Wolves have won the day! Er...I mean... It was probably due to your leadership, s-sir!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I see."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("A-Admiral? You're not...upset at our victory, are you?"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Of course not! How dare you! I am merely displeased Brenner allowed the Lazurians to escape. Until they are wiped from the face of the planet, we will know no peace..."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh! Oh, golly, it's you!"), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm... I'm sorry. Do you know me?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, I doubt you'd be remembering me. I used to run the bakery in this city, don'cha know!"), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh... I'm...I'm sorry. I can't really remember anything."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait! You know Isabella? She's lost her memory. Anything you can tell us about her would be a big help."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, that's just a shame, it is! But I'm afraid I won't be of much use t'ya. I only saw this young lady and her family a few times. Never bought m'pies, don'cha know!"), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("She has a family?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh yah! Had herself a couple'a sisters and a brother, if I'm rememberin' rightly."), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I had a family?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you know their names? Or where they are now?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh no! I don't think anyone here will be tellin' ya any of that. Not quite sure the best way to tell ya this, but... Ya had a bit of a creepy family, if I remember rightly. Folks were scared of 'em."), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, I don't wanna be talkin' poorly about someone's family! Wouldn't be right."), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Please tell me! I want to know!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yah well, we had all kinds of strange rumors flying about! Folks said your family wasn't human! Said they were monsters and the like! ...Don'cha know."), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's...horrible."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh golly, now I gone and done it! But yah, there were a buncha rumors! Folks said that a buncha tough guys caused ya trouble and then died all mysterious-like! And some folks said that if ya got near weapons, they'd explode and such!"), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("But they're just rumors, yah? Don'cha be payin' these things no mind."), "co_civilian3", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen, Isabella, I'm sure that--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who am I? What am I? What if I did terrible things and don't remember it...?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Victory End
    }; // victory

    this.getRandomBuildUnitId = function()
    {
        var units = map.getPlayer(0).getUnits();
        var unitId = units.at(globals.randInt(0, units.size())).getUnitID();
        units.remove();
        return unitId;
    };

    this.turnStart = function(turn, player) { // turnStart
    // precondition
        var variables = map.getGameScript().getVariables();
        var bannedUnit = variables.createVariable("BANNED_UNITID");
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("WELL! Don't just stand there, fool! Give me your report!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Th-thanks to the brave effort of all our units, we crushed the hated Lazurians. Brenner's Wolves fought particularly well. It was only due to their--"), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("SILENCE! I did not summon you before me to hear you sing that damn fool's praises! Where is the 12th Battalion now?"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("On...on the coast, Admiral Greyfield. L-l-looking for the enemy..."), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Send units to assist them. Make it clear that Admiral Greyfield is in command! Brenner will obey my orders to the letter during the next battle!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-but, Admiral..."), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you question me, Davis? Armies need discipline and leadership, and I provide these things! Rogue elements like these...Wolves of Brenner's...must be brought to heel."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-yes, sir! Absolutely right, Admiral. I'll... I'll get right on it."), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain, we have completed the capture of the port. We can now produce naval units!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well done, Will. But the enemy possesses ports also. This may be a long battle. No it won't."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield has sent a token amount of funds and military support. But I think it's mostly for show. I doubt he really supports the 12th Battalion. We'd better make plans to secure our own source of funds and materials."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Got it. Thanks, Lin. All units! Spread out and capture as many neutral bases as you can!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 2) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Advance, my New Rubinelle Army! Failure will not be tolerated!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if ((turn - 2) % 4 === 0 && player === 2) { // 0 Each Day
            // Dialog
            var unitId = gameScript.getRandomBuildUnitId();
            bannedUnit.writeDataString(unitId);
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Report on the units in Brenner's group. Where does their strength lie? ") + Global[unitId].getName() + qsTr("? Understood. Ensure you relay my orders tomorrow!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Each Day End
        if (turn === 3 && player === 0) { // 0 Start Of Turn
            // Dialog
            var unitId = bannedUnit.readDataString();
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner! We're receiving orders from Admiral Greyfield! He says they must be obeyed under penalty of death, sir!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("What?! This is our battle! What does that idiot think he's doing?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sorry, sir! But I'm just the messenger! Please do what he says, sir! I have a wife and kids!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait...the penalty of death applies to you? An enlisted man?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think it applies to all of us, Will. There's no choice. The admiral clearly doesn't like me very much."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is my fault, Captain. I was disrespectful to the admiral..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't blame yourself, Will. Things can't be changed now. I could risk my own neck, but I won't endanger the lives of my men. We'll follow his orders. ...No matter how crazy they are."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("All ") + Global[unitId].getName() + qsTr(", halt! Don't move an inch without my leave!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (turn === 3 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("The enemy units are behaving oddly."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Great! Maybe there's a problem in the ranks!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("I wouldn't want to speculate. Let's just finish the job at hand."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            // Dialog
        } // Start Of Turn End
        if (turn === 6 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Attention, Lazurian commanders! Can anyone hear me?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("..."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("I just hear static... Is this dumb thing broken?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What do you want?"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey! You heard me!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm switching the intercom off now."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait! Please, hold on! I have to talk to you! I need to know why the Lazurians started this war again!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I obey orders. I don't engage in policy discussions."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Orders? But Lazuria is destroyed! Both of our nations are in ruins, and yet we're still at war! Orders don't matter We don't have a reason to fight."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Surely you thought about this! You must know this war is pointless!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's not my concern. Soldiers follow orders, not their hearts. If soldiers acted as they pleased, armies would collapse. We're tools of the government. They tell us what to do, and we--"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("But there IS no government now! I mean, not a real one!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not my problem."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 7 && player === 0) { // 0 Start Of Turn
            // Dialog
            var unitId = bannedUnit.readDataString();
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain? I have more orders from Admiral Greyfield."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Not again! This idiot is risking all of our lives!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain, we can do it anyway! Brenner's Wolves won't be beaten by the Lazurians OR Greyfield!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain, he's right! We can win this!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("All ") + Global[unitId].getName() + qsTr(", halt! Don't move an inch without my leave!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
        } // Start Of Turn End
    }; // turnStart
    this.actionDone = function(action) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var bannedUnit = variables.createVariable("BANNED_UNITID");
        var unitId = bannedUnit.readDataString();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
        var unit = action.getTargetUnit();
        if (unit !== null && unit.getOwner().getPlayerID() === 0 &&
            unit.getUnitID() === unitId)
        {
            // you lost
            map.getPlayer(0).defeatPlayer(null);
            map.getPlayer(2).defeatPlayer(null);
        }
    // preconditionend
        if (map.getPlayer(2).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(0).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
        if (map.getPlayer(0).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(2).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
