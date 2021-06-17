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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAA... Uuurrrgggh! NOOOOOO!!!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("It seems my little experiment has come to an untimely end. Oh well... You served your purpose well, my savage beast. I am quite satisfied. But you are no longer of any use to me. You may continue dying..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hello! Is anyone there? Did anyone manage to survive? Hello? HELLO? It's...it's no use. They're dead. I failed them all..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't be so quick to bury us, Will! We were just lying low until we could be sure it was safe."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dr. Morris!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("No time for lollygagging! I've got wounded coming out of my ears! And Isabella is faring poorly!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("The wounded are in sick bay, sir. The civilians came through largely unharmed."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Where's Will?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("He's with Isabella. Her condition has deteriorated badly. She was hurt in the skirmish, and the virus is continuing to develop."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will she make it?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's touch and go. She's in Dr. Morris's hands now."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Come on, come on... What's taking so long?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will. You're still here? It's been hours."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("How is she, Doctor? Is Isabella all right?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm...I'm sorry, Will. Try and calm down, all right? I don't know if I'm the right person to tell you this, but..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh no... Please, Dr. Morris! It can't be! It can't!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen, I... No, I don't have the words... I think I'll leave it to Isabella to tell you how she's made a full recovery! Oh ho! Oh ho ho ho ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will! "), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm going to be OK, Will. I owe it all to Dr. Morris's help and to everyone's kindness."), "CO_ISABELLA", GameEnums.COMood_Normal, "#0098f8"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's great news!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("It is, isn't it?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're a real piece of work, Doc. Why didn't you just tell the boy she was better?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Come now, Captain! Where's the drama in that? Did you see the joy on Will's face? I felt like the director in one of those TV medical dramas. Oh ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Unbelievable..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("May I assume Isabella's no longer contagious?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("You may. The virus incorporated the nucleoside analogues into its genomes, halting its life--"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stop! Is she OK or isn't she?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("You are a most impatient man, Captain. Yes, yes, she's fine. Although I must confess I had my doubts there for a while. She was on the verge of death, and then... Something changed within her system. While I was recovering from my surprise... Voila! I can offer you no explanation for it."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why not just call it a miracle? Sounds to me like Will's prayers were answered. That's all the explanation I need right there."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm a man of science, Captain. Miracles are not something I deal in. However... Truth be told, I feel rather superfluous in that I truly did nothing to heal her. The one thing I can say for certain is that she's cured. So... Hip hip hoo-rah!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Where've you been, Brenner? I've been waiting to hear your report for hours!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Calm down. Everything's fine. The girl made a full recovery."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("The girl? Who said anything about her? I want to know about the food! Is the factory in working order? Will we be able to grow crops?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes. That's what I've been told. You'll be happy to learn you don't need to worry about food anymore."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("That...that is good news. Now, we've got some additional business to attend to... As the civilian representative, there's something I need to discuss with you."), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Go on."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's about this shelter. Do you think it can sustain this many people living here? We've assessed the situation and feel we need to cut our population roughly in half. There's--"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("You can't be serious. Do you realize how big this place is? There is more than enough room for all of us."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen, Brenner. This is the collective opinion of the entire civilian population, understand? And having soldiers living here is problematic as well. Are we supposed to live in fear of being attacked?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is there a point to all this?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're soldiers. You have the weapons and training to survive anywhere. But we civilians are powerless. This shelter is our only chance for survival. We believe the solution is clear. Am I right, everyone? Shall we vote and decide this like civikized men?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's not a reasonable proposition. In terms of numbers, you civilians have an overwhelming majority. Additionally, a large number of our people are incapacitated by their wounds. Would you use that to your advantage? Is that the type of people we're dealing with?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("How disrespectful! I'm not trying to set up a dictatorship! This new world should be built on democracy. One person, one vote! That's what I say! Or would you rather threaten us with your guns and bully us into submission?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK, yeah. I'm comfortable with that."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog45.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stand down, Lin."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog46.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog47 = GameAnimationFactory.createGameAnimationDialog(qsTr("Very well, then. We vote, and majority rules. Does that sound fair to everyone? All in favor of having the soldiers leave the shelter, raise their hands. ...Come on! Lets see hands!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog47.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog48 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't think so."), "co_civilian3", GameEnums.COMood_Normal, "#ff0000"); dialog48.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog49 = GameAnimationFactory.createGameAnimationDialog(qsTr("Me either."), "co_civilian6", GameEnums.COMood_Normal, "#ff0000"); dialog49.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog50 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wh-what are you doing? Get your hands up! I gave you the plan! Now do as I told you!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog50.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog51 = GameAnimationFactory.createGameAnimationDialog(qsTr("But...they fought for us. They risked their lives for us."), "co_civilian7", GameEnums.COMood_Normal, "#ff0000"); dialog51.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog52 = GameAnimationFactory.createGameAnimationDialog(qsTr("Some of them...died for us. We can't just throw them out of here."), "co_civilian6", GameEnums.COMood_Normal, "#ff0000"); dialog52.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog53 = GameAnimationFactory.createGameAnimationDialog(qsTr("I want them to stay. "), "co_civilian7", GameEnums.COMood_Normal, "#ff0000"); dialog53.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog54 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I'll second that!"), "co_civilian3", GameEnums.COMood_Normal, "#ff0000"); dialog54.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog55 = GameAnimationFactory.createGameAnimationDialog(qsTr("You listening, Mayor?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog55.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog56 = GameAnimationFactory.createGameAnimationDialog(qsTr("It would appear we have a clear majority."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog56.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog57 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-b-but, how can you...? Wh-what are you thinking?! Fine! Do as you please! I don't care!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog57.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog58 = GameAnimationFactory.createGameAnimationDialog(qsTr("Thank you, everyone. You have my gratitude. As you know, my people and I have been on search and rescue 24-7 since the meteors struck. But most of the folks we've \"saved\" didn't last too long after the fact... To be honest, there've been times when I've questioned the value of what we're doing. But knowing that you understand makes it all worthwhile. Thank you."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog58.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog59 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well said, Captain."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog59.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog60 = GameAnimationFactory.createGameAnimationDialog(qsTr("So we're going to stay here for a while. There's plenty of food, and there's safety in numbers."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog60.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog61 = GameAnimationFactory.createGameAnimationDialog(qsTr("We can stay here together? Me too? I...I had nothing. I didn't know where I was or what I should do. I didn't even know my own name. Yet you took me in and cared for me, and now you say I can live here with you. Are you sure? Is it truly all right?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog61.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog62 = GameAnimationFactory.createGameAnimationDialog(qsTr("Of course! And now that you've recovered from the virus, I'm sure you'll get your memory back."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog62.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog63 = GameAnimationFactory.createGameAnimationDialog(qsTr("Perhaps... But I really don't need my memory back. I'm fine not knowing what happened before."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog63.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog64 = GameAnimationFactory.createGameAnimationDialog(qsTr("Really? But..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog64.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog65 = GameAnimationFactory.createGameAnimationDialog(qsTr("You've given me all the memories I need. I'll never forget the kindness you've shown."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog65.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog66 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella, listen. I want to... ...Um, Lietenant? Are you... Lin? Hello?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog66.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog67 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog67.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog68 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAA! How do you do that? And why are you eavesdropping on our conversations?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog68.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog69 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you talking about something you don't want me to hear?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog69.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog70 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, no, but... That's not the point! It was a private conversation, and--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog70.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog71 = GameAnimationFactory.createGameAnimationDialog(qsTr("Whatever. Captain Brenner sent me. We're to report to A Block immediately. If we're going to be living here, we need to set up rules and regulations."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog71.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog72 = GameAnimationFactory.createGameAnimationDialog(qsTr("Fine. We'll be right there. C'mon Isabella. It's time to go!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog72.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog73 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog73.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
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
            dialog51.queueAnimation(dialog52);
            dialog52.queueAnimation(dialog53);
            dialog53.queueAnimation(dialog54);
            dialog54.queueAnimation(dialog55);
            dialog55.queueAnimation(dialog56);
            dialog56.queueAnimation(dialog57);
            dialog57.queueAnimation(dialog58);
            dialog58.queueAnimation(dialog59);
            dialog59.queueAnimation(dialog60);
            dialog60.queueAnimation(dialog61);
            dialog61.queueAnimation(dialog62);
            dialog62.queueAnimation(dialog63);
            dialog63.queueAnimation(dialog64);
            dialog64.queueAnimation(dialog65);
            dialog65.queueAnimation(dialog66);
            dialog66.queueAnimation(dialog67);
            dialog67.queueAnimation(dialog68);
            dialog68.queueAnimation(dialog69);
            dialog69.queueAnimation(dialog70);
            dialog70.queueAnimation(dialog71);
            dialog71.queueAnimation(dialog72);
            dialog72.queueAnimation(dialog73);
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("There are ten levels to the shelter. They're all clear."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("How about the medical facilities?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("The medical bay's in D Block, Sector 1, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's filled with antiviral medicines and the latest medical equipment. The government spared no expense in its preparations to safeguard the political elite."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("I hear you, Lin. Now let's get Isabella and the rest of the civilians in here. On the double!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Right away, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain! The rear guard's under attack! Looks like those raiders are back again!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("What? Someone want to explain how they snuck up on us?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I...I don't know, sir. They didn't appear on radar..."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's just great! What are you standing around for? Let's move!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's those raiders! We're done for! They'll kill us all!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Calm down! Just follow the evacuation procedures we practiced. Do as you were told, and you'll get out of this just fine."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Unnnhhhh..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's all right, Isabella. Try thinking of something pleasant. That should help. It's all right, Isabella. Try thinking of something pleasant. That should help. When I'm stressed, I think of my lab. And perhabs some nice rats or robots!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I-I'm OK... If we never give up..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, of course. We must never give up. That's the key. Of course, some reinforcements would help raise my spirits, but..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! Ungyaahh...graar... Gwar har har! GYAAARRR!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("The raiders are closing in on the civilians! They're going to attack!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! Civilians? You're worried about them? Gwar har har! That settles it! KILL 'EM ALL!! War lesson number one, punk! NEVER CARE! GYAAARRR!!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Leave them alone! They're not soldiers! THEY'RE CIVILIANS!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("It doesn't matter! They're weak! They're defenseless! They're the perfect prey, and that's why they're gonna bleed!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("But why? Why would you... I'm not letting you touch them! We're going to stop you!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har! I'd like to see you try! Do your worst! GYAAARRR!!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! YES! YES! YES! First blood to The Beast! Take them all! Don't let them die easily!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("NOOOO! Get out of there! Do you read?! You have to evacuate now! Dr. Morris? Isabella? Please, someone answer me! You've got to get out of there! Hello? Is anyone there?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har! They're all dead, punk! Your pretty little girlfriend too! You couldn't save 'em. How's it taste? Burns, doesn't it?"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("WHY? Why did you do that? There was no reason..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Reasons? I don't need reasons! I kill because I can! And now I'm gonna kill the rest so I can hear you cry some more. How's that sound? What's that? I can't HEEEEAAARRR you! C'mon, punk! Don't you have anything to say? I wanna hear you beg!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I...I want to kill you."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("You threatening me, boy?"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("You were right. I had no idea what war was really about. But I get it now. It's about anger and fear. It's about hate! My friends weren't looking to fight. They were innocent... You want to live like a beast? Fine! You can die like one too!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har! Now you're talkin', punk! Now you're worth killing! GYAAARRR!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
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
