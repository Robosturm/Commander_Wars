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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Admiral Greyfield, the 12th Battalion commander has arrived."), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("...WELL?! Don't just stand there like a slack-jawed idiot! Send him in!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Admiral."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("So you're the leader of the so-called Brenner's Wolves. Frankly, I'm surprised you're still alive."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("For now."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, I remember you now! An adequate soldier, but crippled by ideological limitations! Your lack of patriotism and devotion was an insult to your Rubinelle heritage."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("The 12th Battalion always fought with honor and respect for the rules of war."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Rules of war? Ha ha ha! Rules... You really are a relic from another age, Captain."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hey, uh... Come on, now. We're all soldiers of Rubinelle, yeah? We should all pull together against the enemy."), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't think much of Davis, but the chump's got a point. We gotta plow the Lazurians into the ground. There's time for dancing when that's done."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, good. Waylon's here."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, you know it ain't a party 'til I arrive, brother man! That's straight-up truth! Oh, yeah, and thanks for bailing me out of that jam. The ladies of the world thank you."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Couldn't resist the draw of the high life, huh, Waylon? This paycheck help you sleep at night?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Like a baby with a bottle! Only a fool'd miss out on this kind of livin'! Round here? They treat me right! Give me anything I ask for. I do whatever I want, too, and everyone's too scared to say boo!
"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Typical."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("L-look...Admiral Greyfield has taken over the former military and built the New Rubinelle Army. But we need help to beat those crafty Lazurians. ...N-not that you need any help, Admiral! Um... Oh Great One..."), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("The Lazurians are an enemy to freedom-loving people everywhere! There can be no peace until they are wiped from the face of the planet."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, so, it's like a war for peace. Right? The enemy forced us into it. Are you with us, Captain?"), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I don't want this idiotic war to continue any longer than ii already has. If my men can end this fighting a day sooner, then we will help."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes! I don't believe it!"), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hmmm...yes. I suppose you may be a modicum of help. But we will have to wait and see... Go to the Rubinelle Sea. The hated Lazurians have been seen in the area. Send them a clear signal that you now fight for Admiral Greyfield! And show no mercy!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I hope this is over soon."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("For a man of Rubinelle, he lacks moral flexibility and respect for power."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-yes, sir! B-but I always remember him as a good fighter..."), "co_davis", GameEnums.COMood_Normal, "#d08000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Have all units been deployed?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, Captain. They are awaiting your orders."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("One wrong move, and the Lazurians will be all over us. Greyfield sent only a handful of units, and this force is formidable."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner?
"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, Will?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why did the Lazurians start this war? Why would they do that? I don't understand what there is to fight over."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("I imagine they saw this as a chance to settle some unfinished business. Sometimes I can almost understand the feeling."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I still don't get it..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, I know how you feel. I know this was seems senseless. But the Great War caused unspeakable horror for our two countries. We've traded black eyes with the Lazurians for over a century. The meteors changed everything but the hatred between our nations."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("But still..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("I fear this war won't end until one side secures military victory. We have to force them to surrender. We have no other choice. Now let's move!"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (turn === 1 && player === 2) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Woo-hoo! Let's hit'em where it hurts!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Requesting permission to engage the Rubinelle forces, sir."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Granted."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("General Forsythe? If you like, I can boost your chances of victory. I have prepared some special gifts for you and your men."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gifts?"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, gifts! Weapons with the power to grant you an effortless victory! Such as a device with the power to freeze troops solid? It uses a specially developed formula created in my labo--"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Good chowder, man! That is enough! I will hear no more talk of this! Weapons of that ilk are banned by international and military law."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Law? What does that mean now? There is no law in this world! A wise man would use all means at his disposal to ensure victory.
"), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I am an old, tired soldier, Caulder, but one who fights with honor. I will not sully the proud name of Lazuria with your foul weapons."), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't think that position will prove popular among your troops."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("My good man, I am grateful for the aid you have given my army. It is thanks to you that we now occupy a position of superiority over Rubinelle. But I simply will not abide the use of such terrible weapons!"), "CO_FORSYTHE", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("That is regrettable."), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("General Forsythe, leave it to me! I'll destroy these filthy Rubes!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um...hello? I mean...attention? Come in, please. I want to speak to the Lazurian commander. Can you hear me? Why are we fighting? The war is over. None of this matters."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Huh? ...John?"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_ANDY", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
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
