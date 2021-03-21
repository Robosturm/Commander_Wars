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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's it! All enemy units have been defeated. We won!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("It wasn't a walk in the park, but you're right. We won."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("And it was all thanks to Isabella! If it hadn't been for her..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("No...I didn't do anything."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't be modest, dear. Now let's check on those crop-growing systems!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Now if we can just find that factory..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's the word, Doc? You and Lin give that system the once-over?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, well, hmm... I've got some good news, and I've got some bad news..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("The system's useless. The factory has been destroyed."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("What are you doing, Lin? I had a joke I wanted to tell first!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Save your jokes for when I'm not around. What do you mean, destroyed? Do you mean we can't repair it?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("That would be quite difficult, I'm afraid. The factory was extremely sophisticated. The components needed to repair it are not likely to be found at the corner grocery. Not that we have one anymore! Oh ho! Oh ho ho... You're not laughing."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's terrible, Dr. Morris. What are we supposed to do now?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm afraid I don't know. And to tell the truth, there's something else that truly disturbs me..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("There is? What?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("It appears the enemy's goal was to destroy the factory all along. If we had gotten the factory up and running, we could have solved our food-shortage problem. We could have lived here peacefully. What did they gain by destroying our factory? It's as if..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's as if they wanted us all to die."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, look at this..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Flowers. They're quite beautiful, aren't they? But there's something odd about them..."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Whatever you do, don't touch those flowers!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dr. Morris?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Pardon me, Lin, but i must ask you a personal question. Are you over twenty years of age?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Maybe. I'm not as young as Isabella, but... Are you implying something, Doctor?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, of course not! And there's no need to glare at me so. It's quite unsettling! I only ask because these flowers have yet to infect anyone over the age of twenty, and..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Infect? What are you talking about?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_town.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Have you heard of Endoflorescens terrbilis virus? It's common name is Creeping Derangea."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh. Look at that. How pretty."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("This young man was about your age, Will."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("...He WAS my age?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("The flowers aren't decorative. They're the result of a parasitic virus. And there is nothing beautiful about them, Lin. This boy is dead."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait, so the flowers...came out of his body?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("It acts like a parasite, but it spreads like a virus. I don't claim to know how it works, I just know that it does. Since the meteors fell, I've buried many victims of this scourge."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("He had roots and leaves growing under his skin... In his ears... In his eyes... Little roots creeping behind the eyes... ...What if you could hear them...?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin, please! Stop talking about it! Curiously, the virus seems to only infect young people. You're in the right age range, Will, which means you're at risk. If you meet anyone suffering from this, you must stay away! Is that clear?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh yeah. It's clear..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flowers2.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("Starvation, raiders, and flowers that kill you. Gotta love this place."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("The world's certainly not making it easy for us, is it? But we can't give up."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("As long as there's life, there's hope."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Exactly."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
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
        // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tell me, my boy, do you know why it is dinosaurs became extinct?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Huh? No, but I bet Captain Brenner does."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not likely. Lin! Front and center."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("There are many possible explanations. I assume you're referring to the meteor theory?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! Right you are. The theory states a meteor struck the planet millions of years ago. The impact caused megatsunamis upward of 300 feet in height to sweep the globe. The shock waves spawn earthquakes reaching 12.0 and 13.0 on the Richter scale. Pieces of the meteor ignited global wildfires, causing enormous devastation... Dinosars, without the benefit of fireproof surfboards, were ill equipped to survive."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Fireproff surfboards? Whatever. The point is, most dinosaurs perished. One day they ruled the world, and the next, they were a few pages in our history books."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("But we survived our meteors."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("So we did, so we did. Cockroaches aren't nature's only durable creature, are they? Three cheers for manking, and for much smaller meteors too! Ho Ho!"), "CO_WILL", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Can we move this along, Doc? What is it you want to tell us?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, of course. Meteors...dinosaurs...cockroaches... Oh yes! The clouds of ash! That's it. The ash blocks out the sun, which of course means that plant life withers and dies. Some may say the fate of the dinosaurs was sealed by similar sunlight-blocking clouds."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("So you're saying we're going to share their fate? Is that it?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("No need to panic, Captain. Unlike the luckless dinosaurs, we possess knowledge and science. If we can get the automated crop factory running, we'll have as much grain as we want."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("A factory with an environmental system designed for the cyclic cultivation of genetically..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Whoa! Spare me the scientific mumbo jumbo. You're giving me a headache. Cut to the chase, and tell me what I need to know--no jokes, no science. Clear?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, of course. Ahem. If we can get the systems running, we can produce food. We won't be able to sustain a population of millions, but we'll be able to feed New Wolfington. I trust that was clear enough. Oh, but there is one thing I worry about..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's that, Dr. Morris?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hmm, I wounder... Yes. Let me ask you a question, young man. Do you believe in evil? The kind that revels in the destruction of the world and of mankind with it? The kind of evil that thrives on the pain, suffering, and sorrow of innocents?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Uh...what are you talking about?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, I'm just thinking out loud. Pay me no mind at all. I've actually forgotten what it was I asked you. Ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Status report, Lin."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Unidentified units in the field, sir. They're ignoring all communications. "), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("They're advancing on our positions! Move out!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tee hee hee! Is it OK, Daddy?"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Of course, little one. Play to your heart's desire. Do remember to record the battle, though. I must know how each soldier fights... and how each one dies."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Father, I cannot condone this. I can see no reason for us to attack these people. It is meaningless--"), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Meaningless? Whatever are you babbling about? Is there a more meaningless expenditure of energy than life itself? I think not!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Get 'em! Get 'em! Boooom! Tee hee!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
        } // Start Of Turn End
        if (variables.createVariable("UnitsSpawned").readDataInt32() === 1 && player === 0) {
            variables.createVariable("UnitsSpawned").writeDataInt32(2); // 0 Modify Variable
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella! What is it? What's wrong?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's...nothing. I'm fine. The factory...  Its systems have been updated. It can now produce anti-tank units."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("What?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("She's right. The system just rebooted."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Anti-tank units will prove to be very effective against the war tank."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think she may be right. Amazing! We may have a chance after all!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella, I want an explanation. Now. How in--"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Later, Lin. We've got a battle to fight. Anti-tank units, huh? Let's see how tough that war tank really is."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("yes..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
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
            var playerOs = map.getPlayer(0);
            playerOs.changeBuildlist("ANTITANKCANNON", false);
        } // Check Variable End
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
        var playerCount = map.getPlayerCount();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            if (player.getUnitCount() === 0)
            {
                if (i === 0)
                {
                    player.setIsDefeated(true);
                }
                else
                {
                    if (variables.createVariable("UnitsSpawned").readDataInt32() === 0)
                    {
                        var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's wrong, pumpkin? Did those bad people blow up all your toys? I suppose you'll be wanting more. Well, what's the magic word?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("PLEEEEEEEASE! I wanna big BOOOOOM!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("How could I deny such a well-mannered little girl? I look forward to seeing how they respond to the world's mightiest tank. This should prove to be most enlightening."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Look! Reinforcements! And... Ohmygosh! Th-that tank! It's enormous!"), "CO_WILL", GameEnums.COMood_Normal, "#f00008"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Bring us to speed, Lin! Give us what you have on that monster."), "CO_BRENNER", GameEnums.COMood_Normal, "#f00008"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sorry, sir. Our database has no intel on that tank."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("That is a... war tank."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("A war tank?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Compared with standard tanks, it has vastly superior armor and firepower. It is currently the most powerful ground unit in the world today."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("And how are we supposed to stand against it? It's too darn big!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
                        var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I don't know."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
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
                        variables.createVariable("UnitsSpawned").writeDataInt32(1); // 0 Modify Variable
                        map.spawnUnit(15, 6, "MEGATANK", player, 10)
                        map.spawnUnit(14, 9, "HEAVY_TANK", player, 10)
                    }
                    else
                    {
                        player.setIsDefeated(true);
                    }
                }
            }
        }
    }; // actionConditions
    // scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
