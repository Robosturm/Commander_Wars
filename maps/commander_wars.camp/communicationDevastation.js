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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It's over!"), "CO_NANA", GameEnums.COMood_Happy, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Not yet. We need to bring Julia in."), "CO_ALEXIS", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You'll never win as long as *checks notes* as long as one Dark Matter solider holds loyal to their -um- contract."), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Okay. Shoot her."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wait, wait! I'm just the stunt double."), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You think we're going to believe that Julia?"), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Look at her eyes, the cheekbones. That's not Julia. Where'd she go?"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("She said I'd be up for a promotion if I stood in for her this moment, then she took off down that hall. I'm not getting the promotion."), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Oh, you'll get something. Hands on your head."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Amy to all troops. Julia has fled the scene. Don't let anyone past your sight."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This is good footage. Let's edit Beast's men to look like Green Earth forces. That'll really sell the angle we're going for."), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Commander, Pink Frontier forces approaching."), "co_fanatic", GameEnums.COMood_Normal, "#800080"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Earlier than expected, but we knew this was coming sooner or later. Everyone, the show's about to begin. Read my notes for the most likely scenarios, and improvise if needed, but don't undermine my narrative."), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Julia, you have a lot of explaining to do, preferably in handcuffs, and starting with why you're staging an invasion."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This isn't an invasion. With the chaos of war, Dark Matter took the initiative to secure our property against any hostile action by Green Earth. We're extending that protection to nearby civilians as part of our good neighbor policy."), "CO_JULIA", GameEnums.COMood_Happy, "#800080"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("None of us are stupid enough to believe that."), "CO_ALEXIS", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Not even Nana."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hey!"), "CO_NANA", GameEnums.COMood_Normal, "#ff33cc"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Julia, you're in the middle of Pink Frontier territory, surrounded, and outgunned. Save the lives of you and your men; surrender."), "CO_ALEXIS", GameEnums.COMood_Normal, "#ff33cc"); dialog8.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I'll give you the same offer. You've walked right into a trap. We've managed to tap into every major communications network in Pink Frontier. Once I rout your forces here, I'll have complete information control in the region. I'm not just able to broadcast anything I want here, I'm able to intercept and decrypt military communications. I can transmit messages to your forces that seem more authentic than the real thing. I can control the population's perception of this war, and spin the truth however I want. If you care about the lives of your people, you'll admit defeat now and save us all the trouble."), "CO_JULIA", GameEnums.COMood_Happy, "#800080"); dialog9.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The tactical situation isn't ideal, I admit that. But my science team says that all that communications infrastructure is routed through a handful of communications towers. If we take those, you have nothing but a handful of mediocre troops cut off from any support."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog10.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's... Julia to all troops, the enemy is hostile. Go to scenario delta. Overload the satellite dish to lethal parameters. All units attack!"), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog11.loadBackground(""); // 1 ScriptEventDialogItem
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
        if (turn === 1 && player === 2) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What ho? I am here to aid you in your quest. Let us send this harridan to sheol!"), "CO_JAVIER", GameEnums.COMood_Happy, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("What! How did you get this far beyond Pink Frontier lines?"), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We let him in."), "CO_AMY", GameEnums.COMood_Happy, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We decided we'd had enough not trusting each other and decided to start not trusting you instead."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It's an ass-kicking party!"), "CO_NANA", GameEnums.COMood_Happy, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("And you're the guest of honor."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Unexpected. But it still won't be enough. I have reinforcements set to arrive in fifteen days. And they're Dark Matter's finest."), "CO_JULIA", GameEnums.COMood_Normal, "#800080"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Then the time is nigh for a fortnight of posterior pugilescence. Tally ho!"), "CO_JAVIER", GameEnums.COMood_Normal, "#00c010"); dialog7.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            // Dialog
            map.spawnUnit(1, 0, "K_HELI", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(0, 1, "K_HELI", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(5, 0, "INFANTRY", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(0, 5, "INFANTRY", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(2, 0, "TRANSPORTPLANE", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(0, 2, "TRANSPORTPLANE", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(0, 0, "ROCKETTHROWER", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(3, 0, "LIGHT_TANK", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(0, 3, "LIGHT_TANK", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(1, 2, "MOTORBIKE", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(2, 1, "MOTORBIKE", map.getPlayer(2), 0); // 0 Spawn Unit
            map.spawnUnit(3, 3, "DUSTER", map.getPlayer(2), 0); // 0 Spawn Unit
        } // Start Of Turn End
        if (turn === 15 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Tremble, as my mighty army comes to sweep away your forces like the tide."), "CO_JULIA", GameEnums.COMood_Happy, "#8227b4"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That's it?"), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("They're functioning at 10% capacity, at best. She was lying this whole time!"), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The battle is not over yet. Tally ho and charge!"), "CO_JAVIER", GameEnums.COMood_Normal, "#00c010"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("With my inspiration, one of my men fights as ten, and ten fight as a thousand!"), "CO_JULIA", GameEnums.COMood_Normal, "#6038a0"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Can we shut her up now?"), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            // Dialog
            map.spawnUnit(9, 0, "LIGHT_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(10, 0, "MEGATANK", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(8, 0, "MEGATANK", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(9, 1, "BOMBER", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(8, 1, "ANTITANKCANNON", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(15, 0, "MEGATANK", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(14, 0, "MEGATANK", map.getPlayer(1), 0); // 0 Spawn Unit
            if(map.getTerrain(9, 0).getUnit() !== null){map.getTerrain(9, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(10, 0).getUnit() !== null){map.getTerrain(10, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(11, 0).getUnit() !== null){map.getTerrain(11, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(12, 0).getUnit() !== null){map.getTerrain(12, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(13, 0).getUnit() !== null){map.getTerrain(13, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(14, 0).getUnit() !== null){map.getTerrain(14, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(15, 0).getUnit() !== null){map.getTerrain(15, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(9, 1).getUnit() !== null){map.getTerrain(9, 1).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(8, 1).getUnit() !== null){map.getTerrain(8, 1).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
            if(map.getTerrain(8, 0).getUnit() !== null){map.getTerrain(8, 0).getUnit().modifyUnit(-9, 0, 0, 0);} // 0 Modify Unit
        } // Start Of Turn End
        if (turn === 15 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("It's day fifteen. Her reinforcements will be arriving any moment. We've lost!"), "CO_AMY", GameEnums.COMood_Sad, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Thus ends our mission."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("If we must die, we will die with a lance in our hands and our enemies before us. But take heart, boon companions. Who knows what fate fair Fortune has in store for us?"), "CO_JAVIER", GameEnums.COMood_Normal, "#00c010"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("He's right. Let's go down fighting and come what may!"), "CO_ALEXIS", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Yeah!"), "CO_NANA", GameEnums.COMood_Happy, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
        } // Start Of Turn End
        if (player === 1) {
            var blackFactory = map.getTerrain(17, 5).getBuilding();
            blackFactory.setFireCount(0);
            gameScript.spawnFactory(map);
        }
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
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We lost!"), "CO_NANA", GameEnums.COMood_Sad, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Even I can see this is hopeless."), "CO_JAVIER", GameEnums.COMood_Normal, "#00c010"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We're in the middle of Pink Frontier. Let's regroup and try it again."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I wonder. Julia said she has a massive army reinforcing her at the end of those 15 days. But she's lied to us about everything so far. Maybe that wasn't the truth."), "CO_AIRA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Hmm..."), "CO_ALEXIS", GameEnums.COMood_Normal, "#bd00a4"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
            variable1.writeDataBool(true);
        } // Player Defeated End        
        if (map.getTerrain(1, 5).getBuilding().getOwner() !== null && map.getTerrain(1, 5).getBuilding().getOwner().getPlayerID() !== 1) 
        {
            if (map.getTerrain(6, 0).getBuilding().getOwner() !== null && map.getTerrain(6, 0).getBuilding().getOwner().getPlayerID() !== 1) 
            {
                if (map.getTerrain(3, 18).getBuilding().getOwner() !== null && map.getTerrain(3, 18).getBuilding().getOwner().getPlayerID() !== 1) 
                {
                    if (map.getTerrain(21, 18).getBuilding().getOwner() !== null && map.getTerrain(21, 18).getBuilding().getOwner().getPlayerID() !== 1) 
                    {
                        if (map.getTerrain(20, 2).getBuilding().getOwner() !== null && map.getTerrain(20, 2).getBuilding().getOwner().getPlayerID() !== 1) 
                        {
                            if (map.getTerrain(11, 13).getBuilding().getOwner() !== null && map.getTerrain(11, 13).getBuilding().getOwner().getPlayerID() !== 1) 
                            {
                                map.getPlayer(1).defeatPlayer(null);
                            }
                        }
                    }
                }
            }
        }
    }; // actionConditions
    this.spawnFactory = function(map)    
    {        
        var weld = map.getTerrain(20, 1);        
        if (weld.getID() === "ZWELD_E_W")        
        {            
            for (var i = 0; i <= 2; i++)            
            {                
                if (i == 1)               
                {                    
                    map.spawnUnit(15 + i, 6, "ZCOUNIT_PARTISAN", map.getPlayer(1), 0);                                
                }                
                else                
                {                    
                    var units = ["HEAVY_TANK", "ANTITANKCANNON", "BOMBER", "DUSTER", "FIGHTER", "MEGATANK", "MISSILE", "NEOTANK", "ROCKETTHROWER", "STEALTHBOMBER"];                    
                    var index = globals.randInt(0, units.length - 1);                                
                    map.spawnUnit(15 + i, 6, units[index], map.getPlayer(1), 0);                
                }
            }      
        }
    };// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
