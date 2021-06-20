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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Those filthy Rubes beat me again! I'm so sorry, General Forsythe! I swear we will have revenge!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will, Lin. Look to the wounded."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK. We'll split up and begin searching for survivors..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, look what the cat dragged in! It's Captain Courageous's cabin boy. And look at you! Running all over hell and breakfast helpin' the wounded."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Waylon?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Waylon, kid! Waylon! No need to sound so uppity! I ain't some fancy-pants general. So how is the good captain, eh? Still tryin' to save the world like a chump?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner is a great man!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("What?! "), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("You can call me kid or chum  or whatever, but never speak badly of Captain Brenner"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Pshaw, kid! I'm just pullin' your chain a little. That's all.  But that reminds me... Ol'Greyfield himself wants to see you."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("He wants to see me?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Fraid so. And he ain't a fella that likes to wait, if you know what I mean. So hit the road, and tell the great man that I said hello. ... What are you still doin' here, brother man?! Get going! Go on! Shoo!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("You wanted to see me, Admiral?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hmm? Ah, yes. Will, wasn't it? Yes, yes. Please, do come in. You kept a cool head in that battle, my boy. I'm impressed. Commanding an army at your age is no mean feat. But you are Rubinelle, and the proud blood of this nation flows in your veins!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Umm... Thank you?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do you think of this life, Will? What do you think of the destruction which gave birth to this brave new world?"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I think it's terrible--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("WRONG! You have made a fundamental error! Do not do so again!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("An error?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Only the weak view this destruction as terrible. Some defeatists may even claim we deserved it... But they are fools! All of this has been but a test!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("A...test?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, a test. A test to see who has earned the right of survival. The weak see it as a catastrophe, but the strong see it as opportunity! You, Will! You could be one of the strong! You passed the test with flying colors."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("But people died! They lost their lives!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("It is natural that the weak should perish! "), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I don't..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("This world as it is now demands strength. It demands the strength of Rubinelle. Our long years of war with Lazuria, and the destruction that followed, were all tests. Those who cannot cope with the harsh reality of today do not deserve to survive."), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("...You're wrong!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("What did you say?"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("My parents were killed in the war! My friends were killed by the meteors. You think it is natural that they died? You think they failed some crazy test? You're wrong! And I'm not afraid to say it!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("How dare you speak to me in such a way! I'm Admiaral Greyfield! I will rule this world!"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#d08000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I thought you were different from that fool Brenner... But I see he has brainwashed you with that nonsense about protecting the innocent! Begone! Leave my sight! If you dare return, I'll see you hang for treason"), "CO_GREYFIELD", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
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
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_glorious_tent_inside.png"); // 1 ScriptEventDialogItem
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
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
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
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr(""), "CO_CAULDER", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
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
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Huh? ...John? ...John? Is that you? JOHN?!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, that's...that's impossible. John is..."), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("What are you talking about?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is Wing Commander Tasha of the Lazurian Airborne. Identify yourself!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr(" Um...I'm Will. I'm a member of the 12th Battalion. Uh...of Rubinelle. But listen! I don't want to fight! The Great War is over!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's not over... It's NEVER over! This is about revenge! We fight and fight until Rubinelle is destroyed!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Look, I know our nations have been at war since before we were both born. And we've all lost friends and family... But revenge won't bring anyone back."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("It may not bring them back, but it will avenge them! And that matters! Could you just forgive and forget! Pah! What a hypocrite!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm not a hypocrite!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're a kid! What do you know about war or revenge? You think you know right and wrong, but you've never been tested! All you have are your words and your false bravado. John was my brother. He was killed by your Rubinelle Army. Revenge! Death to all Rubes! Reveeeeenge!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("But wait...!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've said all I'm going to say. I don't want to fight a kid, but if you're with the Rubes, you're going down!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (map.getPlayer(2).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(0).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
        if (map.getPlayer(0).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(2).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
