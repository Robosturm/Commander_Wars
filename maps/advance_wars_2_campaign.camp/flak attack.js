var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediately without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team, map)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("All right! How's that, Sami? Not a scratch anywhere! "),
                        "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Thanks, Max! But what about your troops? "),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Hah! This is nothing. My troops eat this stuff for breakfast! "),
                        "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Grrr... Blast! "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yo, Flak! You had enough? "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("......"),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Just give it up, already. "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Don't start mouthin' off 'cause you won here today. "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("What?! "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("No one can stop the Black Hole invasion. You'll see. One day, you and your girlfriend'll be begging for mercy! I'll retreat for now, but we'll meet again. So long."),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Girlfriend!? Who does he think he is? Black Hole goons... Not an ounce of decency in any of them. "),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yeah, but they're never gonna take Orange Star. Not while we're here. "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Sami! Max! Are you all right? Hm? The battle... It's... finished? "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Hey there, Nell! You're just in time to join the celebration! "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
        }
    };
    this.gameStart = function(map)
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
    };
    this.actionDone = function(action, map)
    {
        var variables = map.getGameScript().getVariables();
        var rocket = map.getTerrain(13, 8).getUnit();
        var b_copter1 = map.getTerrain(9, 2).getUnit();
        var b_copter2 = map.getTerrain(13, 9).getUnit();
        var mapTown = map.getTerrain(13, 5).getBuilding();
        if (b_copter1 === null || b_copter1.getHasMoved() === true ||
            b_copter2 === null || b_copter2.getHasMoved() === true)
        {
            // read the tower variable or create it
            var b_copterDialog = variables.createVariable("b_copterDialog");
            if (b_copterDialog.readDataBool() === false)
            {
                gameScript.b_copterDialog(map);
                b_copterDialog.writeDataBool(true);
            }
        }
        if (rocket === null || rocket.getHasMoved() === true)
        {
            // read the tower variable or create it
            var rocketDialog = variables.createVariable("rocketDialog");
            if (rocketDialog.readDataBool() === false)
            {
                gameScript.rocketDialog(map);
                rocketDialog.writeDataBool(true);
            }
        }
        if (mapTown.getOwner() !== null &&
            mapTown.getOwner().getPlayerID() === 0)
        {
            var mapTownDialog = variables.createVariable("mapTownDialog");
            if (mapTownDialog.readDataBool() === false)
            {
                gameScript.mapTownDialog(map);
                // enable os map lab
                var campaignVariables = map.getCampaign().getVariables();
                var osLabFound = campaignVariables.createVariable("osLabFound");
                osLabFound.writeDataBool(true);
                mapTownDialog.writeDataBool(true);
            }
        }
    };
    this.b_copterDialog = function(map)
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Battle copters! Let's move! "),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr(" Listen up! B copters are direct-combat units... but they can't fire on fighters or bombers. In a copter dogfight, the attacker wins! Now go make some noise! "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Sir! Yes, sir! "),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };
    this.rocketDialog = function(map)
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("The unit you've selected is a rocket unit. It's an indirect-combat unit with a fairly wide range of fire: 3-5 spaces. For you, though, Max, its range of fire drops to 3-4 spaces. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yeah, I just can't seem to get the hang of indirect combat. "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Don't worry too much, Max. You ARE the king of direct combat, after all. Oh yes, rockets run out of ammo quickly... so make sure you deploy them in areas that are easy to get supplies to. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };
    this.mapTownDialog = function(map)
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Commander! We recovered this in a city we captured! "),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Huh? What's this supposed to be? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Ah! That... That's a map to the enemy lab! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("They left a map? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr(" Actually, my troops have been ordered to find the lab on this map! It seems that Black Hole has these labs scattered here and there. Reports are that they're research centers for new weapon development."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Here and there? So there are labs like this in Yellow Comet and Blue Moon? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("We're not completely sure. They're... hidden and all. Which is why we need maps like this to help us search them out. I'll warn our allies and get this intel to Nell. At the very least, we've pinned down the lab here in Orange Star. After we're through here, we should be able to proceed to the lab via the Map Select screen. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
    this.turnStart = function(turn, player, map)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog(map);
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.initDialogBH(map);
        }
    };

    this.initDialog = function(map)
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Lord Flak, troop deployment is complete. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Look what we have here. Max's boys... I'm gonna enjoy this. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Uh, sir... The unauthorized use of troops for personal vendettas is... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Is what, worm? I'm not gonna let Max get away with what he did. As long as I win, who cares about rules? Now get outta here! "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Understood... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Max! It's an attack! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What? Battle copters? Bombers? Coming with an air assault, huh? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("This doesn't look good. My troops are still recovering, and I don't think they're ready... Infantry vs. planes isn't the best matchup to begin with. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Leave it to me! I'll bring those birds down! "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I'll come with you. I can at least offer some advice. Together, we can get through this."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("All right! "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("They have bombers... There are fighters, too. OK, let's just stay calm. These are A-airs: anti-air units. As the name implies, they're strong against air units. As direct-combat units, they're no slouches vs. infantry, either."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Oh, yeah! I love direct combat! "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Now these are missiles. They're an indirect-combat weapon. They can only fire on air units, but the damage they cause is horrific! For more intel, select the unit"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yeah, and to see the range of fire, press B (default) or the Right Mouse Button, right? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Uh-huh! I was a bit worried, but it looks like it's all coming back to you. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Don't worry; I'll get you home without a scratch! Now it's time to light up the skies! But just remember, you owe me one!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };

    this.initDialogBH = function(map)
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Lord Flak, we've just been told that we're not authorized to use either bombers or fighters! "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Grrr... Who needs authorization?! They were just sitting there! "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Lord Flak, we don't have many soldiers trained to use them... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Trained? I'll train 'em myself! These bombers... They're direct- combat units. They're good against ground units. They're even good on Md tanks. But they can't fire on other air units. They're bombers. They bomb. This here's a fighter. It can attack other air units. It's the best weapon to have when you're stompin' flying stuff. You have to watch fuel with air units. They burn 2 units of fuel per day. Ground units stop moving when they run out of fuel, but that's all. An air unit without fuel falls out of the sky and crashes. You got it? Then attack! The more you take out, the better. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yes, sir! Delta squad is away!"),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

