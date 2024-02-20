var Constructor = function() { // scriptStart
    this.immediateStart = function(map) { // immediateStart
        return true;
    }; // immediateStart
    this.getVictoryInfo = function(map) // victoryInfo
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Young miss, with victory impossible, I am withdrawing and have issued orders to my remaining troops to lay down arms and surrender."), "CO_PETER", GameEnums.COMood_Normal, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("About time. This was starting to get really boring."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I underestimated you, and paid the price. I apologize for my arrogance. Good day."), "CO_PETER", GameEnums.COMood_Sad, "#00c010"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Is that transmission over?"), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes, ma’am."), "co_officier_dm", GameEnums.COMood_Normal, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Good. Hmm…"), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("(Muttering to self) Captain Aira leads her cutthroat crew in search of treasure, the wind their sails and grog on their breath."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("...and we’d sail off with the booty and live like kings."), "CO_AIRA", GameEnums.COMood_Happy, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Ma’am?"), "co_officier_dm", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh. The sea breeze got me thinking. What did you need?"), "CO_AIRA", GameEnums.COMood_Sad, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Call for you, from Amy."), "co_officier_dm", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("*sighs* So be it."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Aira, I hear your forces are almost in position."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yup. We’re getting ready to blow in like a hurricane."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Good to hear. My forces are closing in as well. But we can’t liberate these islands from Green Earth until you take out that Black Cannon they have covering everything."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I know that. I was actually paying attention at the mission briefing this time."), "CO_AIRA", GameEnums.COMood_Happy, "#ff33cc"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, good, so you’re familiar with the capabilities of all your vessels?"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Uh, sorta. There’s a lot of new ones. You upgraded our battleships so they can shoot on the move, right?"), "CO_AIRA", GameEnums.COMood_Sad, "#ff33cc"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yes. All our naval indirects can fire while moving. Even the aircraft carriers."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Right. And battlecruisers I remember are half battleship and half cruiser, able to fire directly on air units and indirectly on naval units, even returning indirect fire. But they can’t fire on stealthed units like cruisers can."), "CO_AIRA", GameEnums.COMood_Happy, "#ff33cc"); dialog12.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Correct."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog13.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The frigates are able to fire on any surface ship, directly or indirectly, so I just figure out which the enemy ship is good at and do the opposite. They also have really good radar."), "CO_AIRA", GameEnums.COMood_Happy, "#ff33cc"); dialog14.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Good, good."), "CO_AMY", GameEnums.COMood_Happy, "#ff33cc"); dialog15.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("And then there’s a lot of little boats which all look similar and all do basically the same thing."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog16.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Not so good. Let’s go over it again."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog17.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Missile boats are like a light frigate, but they only have one salvo of missiles, so after damaging a naval unit they need to resupply. Torpedo boats are brawlers, doing decent damage against naval units, even submerged vessels. Destroyers deal some damage against naval units, but they can also place up to 3 naval mines, which will seriously damage any enemy ship that hits them except a cruiser. Gunboats are crewed by marines that can capture properties on the water, harass land units, and maybe finish off naval units in a pinch."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog18.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("And black boats can capture as well?"), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog19.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Actually, no, that’s not in their capabilities. They can repair and resupply your naval vessels, especially those boats which have limited ammo. They’d also be able to transport your infantry, if you hadn’t forgotten them back at base."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog20.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Um. I got distracted. You think I can still win this?"), "CO_AIRA", GameEnums.COMood_Sad, "#ff33cc"); dialog21.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I know you can. I picked you for this mission because we need your out-of-the-box ingenuity. Taking land bases and airports would be handy, but naval power is what will win this map. You’ll do just fine."), "CO_AMY", GameEnums.COMood_Happy, "#ff33cc"); dialog22.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I do have an idea. Those boat units, the little ones, they seem light enough to go up rivers."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog23.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("They just might be. I think I see what you’re planning there. One last note, my intel suggests that Peter is commanding this area. He’s an aerial specialist, but luckily you’re our anti-air specialist. Peter’s placed a significant number of mines in the harbor, so make sure you patrol ahead with your cruisers when you get past the pipe. And also, the new power plants we developed should be up and running, fuelling your powers."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog24.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh yeah, I feel the charge already. I think his air force is in for a bit of turbulence. Maybe a lot of turbulence."), "CO_AIRA", GameEnums.COMood_Happy, "#ff33cc"); dialog25.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Glad to see you’re in fighting spirits. Any other questions?"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("So why does having power plants on the map give me a greater ability to control the weather? And how are pipes able to keep aircraft from flying overhead? And how come my winds cause submarines to lose extra fuel even when they’re underwater? And what do all the naval units do again? I forgot."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Look, I need to get my preparations finalized. Check the wiki at commanderwars.com if you need a refresher."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
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
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Young miss, I see your fleet in the harbor. I must advise you to turn around immediately. We have managed to rebuild black hole’s defensive fortifications, and I think you will find them more than your match. In addition, my prize air force is doing maneuvers today. A storm is coming, and you would do well to shelter before it hits."), "CO_PETER", GameEnums.COMood_Normal, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hm? Oh, right, the air force. Yeah, I’m not worried. Your weather forecasts stopped being accurate the moment I got here. You think there’s going to be a storm, but I’m expecting clear skies with a light rain of unfueled aircraft."), "CO_AIRA", GameEnums.COMood_Happy, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We shall see."), "CO_PETER", GameEnums.COMood_Normal, "#00c010"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
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
        if (map.getPlayer(0).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Aira, come in. I heard about your defeat. We can’t afford to leave this island in enemy hands. I’m sending in reinforcements, and we’ll have to try again."), "CO_AMY", GameEnums.COMood_Sad, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Peter starts with significant forces, but they have to take the long way to get there. A couple blasts of your normal power, and they should run out of fuel before they reach your army. From that point on, your superpower is your better bet. You can save either power for when the weather is working against you if you wish."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You could send your boats up the lake to the west to get some easy hits on the black cannon, but it might be more efficient to build up a larger naval force and take the harbor bit by bit. You should be able to lock down most of his bases and ports, forcing him to build aircraft which won’t be a match for your superpower."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Peter’s superpower will damage your ships, making your job more difficult. But there’s plenty of oil rigs, harbours, and temporary harbours for them to take shelter in."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            // Dialog
            variable1.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
