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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("What an extraordinary display. You are too much for this old soldier. The war is over for me..."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Finally, the war is over."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tasha. Gage. I'm leaving you in command Protect our troops and our people. But, General--"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("We have no choice. We must surrender or face annihilation. I'll tell them I started this war and that it was my responsibility. I'll tell them that I forced our people to arms."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Never! You fought to protect us all! You can't take the blame for a war you didn't want!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stand down! That's an order, Flight Lieutenant. My final order..."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("General, we can't--"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do not despair. This Captain Brenner is a man of honor. I shall speak to him and stop this madness. Gage? Do you hear what I'm saying?"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("It is an order, sir. And we will carry it out."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("I would ask that you accept defeat and bring this war to an end. There is no need for further bloodshed."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("You are correct, sir. And I will surrender my forces to you on one condition."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is that?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Responsibility and blame for this war rests with me alone. I ask that you show mercy to all troops who served under my banner."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("...A fair request. I accept your terms."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm grateful to you, Captain. I had heard about you, you know? Of the battalion know as Brenner's Wolves. ...And now I know that everything I heard was true. You are a man of unimpeachable honor."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please, do not praise me. I wish only to end this idiotic war and return to helping survivors."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr(" And, Captain? I am grateful that my last opponent was you. Your father would be proud."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("All right, Rube! I'm ready! Now you'll see how a Lazurian faces death!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... I'm sorry, what?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do it! Pull the trigger! I know how cold blooded you Rubes are! What, do you want to shoot me in the back? Fine! I'll turn around! Now do it!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... Look, I don't know what you think I'm like, but the war is over. You surrendered to Captain Brenner, and he's going to honor that. We're not going to shoot unarmed prisoners. That would be horrible."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ha! Where were your noble thoughts after the meteor strike?! You started this war! You offered peace and then stabbed us in the back! You killed CIVILIANS! You and all the other Rubes! I saw it with my own eyes!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("What? No! No, we would... Rubinelle would never attack civilian targets!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tell that to your beloved Greyfield! He spared no one. Women, children...even my own brother John."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's...that's not...possible."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, really? It's not? You don't sound very convinced of that, kid. Listen to me. Greyfield is evil. There is nothing he won't do to seize power. I can't forgive him, and I can't forgive you for fighting at his side. That's why we fought you. To avenge the deaths of the Lazurian innocents!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner? Admiral Greyfield has arrived."), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Too late to help us..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Brenner. ...I suppose you did passably well.Hear me! The New Rubinelle Army has won a decisive and historic victory! And now I, Admiral Greyfield, would speak to the leader of this Lazurian rabble!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("I am General Forsythe, commander of the Lazurian-- AAAAAAARRGGH!"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("You and your wretched army are a pathetic spectacle! A disgrace!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Enough! General Forsythe has surrendered honorably and brought the war to an end. Striking a prisoner is against all rules of military law!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Military law? Meaningless pieces of paper! We are the victors! We have crushed our enemies likes flies! The only law of this land is the law of Greyfield."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Now, Captain Brenner! I will usher in the rule of King Greyfield the Mighty! Waylon! Dispose of this trash."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aaargh!"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("General Forsythe!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner.. R-remember... Your p-promise..."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Damn you, Greyfield! He had surrendered! He was unarmed! Why did you shoot him?!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("He was a Lazurian, and death is all he deserved."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're no leader ...you're a coward! Anf if no one elese will stand up, I'll stop you--"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aw, shucks. I hate to be the bad guy and all, but you're gonna have to stop right there. Don't make me shoot you, now! It's been a long day, and I'm tuckered."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("You will pay for this, Greyfield..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen to you! Squawkin' away like a crow on a wire. This is Greyfield's world now, brother! Ain't no one going to help you!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("Take the Lazurian dogs to the internment center for execution. My great victory is almost complete!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
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
            dialog34.queueAnimation(dialog35);
            dialog35.queueAnimation(dialog36);
            dialog36.queueAnimation(dialog37);
            dialog37.queueAnimation(dialog38);
            dialog38.queueAnimation(dialog39);
            dialog39.queueAnimation(dialog40);
            dialog40.queueAnimation(dialog41);
            dialog41.queueAnimation(dialog42);
            dialog42.queueAnimation(dialog43);
            dialog43.queueAnimation(dialog44);
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("We are approaching the main Lazurian base. This will not be easy. Their forces are led by General Forsythe."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("I learned about him the academy. He was the hero of the Lazurian Seige! But that was twenty years ago... Is he really still fighting?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Forsythe... What are the chances I would be facing him? And here of all places..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you mean, Captain? ...Captain?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin, have we heard anything from Greyfield?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("He's on his way here with the main force of the NRA. We have been ordered to carry out...active surveillance."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um...what's active surveillance?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("It means we push directly into the enemy to assess their strength."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("That doesn't sound like a very good strategy."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's a horrible strategy. At least for us. It would be better to wait for Greyfield to arrive before attacking. I can't figure out if he's a coward or he wants us to die. Perhaps it's both."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I don't think we can rely on his forces anyway. Lin? I'm going to need you at your best. Look sharp and think of a plan."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ah, General Forsythe. How goes your little battle?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("You again, Caulder?"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you still not tempted to try my exotic weapons? You could alter the course of this war with the touch of a button."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Now see here! I told you already--"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Of course, if you used my weapons now, you would kill scores of your own men. But I'm sure that is a sacrifice they would gladly make."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Twenty years ago, I faced Rubinelle in this very spot. Though the battle was long and bloody, it was fought with honor on all sides. No prisoners were killed. No unlawful weapons were used. And when the enemy surrendered, we treated them with dignity! War is cruel and pitiless, but soldiers are still men. Your suggestion is an insult to all who lay their lives down on this soil."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("...You are a strange old man. But it seems clear your mind is made up. Farewell, General."), "CO_ANDY", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            // Dialog
        } // Start Of Turn End
        if (turn === 9 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("General Forsythe, can you hear me?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("I say! Am I speaking to the Rubinelle commander?"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("You are. I want to talk to you about the battle that took place twenty years ago. Do you remember who was leading the Rubinelle forces?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("I never knew his name, but he was a truly great soldier. The battle raged for a hundred days, with neither side willing to yield. Our only pause came when we declared a cease-fire to bury our dead. He was a man of honor and courage. I feel I almost knew him."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("He was my father."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I see... Please know I took no pleasure in his death."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("You have nothing to apologize for. He understood the path he walked. After he died here, our government blamed him for the defeat. It seems only his enemies saw him for the good man he was. Thank you for your words, General Forsythe. May luck be with you."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I wish it had not come to this. But it cannot be helped now. If you are truly your father's son, I\"m proud to face you in my final battle."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
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
    this.actionDone = function(action) { // actionConditions
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
