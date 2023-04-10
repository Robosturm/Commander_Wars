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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No no no! This is so unfair! I let my guard down for an instant, and you go and smash all my nice soldiers! Poo! If I had reinforcements, I would squash you like bugs! But Father wants me to spare you. So I guess I will this time!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What do you mean, she's gone?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm sorry, Will. I guess I should have kept an eye on her."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("There is no evidence that our camp has been infiltrated. It looks like she left of her own volition."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But why?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Perhaps she thought she was saving us."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Dammit! This isn't the way it was supposed to happen! I didn't want to abandon anyone, and now Isabella abandoned us!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You! You are to blame for all of this! It's all your fault!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You again?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Look around you! Look at the wounded and dead! Look at the civilians on the brink of starvation! No one had to fight. No one had to die. If you had just surrendered and handed that girl over, this would never have happened!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("... ... ... ..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You are a sad little man with no idea what he is talking about."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Save your words! All I know is that we can't trust this brat with our troops anymore! Why, if I had been in command, none of this would have happened! Even without training, I would be a far superior commander to this one! No one wants you in command, Will. I speak with the voice of all the people on this matter!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Packing up, huh?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's right."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...I see. You know, a long time ago, I advised you to walk away and not look back. So I guess it would be hypocritical of me to make you stay now, huh? Just tell me you're doing this for the right reasons. Tell me it isn't because of that idiot who calls himself a mayor."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No, it's not that. It's just... I've learned some things about me, Lin. I don't think I\"m the kind of person who should be in charge. I want everyone to be safe, but I'm unwilling to sacrifice even a single person. I fought a needless battle where many people were hurt and killed, and... And I think it was wrong. I should have sided with the group instead of the individual."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...I never expected to hear that from you, Will."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I wanted to be like Captain Brenner, you know? Strong and generous and...well, perfect. But I kinda made a mess of the whole thing. I'm appointing you as our new commander, Lin. These people need your logic and good sense."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Where will you go?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm going to go save Isabella."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("By yourself? I thought you had seen enough needless deaths. Are you really so eager to check out?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No, Lin. I'm not. But I know it's a possibility. But at least this time, I'm the only one being put at risk."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("OK. I won't get in your way. You're free to decide your own path. But for what it's worth... I'm sad to see you go, Will."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Father..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Please don't call me that. I regard you as an object, not as family."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So I am a clone?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes, you are. And you served your purpose well. I am satisfied with what I have been able to learn from you."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But... Why did you create me?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I needed someone to experiment on."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Th-that's the only reason?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That isn't enough?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So that's... That's all I am."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("OK... If I was Isabella, where would I be?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Where are you going?"), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Gyaaa! Who are you?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You should be careful. What if the enemy were to find you here? You are William. You are their commander."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("OK, let me ask this again. Who are you?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I was given the name Cyrus. I am a creation of Dr. Caulder."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You're one of his clones?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wait! Please do not be afraid! I want to help you."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Why would you want to do that?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The girl you call Isabella... I don't know her, but she is my sister. I know how you can save her."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_road_night.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...She's a clone?! "), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("According to Caulder, yes. She's one of group of mass-produced clones. And his daughter. I guess that makes her a clone of Caulder..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hmmm... I'm not sure that is completely accurate."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What do you mean?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Technically, it is possible to create human clones. But even if you duplicate the DNA... The clones are not perfect copies. They're more like a set of twins. We don't know if he used recombinant DNA or an extra chromosomal--"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wait, stop. Small words, please."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Well, if I were to make a clone of myself, I would have a baby with my DNA. But we would have different ages, different environmental influences, and so on. It would not be possible to engineer a perfect copy of my adult self. That's why I think each Isabella is a unique, individual person."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But Caulder talked about them like they were interchangeable."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("There is another possibility. What if you took those cloned babies you were talking about and isolated them? Completely cut them off from the outside world, like rats in a maze. By eliminating all outside influence, you could theoretically mass-produce identical clones."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Well... I suppose... In theory, that might work, yes. But such an existence would likely shatter the mind of anyone who had to undergo it! Only an unethical monster would think of doing such a thing!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We're talking about Caulder, Dr. Morris. I don't think he's ever been concerned with ethics."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes, but..."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Never mind. We're getting distracted. Isabella is what she is. That's not the problem requiring our attention right now. We need to figure out how we're going to survive."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Caulder is threatening to bomb us again. If we don't hand over the girl by tomorrow, they will launch an all-out assault."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I know I'm not a military man, but I don't see how we can survive another assault!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_great_owl_out.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("As long as the Great Owl flies, we have no chance of victory."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_great_owl_out.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But we won the last battle!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("True, but in that battle, bombs were dropped on a fixed area only. They weren't trying to win. They just wanted to display their power. If they decide to start bombing our forces directly... Well, like I said. Victory would be impossible."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I can't stand this! That man is simply toying with us! He can just bide his time until we surrender Isabella to him!"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander? What are we going to do?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Isabella stays with us. We aren't going to abandon anyone."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Perhaps you think it is noble to die for your principles, but i disagree."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Gage!"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I am not going to lead my people on a suicide mission. The life of one girl against the lives of all of us seems an easy choice."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What's wrong with you, Gage? How can you think of handing her over to that monster?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("General Forsythe ordered us to protect the lives of his troops. That is our only duty. Or has your newfound love of Rubinelle clouded your judgement?"), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You're a jerk. You know that? And did you forget what happened to Forsythe? He surrendered with honor and got a bullet in the back for his troubles. Caulder isn't going to take the girl and just walk away to let us live in peace! He'll take her and then bomb us all into the ground! I say we fight to the last man!"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander? We need a decision."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I made my decision. She stays. We all stay. Things looked bleak before, and yet we managed to come around. Right? As long as there is life, there's hope."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's enough! The civilians are tired of your fighting and your lies!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Who let him in here?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The civilians are agreed! We should hand over that stupid girl and end this! Then all our problems will be over!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Get him out of here."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What?! Hey now, stop... Get your hands off me! Don't you know who I am?!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ready yourselves. I think Caulder is coming."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...It's her."), "co_civilian7", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(map, qsTr(" ...Mary? Why are you looking at me like that?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Like you don't know."), "co_civilian7", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("There she is! Gi! Get out of here! Shoo!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wh-what?!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You heard me! You're not wanted here anymore! You're going to get us all killed! Now get out of our sight!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But... I don't understand!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Everyone agrees with me! You're useless baggage! What are you still doing here?! Get out of here! We can't stand the sight of you!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But I... I didn't..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The Great Owl is approaching."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog44.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I know. I can hear it."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog45.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, this is so exciting! I can't wait!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The Great Owl will be in attack range within minutes."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 3 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The Great Owl is starting its bombing run. ...It's targeting our troops."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("All units, disperse! Large groups are an easy target!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            var meteorTarget = co.getOwner().getRockettarget(3, 5);
            gameScript.doGreatOwlDamge(meteorTarget);
            var animation = GameAnimationFactory.createAnimation(map, meteorTarget.x - 2, meteorTarget.y - 3);
            animation.addSprite("explosion+black_bomb", 0, map.getImageSize() / 2, 0, 1.875);
            animation.setSound("explosion+land.wav");
            dialog1.queueAnimation(animation);
            // Dialog
        } // Start Of Turn End
        if (turn === 4 && player === 0) { // 0 Start Of Turn
            var meteorTarget = co.getOwner().getRockettarget(3, 5);
            gameScript.doGreatOwlDamge(meteorTarget);
            var animation = GameAnimationFactory.createAnimation(map, meteorTarget.x - 2, meteorTarget.y - 3);
            animation.addSprite("explosion+black_bomb", 0, map.getImageSize() / 2, 0, 1.875);
            animation.setSound("explosion+land.wav");
        }
        if (turn === 5 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I... I am here. Can you hear me?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ah! It's you! So you remembered how to make emergency communications! Don't push yourself too hard, dear. You know how fragile you are."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Promise me."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hmmm? Promise you what, sweetie?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't care what you do to me... I just want you to leave them alone."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You're a curious specimen aren't you? Your sisters love chaos and death, whereas you embrace mercy. This is a worse design flaw than I anticipated..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm... I'm not like them."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Listen to me, Isabella. ...Bah! What an absurd name... I want you to join us on the Great Owl. I'm sure we can find something for you to do. Some experiment or other..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You seem confused."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You said you wanted to dissect me! To analyze my malfunction! ...Isn't that why you want me?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hmmmm... I did say that, didn't I? Well, I'm afraid that was a lie. You are not important to me in the least. I have countless identical copies of you! What's another, more or less?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Th-then why...?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Dear girl... I was interested in THEM! I contrived a moral dilemma to make those people fight among themselves. I was curious how they would react. That is all."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You're terrible!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Don't take that tone with your father, young lady! ...You're an odd one, aren't you? What did those people do to you? But we can experiment on such things later! Cyrus! Turn about! Secure the product and fly to the nearest base."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Father... When will your madness end?"), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Eh? Do you have something to say, Son?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I know I am not human. I know that you made me in a lab. But I will no longer stand idly by and let you do this! You treat us like puppets! Toys to be used and broken and then discarded."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("My my, Cyrus, you are a perceptive boy. That's exactly what you are to me! A puppet! A plaything!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No! I am a person! I am real!"), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hmmm... I don't much care for this line of reasoning. I may need to get another model."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't understand why, but it seems the Great Owl is retreating."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Who cares why! Let's take our chance and finish this!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground(""); // 1 ScriptEventDialogItem
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
    // preconditionend
    }; // actionConditions
    this.doGreatOwlDamge = function(meteorTarget)
    {
        var fields = globals.getCircle(0, 2);
        // check all target fields
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var x = fields.at(i).x + meteorTarget.x;
            var y = fields.at(i).y + meteorTarget.y;
            // check if the target is on the map
            if (map.onMap(x, y))
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null)
                {
                    var hp = unit.getHpRounded();
                    if (hp <= damage)
                    {
                        // set hp to very very low
                        unit.setHp(0.1);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
            }
        }
    }

// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
