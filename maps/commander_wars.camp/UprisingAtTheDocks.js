var Constructor = function() { // scriptStart
    this.immediateStart = function(map) { // immediateStart
        return false;
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
        if (team === 1) { // 0 Victory
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Where did I go wrong?"), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("This was always a desperate fight. We can’t win without the resistance’s help. Get that APC on Beast’s HQ, and the rest of the battle should swing in our favor. But, I’m not sure about this Beast. I’ve been doing some digging and he might not be who we were told he is. Be careful and watch your back."), "CO_AMY", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            // Dialog
        } // Victory End
        if (team === 0) { // 0 Victory
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Men, keep going. Kill and loot. Loot and kill. Cover this land in blood. Men? Men?! Augh."), "CO_BEAST", GameEnums.COMood_Normal, "#800080"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("That was brutal. But it’s over now."), "CO_MINA", GameEnums.COMood_Sad, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I told ya, lass. They don’t call him Beast because he's a nice person."), "CO_DRAKE", GameEnums.COMood_Normal, "#00c010"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Shut up. Get out of here before I send a unit out to sink your dinghy."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Aye. The port is yours, for now. But be thinking about who’s putting scuttlebut in your ear."), "CO_DRAKE", GameEnums.COMood_Normal, "#00c010"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
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
        var variable1 = variables.createVariable("variable1");
        var variable2 = variables.createVariable("variable2");
        var variable3 = variables.createVariable("variable3");
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            variables.createVariable("rebellionState").writeDataInt32(0); // 0 Modify Variable
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Mina to Beast, Mina to Beast, do you copy."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I haven’t been called Beast in awhile. Or are ye lookin’ for that sea rat that’s hiding in these parts."), "CO_DRAKE", GameEnums.COMood_Happy, "#00c010"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("How did you get this frequency?"), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Naval intelligence isn’t always a contradiction in terms."), "CO_DRAKE", GameEnums.COMood_Normal, "#00c010"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Even still, we’re here, and we’re going to give the people of Green Earth the tools they need to overthrow your corrupt dictatorship."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Stow that load of bilge water."), "CO_DRAKE", GameEnums.COMood_Normal, "#00c010"); dialog5.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("End transmission. All right, we’re outmatched here, but our first step is to just get this APC full of supplies to the resistance HQ in the middle of the map. Once we do that, they’ll help us push Green Earth out of this port. And after that, it should spread to the rest of the continent."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog6.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            // Dialog
        } // Start Of Turn End
        if ((turn - 1) % 1 === 0 && player === 1) { // 0 Each Day
        if (variables.createVariable("rebellionState").readDataInt32() === 4) { // 0 Check Variable
            variables.createVariable("rebellionState").writeDataInt32(5); // 0 Modify Variable
            map.replaceBuilding("FACTORY", 12, 1); // 0 Spawn Building
            if(map.getTerrain(12, 1).getBuilding() !== null){map.getTerrain(12, 1).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.replaceBuilding("ZMINICANNON_E", 11, 2); // 0 Spawn Building
            if(map.getTerrain(11, 2).getBuilding() !== null){map.getTerrain(11, 2).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(10, 1).getBuilding() !== null){map.getTerrain(10, 1).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(10, 3).getBuilding() !== null){map.getTerrain(10, 3).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(12, 3).getBuilding() !== null){map.getTerrain(12, 3).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.spawnUnit(12, 1, "MECH", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(10, 3, "ZCOUNIT_HOT_TANK", map.getPlayer(1), 0); // 0 Spawn Unit
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("The last of you got your supplies. From here, everything else we get, we take with a gun or a knife."), "CO_BEAST", GameEnums.COMood_Normal, "#800080"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Beast, come in. Beast, what’s going on. I’m getting reports of attacks against civilians. Of, of things I don’t want to believe."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Stupid woman. What did you think would happen? This is war. And in war, you survive or you wind up as food."), "CO_BEAST", GameEnums.COMood_Normal, "#800080"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            // Dialog
        } // Check Variable End
        } // Each Day End
        if ((turn - 1) % 1 === 0 && player === 1) { // 0 Each Day
        if (variables.createVariable("rebellionState").readDataInt32() === 3) { // 0 Check Variable
            variables.createVariable("rebellionState").writeDataInt32(4); // 0 Modify Variable
            map.replaceBuilding("FACTORY", 8, 15); // 0 Spawn Building
            if(map.getTerrain(8, 15).getBuilding() !== null){map.getTerrain(8, 15).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.replaceBuilding("ZMINICANNON_E", 7, 16); // 0 Spawn Building
            if(map.getTerrain(7, 16).getBuilding() !== null){map.getTerrain(7, 16).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(6, 15).getBuilding() !== null){map.getTerrain(6, 15).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(6, 15).getBuilding() !== null){map.getTerrain(6, 15).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(6, 17).getBuilding() !== null){map.getTerrain(6, 17).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(8, 17).getBuilding() !== null){map.getTerrain(8, 17).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.spawnUnit(8, 15, "MOTORBIKE", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(6, 17, "INFANTRY", map.getPlayer(1), 0); // 0 Spawn Unit
        } // Check Variable End
        } // Each Day End
        if ((turn - 1) % 1 === 0 && player === 1) { // 0 Each Day
        if (variables.createVariable("rebellionState").readDataInt32() === 2) { // 0 Check Variable
            variables.createVariable("rebellionState").writeDataInt32(3); // 0 Modify Variable
            if(map.getTerrain(10, 15).getBuilding() !== null){map.getTerrain(10, 15).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(10, 17).getBuilding() !== null){map.getTerrain(10, 17).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(12, 15).getBuilding() !== null){map.getTerrain(12, 15).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.replaceBuilding("ZMINICANNON_E", 11, 16); // 0 Spawn Building
            if(map.getTerrain(11, 16).getBuilding() !== null){map.getTerrain(11, 16).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.replaceBuilding("AIRPORT", 12, 17); // 0 Spawn Building
            if(map.getTerrain(12, 17).getBuilding() !== null){map.getTerrain(12, 17).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.spawnUnit(12, 17, "DUSTER", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(10, 15, "ZCOUNIT_PARTISAN", map.getPlayer(1), 0); // 0 Spawn Unit            
        } // Check Variable End
        } // Each Day End
        if ((turn - 1) % 1 === 0 && player === 1) { // 0 Each Day
        if (variables.createVariable("rebellionState").readDataInt32() === 1) { // 0 Check Variable
            variables.createVariable("rebellionState").writeDataInt32(2); // 0 Modify Variable        
            map.replaceBuilding("FACTORY", 8, 3); // 0 Spawn Building
            if(map.getTerrain(8, 3).getBuilding() !== null){map.getTerrain(8, 3).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(6, 1).getBuilding() !== null){map.getTerrain(6, 1).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(6, 3).getBuilding() !== null){map.getTerrain(6, 3).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            if(map.getTerrain(8, 1).getBuilding() !== null){map.getTerrain(8, 1).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.replaceBuilding("ZMINICANNON_E", 7, 2); // 0 Spawn Building
            if(map.getTerrain(7, 2).getBuilding() !== null){map.getTerrain(7, 2).getBuilding().setOwner(map.getPlayer(2 - 1));} // 0 Change Building Owner
            map.spawnUnit(8, 3, "ZCOUNIT_SMUGGLER", map.getPlayer(1), 0); // 0 Spawn Unit
            map.spawnUnit(6, 1, "ZCOUNIT_AT_CYCLE", map.getPlayer(1), 0); // 0 Spawn Unit
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You got your weapons, men. So use ‘em. Kill ‘em all. Take anything you can. We rise up now!"), "CO_BEAST", GameEnums.COMood_Normal, "#800080"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Check Variable End
        } // Each Day End
    }; // turnStart
    this.actionDone = function(action, map) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
        var variable4 = variables.createVariable("variable4");
        var variable5 = variables.createVariable("variable5");
        var variable5Value = variable5.readDataInt32();
        if (variable5Value === 0){variable5.writeDataInt32(map.getTerrain(0, 9).getUnit().getUniqueID());}
        variable5Value = variable5.readDataInt32();
    // preconditionend
        if (map.isUnitInArea(Qt.rect(9, 9, 1, 1), variable5Value) && variable4.readDataBool() === false) {// 0 9 0 Unit in Area
            variables.createVariable("rebellionState").writeDataInt32(1); // 0 Modify Variable            
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We’re here. Where’s the contact?"), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Beast welcomes you and your guns."), "CO_BEAST", GameEnums.COMood_Happy, "#800080"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("You can get these to resistance?"), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("My men will be shooting and looting in no time."), "CO_BEAST", GameEnums.COMood_Happy, "#800080"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Wait, what do you mean looting? Get back here. He’s gone. I hope this goes well."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            // Dialog
            variable4.writeDataBool(true);
        } // Unit in Area End
        // handle victor
        var minaPlayer = map.getPlayer(0);
        var beastPlayer = map.getPlayer(1);
        var beastPlayerHqCount = beastPlayer.getBuildingCount("HQ");
        var beastUnitCount = beastPlayer.getUnitCount();
        var variableBeastUnitCount = variables.createVariable("beastUnitCount");
        var drakePlayer = map.getPlayer(2);
        var drakeHqCount = drakePlayer.getBuildingCount("HQ");        
        var drakeUnitCount = beastPlayer.getUnitCount();
        var variableDrakeUnitCount = variables.createVariable("drakeUnitCount");
        if (drakeUnitCount > 0)
        {
            variableDrakeUnitCount.writeDataBool(true);
        }
        if (drakeUnitCount > 0)
        {
            variableBeastUnitCount.writeDataBool(true);
        }
        var drakeHadUnits = variableBeastUnitCount.readDataBool();
        if ((drakeHqCount <= 0 ||
            (drakeUnitCount <= 0 && drakeHadUnits)) &&
           !drakePlayer.getIsDefeated())
        {            
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Haul anchor, lads, we’re outmatched here."), "CO_DRAKE", GameEnums.COMood_Sad, "#00c010"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("We’ve won."), "CO_MINA", GameEnums.COMood_Happy, "#ff33cc"); dialog1.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("And to the victor go the spoils! Take whatever you want, boys. And make sure you shoot some people to show them who’s in charge."), "CO_BEAST", GameEnums.COMood_Happy, "#800080"); dialog2.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("I’m not letting that happen. We’re here to liberate the people of Green Earth."), "CO_MINA", GameEnums.COMood_Normal, "#ff33cc"); dialog3.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, qsTr("Well, it looks like the Pinks are as weak as we’d heard. Alright, Beast will take you on too. Beast will take on anyone. Death and blood!"), "CO_BEAST", GameEnums.COMood_Normal, "#800080"); dialog4.loadBackground(""); // 1 ScriptEventDialogItem
            dialog0.queueAnimation(dialog1);
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            beastPlayer.setTeam(5);
            if (drakeHqCount <= 0)
            {
                drakePlayer.defeatPlayer(map.getCurrentPlayer());                
            }
            else
            {
                drakePlayer.defeatPlayer(null);                
            }
        }
        if (beastUnitCount > 0)
        {
            variableBeastUnitCount.writeDataBool(true);
        }
        var beastHadUnits = variableBeastUnitCount.readDataBool();
        if (beastPlayerHqCount <= 0 ||
           (beastUnitCount <= 0 && beastHadUnits))
        {
            if (beastPlayerHqCount <= 0)
            {
                beastPlayer.defeatPlayer(map.getCurrentPlayer());                
            }
            else
            {
                beastPlayer.defeatPlayer(null);                
            }          
        }
        if (minaPlayer.getBuildingCount("HQ") <= 0 ||
            minaPlayer.getUnitCount() <= 0)
        {
            minaPlayer.defeatPlayer(null);
        }
        
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
