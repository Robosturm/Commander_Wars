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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Enemy reinforcements will be here in thirty seconds."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("All units, regroup! Regroup! We're not going to surrender!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait! Commander..."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it...?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("The enemy appears to be retreating..."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Traitors! I ordered you to fight to the death! Advance ADVANCE!! You spineless cowards! You dishonor the proud name of Greyfield! I'll see you all hanged for this!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What the heck happened? I thought we were done for."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I may have an idea. We intercepted some enemy communications as they were retreating. It seems there was some unforeseen event that demanded their full attention. We captured a number of retreating soldiers. Let me talk to them. ...And if that doesn't work, I'll have Gage talk to them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't know what happened. Some urgent order came in and told everyone to pull back. Bush league if ask me. We were about to steamroll you guys."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Save the commentary. Why did you pull back?"), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not sure. I heard rumors that the rear guard had rebelled against the admiral. My guess? They sent frontline units back to put down the uprising. Last I heard, things were getting out of hand pretty fast."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("You were on the verge of destroying us. That's an odd time for an uprising. Your troops seemed to be in high spirits, and their health was good--"), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ha! What, you ain't heard about the disease?"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you referring to Endoflorescens terribilis?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("We call it the Creeper."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah? Well we call it the Green Thumb, and it's spreading like wildfire. Folks are scared. Panicking. They were starting to desert even before that last battle. Greyfield is executing anyone who catches it. He says they're weak and they deserve it. High spirits? Ha! We don't know if we're gonna get shot or have plants explode outta our heads."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("That must be why they withdrew... This shows there is always hope, even in the worst situations!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("The same virus that almost caused rebellion in our ranks ended up saving us. ...Huh. You know what? Life's a funny thing."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("I have the final test results. I think it's best to tell evertyone at once. The infected will be quarantined while we look for a cure. Those who aren't infected yet are free to stay or go as they see fit. I hope this doesn't plant a ...SEED of dissent! Oh ho! Oh ho ho ho ho... What? You're not laughing."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Thank you, Doctor."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("But seriously, I am concerned that this could cause much unrest among your troops."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("We have to take the risk. We can't force healthy people to stay with us against their will. Well, I guess I should know. Do I have it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Let's see... Nope! You're as healthy as a horse. Maybe I should get you some hay! Oh ho ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Er...yes. Thanks."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is your plan, Will? If you stay here, you will most likely contract this disease."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm not going anywhere, Doctor. I command this unit, if in name only. I will face the risk along with the troops."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Blast! I thought you were going to say that! Under normal circumstances, a doctor would never allow a patient to take such risks."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("But these are not normal circumstances. We will stick together. Whatever problems face us, we will overcome them as one. The clock is ticking, Doctor. You must find a cure for the virus. There's no point in my survival if I lose all of my friends."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you scared?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you scared of the Creeper taking over your body? Of those flowers blooming? What if there is no cure? What if we all suffer that fate? I know a lot of ways to die, but having flowers burst out of you... If you aren't scared, Will, you're a fool."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Of course I'm scared! But that's why I need to find a cure. So no one else has to suffer like that."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, I... I talked to Dr. Morris. I have the virus."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, Lin! No!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you sure you don't want to leave? No one would think less of you."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("No. I won't run away. We will find a cure. I have hope."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield has found us. Recon units report a large enemy force approaching from the southeast."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is a long way to come just to wipe out our merry little band. Will must have really ticked him off."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("He won't stop until he's destroyed every last Lazurian. I think he's kinda crazy."), "CO_TASHA", GameEnums.COMood_Normal, "#f00008"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Any farther north and we'll be swimming. We're out of places to hide. Commander? What are your orders?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("What can we do? We make a stand here and hope for the best."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("They have more troops, more experience, and better weaponry. I don't think we can win this fight."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I know, Lin. But we don't have a choice."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("No. There is a choice."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it, Tasha?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hand us over to Greyfield."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Out of the question! You're our allies. Our friends. We would never do that!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's your best option. Greyfield is obsessed with Lazuria. Once he has us, he'll likely back down."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("What about it, Lieutenant? I bet you thought of this a while ago, huh?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I considered it."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Look, Will... I mean, Commander. We're grateful for all that you've done for us. You taught me that every nation has its flaws. And you taught me that there is some good in Rubinelle. Don't worry. I've had a good run."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm ready."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tasha! Gage! No! This is madness!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, listen--"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! I won't listen! We're not doing this! Captain Brenner saved your lives. He would never hand you over to Greyfield, and I won't do it either. We live together and we fight together. And if we die together, so be it."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you sure?"), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sure. But that's not going to happen. Greyfield isn't going to beat us."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("If that is your decision Commander... Then we will follow you."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! Sorry to interrupt, but I have to speak to you right now!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("This can't be good."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've finished screening people for the Creeper, and the results are disheartening..."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Half of us are already infected?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm afraid so. The virus has spread to civilian, Rubinelle, and Lazurian alike. I think the infection started before we even entered the trading outpost. ...I can't even think of a joke for this. And I've been trying for a while."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("But everyone seems fine!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("It seems the disease now spreads without detectable symptoms until it is too late. If we don't find a cure soon, it's going to kill us all! Oh ho ho... Wait, that isn't funny."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is not what we needed..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir, the enemy has mobilized. We are awaiting your orders!"), "co_officier_ti", GameEnums.COMood_Normal, "#d08000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ho! What a splendid sight! Look at those treacherous dogs who dared to betray me! This will be fun! Give my regards to Brenner, swine!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr(" ...I knew we were outgunned, but this is absurd. First the meteors, then the virus, and now this. I've had better years, I'll tell you that much."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("At least Greyfield's guns will be quick and painless. Unlike the Creeper..."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! What's wrong with you! Stop talking like that! We still have hope. We will find a way to win."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Commander! We've got a problem. Our western front is in chaos. The units are ignoring our orders."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("What are they thinking?! We're in the middle of a battle!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Word of the virus has spread, sir. The people are terrified. I think they're looking for someone to blame..."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Open a channel! We have to talk to them."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("I already tried, Commander. There is no response. It's utter chaos out there. I think they've lost the will to fight."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! This isn't happening!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aaaaaah! It's over, man! It's all over! We're dead meat."), "co_civilian4", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("If Greyfield don't get us, that virus will!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I heard the Creeper turns you into a plant! A freakin' plant!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("My eye itches! I think I got it! Noooooo! NOOOOOOOO!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("We got no chance! We're all gonna die! We're all gonna die!"), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("We've been tricked! Someone has done this to us!"), "co_civilian7", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's your fault! You civilians! You brought the Creeper into camp!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("No way! It was you soldiers! You brought it from the battlefield!"), "co_civilian4", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I heard you gave it to us to try and keep us in line!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Our allies are in complete disarray. Our orders can't get through to them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Our western front won't even fight. I... I'm not sure what to do, sir."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("There must be something... Wait! That communications tower! If I can get to it, I can convince them to fight!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
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
        if (variable2Value === 0){variable2.writeDataInt32(map.getTerrain(6, 7).getUnit().getUniqueID());}
        variable2Value = variable2.readDataInt32();
    // preconditionend
        if (map.isUnitInArea(Qt.rect(4, 12, 1, 1), variable2Value) && variable1.readDataBool() === false) {// 6 7 0 Unit in Area
            if(map.getTerrain(0, 11).getUnit() !== null){map.getTerrain(0, 11).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(1, 12).getUnit() !== null){map.getTerrain(1, 12).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(0, 13).getUnit() !== null){map.getTerrain(0, 13).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(2, 13).getUnit() !== null){map.getTerrain(2, 13).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(1, 14).getUnit() !== null){map.getTerrain(1, 14).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(3, 14).getUnit() !== null){map.getTerrain(3, 14).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(0, 15).getUnit() !== null){map.getTerrain(0, 15).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(1, 16).getUnit() !== null){map.getTerrain(1, 16).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            if(map.getTerrain(3, 16).getUnit() !== null){map.getTerrain(3, 16).getUnit().setAiMode(GameEnums.GameAi_Normal);} // 0 Change Unit AI
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("My eye! It's watering! I'm gonna turn into a plant any second now!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'd rather get shot than have that thing take me!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("It must be the Lazurians! We never should have trusted them!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ha! You blame us when your own leader is trying to kill you? We should never have joined you!"), "co_officier_bd", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's right! You never should have joined us, losers! It was bad enough when we had these useless civilians to deal with!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't call us useless, you gun-toting warmongers! You're the ones who dragged us into your stupid battles! You should have left us in peace! Now I'm gonna catch the Creeper and die!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen to me! Everyone! Stop arguing. Stop looking for people to blame."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Huh?"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's him! It's the commander! He's responsible for this!"), "co_civilian4", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who put that kid in charge anyway? No wonder we're all gonna die!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I... Wait, listen, just... Just hold on one second..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Forget this, man! I'm outta here!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you mean?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I mean that I'm gone! I quit! Presto! Poof! See ya later, chumps! I ain't letting some kid send me to die. I'll take my chances on my own!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Really? What's your plan? Are you going to walk over to Greyfield and tell him how sorry you are?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("...It's better than staying here."), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("If you want to leave, that's your choice. Good luck. But I won't abandon anyone. I'm tired of running."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Big words from a small man! You think you're noble? Is that it?"), "co_officier_bd", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("This isn't about sounding noble. It's about survival. We either pull together and make a stand, or we die here and now."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I don't wanna die!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Look, I won't lie. Greyfield has us outnumbered, and this won't be easy. But if we fight among ourselves, we don't have any chance. Fight for yourself or fight for your friends. I don't really care. Just fight! Don't lie down and die!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("I dunno..."), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("It doesn't matter where you're from or what has happened in the past. We're done talking. It's time to stand together and fight!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I guess..."), "co_officier_bd", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Er... Sorry, Commander. Got a little freaked out there for a second... Let's do it! Let's get Greyfield!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah! Let's do it!"), "co_officier_bd", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah! We're with you too! Let's take him down!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("...My throat hurts."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well done, Commander. ...A bit crazed, but well done."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sorry. I got a bit carried away there. This would never have happened if Captain Brenner was still here..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Believe it or not, emotion has a place on the battlefield. But that time has passed. Your men are ready. Let's finish this."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
            variable1.writeDataBool(true);
        } // Unit in Area End
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
