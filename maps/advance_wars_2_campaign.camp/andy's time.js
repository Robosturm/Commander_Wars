var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.getVictoryInfo = function()
    {
        return qsTr("Destroy the enemies Black Cannon.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wahoo! I won! I won! "),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grrr! Now you've done it, runt! "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I'm no runt! My name's Andy! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Name!? I didn't ask for your name! Argh! I can't lose any more units! I can't! Sound the retreat! Head back to base and regroup! "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));

            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" As long as I'm here, you'll never take Orange Star! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yo, Andy... You OK? "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Max! How was I? Did you watch the whole thing? "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yeah... I watched. That was a little close. "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! Those tanks back there... Are those your troops, Max? "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hm? Oh, um... those are...  "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander Max! We received your orders and moved out at once! "),
                        "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well, no one ordered you to broadcast it to the entire world, soldier! "),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hmmm... "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What?!? It's a training exercise! Gotta check those engines every now and then! Geez... C'mon, boys. Let's go home. "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You know, Nell, I'm really glad to have Max on our side! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("So am I, Andy. So am I. "),
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
            dialog13.queueAnimation(dialog14);
            dialog14.queueAnimation(dialog15);
            dialog15.queueAnimation(dialog16);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        var osList = campaignScript.getOSBuildList();
        map.getPlayer(0).setBuildList(osList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var blackCannon = map.getTerrain(3, 0).getBuilding();
        if (blackCannon.getBuildingID() === "ZBLACK_BUILDING_DESTROYED")
        {
            // set victory to true
            map.getPlayer(1).setIsDefeated(true);
        }
        else if (blackCannon.getFireCount() === 0 && map.getCurrentDay() > 1 &&
                 map.getCurrentPlayer().getPlayerID() === 1)
        {
            // create the cooldown counter for the black cannon if it fired
            var cannonCounter = variables.createVariable("cannonCounter");
            var value = cannonCounter.readDataInt32();
            if (value === 0)
            {
                cannonCounter.writeDataInt32(2);
            }
            // create first shot dialog
            var cannonFiredDialog = variables.createVariable("cannonFiredDialog");
            if (cannonFiredDialog.readDataBool() === false)
            {
                gameScript.cannonFiredDialog();
                cannonFiredDialog.writeDataBool(true);
            }
        }

    };

    this.cannonFiredDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yippee! It fired! Black Cannon... attack! Tee hee! "),
                    "co_lash", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Contact at last. Andy, are you OK? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah,... I think so. "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well don't overdo it, all right? By the way, a few of the gray stars in the window displaying your funds have changed color, right? That's your CO Power Meter. Power builds up slowly during combat. When the stars are flashing, your CO Power is ready for use. To see intel on CO Powers, select CO from the Map menu. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };

    this.turnStart = function(turn, player)
    {
        //
        var blackCannon = map.getTerrain(3, 0).getBuilding();
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.initBHDialog();
            // disable black cannon on day 1
            blackCannon.setFireCount(0);
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
        else if (turn === 3 && player === 0)
        {
            gameScript.day3Dialog();
        }
        else if (player === 1)
        {
            // this part deals with the cooldown of the black cannon for this map
            var variables = map.getGameScript().getVariables();
            var cannonCounter = variables.createVariable("cannonCounter");
            var value = cannonCounter.readDataInt32();
            value -= 1;
            if (value < 0)
            {
                value = 0;
            }
            if (value > 0)
            {
                blackCannon.setFireCount(0);
            }
            cannonCounter.writeDataInt32(value);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Flak! Flakalakalakalaaaaaak!!! The Black Cannon's ready to go! "),
                    "co_random", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Got it. Thanks, Lash. Now I'll smash 'em good! "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't forget, you promised to be a guinea pig in my next experiment! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... Yeah, I remember. "),
                    "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, I'm off! Toodles! Have fun! Oh, yeah! Almost forgot, Hawke said to hurry and conquer Orange Star! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hawke said... Crush. I gotta crush 'em here. I have to... crush 'em all... "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Pardon me, ma'am, Commander Andy's arrived. "),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Thank you. Send him in. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey, Nell! How're things looking? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's what concerns me... Do you see that giant cannon? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Wow! It's huge! I've never seen anything like it! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A cannon that size is a first for me, too. The Black Hole Army seems to be much more of a threat than last time. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You got that right! Nell, let me take care of this. "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Max? What are you doing here? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("C'mon, Nell. This job's too much for Andy. You need someone with my strength to take out that cannon. "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! I can do this! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Knock it off, Andy. War isn't a game. You'll just get hurt. "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Max, I have faith in Andy. Even if he's not as powerful as you are, he can do things no one else can. I think this will be a good experience for him, so let's let him handle this. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmph! Don't come cryin' to me if he gets hurt! Yo, Andy! You sure you're up for this? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You know it! Sit back and watch, Max! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! What's up with this? There are hardly any units deployed! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's up to you to decide which units you need and where to deploy them. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Deploy them? You mean I have to form units as I go along? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's exactly what I mean. See those 2 properties next to your HQ? Those are deployment properties called bases. You can deploy ground units from them. To deploy units, select a base and Space (default) or the Left Mouse Button. Now, there's a window at the top of the screen, right? The 6,000 you see here is the amount of funding you have. Funds increase depending on the number of properties you control. You receive 1,000 per property each day. You currently have your HQ, 2 bases, and 3 cities. That amounts to 6,000 in funds."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I get it! Funds increase on the day after I capture a new property! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" That's exactly right, Andy. Then you use that money to deploy more troops. Select a base and press Space or the Right Mouse Button to select the type of unit you want to deploy. The number next to the unit is its deployment cost. Remember, units cannot move on the day that they are deployed. In this case... Yes, that should work. First, you need to increase your funds, so deploy some infantry or mech units and capture some neutral properties. Don't forget to use your APCs and T copters to transport to properties farther out, too. As for combat, you've got some room to maneuver, so I'd advise advancing with tanks and artillery. When you can afford, deploy some more powerful units, like Md tanks. Once you can capture neutral bases, you can deploy troops from them. Of course, the enemy can do the same, so don't dillydally! I'll contact you as soon as I have any intel on that giant cannon. This may be rough going, but do your best! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Here I go! Keep watching, Max! I'm gonna show you something! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };

    this.initBHDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So? Is it ready? "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... The cannon? Nope, not yet. It's going to take the rest of the day just getting ready to fire. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grrr... Is that so? Guess I'll go take charge of the battle then.  "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK! Toodles! Tomorrow's the day! Tee hee! "),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Andy! I've got some intel on that monstrosity! It's called a Black Cannon. It's supposed to have a huge range of fire. Use B (default)  or the Right Mouse Button to check it out. That's just a prototype, though, so it can only fire once every 2 days. It's designed to fire on the strongest unit within range. I'll get back to you once I get some intel on its weak points. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
    };

    this.day3Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Andy, I've discovered the enemy's weak point. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("All right! Way to go, Nell! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" When you attack the Black Cannon, aim here, for the door. Check the cursor window to see the unit's HP. If you can take out the Black Cannon, victory's yours! That thing surprised me at first, but it's nothing to fear now. Good luck, Andy! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No problem! I'll take care of it!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

