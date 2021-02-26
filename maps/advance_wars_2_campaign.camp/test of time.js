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
        return qsTr("Capture the enemy labor.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Lord Flak... The new weapons plans have been stolen... "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(".........Assemble everyone in the factory. "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("In the factory... Yes, sir! "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Blast! "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));

            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("All right! We've shut down the enemy lab! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Excellent work, in spite of difficult conditions. With these plans, we should be able to develop neotanks of our own. I think we'll have more intel on them by the time you deploy them. Anyway, congratulations! This was a great victory. "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sami's intel really helped a lot! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I just wish I could've done more. "),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The two of you really make a great team. Let's head for home and celebrate! "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wahoo! Let's have a BBQ! "),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
            dialog9.queueAnimation(dialog10);
            // store neo tank data
            var campaignVariables = map.getCampaign().getVariables();
            var osNeotanks = campaignVariables.createVariable("osNeotanks");
            osNeotanks.writeDataBool(true);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_TURNLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_TURNLIMIT"); // loose if the turn limit is gone
        turnLimit.setRuleValue(7, 0);
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var labor = map.getTerrain(12, 6).getBuilding();
        var inf0 = map.getTerrain(1, 8).getUnit();
        var inf1 = map.getTerrain(6, 8).getUnit();
        if (labor.getOwner().getPlayerID() === 0)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
        if (inf0 === null || inf0.getHasMoved() === true ||
            inf1 === null || inf1.getHasMoved() === true)
        {
            // read the tower variable or create it
            var infDialog = variables.createVariable("infDialog");
            if (infDialog.readDataBool() === false)
            {
                var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Infantry and mech units can do more that just capture, you know. They can increase their vision range by climbing mountains! Since we have some mountains so close by, you should test it out. "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                infDialog.writeDataBool(true);
            }
        }

    };
    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hurry up! We gotta stop their advance here. "),
                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir. "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            dialog1.queueAnimation(dialog2);
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
        else if (turn === 3 && player === 0)
        {
            gameScript.day3Dialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lord Flak! The Orange Star Army! "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? How did they find us here?!? "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I... I don't know, sir. Maybe a map fell into their hands? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grrrr! How long will it take to erase all of the weapon data? "),
                    "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("10... No, we should be able to do it if we have 7 days.  "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, get started! Don't let them get that intel! "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you see that? It's a Black Hole lab. If this goes well, we might be able to seize their new weapon designs. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nell! We're ready to advance! But... shouldn't this be Sami's mission? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No can do, Andy! I'm still gathering my troops. My last battle left me with a shortage of combat-ready soldiers. But to be honest, I'd love to try and infiltrate their base by myself. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you honestly think that I would approve such recklessness? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" It's OK, Sami! Helping each other is what we're all about! My troops are fresh and itching to go! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's move out, Andy. We're in Fog of War, so let's exercise caution"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Fog of War... That's where you can't see very far, right? It's tough fighting in this gunk! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Everything will be fine. It just takes a little getting used to is all. Let's get started. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("As I mentioned earlier, this terrain is hidden by Fog of War. It prevents units from seeing beyond their individual vision ranges. For example, the vision range of infantry is 2 spaces, so they can see 2 spaces from the space they're deployed. Your own properties are lit up, but enemy and neutral properties aren't. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's a recon unit. Its vision range is terrific! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("With a vision of 5, recon units are indispensable in Fog of War. They also have extended movement ranges and can take on infantry and mech units with decent success. They're handy units to have around. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I can't see into those woods! What's going on? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You can't see into woods even if they're within your vision range. The only way to tell if an enemy unit is hiding there is to move to the space next to the woods. The same goes for the enemy, too. If you conceal a unit in the woods, it won't be spotted unless the enemy gets right next to it. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So battling in Fog of War's like a big game of hide-and-seek! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You know, Andy, you're right. It is. You'll also have to watch out for ambushes in Fog of War. These occur when you try to move to or through a space occupied by an enemy unit. If this happens, you'll stop moving, and you won't be able to move until the next day. There are lots of ambushes in woods, so be extra careful. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Got it! "),
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
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Andy, I have some soldiers out reconnoitering the field. According to my reports, there's an indirect-combat unit hiding in one of the 3 woods at the top of the map. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Really? OK, I got it! I'll advance carefully. Thanks a bunch, Sami!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How's the Fog of War? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It really slows things down. Strong units are useless if they can't see anything. It's tough if you don't use recon units well. "),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understanding that's half the battle! The rest comes with experience. Oh yes, there's something I forgot to tell you yesterday. In addition to woods, there's one other terrain type that conceals units until you're next to it. It's reefs. Like this one here. If you're looking to take out all enemy units, don't forget to look here! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };

    this.day3Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I have some new intel, Andy. Somewhere in the woods near the lab is another indirect-combat unit. If you suddenly come under fire, check the woods first. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Understood! Couldn't do this without you, Sami! I'll try and hold up my end! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

