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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("I just don't get it! I just don't get people like you! The world's gone, brother! Ain't nothing out there worth fighting for! And yet here you are with your friends and your...hope! I don't get it! You oughta worry about YOU! Just like me!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("We'll need our hope when we rebuild this world. That's what you'll never understand."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("We got wounded off the battlefield... Is everyone accounted for?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um...Will?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Over there... There's someone in that building..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK. Let's check it out."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Sorry, Isabella, but I just don't see anybody. And I can't imagine anyone would be here during a battle--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Good day. Please, don't mind me. I'm just conducting a little research."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... Hi there."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ah, Will. It is so very good to finally meet you. I believe you are the new commander of Brenner's Wolves. Congratulations are in order."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("What in the... How do you know that? Who are you? And why are you taking notes in the middle of a war?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("My name is Dr. Caulder. And as for my copious note taking... Well, I am researching the effects of warfare of the human spirit."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're doing what now?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ever since the meteors, I have traveled this world observing and recording. Those falling stars were a true boon for men of science like myself."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're...researching us?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I have been able to witness and documents the true nature of humankind! I have watched people fight among themselves as their food supplies ran dry... I have seen villages panic at mere rumors of disease and famine... I have observed it all. And I have found it to be FASCINATING!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're disgusting!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Merely curious. At first, I allied myself with the Lazurian Army. I wanted to see how war would be conducted in a world without law. But that Forsythe had a misplaced chivalrous spirit and was of little use to me. I offered him all manner of terrible new weapons, but he denied me at every turn. Little men such as he have always stood in the way of human progress."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("General Forsythe was a decent, honorable man."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Qualities that I hope will no longer be respected in this new world of ours! At any rate, I had no use for him. So I turned instead to one devoid of all morality...Admiral Greyfield. He embraced my weapons! Even now, he prepares to use my masterpiece!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("You... You're evil!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Eh?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Dr. Morris once asked me if I believed in evil. In people who felt glad the world was destroyed. Now I know they exist. I'm looking at one."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Were I a more emotional man, I might be hurt by such words! I have done nothing wrong, Will. I am the very model of an objective, scientific observer. Like all men of science, I have merely followed my heart and studied what interested me. But I see you will never understand that. A pity!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr(" Oh, before I forget... I must recognize all the hard work my daughters have been putting in..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("We are making steady progress. Do carry on."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aah... AAAAAAAAAAA!!!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella! What did you do to her?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I suggest you speak to her instead of me. She doesn't have much time left."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("You!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield is on his way with my masterpiece in his hands. Whatever will happen...? Hmmm... I think I will need more notepads."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("...UNgh..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella! Can you hear me! Are you OK?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-yes... I'm s-sorry, Will."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("I was so worried."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("That man... I've seen him before."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("Where?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't know. I don't know who he is or where I met him. But I know him. He is a very, very frightening man..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("Good heavens! Dr. Caulder is alive?!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("You know him?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("I know of him! And more than I'd like... Caulder is an infamous figure in the medical world. A mad genius despised by all his contemporaries. He was thrown out of the medical academy for conducting bizarre, unethical experiments. A year later, he sustained terrible injuries in a suspicious plane crash. But this is only emboldened him to continue his horrible research. This world is a much more dangerous place with him in it."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait. Are we talking about the same Caulder? The one in charge of IDS?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(qsTr("IDS?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog45.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(qsTr("IDS stands for Intelligend Defense System. Before the meteors, they were the world's largest military contractor. They had connections at the highest levels of Rubinelle government, with contracts in everything from weapons to medicine to food. The war between Rubinelle and Lazuria was especially profitable for them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog46.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog47 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why, he's nothing more than a common war profiteer!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog47.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog48 = GameAnimationFactory.createGameAnimationDialog(qsTr("Caulder said he gave Greyfield all of his new weapons. He was happy too. Like a kid with a new toy. He said he wanted to study the results when Greyfield used his masterpiece."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog48.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog49 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh dear! I don't even want to know what that is."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog49.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog50 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella? Something tells me you know about this."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog50.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog51 = GameAnimationFactory.createGameAnimationDialog(qsTr("HIs masterpiece... It must be the Caulder Missile."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog51.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog52 = GameAnimationFactory.createGameAnimationDialog(qsTr("That doesn't sound good."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog52.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_ids.png"); // 1 ScriptEventDialogItem
            var dialog53 = GameAnimationFactory.createGameAnimationDialog(qsTr("Both nations possessed enough missiles to destroy the other ten times over. Caulder Missiles were developed by IDS and located in Rubinelle and Lazuria. They were designed to launch at the same time. All of them. In theory, they functioned as deterrents. But now..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog53.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_missile_impact.png"); // 1 ScriptEventDialogItem
            var dialog54 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield, you madman..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog54.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog55 = GameAnimationFactory.createGameAnimationDialog(qsTr("If those missiles launch, it's going to make the meteors look like... like FUN.teors! ...Sorry. Even I thought that was bad."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog55.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog56 = GameAnimationFactory.createGameAnimationDialog(qsTr("We have to stop him."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog56.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("We have made a small breakthrough and slowed the progress of the virus. By using a few common materials, we've managed to create a vaccine of sorts."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's great news, Doctor!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Unfortunately, we are running low on materials to create more of it. The nearby medical facilities have all been looted, most likely by raiders."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Greyfield."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hmmm? What do you mean?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Admiral Greyfield's number-one priority is his own safety and well-being. I'm sure he is behind the looting of those facilities you mentioned. And I have no doubt he possesses huge quantities of the vaccine."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("First Captain Brenner, and now this?! I'll kill him!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Remember how I said emotion had a time a place, Will? This isn't it."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sorry! Sorry. You're right. Let's just track down Greyfield and finish this once and for all."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Keep it simple--never give up. If you can do that, anything is possible."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Got it!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's the spirit."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("I understand, Captain."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will? Are you all right? You seem distant."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr(" ...Huh? Oh, sorry, Isabella. I was just remembering what Captain Brenner said after he rescued me. Whenever things are at their darkest, I try to think about those words. I owe my life to Captain Brenner, so it's just... It's my way of remembering him."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Can I... Can I help, Will?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("No. Don't worry about me. I know what I need to do. Greyfield and Waylon are responsible for Captain Brenner's death, and it's time for them to pay. I'm going to end this war."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lazurian recon units... I mean, OUR recon units have spotted the enemy. ...Wow, that's going to take a little getting used to... Commander! Enemy units are attacking from the rear."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ground or aerial?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aerial."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is it Waylon?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Looks like it."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Let's go! All units, move out! And show no mercy!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (turn === 1 && player === 2) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("One more mission and I can retire to my burned-out mansion! Life of luxury, here I come! Now hit me! Whooooo!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 2) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, I'll be dipped in nacho cheese... Someone put the damn kid in charge! You think you got a chance against me, brother man? Think again! I'm gonna get you like I got Brenner! Whoooooo!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Keep talking, Waylon! It'll be the last thing you ever do!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aw, don't sound so hostile, kid! I know you missed me! You gotta live my life, brother man! Food, drink, girls... I got it all! I'm living the high life while you're out there eatin' bugs and whatnot. So how about it? Make things easy on your old selves... Just hand over the Lazurians, and I'll see that Greyfield rewards you. What do you say, brother man?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Save it, Waylon! We're going to fight and we're going to win!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aw, hell! You're as bad as ol' Brenner! All ready to die just to defend a buncha damn Lazurians. Well, nothing for it! Looks like I'm gonna have to teach you a lesson!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't need your lessons, Waylon."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stuff it junior! You and your little toy soldiers make me sick! All self-righteous... Struttin' around like you own the damn joint... You ain't helpin' people because you care! You do it to feel important! You're a hypocrite, and you ain't better than me! Least I'm honest! Least ol' Waylon's honest about who and what I am."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Maybe you're right."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Praise Greyfield! The boy sees the light!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Waylon...Maybe we are the same. You do what you want to do, and so do I. You fly around and act like a jerk because it's what you want to do. And I help people because it's what I want to do."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Why, you little sack of crap."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Does that make me a hypocrite? If I do nothing, people will suffer. People will die. You say I do this to feel important, and maybe you're right. But if that saves even one life, then it's worth it. Understand?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aw, nuts! I heard enough! Folks like you never live very long, anyway. I'm gonna bury you, kid. Give my regards to Captain Courageous!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
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
