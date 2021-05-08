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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYYYAAAAAA! They defeated me! This ain't happenin'! The world is over! Why do these do-gooders care what we do?! I'm The Beast! No one says no to me! Not even a bunch of soldier boys! I'll be back, losers! And I'm gonnakill every last one of ya!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("I never miss a thing."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("You did it! You drove off those savages!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't think they'll bother you again."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Thank you, Captain Brenner! I think my village can sleep soundly tonight."), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm glad. Now let's talk about your side of the bargain. We drove them off. Now you need to provide Will and the other civilians with shelter."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I didn't agree to that."), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What are you doing? We saved your village. You promised to take in the civilians."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I said nothing of the sort, Captain. All i said was that if you scared off those savages, then i would CONSIDER it. Isn't that right, people? That's what I said! Your mayor is a man of his word!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah! That's right! That's what he said!"), "co_civilian1", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't believe this!"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain, please! Control yourself! Even you can see that this \"bargain\" of yours affects my entire village. I may be the mayor, but I do not rule by fiat. We are civilized men, after all. We will come to a decision in due course. Until then...you must be patient."), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why, you double-crossing--"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("You have only yourself to blame for your anger, Captain. If you misunderstood my words, then the fault is yours alone. I have done nothing wrong."), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I see."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will you threaten us now? Will you let your guns take the place of your honor? Perhaps we should have taken our chances with the savages!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("...No, Mayor. I won't threaten you. I won't do anything to unnerve these people that you pretend to care for."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wh-what? How dare you!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("I remember you. I remember how you were. Scared. Alone. On the verge of death. Do you remember your words to me when my men gave you food and shelter? I will save others as you have saved me, Brenner. I will construct a village where all can live in peace and rebuild their lives. Do you remember that? Or did I misunderstand your words again?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who are you to say such? You're a killer! A soldier! A relic of the past! I give these people peace. I give them hope. You give nothing but horror and death!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Will?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir, I know you have our best interests at heart, but it's OK. Look, I know it might be safer here in this village, but I don't want to stay. I want to serve with you, Captain. You and the battalion."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Out of the question, Will. It's too dangerous. People who truck with us don't live very long. You're young. Your life is ahead of you."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please, Captain. Please. Let me stay. Let me fight with you."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Honestly, I don't seem to have much choice. And I admire your courage... All right. Lin! Will! Prepare to move out! ...Good luck, Mayor. I think you'll need it."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ruins_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey. How's she doing?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, Will! Thanks for coming again today! She's going to be very pleased."), "co_civilian7", GameEnums.COMood_Normal, "#0098f8"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I hope so. Sometimes, I... I just don't know if she even gets it. You know?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("She looks forward to your visits, Will. When you're here, she is bright and cheerful. Otherwise, she seems a bit...sad."), "co_civilian7", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Really?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Really. But don't worry about it. We'll take care of her. She's going to be OK."), "co_civilian7", GameEnums.COMood_Normal, "#0098f8"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... Hi. How's it going today? I... brought you something! I hope you like it."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("A flower?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah. Well, it's not a real flower. It's plastic. You can't find many real flowers anymore. I found it while I was on a recon mission a few miles over."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's... It's beautiful. What do you call it?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Huh? Oh, I don't know. I'm not much of a flower... scientist...guy. Let me ask Lin."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, I wait! I remember... This is a Cattleya isabella. It's a natural hybrid of the orchid family. Cattleya... Yes, that's it! That will be myname. I will be named after this beautiful flower you brought me. My name is Cattleya"), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Uh...Cattelya? Wow, that's...that's uh... That's kind of hard to say, actually. How about we call you Isabella? That's really pretty. It fits you better."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella? Oh! I love it, Will! I'm so happy!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah? Oh, good! I think it suits you."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("Thank you, Will!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("Boy meets girl. Boy gives girl flower. Boy names girl. ...What's wrong with this world?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lieutenant! Um...what are you doing here?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've been here. The whole time. Juuuust keeping an eye on you."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen up, roaches! We're low on supplies, so it's time to swarm village! Take what ya want! Steal what ya want! Kill who ya want! There's no law but The Beast's law! No prisoners! No mercy! Gwar har har har!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_dust.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir, the mayor was right. Those raiders are camped at an old army base. That's probably where they scavenged their weapons and vehicles."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_dust.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("They lack the discipline of a comptent army unit, but not the fighting skill."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_dust.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Agreed. And we know there are former soldiers and mercenaries among their number."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_dust.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("They may have been fellow soldiers once, but they're just a pack of dogs now. Let's put them down before they do any more harm to the survivors."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_dust.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner! All units have been deployed, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Good. Still no sign of the enemy?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, sir. But this dust prevents us from seeing much of anything. Of course, it keeps us hidden too, so I guess it's not all bad!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("My training was in reconnaissance and intelligence. Leave this to me. All units, move out!"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Scount unit Bravo reporting. Visibility is poor, and we've lost visual contact with the enemy. I can hear something out there... I think they know we're here, sir."), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Understood, Bravo. Get somewhere safe. We're on our way."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Watch the woods, sir. I think the raiders are hiding there."), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Surprise, little man! The beast is coming out of his cave! No one attacks The Beast in his own home! No one! Swarm, roaches! Show no mercy!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Scout unit Brave has been hit!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm trying to raise them on the radio... Bravo, do you read? Over."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Scout unit Bravo reporting! We're taking heavy fire! We can't continue the mission! We have to retreat!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Understood. Double-time it back to base, soldier."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Roger that, sir!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Pay attention, Will. Remember that soldiers are a precious resource. We know there is risk in war, and some casualties are inevitable. But it's our duty to get as many of them home as possible."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
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
