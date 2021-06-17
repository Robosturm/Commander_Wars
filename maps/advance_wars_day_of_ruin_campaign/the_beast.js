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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! You're dead, punk! The Beast will destroy... Destroy...I'm...coming for...you... NO! My head...urgh..."), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Now, let's investigate the shelter."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is it. These are the coordinates Isabella gave us."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! There really is a shelter here! What did I tell you, folks? That girl hasn't got a lying bone in her body. Knew it right from the start!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("All we need now is a way to open the security doors. Ideas, anyone?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Welcome to Fortress. Enter access code."), "CO_RANDOM", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("The access code is of standard military issue, but I don't have the proper security clearance. Captain, do you think your clearance is high enough?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I doubt it, but we've got to try. I'm only a captain, but here goes... Code number 0848124390. Damn... Guess the politicos never planned on letting grunts into their private club. Looks like I should have taken those promotions when I had the chance."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("More importantly, Captain, we only have one chance remaining. I think we're done."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Don't say that! We've come so far... There must be something we can do. We have to get inside. We have to! Otherwise, Isabella will...she'll..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("We need someone with the right clearance, but..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't see any generals or high-ranking government officials in the area, Doctor. Did you happen to see the president's corpse on the way here? Perhaps he's carrying the code."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's morbid, Lieutenant. And decidedly unhelpful. Truth be told, we don't have time to go looking for the code. It's not sophisticated, but how about starting at 0000000000 and working our way up?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Now who's being unhelpful? We only have one chance left."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm merely offering suggestions! There's no need to upbraid me so!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm merely stating the facts. And the fact is, we have no chance of guessing the correct access code. We have one more try to get a ten-digit code correct. The odds of guessing it right... ...are almost zero."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're talking a probability of one in ten billion. Now, if this was just a story and I was the hero, something miraculous would happen..."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'd rather try and fail than just walk away. Anyone have a favorite ten-digit number?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Ten digits... Wait a second!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it, Will?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("9693872914... That's ten numbers, right?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Don't tell me you actually HAVE a favorite ten-digit number."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um...Not exactly, no. I just have a feeling. I think that may get us inside."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well...what have we got to lose? Go ahead, Will."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Right. Please let this work... 9693872914..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Identification accepted. Security locks disengaging."), "CO_RANDOM", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_closed.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("What in the world?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("You did it, Will!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I don't know what to say."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you some sort of idiot savant? Or wait...Perhaps YOU are the president's secret child! Oh ho! Oh ho ho ho ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("What?! No...that's not it. It's the number Isabella was whispering."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("What are you talking about?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("She's been saying it randomly ever since she remembered this shelter was here. In fact, she said it again just before she got sick."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("So once again, it's Isabella to the rescue. I feel like we've got our own guardian angel. Do you think she really is the daughter of some high-ranking official?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's the one explanation I would be comfortable with."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_bunker_open.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, in any case, we're in! And that's something to celebrate, my friends! Oh ho ho ho! Well, in any case, we're in! And that's something to celebrate, my friends! Oh ho ho ho!Well, in any case, we're in! And that's something to celebrate, my friends! Oh ho ho ho! Now that we're in, we should be able to find the machinery and medicine we need."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'll go back down and get Isabella. "), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, you won't. You're not allowed to have any contact with the patient, remember? I'll take care of Isabella. It'll give me the chance to tell the mayor that we're inside."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Take some of the men with you. There's no sign of hostiles, but we can't let our guard down."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'll take charge of reconnoitering the shelter. Will, you're with me."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_parking_lot.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Brenner! I need to talk to you!Is it true that the girl traveling with you is sick?"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, it's true. It's nothing you need to worry about, though. The virus only affects people under a certain age. No one in your group is in danger."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's not the point! Sickness and disease make people nervous, and nervous people panic. We can't have that! You've got to get rid of her. We can't afford the risk."), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think you need to stop talking now. We're in this together. All of us. Is that clear?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, it's not! Would you put us all in danger to protect one girl? We don't know who she is or where she came from! She has to go, Brenner!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen carefully, because I'm only going to say this once. No one gets left behind. Not you, or your people, not Will, and not Isabella. If you have a problem with that, you're free to leave whenever you want."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("You...you fool! You're too soft, Brenner! If this goes wrong, it's on your head! I'll hold you personally responsible!"), "co_major", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("How's Isabella doing, Dr. Morris?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're going to worry no matter what I tell you, so here's the truth. It's not good, Will. She needs medicine, and she needs it now."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Can I see her?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Absolutely not! We've had this conversation before. This virus is most dangerous to people of your age. I won't allow you to see her."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("But, Dr. Morris, I--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Once we get to the shelter, there's a good chance I'll be able to treat her. But until then you have to stay away. You don't want to get sick too, do you?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't care! What would it matter anyway? If I don't see her now, I may never get the chance to apologize. So please..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Your apology can wait, boy. I'm more concerned with her well- being, and you should be too."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's that supposed to mean?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't wish to betray doctor-patient confidentiality, but I have spoken to her. What's troubling her most is the fear that she's become a burden to us. She thinks it would be better if she'd died before you found her."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("How can she think that?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("But she hasn't given up hope. She never even complains about the pain. The other Creeper patients I've treated have all begged me to...end their suffering. But Isabella has chosen to live. And do you know why? It's because she owes that life to you."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("She believes that if she refuses to give up, we'll find a way to cure her. Her first memory is of you telling her she was going to be all right. She trusts you implicitly, the way a newly hatched chick trusts its mother. She believes everything you told her about having hope. I suggest you believe it too."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("You...you're right. I'm sorry, Dr. Morris. Thank you."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("No need to apologize. I can't allow you to see her, but... Oh ho! Yes, you could try speaking to her through the door."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Really? Would that be OK? "), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Certainly. Just for a little while, though."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_desert_rock.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("This route leads directly to the foothills of the Seratta mountain ramge. However, recon reports those raiders are back and blocking the way."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Set up an ambush, have they? Seems they really are hell-bent on revenge. We're going to have to finish this once and for all. Advance!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("GYAAARRR! Where the hell am I? Where did all these copters and tanks come from? Oh, my head... Wait! It's SOLDIER BOY!!! Time for The Beast to hunt! GYAAARRR!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("'Bout time you showed your ugly mugs!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why are you doing this? Get out of our way! We have to save Isabella!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isa-whatta? You think I care what you're doing? I want BLOOD!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("You may have tanks and guns and men at your command, but... You're not a soldier! You're nothing but a murderer!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gwar har har har! Murderer? ME? And you're a snot-nosed punk!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("War IS murder! You hear me?! Soldiers are trained to KILL! That's battlefield learnin', punk! Your textbooks teach you that? HUH?! You wanna learn 'bout war? Fine by me! The Beast'll teach you good!"), "CO_BEAST", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
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
