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
    this.victory = function(team, map) { // victory
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (team === 0) { // 0 Victory
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Sir, the Talon Gun has been destroyed! ...Please don't hurt me."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Drat! They looked so pitifully weak, and yet they broke my toy! Hmph! I clearly was going too easy on them. I'll teach them to be so mean! Initiate plan Ultra! Let's hit them with something harder!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's enough, Tabitha. Today's experiment is complete. We must retreat so I can analyze these new numbers."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But, Faaaaaather! I wanted to crush them like flies!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander, we should return to base camp."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Where's Dr. Morris?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("He's in one of those buildings, looking for survivors."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("He really is a good person. Though you wouldn't think so from his jokes..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Agreed."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Everyone! Listen up!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Dr. Morris? What's wrong?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We have an emergency! Come to my mobile laboratory now."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Come on! Come on! There isn't time for this! Don't just stand around!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Doctor, please! What's going on?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This better not be one of your jokes."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ruins_night.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Look at this picture..."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh... Oh no."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I found it in the village's medical center. I think it was taken recently."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It's the Creeper. But it looks different..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Viruses are capable of mutation, so that's what I thought this was. But this is a dramatic change. It's too perfect."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Explain."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I think it's a biological weapon."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No, that's... That's impossible! Who would do such a thing?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I haven't even told you the worst part."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("There's a worst part?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This victim was twenry or thirty years old. He was an adult."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...That's bad."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So the new disease is no longer restricted to children."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This is a very serious problem. I'll start working on a cure right now. But first we should see if anyone else has been infected."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flowers3.png"); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Victory End
    }; // victory
    this.turnStart = function(turn, player, map) { // turnStart
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander, this is our present position. We're heading toward this village in the north."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Do you know the village?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It's a small trading post. The community has always been friendly toward us. We should be able to resupply there."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't get it. There's no one here."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...I don't like this. Something's wrong."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Do you think they were attacked? Maybe Greyfield beat us here."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Blast it all! Well, whatever happened, we need to look for survivors. Let's split up and search the village."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I can help too."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Anything?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("All we found are bodies and traces of small-arms fire. It looks like these people tore themselves apart."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why would they do that?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Who knows why these kinds of civil wars start? It could have been hunger or disease. Or mybe they just couldn't stand each other anymore."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This is terrible..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("They're all dead. I'm sorry, Will."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_flower_field.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Father! How long must we stay in this wretched place? I'm growing weary of all this muck and dust. And my shoes are in a terrible state!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("My dear, be patient. There is a wealth of information for us to discover here. By the way, it seems that we have visitors. They are standing in the central square. Tabitha, would you like to test out that new toy of yours?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, Father! Can I really?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("May I, Tabitha! May I! Good grammar is important for a little girl. And yes, you may. Nothing is too good for my little Tabitha. But remember, this is serious work! I must watch and record the results. I am fascinated to see how they will react to this little weapon of mine."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You never tire of your research, do you, Father?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_ruins.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander, come in! Commander! ...Dammit, Will, talk to me?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Tasha! What's wrong?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Someone's trying to kill me, that's what's wrong! I hate it when people do that! I'm going to take command and make 'em pay! Oh, and Will?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I was... I mean... Look, this ain't easy for me, but... I... I said a lot of bad things about Captain Brenner, and I was wrong. You were right. He was a good man. ...Sorry."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You don't have to apologize, Tasha. I'm just happy that you know."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The captain gave his life for us, so now I'll fight with you. You may be Rubinelle, but we're all on the same side now. I will find the people who killed your captain, and I will have revenge! All right, you devils! I'm gonna come in there and blast every last... ...The hell is THAT thing?!"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It looks like a weapon."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I've never seen anything like it before."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It is a Talon Gun. It was developed by the private defense contractor IDS:"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Isabella?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Development began six years ago at the request of the Lazurian government. However, the cost of the project quickly spiraled out of control. Work has halted after only a few prototypes were constructed. That is one such prototype."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("How the heck do you know that?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(map, qsTr(" I...I don't know."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It is odd that you know about both Rubinelle and Lazurian weaponry. Very odd."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I-I'm sorry, Lin. I'm sorry. I don't know why I know all these things..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's all the enemy has? Pssh! This is too easy! I feel insulted! But Father must be obeyed! I will swat them like the puny flies they are."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This weapon is huge, but structurally similar to regular cannons."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Great! If it bleeds, we can hurt it! I mean...if it bleeds oil. Or something. Never mind! Can we check its attack range?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We learned how to check attack ranges in the military academy. Um...hold on. Lemme get my textbook here... Darn, where is it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ahem. Try page ten, Commander. It was a basic lesson. Of course, I'm not suggesting that our commander has forgotten this..."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No, of course not! No, I was just... seeing what the textbook said..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
        } // Start Of Turn End
    }; // turnStart
    this.actionDone = function(action, map) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
        var variable1 = variables.createVariable("variable1");
    // preconditionend
        if ((map.getTerrain(8, 5).getBuilding() === null || map.getTerrain(8, 5).getBuilding().getHp() <= 0) && variable1.readDataBool() === false) {// 0 Building Destroyed
            map.getPlayer(1).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Building Destroyed End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
