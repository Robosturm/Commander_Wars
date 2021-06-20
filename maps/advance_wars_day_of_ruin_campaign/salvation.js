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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Nooooooooooooooooooooo!!!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("The rest of the mob has lost the will to fight. And we have located this worm that they claimed would save them."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is he the one behind all of this?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Actually, it's... It's... Just come with me."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("...There's no one here. Where is he? Where is the Worm?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Right there. In the corner."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait, but that's..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("According to the villagers, this is the one and only Worm. The one that was supposed to save them from the Creeper."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("But it's... It's just...It's an earthworm."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yep."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("They were prepared to kill for that?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Once people lose all hope, they become willing to believe anything. Let's go. There's nothing here for us."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("It was a lie... All a lie..."), "co_civilian2", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Who will save us now? Who can we believe in? What's the use of going on?"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho ho! My friends, please! You mustn't give up so easily! After all, there's no 'you' in surrender! Oh ho! Oh ho ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's that supposed to mean? You were lied to! The mark has appeared on skin, and now the flowers will blossom!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait! Wait, listen to me! I'm a doctor! I will find a cure for this plague! I need you to hold on to your hope!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're a doctor...?"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("He can cure us!"), "co_civilian2", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("Hurry up! Cure me! Cure me! I don't want to die! Cure me!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sorry, but it's not that simple. I am looking for a cure. I haven't found one yet. You'll have to be patient."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("How can we be patient?!"), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("I am just a human being. I don't perform magic, and I can't cure you instantly. I won't tell you what you want to hear or make false promises I can't keep. I offer no guarantees. Only hope. In truth, some of you will perish before I am able to find a cure."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's no good!"), "co_civilian2", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("But I do promise you this! I promise that I will never give up. As long as there is breath in my body, I will work to heal as many as I can."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("But..."), "co_civilian5", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Trust me. Be patient. Try to hold on until I can find a care."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_wasteland.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("It's OK, Doctor. Go to your work. I'll make sure these people are comfortable."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("Say... Will? You remember when Davis talked about 'the mark'? I think I know what he was talking about."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh yeah?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's a diamond-shaped mark on my left shoulder."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("...What?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think this mark is the only visible symptom of the virus. And if I'm right, my flowers will bloom soon. Not very comforting, huh?"), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh, Lin."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog33.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("We'll find this mark on some of the infected. But it probably doesn't appear until late... I'm running out of time."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog34.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("OK, so we work harder! We help Dr. Morris, and we work to--"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Why won't you leave, Will? Why won't you go? Are you trying to prove something? I know I asked you to be our commander, but this is beyond duty. It's suicide. No one doubts your commitment or your courage. Go, Will. Go. Walk away and don't look back. We'll be OK."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("No. Not after all this. I'm not staying here to sacrifice myself, Lin, or to be...noble or make a point or something. I'm staying here to help. Where there's life, there's hope. I believe that. And if I run, I... I take that hope from everyone. Does that make sense?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog37.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Brenner had you pegged from the start. OK, Will. I get it. I won't ask again. And I'll try to pretend that there's still hope."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog38.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
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
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Pssst! Hey, listen! I have some more info for you. The guys that deserted Greyfield's army were heading to a town called Salvation."), "co_officier_ti", GameEnums.COMood_Normal, "#d08000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's a weird name."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yeah, well, this place is special. Seems that someone there has learned how to cure the Green Thumb. Someone MAGICAL!"), "co_officier_ti", GameEnums.COMood_Normal, "#d08000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("What rubbish! What unscientific balderdash! Who would ever believe such nonsense?! Our hope lies in science, not some snake-oil-peddling witch doctor! Only slow, painstaking researching can hope to present us with a cure."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Easy for you to say, sawbones! You ain't got the Green Thumb! And what if ya did have it? Huh? I suppose you'd just sit in your lab and run tests, yeah? Maybe read some books or somethin'? Well, nuts to that! I'd do anything I could to get cured, even if it sounded crazy."), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("I wonder if someone here really can cure the virus?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Doubtful."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, I am afraid that this is simply a refuge for desperate, gullible people... There is no scientific explanation for this. ...Magical cures. What rubbish!"), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_snow_mountain.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Worship him, my children! Praise him, my children! Worship him! Worship the Worm!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Where's that voice coming from?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("There are loudspeakers set up around the village. Although I have no idea how they are getting power..."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you believe in magic, my children? Do you seek a cure for your illness? The Worm demands sacrifice! Only then will you be cured! You must spill blood in the name of the Worm! Only then will you be free!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Ooooookay. I could be wrong, but I think this guy's crazy."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't like this one bit. This kind of fanaticism is completely outside my field of expertise."), "co_dr_morris", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's something on the radar... Something big. It's coming this way."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'll deal with this."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's wrong, Gage? Feeling jealous of my awesome commanding skills?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("I think that mob is going to attack us."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Poor guys. That disease is driving them insane. This is going to be a weird battle. Is that why you want command?"), "CO_TASHA", GameEnums.COMood_Normal, "#ff0000"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("We can't afford to hesitate. It could give the enemy a chance. Will and his men may have concerns about fighting armed civilians. But I do not. I am a soldier, and I will do what is necessary. Brenner's Wolves saved us. I will take command to repay my debt."), "CO_GAGE", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's no hope for me now! I dont't wanna die! I don't wanna die! I always sided with the winning team! I did my best to protect myself! But look at me now! LOOK AT ME!"), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Davis?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("I've seen what the Green Thumb does to people! I've seen the flowers! I got the mark, man! I got the damn mark!"), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait! What mark?!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("I saw it! With my own eyes! Once the mark appears on your flesh, the flowers start to blossom... They're gonna burst, man! Gonna burst right outta me!"), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Davis! Calm down! We're here to help you!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("I don't wanna die! I don't wanna die! Dontwannadiedontwannadieddont wanna dieddontwannadie GYYAYAAGGHHAA!"), "co_davis", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            // Dialog
        } // Start Of Turn End
        if (turn === 2 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Go, my children! Destroy the intruders! The Worm demands it!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Stop it! You're insane! None of this will cure the virus!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("You say it will do nothing... But who will believe you? NO ONE! My children need a cure! They need their precious hope! Would you steal that from them? My children need hope to live! They need my lies to live!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("...Wait. So you don't believe what you're saying? Why would you fool people?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr(" I fool no one! They want to believe! This world is cold and hard! I give them peace! In truth, they cannot be cured. But if I told them that, they would despair. So I gave them hope! I gave them... Salvation! Who would not choose to live in a dream over this cruel world?"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("B-but..."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Without my Worm, all these people would have perished from grief. I give them hope! Without hope, there is nothing!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("But it's wrong!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr(" No! It is YOU who are wrong! You demand that people face reality! That's why you must be wiped out... You know the truth. Take up your weapons, my children! The Worm demands it!"), "co_fanatic", GameEnums.COMood_Normal, "#6038a0"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
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
