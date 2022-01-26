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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Father, do something! This can't be happening!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Failures... FAILURES! All of my children... Abject, useless FAILURES! I must retreat to The Nest and plan my next move..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...William... Good to s-see you again..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Cyrus!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Knew you would...survive..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Help! Medic! Someone get Dr. Morris right now."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Don't worry... T-too... Too late for me. ...You survived. That is...enough..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No, Cyrus, don't give up! You have to hope! There's a doctor on the way right now!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Have one...request. Please listen..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We owe you everything, Cyrus. I will do whatever you ask."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Live."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...I'm dying. Death should...should mean something. Defeat...Caulder. L-live in...in peace... That is my wish."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We will defeat him, Cyrus. I swear it."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Good. He is...north of here. His main lab...called...The Nest... Antidote...there... Enough for...for all..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Cyrus, hold on!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...My life has...meaning...now. That was all I ever wanted."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...He's gone. Cyrus, I will keep my promise to you. We will live. Your death will have purpose."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm here as the civilian representative. We fulfilled our side of the bargain, Dr. Caulder. Now we expect you to be true to your word and give us what we need."), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Of course, of course! I am a man of science, not some kind of despot."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We need medicine! We need the antidote to that wretched virus."), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes, I understand. Without this antidote, you will surely die. But this is not easy to mass-produce. It will take time until I have enough for all of you. Right now I have just this single dose, which is surely--"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Give it to me!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But, Mr. Mayor, this is only enough for one person. And without it, I may have trouble producing enough for the rest of your people."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's fine, that's fine! Just give it to me!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Very well. Just press this against your skin to inject it."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's it, right? Now I won't die from that terrible virus?"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That is correct. Congratulations."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Now, about food supplies. We require a large quantity of... Ho, there! Where do you think you're going?"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm sorry, I have an experiment that is about to begin. It involves those remaining soldiers from your battalion."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wait a second! What do you mean by that! You said that if we handed over the girl, you would give us what we needed!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hmmm? Oh, yes, that. I'm afraid I was lying."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("B-but...you promised! We gave you the girl! You have to help us! Help me at least! This will not... Not... UUUURRRGGGGHHHH!"), "co_major", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, and that medicine you took was not the antidote. Hello? Can you hear me? Mr. Mayor? ...Fascinating."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("... ... ... ..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So, young Cyrus. It is your turn to die. Does my treacherous son have any final words? An apology perhaps?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I do not feat death. And I do not fear you. My death will not be in vain. They will defeat you."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It is too late, Son. Your champions perished when the Great Owl crashed."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I do not believe you! You are capable of nothing but falsehoods and lies."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No, I speak the truth. Your pitiful attempt at a rebellion was entirely meaningless. I will live on, Cyrus. But this represents the end of days for you. Have you no desire to apologize for all the trouble you've caused me?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'll never ...urgh...apologize to you. They are ...alive. And they...will defeat you..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Are those truly your final words? Oh, how you disappoint me. The end will not come quickly. That's not how this particular poison works, you see? You will suffer terrible pain, and death will be long in coming. I must record it all, you see."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...They will...defeat you..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Gage!"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Tasha, the enemy forces are mobilizing. It seems the girl was not all they wanted."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I told you we couldn't trust Caulder! We never had a chance."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So it seems."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Everyone criticized Will, but I knew it would end like this."), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh well! What are you going to do? Let's go down swinging, eh, Gage?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Agreed. All units...you're on your own. Try to hang on as long as you can."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Don't give up! There still might be a way for us to pull this out! ...And if not, let's give 'em holy hell!"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Once you are out of the way, Father's experiments will be over. Then the world will belong to Father and me, with no one around to bother us!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 3 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, poo! Father!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What is it, my dear?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I just heard that additional enemy forces are heading this way."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Sweetheart, you must be mistaken. There are no soldiers left to..."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Tasha!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Will! Holy cow, is it really you?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The Great Owl has been destroyed, and we're here to help! Let's take down Caulder and end this thing right now!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But this... This is impossible! I ordered Penny to crash the plane and kill everyone on board!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Thank you, Penny! Thank you for listening to me!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...Penny doesn't understand! Daddy told Penny to die, but you told Penny to live! But then Mr. Bear said he didn't want to die and Daddy was stupid! Penny is confused..."), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It's OK, Sister. Everything is going to be OK."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
            map.spawnUnit(3, 0, "ANTITANKCANNON", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(1, 1, "INFANTRY", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(3, 1, "ROCKETTHROWER", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(4, 2, "INFANTRY", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(0, 3, "FLAK", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(3, 3, "MISSILE", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(5, 3, "FIGHTER", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(1, 4, "T_HELI", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(2, 4, "MEGATANK", map.getPlayer(0), 0); // 0 Spawn Unit
            map.getTerrain(2, 4).getUnit().makeCOUnit(0);
            map.spawnUnit(4, 4, "BOMBER", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(6, 4, "BOMBER", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(5, 5, "FIGHTER", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(3, 5, "MECH", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(0, 5, "FLAK", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(2, 6, "APC", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(4, 6, "T_HELI", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(3, 7, "MEGATANK", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(1, 7, "INFANTRY", map.getPlayer(0), 0); // 0 Spawn Unit
            map.spawnUnit(3, 8, "INFANTRY", map.getPlayer(0), 0); // 0 Spawn Unit
        } // Start Of Turn End
        if (turn === 4 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why do you care so much about her little boy?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You must be Isabella's--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("My name is Tabitha, and I am a true little girl! The one you call Isabella is nothing but a lab rat."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You're wrong. She's a human being."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("She is nothing more than a mass-produced imitation. A cheap toy! She as created as a subject for my father's experiments. Nothing more. I suppose I should feel sorry for her, but that was her fate."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You never had a chance, did you Tabitha?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Quiet, you! Such experiments benefit all mankind. Sometimes sacrifice is necessary in the name of progress. My father says so."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Isabella is one of us."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ha! I could make ten more tomorrow and you would never know the difference."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wrong. I would know. There is only one Isabella. She is unique."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Psh! This is like trying to reason with an infant. I grow weary of talking to you!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
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
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
