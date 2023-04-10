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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Reinforcements have been sighted."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr(" Pull back! Pull back! We need more time to prepare our forces!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes, you probably do."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You...!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We are readying another attack, but I will ask you once more... Will you give me the girl? In exchange for the safety of your troops?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why do you want her?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't want her. I own her. She is one of my products, and I need her back."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("One of your products?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why, yes. Surely you sensed something strange about her? That girl is my daughter, though not in the sense that you imagine. I built her. I created her."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh no..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("My daughters are clones. I created them in laboratory. They were the final step in building an army of low-cost, cloned soldiers."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("IDS specializes in products that give generals control during battle. Armies have long been ungainly things with many communication problems. Even modern armies suffer from late or misunderstood communications. But my products--my children--can control entire armies single-handedly. Like a giant with a hundred arms, they can wield innumerable weapons! ...Although I must admit, they have been anject failures as humans."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_clones.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Isabella is a clone..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Whyever did you give her that insufferable name? And yes, she is a clone. A mere doll designed and controlled by me. But one day she went... haywire and fled my laboratory. This was highly unusual, and I must discover its cause. I need to disassemble her and locate the source of the malfunction."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...You can't!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I can and will, and you will give her to me, or I will take her by force. It seems you have developed a certain attachment to my daughter. You are being too emotional. She is a product! An experiment! A nothing. And she belongs to me!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Enough!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I await your answer."), "CO_CAULDER", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("How odd. The war is over."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Who wins, Daddy?"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The side that lacked military might but had superior tactics. I am surprised. They may prove worthy specimens for further research."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("May I play with them, Father? Please? I promise to be ever so good."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, Tabitha. You know I can't say no to you. Of course you can play with them! Cyrus, Tabitha, Penny... Daddy has a present for you! It's a new toy. I think you will enjoy it very much."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, Father! I do love you so! I just hope it doesn't break like the last one..."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You must treat this one more carefully than you do your other toys. I want you to take your time and really enjoy it! Have fun!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Tee hee hee! BOOOOOM!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("... ... ... ..."), "co_cyrus", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Now, my children, let us prepare the next experiment."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_ids_family.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Unidentified plane inbound!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_great_owl_out.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I say! Is it one of ours?"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_great_owl_out.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't think so. It looks like a bomber, but... This is impossible... No plane is this big."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_great_owl_out.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The plane is parachuting troops! They've taken up battle positions. They have twice our numbers, sir. And they're still coming!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_great_owl_out.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...I don't get it. Where did an army of this size come from?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Why, from me, of course! My new automated troops are on the front line of military technology. With them, a single commander can give orders to every unit at once!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr(" I took the liberty of recording your previous battle. It was a fierce fight, and you sustained a great deal of damage. Now I wish to see if your decimatd forces can rally."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("No! You can't!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("But first, I hoped to have a talk with you, Will."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...A talk? Are you serious?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, yes. I have something important that I need to discuss with you. I will tell you the location now, and you will come to me. Alone. If you do not agree to meet me, I will attack immediately."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm going."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Caulder isn't a man you can reason with. Or one that you can trust. This meeting of his is all wrong."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I know. But I'm going anyway."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I thought you'd say that. But I'm afraid I can't allow it."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Well, I don't think it's up to you. If I go, it buys us time to prepare our troops. It gives us hope."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It does that, yes. But I don't think it matters. He's toying with us."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Probably. But it's still going to buy us time."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Will, if he takes you hostage, if he starts demands... I won't rescue you. You know that. Right?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(map, qsTr(" I know. But I've made up my mind. I'm going."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ah, Will! Please, come. Sit down.You truly are a wonder. I don't know how you managed to defeat Greyfield."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What do you want?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Patience, Will, patience. I just have a favor to ask of you."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Really."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I need to observe your troops, but my current equipment is somewhat lacking. Would you mind if I attach these small devices to your men? They allow me to monitor heart rates, perspiration, and fear levels."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...You can't be serious."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("They won't affect your fighting capability in any way, I assure you. Doing so would compromise my position as an impartial observer. I will attack you regardless, but this way science can benefit. Do you see?"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's a great offer, but I think we'll have to pass."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Lin!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I thought we agreed that you would come alone."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't give a damn what you thought. Will, this man is a psychopath. Don't listen to another word he says."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Please! I am a scientist! I seek only to learn! But it seems you don't want to cooperate. Very well."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Are you really just walking away?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That was my intention, yes. Will and I had an arrangement."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'm not Will. And I'm not letting you leave this mountain."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog45.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ah, yes. I figured that something like this might happen."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog46.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog47 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hello, Father."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog47.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog48 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Um... Who are you?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog48.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog49 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This is Tabitha, my first daughter. ...First daughter? Third? I've lost track. Her reflexes are far superior to yours. If you reach for your weapon, she will kill you. Oh! I almost forgot. I have one more request... The girl you call Isabella. I want you to give her to me."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog49.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog50 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I don't believe this!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog50.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog51 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Will you hand her over to me? Yes or no? It's a simple question. If you give her to me, I will guarantee the survival of your people."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog51.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog52 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("How's that?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog52.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog53 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I will call off my forces. I can also provide a cure for the virus you call the Creeper. All at the cost of a small girl who can't even lift a rifle. It is a very generous offer."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog53.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog54 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Isabella stays with us."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog54.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog55 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Such a fascinating little man... I thought you might say something like that. But I think you will reconsider once I introduce you to the Great Owl."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog55.loadBackground("maps/advance_wars_day_of_ruin_campaign.camp/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog56 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("All units prepare for battle. We still don't know the size of the enemy forces, so be careful."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog56.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog57 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The Great Owl is ready... Let the bombing commence!"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog57.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog58 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Sir! That huge plane is bombing the hell out of us! The entire center of the battlefield was devastated!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog58.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog59 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Proceed with caution! All units, avoid the impact area!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog59.loadBackground(""); // 1 ScriptEventDialogItem
            var animation = GameAnimationFactory.createAnimation(map, 7, 3);
            animation.addSprite("explosion+black_bomb", 0, map.getImageSize() / 2, 0, 1.875);
            animation.setSound("explosion+land.wav");

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
            dialog57.queueAnimation(animation);
            animation.queueAnimation(dialog58);
            dialog58.queueAnimation(dialog59);
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We bom now? BOOOOOOOOM Tee hee hee!!!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if ((turn - 2) % 1 === 0 && player === 0) { // 0 Each Day
            var animation = GameAnimationFactory.createAnimation(map, 7, 3);
            animation.addSprite("explosion+black_bomb", 0, map.getImageSize() / 2, 0, 1.875);
            animation.setSound("explosion+land.wav");
            gameScript.dealGreatOwlDamage();
        } // Each Day End
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
    this.dealGreatOwlDamage = function()
    {
        var x = 9;
        var y = 6;
        var fields = globals.getCircle(0, 3);
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var terrain = map.getTerrain(x + point.x, y + point.y);
                var unit = terrain.getUnit();
                if (unit !== null)
                {
                    var hp = unit.getHpRounded();
                    if (hp + 0.1 <= damage)
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
