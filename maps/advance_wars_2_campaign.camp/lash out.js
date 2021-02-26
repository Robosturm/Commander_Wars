var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Congratulations, Sami! You won! "),
                        "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Whew... I came through somehow. "),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Now, I'd like to talk to our energetic little friend again. Sami! Drag her over here, please. "),
                        "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Uh, I... Yes, ma'am! "),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("......"),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wait a minute... This isn't that brat! This... This is..."),
                        "co_nell", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's a dummy. "),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("She escaped!? Unbelievable! She played us like a pair of fools! Flak's troops are closing in. Let's get out of here! "),
                        "co_nell", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What a troublesome pair they turned out to be... "),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        map.getGameRules().addVictoryRule("VICTORYRULE_TURNLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_TURNLIMIT"); // loose if the turn limit is gone
        turnLimit.setRuleValue(7, 0);
        var osList = campaignScript.getOSBuildList();
        map.getPlayer(0).setBuildList(osList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        var t_copter = map.getTerrain(12, 4).getUnit();
        var lander = map.getTerrain(8, 12).getUnit();
        var cruiser = map.getTerrain(12, 10).getUnit();
        var submarine = map.getTerrain(12, 11).getUnit();
        var battleship = map.getTerrain(11, 11).getUnit();
        var power = map.getPlayer(0).getCO(0).canUsePower();

        if (t_copter === null || t_copter.getHasMoved() === true)
        {
            // read the tower variable or create it
            var t_copterDialog = variables.createVariable("t_copterDialog");
            if (t_copterDialog.readDataBool() === false)
            {
                gameScript.t_copterDialog();
                t_copterDialog.writeDataBool(true);
            }
        }
        if (lander === null || lander.getHasMoved() === true)
        {
            // read the tower variable or create it
            var landerDialog = variables.createVariable("landerDialog");
            if (landerDialog.readDataBool() === false)
            {
                gameScript.landerDialog();
                landerDialog.writeDataBool(true);
            }
        }
        if (cruiser === null || cruiser.getHasMoved() === true)
        {
            // read the tower variable or create it
            var cruiserDialog = variables.createVariable("cruiserDialog");
            if (cruiserDialog.readDataBool() === false)
            {
                gameScript.cruiserDialog();
                cruiserDialog.writeDataBool(true);
            }
        }
        if (submarine === null || submarine.getHasMoved() === true)
        {
            // read the tower variable or create it
            var submarineDialog = variables.createVariable("submarineDialog");
            if (submarineDialog.readDataBool() === false)
            {
                gameScript.submarineDialog();
                submarineDialog.writeDataBool(true);
            }
        }
        if (battleship === null || battleship.getHasMoved() === true)
        {
            // read the tower variable or create it
            var battleshipDialog = variables.createVariable("battleshipDialog");
            if (battleshipDialog.readDataBool() === false)
            {
                gameScript.battleshipDialog();
                battleshipDialog.writeDataBool(true);
            }
        }
        if (power === true)
        {
            // read the tower variable or create it
            var powerDialog = variables.createVariable("powerDialog");
            if (powerDialog.readDataBool() === false)
            {
                gameScript.powerDialog();
                powerDialog.writeDataBool(true);
            }
        }
    };
    this.t_copterDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If we leave this T-copter here, those 2 A-air units will take it. If we go to sea, it becomes vulnerable to that cruiser. Both of these units can fire directly on air units. If that T copter gets hit, it doesn't stand a chance. Now, we could rely on its movement range and flee... But there's another way to keep it safe. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Easy! Cruisers can carry up to 2 B copters or T copters! As long as the cruiser's safe, so are its passengers. As luck would have it, you have a cruiser under your command. Why don't you try it out? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };
    this.landerDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What we have here is a lander. It transports troops in exactly the same way APCs do. It's also restricted to dropping units on terrain those units can traverse. It's unique in that it can carry 2 ground units at once. On this map, the only place it can drop troops is on shoals. It can normally use ports as well, but since there are no ports here... "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Under these conditions, this lander is a unit I just can't do without. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I agree. You must be careful to keep it from being fired upon. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };
    this.cruiserDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK, let's take a look at the cruiser you had selected. It's a direct- combat unit, but it can only fire on certain units. It can only attack air units and subs. I believe it can reach the enemy sub from here, so go ahead and attack. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
    };
    this.submarineDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You've selected a sub. Subs are direct-combat units. Subs can fire on all naval units, including the cruiser above. However, they're rather ineffective vs. cruisers, so don't expect too much. Subs also have the ability to dive under the surface of the sea. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Dive? That means they can't be attacked while submerged, right? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's half true. They can't be fired upon by most units while submerged. However, other subs and cruisers can attack at any time. As you know, when naval units run out of fuel, they sink. Submerged subs use a lot more fuel. In fact, they use 5 units per day. If in danger, select Rise to surface, and head to port... Well, there are no ports here, so use an APC to refuel. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };
    this.battleshipDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This is a battleship. It's an indirect-combat naval unit. It can fire on both ground and naval units. It also has the largest range of fire of all indirect-combat units. It should be able to reach that rocket unit from its current location. Give it a try!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
    };
    this.powerDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sami! Your CO Power Meter's full! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? It doesn't look full to me... "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Your Power Meter has two levels: a normal CO Power level and a Super CO Power level. You have enough energy now for a normal CO Power. You can use it now or wait and save up for a Super CO Power. How you use your CO Powers is completely up to you. The well-timed use of a CO Power can alter the course of a battle. To use a CO Power, select Power from the map menu. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };
    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
            // small modification of sami's power bar
            map.getPlayer(0).getCO(0).setPowerFilled(2.5);
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.initDialogBH();
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
        else if (turn === 6 && player === 0)
        {
            GameAnimationFactory.createGameAnimationDialog(
                                qsTr("Flak's troops arrive tomorrow. I wonder if Sami's OK. "),
                                "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("According to our intel, it's just a little farther ahead... "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sami, how sure are you about this information's accuracy? It seems odd that Flak would be out here resting his troops."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I know, but do you really think Flak's capable of setting a trap? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, since you put it that way... "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're right! Flak didn't set a trap for you... I did! Flak doesn't have a cunning bone in his body! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Who's that? A little girl? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A Black Hole CO perhaps? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee hee! You got it! My name's Lash! Nice to meet you! Would you like to play with me? What do you say, grandma? "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I do believe I'm being taunted. Sami? Bring her to me... alive. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uh... OK. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Look at this terrain... I think we'll need some naval units here. See that base up there? There's nothing we can do to keep it from the enemy. But if we ignore it, they'll use it to deploy ground unit after ground unit. Which means... there's only one way for us to proceed. We have to get to that isle and capture the enemy HQ! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! If victory relies on capturing, then I'm the right CO for the job! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("As well you should be! You're quite skilled at securing properties. Plus, your transport units possess superior movement range. That goes for T copters and this lander as well! I'm sure you'll be able to handle all transports with ease. First, let's get that T copter out of trouble! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };

    this.initDialogBH = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, I almost forgot. I have something to tell you! You know, I didn't lead you all the way out here for nothing. On day 7, Flak's bringing his troops, and we're gonna pinch you like a bug! "),
                    "co_Lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What?!? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yep! You're running out of time! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, isn't this an unpleasant surprise. This Black Hole CO has a brain. OK, Sami, you heard her! Flak and his goons are closing in! We've got to clear this map within 7 days! Here's your chance to prove your rep for surprising the foe is deserved. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! I'll give it my best shot! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Day 2... Let me tell you a bit more about these naval units. At the beginning of each day, naval units burn 2 units of fuel. If their fuel supply runs out, they sink and vanish. To refuel, place naval units in ports or move them next to APCs. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK. I'm going to leave the rest of this battle up to you. Good luck! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

