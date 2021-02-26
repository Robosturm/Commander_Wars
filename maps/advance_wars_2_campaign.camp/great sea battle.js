var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Is this the end?"),
                        "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander? What are your orders, sir?"),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Do as you please."),
                        "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Um... and you, Commander? You're not planning on going down with the ... er... factory?"),
                        "co_officier_bh", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Fool! This place may be important, but I've no desire to die with it. Now go! If you want to save your skin, you must do it yourself!"),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir. Farewell, sir."),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hah... This is one situation I never expected myself to be in. I survive, though. And where there's life, there is opportunity"),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The factory is captured"),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("And the enemy CO?"),
                        "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Missing in action, sir. He disappeared at the time the factory was secured."),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Really... Nice work, soldier."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Thank you, ma'am!"),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's over at last."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Mm. Peace will now return to Green Earth."),
                        "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Whew... That's nice."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh? That was unexpected."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hm? What was?"),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("'Nice'? I thought you'd be more... excited."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Oh, I'm thrilled. I just want to take a nap is all. Yaaaaawn! I'm bushed!"),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hm hm hmm..."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ha ha haa..."),
                        "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Congratulations on your victory. As your ally, we are very happy"),
                        "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));

            var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We couldn't have done it without your assistance. On behalf of the people of Green Earth, I thank you."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hearing that makes the journey worthwhile."),
                        "co_kanbei", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, it does. Now, Commander Eagle, I think you grasp the situation, but..."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes. The war is not yet finished."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Correct. You know where that pipe leads... Until we've defeated Black Hole on their own turf, the war will never truly be finished."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It sounds like things are just going to continue getting tougher."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's not like the combat we've seen up to now has been child's play. No matter who the foe may be, we won't lose."),
                        "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
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
        var geList = campaignScript.getGEBuildList();
        map.getPlayer(0).setBuildList(geList);
        campaignScript.setArmyData(1, campaignScript.getArmy(map.getPlayer(1).getCO(0)));
        campaignScript.setArmyData(2, campaignScript.getArmy(map.getPlayer(2).getCO(0)));

        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(3).setBuildList(bhList);
        map.getPlayer(3).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function(action)
    {
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var weld = map.getTerrain(36, 4);
        if (weld.getTerrainID() !== "WELD")
        {
            // set victory to true
            map.getPlayer(3).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        var blackFactory = map.getTerrain(32, 3).getBuilding();
        blackFactory.setFireCount(0);
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }

        // factory spawning comes here
        if (turn === 1 && player === 3)
        {
            map.spawnUnit(31, 6, "T_HELI", map.getPlayer(3), 0);
            map.spawnUnit(32, 6, "INFANTRY", map.getPlayer(3), 0);
        }
        else if (turn === 2 && player === 3)
        {
            map.spawnUnit(31, 6, "K_HELI", map.getPlayer(3), 0);
            map.spawnUnit(32, 6, "K_HELI", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "K_HELI", map.getPlayer(3), 0);
        }
        else if (turn === 3 && player === 3)
        {
            map.spawnUnit(32, 6, "HEAVY_TANK", map.getPlayer(3), 0);
        }
        else if (turn === 4 && player === 3)
        {
            map.spawnUnit(31, 6, "ARTILLERY", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "ARTILLERY", map.getPlayer(3), 0);
        }
        else if (turn === 6 && player === 3)
        {
            map.spawnUnit(31, 6, "K_HELI", map.getPlayer(3), 0);
            map.spawnUnit(32, 6, "T_HELI", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "K_HELI", map.getPlayer(3), 0);
        }
        else if (turn === 7 && player === 3)
        {
            map.spawnUnit(24, 6, "NEOTANK", map.getPlayer(3), 0);
        }
        else if (turn === 9 && player === 3)
        {
            map.spawnUnit(8, 6, "FLAK", map.getPlayer(3), 0);
        }
        else if (turn === 11 && player === 3)
        {
            map.spawnUnit(31, 6, "FIGHTER", map.getPlayer(3), 0);
            map.spawnUnit(32, 6, "K_HELI", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "BOMBER", map.getPlayer(3), 0);
        }
        else if (turn === 13 && player === 3)
        {
            map.spawnUnit(31, 6, "BOMBER", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "BOMBER", map.getPlayer(3), 0);
        }
        else if (turn === 16 && player === 3)
        {
            map.spawnUnit(31, 6, "NEOTANK", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "NEOTANK", map.getPlayer(3), 0);
        }
        else if (turn === 17 && player === 3)
        {
            map.spawnUnit(31, 6, "MISSILE", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "ROCKETTHROWER", map.getPlayer(3), 0);
        }
        else if (turn === 20 && player === 3)
        {
            map.spawnUnit(31, 6, "K_HELI", map.getPlayer(3), 0);
            map.spawnUnit(32, 6, "FIGHTER", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "K_HELI", map.getPlayer(3), 0);
        }
        else if (turn === 23 && player === 3)
        {
            map.spawnUnit(31, 6, "FLAK", map.getPlayer(3), 0);
            map.spawnUnit(32, 6, "FLAK", map.getPlayer(3), 0);
            map.spawnUnit(33, 6, "FLAK", map.getPlayer(3), 0);
        }
        else if (turn > 25 && player === 3)
        {
            // end of fixed scripted spawn random spawn happens now
            gameScript.spawnFactory();
        }

    };

    this.spawnFactory = function()
    {
        // first deal with the spawning count
        var amount = globals.randInt(0, 5);
        if (amount <= 4 && amount >= 2)
        {
            amount = 0;
        }
        else if (amount === 5)
        {
            amount = 1;
        }
        else
        {
            amount = -1;
        }
        // than spawn some random units :)
        for (var i = 0; i <= amount; i++)
        {
            var units = ["BOMBER", "FIGHTER", "NEOTANK", "LIGHT_TANK", "FLAK", "ARTILLERY", "MECH", "HEAVY_TANK", "INFANTRY", "K_HELI", "T_HELI"];
            var index = globals.randInt(0, 10);
            var pos = globals.randInt(0, 2);
            map.spawnUnit(31 + pos, 6, units[index], map.getPlayer(2), 0);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lord Hawke, the enemy has finished establishing their field HQ."),
                    "co_officier_bh", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It has come to this... brought to bay against our own factory."),
                    "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hawke..."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Here, Lord Sturm."),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Explain yourself. The enemy has routed your wretched companions... And here you are, cringing like a dog with its tail beneath its legs."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Protect my factory at any cost. If you fail, you will know no peace and no forgiveness, cur."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, my liege."),
                    "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));

        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("As soon as we're ready, begin the attack on that factory."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Think we can handle this on our own? It looks like Black Hole is mustering all its troops."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This war won't end until we destroy that factory. If we don't do this, no one will."),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's not exactly true."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander Kanbei!"),
                    "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If we pool our might and fight as one, our chance of victory increases. Isn't that so, Nell?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, it is. We want to help as well. Blue Moon can't send soldiers, but their support is unwavering."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I understand... It's not like we've been fighting this war on our own."),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Together, we stand strong. Now let's go liberate Green Earth!"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

