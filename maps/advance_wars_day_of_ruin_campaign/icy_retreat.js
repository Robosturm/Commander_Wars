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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're clear! Now move! Get out across that ice!"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Move it! We can't let Brenner's sacrifice go to waste!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will! Take command of the left flank! We're getting out of here!"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("But, Captain Brenner--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Brenner made his choice. Now move!"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Keep moving! I'll buy you time!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think we list them. But they're going to be looking for us."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Probably."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Any word from Captain Brenner?"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("No. Will is trying to establish radio contact, but so far, nothing."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain! Captain Come in! Can you hear me?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_RANDOM", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner, this is Will! Please respond! Please!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ... ...Will, come... Capt..."), "CO_RANDOM", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin! I got him! I got him! Captain, where are you?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("...126...272... Derelict building in... ...south of ...ice field..."), "CO_RANDOM", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I'm surrounded by...enemies... ...They shot me...couple of times... ...Things didn't go...quite according to plan...eh, Will...?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/brenners_death.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Idiots! Mule-brained bumblers! Pathetic, useless, lack-willed imbeciles! Brenner cannot be allowed to live! I want him found if you have to search every inch of this country!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("W-we think he's somewhere in the ruined city south of the ice, sir! We...we're getting close to--"), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("I didn't ask for excuses! I asked for results! Bring me Brenner! King Greyfield will brook no disobedience to his supreme rule! If he is not found within the hour, I will begin executing commanders! Is that clear?!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("If i may, Admiral... This seems to be a perfect opportunity to test the device we discussed earlier..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes.. Yes! YES! That is the key! Get me your device! Let nothing live! I want not so much as a blade of grass left in that area!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("You will not be disappointed, Admiral. I look forward to seeing the results."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr(" B-but, Admiral... Our forces are surrounding the city. Your...your men are there."), "co_davis", GameEnums.COMood_Normal, "#e88613"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("...And your point is?"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's just that... Well, if we set the device off with all of...your troops...they're... Nothing, sir. Nothing. No problem."), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tabitha, can you hear me? Are you ready to play with Daddy's new toy?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("I can't wait! Do it! Raze the entire area to the ground!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain, we're heading to your position now! You have to hold on!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("...No, Will... Don't come...back for me... ...Stay...away... ...Greyfield...wants me... I can... ...buy you time... ...That's... an order..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/brenners_death.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're not leaving you, Captain!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I'll...join you later... ...Just need to...rest here... ...for a while..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/brenners_death.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Never wanted you to...see me like this... ...It's... up to you...Will..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/brenners_death.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("NO! Captain, please! Don't give up! There's still hope, Captain! There's still hope!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("BATHE HIM IN FIRE!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Look at that... It's...beautiful... ...It looks...like the sun..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/brenners_death.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain! Captain Brenner!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("...General Forsythe has been executed?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Those dogs! We must avenge his death!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("I hear that Greyfield has ordered us all to be executed."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Rubinelle filth! They were planning this eve as they accepted the general's surrender."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're wrong! Captain Brenner would never allow that! He promised to protect you, and that's what he's going to do. I'm sure of it."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Like he protected General Forsythe? Ha! You Rubes were killers before the meteors, and now your true colors shine again..."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're...you're wrong!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you know, kid? No one tells you squat. ...Stupid. I should have seen this coming."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK, yeah. You're right. There's a lot of stuff that I don't know. But I do know Captain Brenner, and I know he would never betray you!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Regardless, General Forsythe is still dead. Whatever your feelings for Captain Brenner, he's a soldier, and soldiers follow orders."), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're wrong! Both of you! The captain... Th-the captain..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's enough, Will."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner! These people are saying that--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm only going to ask you this once. If you can't give me a straight answer, you might as well shoot me now. Is it true that General Forsythe was executed?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Yes. It's true."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'll kill you!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tasha! Stand down! Who was responsible?"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield gave the order, and Waylon pulled the trigger. But the final responsibility for his death lies with me. I underestimated Greyfield. I didn't think he would kill an unarmed man. But I accepted your general's surrender, and now he is dead. I can make no excuse. I'm sorry."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("How do we know you're telling the truth?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Believe me or not. I don't care. I'm not here to justify myself. We're out of time. The 12th Battalion is leaving, and we're taking the Lazurians with us."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("..You're doing what?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield is going to execute all of you in about six hours. But I've had enough. I'm not going to sit around and wait for his orders to be carried out. You need to escape. Now."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why should we trust you?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Because if you don't, you'll die. General Forsythe entrusted you with the safety of your fellow troops, right?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I still don't trust you."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's your plan?"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("This place is surrounded by Greyfield's men. My battalion can't possibly take them all on. We're going to make a break and smash through their rear defenses. Once we clear the Northern Ice Plains, you're on your own."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's a risky plan. Who's going to provide covering fire?"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("We will."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're going to fire on Greyfield's men? Aren't they your own people?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I promised your general that i would protect his troops, and i keep my word. Besides, Greyfield is mad. I can't accept his command anymore."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I don't know..."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, you've got about ten minutes to make up your mind. Prepare your men."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey... HEY! Escape! The prisoners are escaping! And Brenner's Wolves are with them!"), "co_officier_ti", GameEnums.COMood_Normal, "#d08000"); dialog34.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("BRENNER! What in the name of Rubinelle do you think you're doing?! I order you to turn around and--"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog35.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("The 12th Battalion doesn't take orders from you anymore, Greyfield."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is high treason, and the reward shall be death! Death for you and every member of your pathetic battalion!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog37.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're a coward, Greyfield. And you don't frighten me."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("How dare you!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog39.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're a strongman who seized power on the backs of others. I'll die before I serve you for another second."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dog! Prepare for your final hour!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog41.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("All units! Make for the city to the west! Once you clear it, push hard and don't look back!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I..."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog43.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("Understood, Captain. Tasha! Let's go! Come on!"), "CO_GAGE", GameEnums.COMood_Normal, "#0098f8"); dialog44.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't worry about me. Save yourself! I'll keep Greyfield busy. Now make for the city! Go!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog45.loadBackground(""); // 1 ScriptEventDialogItem
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
            map.getTerrain(0, 4).addTerrainOverlay("map_marker", 0, 0, "#ffffff", -1 , 0.8);
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner! Come on! You have to go now!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Brother man, I don't think this little bird's flyin' anywhere. We got some business to settle... Ain't that right, Brenner?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why don't you come down here so we can talk it over, Waylon?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("No thanks, brother man. My mama didn't raise no dummies. Why are you sticking your neck out like this anyway, Brenner? You think any of this makes a difference? I just don't get it."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("And you never will."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Fine by me, brother man. Fine by me. Say, you remember what I said the first time we met? I TOLD you this responsibility gig would lead to an early grave."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Better early to the grave than to your own humanity, Waylon"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ain't that a thing. Almost makes me feel bad about what I'm gonna do. All units! Target Captain Brenner! Time to bag ourselves a hero!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            // Dialog
        } // Start Of Turn End
        if (player === 0)
        {
            map.getTerrain(17, 5).getUnit().setHasMoved(true);
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
        var variable2 = variables.createVariable("variable2");
        var variable2Value = variable2.readDataInt32();
        if (variable2Value === 0){variable2.writeDataInt32(map.getTerrain(17, 5).getUnit().getUniqueID());}
        variable2Value = variable2.readDataInt32();
        var variable3 = variables.createVariable("variable3");
    // preconditionend
        if (map.getUnit(variable2Value) === null && variable1.readDataBool() === false) {// 17 5 0 Unit Destroyed
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will! Lin! I'm abandonning the tank and continuing on foot!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain! Come on! This way!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("I told you to run, Will, and that's a damn order!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("But, Captain--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin! Get him the hell out of here!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Brenner!"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen, Lin. There's...there's a lot of things I never got a chance to say... But I know you. And I know you understand what I'm about to do... Now move! MOVE!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Understood, Captain. We're gone. ...Good-bye"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            // Dialog
            variable1.writeDataBool(true);
        } // Unit Destroyed End
        if (map.isPlayersUnitInArea(Qt.rect(0, 4, 1, 1), [0]) && variable3.readDataBool() === false) {// 1 Player in Area
            map.getPlayer(2).defeatPlayer(null); // 0 Defeat Player
            variable3.writeDataBool(true);
        } // Player in Area End
        if (map.getPlayer(1).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(0).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
        if (map.getPlayer(0).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(1).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
