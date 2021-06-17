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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("NOOOOOOO! Must...escape... But this was just a taste losers! Now that I've got a weapons supplier, I'll be making your lives miserable. Sleep lightly, soldier boy! The Beast rules the night! GYAAARRR!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("All enemy units have been accounted for. This battle's over."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("omebody tell me where those raiders got all that firepower?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I can't understand it, sir. We destroyed everything they had..."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Looks like he hid in a pile of corpses then snuck off when it was safe."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("That doesn't bode well... I'm sure we'll see his ugly face again. But we've got our own goal to reach. Let's move out!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh...I...What's...Ungh..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella? Is something wrong? Are you feeling all right?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I d-don't know..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella! Talk to me! What is it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("So...dizzy...my head...9693872914... 9693872914... I feel... funny..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait a minute..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("This young man was a victim of the plant. He was about your age, Will. Curiously, the virus seems to infect only young people. You're in the right age range, Will, which means you're at risk. If you met anyone suffering from this you must stay away! Is that clear?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! It can't be!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("W-Will?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm going to get Dr. Morris! Wait here, Isabella!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please...don't leave..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_dust_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's all the commotion over here? Oh no... Get the doc over here! On the double!"), "CO_BRENNER", GameEnums.COMood_Normal, "#f00008"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner...Judging from the symptoms, there's a good chance Isabella's infected with Creeping Derangea."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("The plant virus? "), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes. I've placed her in a quarantine in a temporary laboratory. She must stay there for now. There's no danger of us getting sick, but young people like Will are at risk."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("What are you doing for her? How long until she's cured?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will's right about you, Captain. You truly are a caring man. But to have any hope of curing her, I would require highly specialized equipment and facilities."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("You've got what you've got, Doc. Do your best."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("If I may, Captain? I have a thought."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Go ahead."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("The shelter the girl told us about. If it truly does exist... Perhaps it contains the medical equipment Dr. Morris requires."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("You may be onto something there. A shelter like that would be completely airtight... Which means an infectious disease would spread like wildfire... Yes, they must considred it. It's very possible they would have a contingency plan for a situation such as this."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("I see. Good work, you two. As long as there's a chance, we keep moving."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will? What are you doing out here all along?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Nothing. I'm just..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I know you're worried about Isabella. That's a normal reaction. But listen, I don't want you to lose hope. As long as there's life--"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("I ran away. I... I ran away from her."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("What are you talking about?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("When I knew...when I knew Isabella was sick. I didn't go to her. I didn't want to get sick too. I was only thinking about myself. She was really scared, you know? I could see it in her eyes. She wanted me to help her, but... ...I ran away."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("You did the right thing, Will. If you'd gone to her, if you'd come into contact with her, you'd be in quarantine too."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("But you would have stayed with her, Captain! You help people. That's what you do. You're committed. You're not like me..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've spent more time running than you'll ever know, kid. Hell, I'm still running."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you mean?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("All the people I couldn't help, all the soldiers who died because I wasn't good enough... I've spent many sleepless nights telling myself it couldn't be helped, that i did all i could. That's just the sound of me running, Will. Running from a past I'll never escape... "), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("You disappointed? I was hoping I could protect the illusion for you. No matter how I grieve or regret my actions, the men who've died because of me will still be dead. But Isabella's still alive. You've got nothing to regret. There's plenty you can still do, right? For her?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir... Yes, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("Just remember, Will. Where there's life..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's hope!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_forest_night.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! The Beast is hurting! I need medicine! What are you waiting for, roaches? Swarm and plunder! And don't come back without something to ease my pain!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... It's not that easy. We're low on fuel and ammo. And most of our weapons were destroyed in the last battle, so--"), "co_officier_ti", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're THIS close to dying, boy! Whatever you need, you steal! UNDERSTOOD?"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-yes, sir!"), "co_officier_ti", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("My leg's on fire! I can't believe those soldier boys hit ME! GYAAARRR! Gotta kill this infection and then kill me some dogfaced do-gooders!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you truly harbor such hatred for Captain Brenner?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("HEY! Wh-where did you come from, you freakin' ghost?"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("My dear Sergeant... Wait, you have no name now, do you? You're just The Beast. How appropiate. Your propensity for violence saw you expelled from the military, yes? And these past few years, you've spent each day slaking your thirst for blood and mayhem."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-but...how do you..."), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("All in good time, my friend. When the meteors came, you knew just what to do, didn't you? You stole weapons and access codes, then began plundering whatever you could find. For most, the world was a living hell, but for a man like you, it was paradise. At least until Captain Brenner and his good samaritans arrived..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! I hate him! He ruined everything! When I find him, I'm gonna rip--"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("You will have your vengeance. I will provide you with supplies and weapons. You desire chaos and destruction, yes? I shall see you are equipped to sate that desire."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is the quickest route to the Seratta mountain range."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("So we'll be passing by Freehaven again, will we? I doubt they'll be happy to see us, but we should check to see if the people are safe."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Permission to speak out of turn, sir, but I believe you're too forgiving. How can you be concerned for their welfare after the way their mayor behaved?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("That doesn't mean we can abandon them."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Doesn't it? They insulted you, sir. I for one couldn't care less if they were wiped from the earth."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's enough of that, Lin. "), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it, Will? Are we under attack again?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Huh? No, it's those villagers. Remember the ones we met before? They say they want to speak with you, sir."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is that so?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("So this kast quake destroyed the remaining buildings, leaving us without food or shelter. We can't survive without assistance."), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("And what has this got to do with me?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please, Brenner... Captain. We need your help. You have my apologies for any past--"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Understood. Pack your things. You're coming with us. If we can locate that factory, there should be enough food for everyone."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Factory?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's right. If you need details, find Dr. Morris. I deal in weapons, not test tubes. All i can tell you is, it's key to getting the food we need to survive. Clear?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, of course. You have my gratitude. Speaking of food, my people haven't eaten in days. May I ask--"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("For someone who hasn't eaten for days, you look pretty well fed to me."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("I-I beg your pardon?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm rather curious as to what your villagers are carrying in all those trucks. It wouldn't happen to be provisions, would it?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("N-no, it's merely... How dare you? Are you implying that I'm lying?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("I said no such thing. I merely inferred from your robust condition that you've been eating--"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Enough! You...you uppity--"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("Attack! We're under attack!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gather your people and take cover in the rear. We'll continue this later if we survive."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's that same group of raiders. How is it they're still operating? And they're using this poor visibility to cover their attack. Someone must be helping them... All units, advance! Keep your eyes peeled!"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog37.loadBackground(""); // 1 ScriptEventDialogItem
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
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har! They ain't happy to see us, roaches! Let's bury..ungh...Ungaah...my head...so dizzy... Must be the medicine that freak gave me... But it feels good! My blood's on fire. The rage is burning me up! YESSSSS! I am reborn! The Beast will not be denied! GRAAAARRR!!! The Beast will hunt! Prepare...to...DIE!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
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
