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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("All enemy units destroyed!"), "co_officier_ac", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("All units, to the cockpit! We have to seize control now!...I hate flying."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sir, Sector C sustained severe damage during the fight. Um...yeah, we're in trouble. I don't think we're gonna reach the airfield. Suggest emergency landing procedures, sir."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("NO! NO NO NO! Mr. Bear doesn't want to land here! Mr. Bear hates it here! Fly more! Fly more!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... But, sir, I reeeeeally think... Oh dear..."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Isabella!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will? Why did you come...?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're here to rescue you! Come on!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("...No. You must leave me. I am not a human being. I am a clone. A thing. A little rat. I don't deserve to be rescued."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("What? No, you're one of us, and we're getting out of here now."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't even have a name..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Your name is Isabella!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Since I came back, I... I remembered things. We were raised in this laboratory. All the Isabellas. All the same. Caulder created lots and lots of us and experimented on us. I saw...horrible things... I always knew it would be me someday."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Listen, I know things were really bad, but you have to--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("They meant nothing to me. They were spare parts, just like me. That's why I exist. I have no memory. I have no soul. I am nothing. You must leave--"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("NO! STOP IT! STOP IT RIGHT NOW! Your name is Isabella! You are the first person I ever saved! You joined us and traveled with us and fought with us! You are one of us! I know you remember that!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("I-I remember...But...why do you help me? You know--"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("What, that you're a clone? So what? I don't care if you come from a mother or a tank! Isabella, you're a person! A real living person with ideas and dreams and...and stuff like that! You have meaning and strength and beauty and...and I... I..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Will... Oh, Will..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("Come on. Don't let that freak Caulder decide how your life plays out. You decide. Right now! What do you want? Come or go, you gotta decide."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("I want to live! I want to live with you, Will! Forever!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("YES! I knew it! I mean... I hoped you would say that, but part of me really knew, and... Listen, never mind. We gotta run. Come on!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I can't believe it! No one has ever wanted me before..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_isabella.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah? Well, I do. Now come on!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_isabella.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, Will..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_isabella.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're losing altitude! Oh man, i knew this wasn't going to end well...7,500 feet... 7,000 feet... 6,500 feet..."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tee hee hee! Daddy will be happy! Daddy gets to watch the BOOOM!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Penny!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who's there! I don't like you!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Penny, listen to me! I know you think you have to do what Daddy tells you. But you mustn't obey him anymore! He is evil! Do you understand?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is the strange girl saying? Mr. Bear doesn't understand."), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("Um... 4,500 feet! 4,000 feet! 3,500 feet! ...Screw this, I'm gettting a parachute."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("I understand you, Penny. We're the same. I want you to come live with us."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're weird! Mr. Bear doesn't like you!...And your jacket smells bad! Why are you saying these things to Penny? ...P-Penny? Mr. B-B-Bear? ...EEEEEEEEEE! No one understands anything! I HATE ALL OF YOU!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("2,500 feet! 2,000 feet! 1,500 feet! ISN'T ANYONE ELSE WORRIED ABOUT THIS?!"), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Please, Penny! Listen to me! I want to help you!"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-but Daddy told me to..."), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait, this isn't a parachute! It's a sleeping bag! ...Ah, crap."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
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
        var variable2 = variables.createVariable("variable2");
        var variable3 = variables.createVariable("variable3");
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("...We knew her as Lutaria. She lacked emotion or subtleness of any kind. She was a walking library with every conceivable military fact downloaded into her."), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Well, she's different now. Where can I find her?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("She is presently inside the Great Owl. I can get you on board."), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why should I trust you? Why would you betray your own father?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Let me ask a question of you. What is it that makes a human being?"), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Huh?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is it blood? Emotions? DNA? Is it heredity or memories or just their very environment? What decides if a human is good or evil?"), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sorry, I'm a bit lost."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("I am a clone of Dr. Caulder. He conducted experiments to see if he could transplant his mind into a younger body. I was created to aid his quest for immortality. Nothing more. And yet, I feel that there is more to me than that. And I will prove it to him. Come. Time grows short, and we must hurry. The Great Owl is repaired and refueled by machines. There are no people here. All security systems and surveillance are computer operated."), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why is that?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("My father does not trust other humans. But machines, too, have their drawbacks. If I can hack into the system, we can board the Great Owl without detection."), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("I see."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("We are close. I ask that you follow me."), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("...I hope you're right about this."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Me too."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Lin! What are you doing here?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, just...you know. Hanging around."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("You followed me!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not just me. I brought a few units along for the ride. It's not an overwhelming force, but it should be enough."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("I told you! I don't want your help! I don't want to risk anyone else."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog60 = GameAnimationFactory.createGameAnimationDialog(qsTr(" Too late, Will. You resigned, remember? You don't give the orders anymore. Everyone made the choice to come with me. They want to be here."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog60.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I can't..."), "CO_WILL", GameEnums.COMood_Normal, "#0098f8"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("I am envious of you. Such companionship is an experience that will forever elude me. Enough. We have no more time to waste. Follow me, and quickly."), "co_cyrus", GameEnums.COMood_Normal, "#0098f8"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("And how are we doing today?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("I hear from Father that you will spend the rest of your life on board this plane! If you are lucky, he will find a use for you in one of his experiments. I can't wait to find out what it will be... Maybe he'll drop you out of the Great Owl with no parachute! How about that, hmmm?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Poo! You're so boring! Haven't you got anything to say after all this time? Are you even awake? Hellooooooo! Hello in there!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Y-you are... Tabitha... You are my sister."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sister?! Your SISTER?! Me?! Ha ha ha! Oh, you do make me laugh!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why do you laugh? We are sisters."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("You are so very naive! You have no concept of how the world really works."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("But you are my sister. We even look alike."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("People are so funny sometimes... You see someone who looks familiar, and you say to yourself, Hey! She looks like me! But did you ever stop to consider another possibility? Like, YOU look like HER! Get it? Like SHE'S the original, and you're nothing but a copy? A fake?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("I-I don't understand..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("SIIIIIGH! All right, let me try to explain this to you. I'll use small words, OK? Father had four children. Cyrus, Penny, Lutario and Tabitha. ...That would be me! Then Lutaria died when I went too hard on her in a training battle. All terribly sad... But it was fine, because there were lots of spare Lutaria clones stored on the Great Owl. Just another lab rat, which is what you are! A lab rat!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("That was... That was me?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's why you have no memories, little rat. Because you were nothing! You have no life, no name, no feelings, no nothing! Just another rat raised in a tank of goo by Father."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("I always...wondered who I was. And now I know. I am nothing."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, little rat! This is too much! Tell me, does someone who is no one feel sad? Do they fear death?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(qsTr("I... I don't know."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog39.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(qsTr("Psh! You're so boring, it's not even worth the trouble of picking on you. ...PENNY!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog40.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tee hee hee!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog41.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Must you insist on giggling like a crazy person all the time? Really! Did you understand Father's instructions? He wants you to put little rat on board and then prepare for takeoff."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog42.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(qsTr("Penny knows! Penny is good! Tee hee hee! Sister is coming?"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog43.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(qsTr("No! Sister is not coming! Sister is helping Father with very important experiments."), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog44.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tee hee hee! Sister is smart!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog45.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Will."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog46.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog47 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hmmm? Was that a squeak, little rat?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog47.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog48 = GameAnimationFactory.createGameAnimationDialog(qsTr("Will."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog48.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog49 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Will? Penny, is there anyone else in this hangar?"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog49.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog50 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hold on! Let Penny ask Mr. Bear!... ... ... ... Mr. Bear says no! Mr. Bear says we are all alone. ...He also hates you."), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog50.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog51 = GameAnimationFactory.createGameAnimationDialog(qsTr("You better be right. About the people, I mean. ...I couldn't care less about your stupid bear. I'm sure little rat is just seeing things. And take a bath, why don't you! That dirty old uniform really smells!"), "CO_TABITHA", GameEnums.COMood_Normal, "#6038a0"); dialog51.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog52 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Will..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog52.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_factory.png"); // 1 ScriptEventDialogItem
            var dialog53 = GameAnimationFactory.createGameAnimationDialog(qsTr("Commander, all systems are go. Takeoff has been successfully completed. The Great Owl will now be switched to autopilot."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog53.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog54 = GameAnimationFactory.createGameAnimationDialog(qsTr("Goody goody! Mr. Bear is sleepy."), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog54.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog55 = GameAnimationFactory.createGameAnimationDialog(qsTr("What in the...? Commander! We have an emergency! There are intruders on board! There's a firefight in progress in Sector C! ...Oh good gravy, they have TANKS! The fighting has damaged the fuselage of the Great Owl!"), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog55.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog56 = GameAnimationFactory.createGameAnimationDialog(qsTr("Mr. Bear said no one was here! Mr. Bear is a big liar! ...Daddy is going to be mad."), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog56.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog57 = GameAnimationFactory.createGameAnimationDialog(qsTr("Sweet corn casserole! They have freakin' ROCKETS! Permission to engage, sir!"), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog57.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog58 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tee hee hee! Playtime! Playtime! Yaaaaaay!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog58.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_great_owl.png"); // 1 ScriptEventDialogItem
            var dialog59 = GameAnimationFactory.createGameAnimationDialog(qsTr("Our units shifted during takeoff. Now they're spread all over the place. It may be difficult to bring our forces back together. Every man for himself! Fight your way to a better defensive position!"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog59.loadBackground(""); // 1 ScriptEventDialogItem
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
            dialog19.queueAnimation(dialog60);
            dialog60.queueAnimation(dialog20);
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Tee hee, Mr. Bear! It's a Great Owl fight! Pschew! Boooooom!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
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
		var playerCount = map.getPlayerCount();
		for (var i = 0; i < playerCount; i++)
        {
			var player = map.getPlayer(i);
			if (i === 1 && player.getUnitCount() === 3)
			{
                if (variables.createVariable("EventCounter").readDataInt32() === 0) {
					// Dialog
					var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Er... The enemy has wiped out almost all of our units, sir. This isn't good."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
					var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Boo! Get more units!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
					var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("We're preparing more units to enter from the left and right sides, sir! But I fear that continued fighting may result in the plane losing--"), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
					var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yaaaaaaaay! More units! Mr. Bear loves units!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
					dialog0.queueAnimation(dialog1);
					dialog1.queueAnimation(dialog2);
					dialog2.queueAnimation(dialog3);
					// Dialog
					variables.createVariable("EventCounter").writeDataInt32(1); // 0 Modify Variable
				}
                else if (variables.createVariable("EventCounter").readDataInt32() === 2) {
					// Dialog
					var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Er... Our reinforcements are being destroyed, sir."), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
					var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("MORE! MORE!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
					dialog0.queueAnimation(dialog1);
					// Dialog
                    variables.createVariable("EventCounter").writeDataInt32(3); // 0 Modify Variable
				}
                else if (variables.createVariable("EventCounter").readDataInt32() === 4) {
					// Dialog
					var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("OH NO! The second wave is going down, sir. And the Great Owl is barely holding together!"), "co_officier_dm", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
					var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("More more more! Moooooooore!!"), "CO_PENNY", GameEnums.COMood_Normal, "#6038a0"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
					dialog0.queueAnimation(dialog1);
					// Dialog
                    variables.createVariable("EventCounter").writeDataInt32(5); // 0 Modify Variable
				}
			}
			if (player.getUnitCount() === 0)
            {
                if (i === 0)
                {
                    player.setIsDefeated(true);
                }
                else
                {
					if (variables.createVariable("EventCounter").readDataInt32() === 1) { // 0 Check Variable
						// Dialog
						var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("The enemy is sending reinforcements! Engage them immediately!"), "CO_LIN", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
						// Dialog
						map.spawnUnit(0, 2, "LIGHT_TANK", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(0, 5, "LIGHT_TANK", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(0, 8, "LIGHT_TANK", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 2, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 5, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
                        map.spawnUnit(15, 8, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
                        variables.createVariable("EventCounter").writeDataInt32(2); // 0 Modify Variable
					} // Check Variable End
                    else if (variables.createVariable("EventCounter").readDataInt32() === 3) { // 0 Check Variable
						// Dialog
                        var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("More reinforcements arriving! Assume defensive positions!"), "CO_LIN", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
						// Dialog
						map.spawnUnit(0, 2, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(0, 5, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(0, 8, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 8, "LIGHT_TANK", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 5, "LIGHT_TANK", map.getPlayer(1), 10); // 0 Spawn Unit
                        map.spawnUnit(15, 2, "LIGHT_TANK", map.getPlayer(1), 10); // 0 Spawn Unit
                        variables.createVariable("EventCounter").writeDataInt32(4); // 0 Modify Variable
					} // Check Variable End
                    else if (variables.createVariable("EventCounter").readDataInt32() === 5) { // 0 Check Variable
						// Dialog
						var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("More? ...Are you kidding me?"), "CO_LIN", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
						// Dialog
						map.spawnUnit(0, 2, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(0, 5, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(0, 8, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 8, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 5, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
						map.spawnUnit(15, 2, "ROCKETTHROWER", map.getPlayer(1), 10); // 0 Spawn Unit
                        variables.createVariable("EventCounter").writeDataInt32(6); // 0 Modify Variable
					} // Check Variable End
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
