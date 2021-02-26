var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.getVictoryInfo = function()
    {
        return qsTr("Destroy the Weld of the Black Hole Factory.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The factory... It's completely. It's the blasted weather! That was the problem! Hssss!"),
                        "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The Black Hole Army's in full retreat."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes! Father! Sensei! We won! We won!"),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("That we did. And an impressive win it was."),
                        "co_sensei", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Today's battle will long be remembered. Through our combined efforts, our common foe was defeated. I offer my gratitude. Thank you for your aid."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't be foolish! You would've done the same for us. Actually, Black Hole was foolish enough to attack us as well. Of course, we defeated them soundly. With... some help from Orange Star. Well, well... Here come Nell and her troops now."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Long time no see, Kanbei!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It seems that you have crossed swords with the Black Hole Army, too."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, we have. The strength of the Black Hole Army is immeasurable. Especially the CO named Hawke. We've heard reports that he has Green Earth on the verge of defeat."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Impossible!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("There's no time to spare. We must get to Green Earth on the double!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yellow Comet is with you! Our strength is yours! If we all stand together, there is nothing we need to fear!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var ycList = campaignScript.getYCBuildList();
        map.getPlayer(0).setBuildList(ycList);
        campaignScript.setArmyData(1, campaignScript.getArmy(map.getPlayer(1).getCO(0)));
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(2).setBuildList(bhList);
        map.getPlayer(2).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function(action)
    {
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var weld = map.getTerrain(17, 1);
        if (weld.getTerrainID() !== "WELD")
        {
            // set victory to true
            map.getPlayer(2).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        var blackFactory = map.getTerrain(25, 3).getBuilding();
        blackFactory.setFireCount(0);
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        // factory spawning comes here
        if (turn === 1 && player === 2)
        {
            map.spawnUnit(25, 6, "T_HELI", map.getPlayer(2), 0);
        }
        else if (turn === 2 && player === 2)
        {
             map.spawnUnit(24, 6, "BATTLESHIP", map.getPlayer(2), 0);
        }
        else if (turn === 3 && player === 2)
        {
            map.spawnUnit(24, 6, "K_HELI", map.getPlayer(2), 0);
            map.spawnUnit(25, 6, "K_HELI", map.getPlayer(2), 0);
            map.spawnUnit(26, 6, "T_HELI", map.getPlayer(2), 0);
        }
        else if (turn === 5 && player === 2)
        {
            map.spawnUnit(26, 6, "HEAVY_TANK", map.getPlayer(2), 0);
        }
        else if (turn === 8 && player === 2)
        {
            map.spawnUnit(24, 6, "K_HELI", map.getPlayer(2), 0);
            map.spawnUnit(25, 6, "K_HELI", map.getPlayer(2), 0);
            map.spawnUnit(26, 6, "T_HELI", map.getPlayer(2), 0);
        }
        else if (turn === 9 && player === 2)
        {
            map.spawnUnit(24, 6, "SUBMARINE", map.getPlayer(2), 0);
        }
        else if (turn === 10 && player === 2)
        {
            map.spawnUnit(8, 6, "NEOTANK", map.getPlayer(2), 0);
        }
        else if (turn === 11 && player === 2)
        {
            map.spawnUnit(25, 6, "BATTLESHIP", map.getPlayer(2), 0);
            map.spawnUnit(26, 6, "MECH", map.getPlayer(2), 0);
        }
        else if (turn === 13 && player === 2)
        {
            map.spawnUnit(26, 6, "FLAK", map.getPlayer(2), 0);
        }
        else if (turn === 14 && player === 2)
        {
            map.spawnUnit(24, 6, "BATTLESHIP", map.getPlayer(2), 0);
            map.spawnUnit(25, 6, "CRUISER", map.getPlayer(2), 0);
            map.spawnUnit(26, 6, "HEAVY_TANK", map.getPlayer(2), 0);
        }
        else if (turn === 16 && player === 2)
        {
            map.spawnUnit(26, 6, "NEOTANK", map.getPlayer(2), 0);
        }

        else if (turn === 18 && player === 2)
        {
            var unit = map.spawnUnit(24, 6, "CRUISER", map.getPlayer(2), 0);
            // make cruiser a hold position unit after spawn
            if (unit !== null)
            {
                unit.setAiMode(GameEnums.GameAi_Defensive);
            }
            map.spawnUnit(26, 6, "LIGHT_TANK", map.getPlayer(2), 0);
        }
        else if (turn === 19 && player === 2)
        {
            map.spawnUnit(26, 6, "FLAK", map.getPlayer(2), 0);
        }
        else if (turn > 19 && player === 2)
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
            var units = ["LIGHT_TANK", "FLAK", "ARTILLERY", "MECH", "HEAVY_TANK", "INFANTRY", "K_HELI", "T_HELI"];
            var index = globals.randInt(0, 7);
            var pos = globals.randInt(0, 2);
            map.spawnUnit(24 + pos, 6, units[index], map.getPlayer(2), 0);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("They... They've come this far? That must mean..."),
                    "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Adder..."),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Who's there? Who dares address me?!? Oh! Hawke! It's you."),
                    "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It appears that you're behind schedule. What've you been doing?"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I-I've been delayed. But only slightly. It's no problem. We'll be in control of this tiny country soon. You have my word."),
                    "co_adder", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you understand what it means that I am here, Adder?"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If-If I win... If I win here, there's no problem! That's correct, isn't it?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... IF you win."),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("At last, the enemy's factory? If we claim victory here, we can drive the enemy out of our homeland."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes! Let it begin!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! The leader of the Blue Moon Army to see you."),
                    "co_officier_yc", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's been a long time, Kanbei."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If it isn't Olaf! Why are you here?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I've brought along my men to lend Yellow Comet my aid. The Black Hole Army threatens us all. Blue Moon will stand beside you in this fight."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ah! Kanbei is grateful! With the power of Blue Moon beside us, we cannot lose!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

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
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

