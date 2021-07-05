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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, no, no, no. no! Idiots! Wretched, lousy, foul, incompetent idiots! All my dreams..."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("We did it! We actually did it!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Retreat! Retreat! All units, retreat! When I find out who is responsible, I will have them executed immediately! I am the supreme leader of the world! I am the strongest of all men! Defeat is only for the weak!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're right."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("EEEEEK!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Defeat is for the weak. You were defeated because you were weak. And now you're going to die."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-you wouldn't dare!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Actually, I would. Will wouldn't. Will would try to put you on trial or something."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("But Will isn't here. Is he?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_lin.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("You wouldn't dare...! Y-you can't...! You can't do this!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_lin.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("You are responsible for the death of Captain Brenner. You killed him, and now I'm going to kill you. Will and the others won't like it... But it's what needs to be done."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_lin.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stop! I... I... I surrender! I surrender my forces to you! Th-there! You can't hurt me now! I am a prisoner of war! C-C-Captain Brenner would never hurt a prisoner! It's murder! If you kill me, you're as bad as me! You'll be just the same as me!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_lin.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Huh. I guess you're right."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_lin.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("That was for you, Brenner. ...I think I'll see you soon."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("We did it! We won!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("The war is over! You did it! Hurrah for Brenner's Wolves!"), "co_civilian7", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will! Everyone! Listen up! I just discovered huge stocks of the vaccine we need in Greyfield's headquarters! I think we can keep the infected alive long enough for me to find a cure. We did it! We did it! Oh ho ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, we... We did."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! This is no time for frowns. It's over! We won! It's time for the biggest celebration this world has ever seen!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dr. Morris? I don't think Will..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("He should turn that frown upside down! Oh ho! Oh ho ho ho ho... ...I'm sorry, Will. What is it?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("We... We lost so many. So many people. A better commander could have prevented such losses. I'm just...not in the mood for a party. I'm sorry."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Typical. If you were bigger and had better hair, I'd swear you were actually Brenner."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin, I..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("We all love ya, Will, but please... Stop feeling sorry for yourself. You're the cadet who defeated an admiral. I think that's good enough."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("But without you and Isabella and--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("What you lacked in talent, you made up for with heart and guts. And you inspired every soldier to do the same, even those who never came home. That's what led us to victory. Not my tactics. Not Isabella's...whatever she does. It was you, and you should be proud. So let's go have a party."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Yeah, all right."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho ho ho! Now THAT'S the spirit! Let's party! I've got a few classic jokes that I save for special occasions..."), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ah. Admiral Greyfield. How can I help you?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Caulder! What in the name of Rubinelle do you think you're doing?!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Whatever do you mean?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't play the fool with me! That virus is tearing through my troops! Where is my cure? I ordered you to find one!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ah yes. The Creeper. The Green Thumb. I hear it is causing...problems in the ranks. It seems your policy of executing the infected has actually worsened morale. Fascinating..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("How dare you question me?! Who are you? Just some pathetic man in a lab coat!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Treating your infected troops in such a manner has sowed the seeds of discontent. Rather than seek help, infected soldiers conceal the truth for fear of the firing squad. This has allowed the disease to spread almost unimpaired through your army."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("S-silence! I will hear no more of this! I am not a man to be taken lightly, Caulder! If you had done your job, none of this would be a problem. Get me a cure by the end of the week, or I will have you executed!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hmmm... Is everything all right, Admiral? You look somewhat unwell."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wh-what do you mean?! How... How dare you mock the mighty Greyfield!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ah ha! Not another word, Admiral! Your eyes tell me all I need to know. You too are infected with the virus! And that vaccine of yours can only hold it off for so long..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-bah. Bah! Admiral Greyfield will not be brought low by some common virus! Only the weak succumb to this disease, and I am not among their number! I am strong!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Your fear is palpable! If word spread that you had been infected, your army would crumble. You said yourself that this virus affects only the weak, did you not? I wonder, Admiral... Will you order your own firing squad to execute you as well?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Insolence! I'll see you hanged for this!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tsk tsk, Admiral. If you set me to swing, you will never find your cure. But worry not. Your little secret will be safe with me. And as we speak, I am but hours away from perfecting a cure."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("...T-truly?"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Truly."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Then all is forgiven! Ha ha ha! Oh, Caulder, you devil! I was right to trust you! I want this cure delivered to all of my troops without delay. ...But first, give it to me! N-not that a man of my strength will need it! It's just to show the men that I am one of them."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, yes. You and your troops will get your medicine in due time. But first, we must discuss my masterpiece. The preparations are almost complete."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ha ha! My time of destiny is at hand! Admiral Greyfield shall rule the world as king! Show me how to operate it! Show me your masterpiece..."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("From this room, you can control the launch systems for the Caulder Missiles. You will rain destruction down upon the entire world."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("You outdo yourself, Caulder! No leader has ever wielded the power I now possess. With this button, my destiny is fulfilled. I will become King Greyfield the Mighty! Now where are those miserable traitors cowering? They shall get the first taste... With this power, I control a charred wreck of a world with most, if not all, life dead! WAHAHAHAHAHAHAHAHA!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do calm yourself, Admiral. The launch system was heavily damaged by the meteor strikes. There are a number of locking mechanisms that must be bypassed before it can be used. My staff estimates that it will be forty days before the launch can commence."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("That is unacceptable! Tell them to hurry! I want those wretched traitors to feel the full force of my wrath! Make them hurry! Tell them King Greyfield orders it to be so!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Commander? Greyfield's army is here. ...All of them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("That force is huge. Maybe we should retreat and plan a strategy."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("There isn't time. We must strike them now and find Greyfield. We need to learn the status of those Caulder Missiles."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I can help. The launch system was damaged but is being restored as we speak. The process will be complete in forty days. Once launched, the missiles will cause anyone in the blast range to suffer--"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Thank you, Isabella. Don't think I need to hear all the details."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("All right. We have to stop him. I'll take the troops and--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("No. I'll do it. I mean... Permission to lead the troops, Commander?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("I know you and Brenner were close, but he was my...my captain. I failed him. I owe him this. Plus, I have the virus and you don't. ...I have nothing to lose."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Restoration of the Caulder Missile launch system is now 2.5% complete. It will be fully operational in 39 days."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Splendid! I can hardly wait! My glorious reign as king is about to begin! Those traitors will serve as an example for all the world!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Treacherous swine! Inglorious, backstabbing vermin!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("You are one charming fellow, Greyfield. "), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("You pathetic traitors understand nothing! You can't see that war is caused by the fact that other nations exist. I shall be the man to unite all the world into one nation! I will be the man to end all war forever!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("And humble too. I like it."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Save the sarcastic asides for your tombstone, dog! That meteor strike and all that followed was a test! The situation demands that a natural leader unite the survivors below him. And I, King Greyfield, shall be that leader!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("That is a great plan. Let me ask you something. Once everyone is below you, will you stop being such a coward?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wh-what...?! You dare to--"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've seen your records. I know you were a subpar commander at best. And you know it too. That's why you strut around like a playground bully. You hid your shame by faking results and taking credit for the work of--"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Silence! Enough of your loathsome falsehoods, you treasonous hound!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! That's enough of YOU, you lying, sniveling, fat man! You're a coward and a liar and I have had enough of it! You started this fight. Now I'm going to end it."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
            // Dialog
        } // Start Of Turn End
        if (turn === 10 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Restoration of the Caulder Missile launch system is now 25% complete."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 20 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Restoration of the Caulder Missile launch system is now 50% complete."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 30 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Restoration of the Caulder Missile launch system is now 75% complete."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 40 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Restoration of the Caulder Missile launch system is now 100% complete."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
            map.getPlayer(0).defeatPlayer(null); // 0 Defeat Player
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
