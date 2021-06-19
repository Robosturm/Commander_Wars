var Constructor = function() { // scriptStart
    this.immediateStart = function() { // immediateStart
        return false;
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("AW, man! No! No, don't do that! Aaaawwww...crud. We gotta retreat! We need reinforcements!"), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Bah! Unacceptable! You weak-minded, cowardly, pathetic simpletons! I took care of that traitor Brenner and yet you cannot finish the job?! I'll see you hanged for this! All of you! No one fails Admiral Greyfield!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Shuot up, Greyfield!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("WHAT?! How dare you address me like that!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("You'll never defeat us, you bloated gasbag! You hear me? Never! And you're going to pay for what you did to Captain Brenner!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("What an insolent whelp you are! Davis! Gather all of my forces! I want these worms crushed NOW!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey, Lin. Um...I mean, sir."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("At ease, Will. I'm glad to see that you're looking better."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah. I know I've been pretty pathetic lately, but I'm pulling it together. "), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't be too hard on yourself. This is a rough patch for everyone. It has only been a few days since Captain Brenner... Well, since..."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's OK, Lin. Really."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("I know you took his death harder than most. To be honest, I wondered if you would be able to fight for us again."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, I was in a bad way. But I thought about what the captain would say if he was here, and... Well, that turned it around. I guess I kind of idolized him, huh?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("You could do much worse in that department."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("He would have told me to stop being such a big baby and get to work! Of course, he would have been nice about it... Anyway, that's why I decided to fight. I'll make time to mourn the captain later."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("I may have misjudged you, Will. You're stronger than I thought."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not as strong as you! I saw how you pulled us together after we lost Captain Brenner. I know people say you're kind of coldhearted... Well, never mind. Whatever they say, you're a great leader."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Actually, Will, that brings me to my point. I have something to ask of you."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, OK. What is it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Let's go inside. I don't want any little birds overhearing this."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("YOU WHAT?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("...You're not filling me with confidence here, Will."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-but, Lin! You can't be serious! I can't lead an army!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Brenner's Wolves has lost its commander. We need to find a successor. After considering various options, I feel that you are the best choice for the job."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's crazy, Lin! YOU should be the leader, not me! I'm an idiot!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, are you aware of how many people are currently in this camp?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("You mean exactly? Um... Well, let me think...OK, there are 259 men in the Wolves and 232 civilians in training. Let's see... Oh, we've also got 709 noncombatants."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's also 613 Lazurian troops waiting to be integrated. Add them up, and we're actually larger than a standard battalion. But putting all these people together is going to require a delicate touch. And a young man like you is more likely to be accepted by the Lazurians."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, but..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("And there are civilians to think of. They like and trust you.  ...They don't like me. I left some of them behind. They think I did it out of spite or malice. They think I'd leave them all if it was in the best interest of the Wolves."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("But that's wrong! Listen, I'll talk to them, I'll tell them--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, it's not, Will. It's not wrong. If I had to, I'd leave every single one of them behind. Brenner didn't abandon anyone, but he wasn't a typical soldier."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin, I can't believe..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you think me harsh, Will? If so, you should become our leader."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("No one will understand why I'm in command instead of you."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'll talk to the Rubinelle soldiers. You can talk to the Lazurians. A few civilians may gripe, but they'll follow you in the end."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I don't know, Lin. This seems like a bad idea. I'm just a kid."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's right. You're a kid. You're a wholesome, likeable kid, and that's what we need. We need someone that all of us can rally around. That's all a good leader really is."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sooooo... I'm in charge, but you're really in charge. Is that it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("Precisely. I know you lack military knowledge and all but the most basic tactical awareness. Right now, I'd only trust you with low-level military tasks. Like cleaning."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ouch! Geez, Lin, don't sugarcoat it for me or anything."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What? Too harsh? OK, hold on a second. OK, imagine that I'm a big car, and you're a little car. Now, when the big car--"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stop. Please stop right now. I get it. Please don't talk to me like a little kid."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("We have to tread carefully. Between the civilians and the Lazurians and the lack of food... Our force should be on the verge of collapsing from the inside out."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("Really?! You think they might..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(qsTr("Mutiny? Yes, I think they might. That's why you need to hold them together. At least until we can deal with Greyfield."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog45.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(qsTr("If it helps to keep our forces together, I'll do whatever I can."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog46.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog47 = GameAnimationFactory.createGameAnimationDialog(qsTr("Good. Thank you, Commander."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog47.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog48 = GameAnimationFactory.createGameAnimationDialog(qsTr("Commander...? Huh. It's going to take a bit for me to get used to that, "), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog48.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog49 = GameAnimationFactory.createGameAnimationDialog(qsTr("You'll get used to it, Commander. You have to. Commander."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog49.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog50 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK, OK! I'll get used to it!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog50.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog51 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm glad, Commander."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog51.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
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
            dialog44.queueAnimation(dialog45);
            dialog45.queueAnimation(dialog46);
            dialog46.queueAnimation(dialog47);
            dialog47.queueAnimation(dialog48);
            dialog48.queueAnimation(dialog49);
            dialog49.queueAnimation(dialog50);
            dialog50.queueAnimation(dialog51);
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's our status?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir, following our escape, some troops have deserted. Also, some of the civilians are struggling to keep up with us."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("They'll have to be abandoned. Our priority is to keep moving."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir?!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Now see here! I know I'm not a tactician like you, but we can't simply--"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes. We can. We can and we will. A commander makes difficult decisions, Doctor. That's part of the job. If you don't like it, you're free to leave."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("...So it's like that, is it?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("We have Lazurian troops, but no time to assimilate them. We have civilians and wounded in our ranks, and no way to keep them safe. We're in a bad spot, Doctor. This is far from an optimal fighting force. Right now we need time and space to hunker down and reorganize."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin, I know you're in a difficult position, but I just can't stomach abandoning people. Why, if Captain Brenner were here, I think he would find a way to..."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Go on."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sorry. That was toughtless of me."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're all tired, Doctor. Manners are the least of my concerns. Talk to me. How are the civilians coping?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("They're scared and restless. All of this running has worn them down to the last nerve. I understand their fear. That Greyfield fellow is a complete madman."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yet you don't seem frightened, Doctor."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Me? I didn't realize I was that good of an actor! Oh ho ho ho! No, I'm afraid I'm as scared as I've ever been. But it would do no good to panic. As the civilians' representative, I must maintain an air of calm at all times. I think they just want to know what is going on."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("...All right. I'll tell them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("With Greyfield's men on our heels, the slightest shock could send them into a full panic. Yes, but... Please, Lin. I know that soft speech is not your forte, but do try to tread lightly."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("With Greyfield's men on our heels, the slightest shock could send them into a full panic. Until we get through this crisis, things will be difficult for us all."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please, good people! We must listen to Lieutenant Lin and pull together. This is our darkest hour since the meteors, but we can get through it if we work as one!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Your key to survival is following orders. Do what you're told when you're told. Your main task is providing support to the combat units. Is that clear?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("We ain't soldiers! We're victims! Why we gotta follow your orders?! Bah! This isn't our war! Why ya have to involve us in your fight?!"), "co_civilian4", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("I blame this on Captain Brenner! His ego got in the way of his judgement, and now we're on the run! That Greyfield is a lunatic! He'll track us down and kill us all!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Let me ask you a question. Do you value your lives? "), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("What? What kind of question--"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is not a camp of slaves. This is a camp of free men who can live or die as they please. Anyone who doesn't wish to obey my orders can take their chances with Greyfield. But know this... I am not Captain Brenner. If you can't cut it out there, you're on your own. I won't come back for anyone."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin, please! Must you be so harsh?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's reality, Doctor. I won't temper it for them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, yes, but pehraps you could be less confrontational? You know, throw in some jokes, like I do?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Perhaps you're right. How would you suggest I go about being more... gentle?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think it's a bit late for that!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("I-I..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it, Isabella?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen, everyone... Lin is right. We have to work together. We have to help each other."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! Now that's the attitude we need! Come on, everyone! Let's pull together!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think we just prevented a rebellion."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("The danger's clear to us all, Doctor. It's only a matter of time before this all blows up. Will?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Mmm?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will! Snap out of it!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-but..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("I know how you feel about Brenner, but we've no time to grieve. The enemy is closing in. We need a plan to evade them for a little while longer."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-you're right. I'm sorry."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir! We have an emergency!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, what now?!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("Some of our rear-guard units have been captured by the enemy!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Damn. I hoped we had more time. We can't worry about them now. We have to break camp and get out of here. I want--"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog44.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(qsTr("Too late, sir. We're under attack."), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog45.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is bad. The vanguard's srrounded. All right. First, we need to pull our infantry back to a safe position."), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog46.loadBackground(""); // 1 ScriptEventDialogItem
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
            dialog44.queueAnimation(dialog45);
            dialog45.queueAnimation(dialog46);
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("You treacherous swine are about to get your punishment. Prepare to meet your precious Captain Brenner...in hell!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("We can't give up. We can't! Captain... Why aren't you here? I can't do this without you..,"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella: Oh, Isabella. Hi."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, I'm worried about you."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sorry, Isabella. I didn't mean to worry you. I'll pull myself together."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's OK..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, it's not. Dr. Morris is right. I can't afford to act like this. It's just that... When Captain Brenner saved my life, everything seemed so clear."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you mean?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I owed my life to him. I knew my role was to help him however I could. But now he's dead, and I'm just...lost. I don't know what to do."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, I... I don't know what you should do either. All I know is that... I'm sad. I'm sad because you are."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella, look--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't know why I feel this way! I don't know anything, and I HATE IT! But when you're happy, I am too. And I don't want to be sad anymore."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Thanks, Isabella. You've made everything clear. I have to go."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Where?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner saved my life. Now I must use it to save others. Lin! Everyone! Hold on! I'm coming!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
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
