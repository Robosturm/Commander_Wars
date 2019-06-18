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
        // map.getGameRules().changeWeatherChance("WEATHER_1SUN", 90); // sets the weather chance of sun to 90. The actual chance is the the value divided through the sum of all chances
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var ycList = campaignScript.getYCBuildList();
        map.getPlayer(0).setBuildList(ycList);
        campaignScript.setArmyData(1, campaignScript.getArmy(map.getPlayer(1).getCO(0)));
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(2).setBuildList(bhList);
        map.getPlayer(2).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function()
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

