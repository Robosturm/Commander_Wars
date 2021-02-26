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
        return qsTr("Survive for 10 rounds without loosing your missile.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Curses! I've been defeated. Time to hoist sail and flee!"),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));

            var campaignVariables = map.getCampaign().getVariables();
            var drakeCounter = campaignVariables.createVariable("drakeCounter");
            drakeCounter.writeDataInt32(drakeCounter.readDataInt32() + 1);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player

        var list = campaignScript.getBasicBuildList();
        map.getPlayer(0).setBuildList(list);
        map.getPlayer(1).setBuildList(list);
    };
    this.actionDone = function(action)
    {
        var count = map.getPlayer(0).getUnitCount("MISSILE");
        if (count === 0)
        {
            map.getPlayer(0).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 10 && player === 0)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("By Neptune's beard, that was a rough battle!"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Come on, Drake! We'll be beaten again if you don't get moving!"),
                    "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Quit your jabbering and look around, Eagle. Only fools sail onto uncharted waters blindfolded."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are you talking ab...?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Orange Star did not attack Green Earth."),
                    "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But, we saw..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We saw what we were meant to see! And this goes beyond the battles in Green Earth. This entire conflict has been orchestrated from the beginning."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But how..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Quiet! Now, if I'm right, we'll have to turn the tides on our mystery foe. The best way to test my theory is to battle Orange Star again."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You mean... that wasn't Andy?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("") + playername + qsTr("! Andy! This is Nell! Can you read me?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What's wrong?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("CO Drake has hit us again! We need you to come in and protect this missile unit. It's vital to Orange Star's strategic success. Watch out for the enemy battleship to the south; it's moving in to attack."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I understand! I just have to protect this missile unit, right?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You've got it. Don't worry about the cities or your HQ. Ten days, Andy! If you can keep that unit safe for ten days, I'll take care of everything else. Don't let us down, ") + playername + qsTr("!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

