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
            var playername = settings.getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("A job well done! You've beaten me."),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You're a Green Earth Co?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("That I am. I wanted to find some answers here, but all I've netted are more questions. I'll be seeing you again, lad."),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You were so close..."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);

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
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player

        // set building limit to 12
        map.getGameRules().addVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        turnLimit.setRuleValue(12, 0);

        var list = campaignScript.getBasicBuildList();
        map.getPlayer(0).setBuildList(list);
        map.getPlayer(1).setBuildList(list);
    };
    this.actionDone = function(action)
    {

    };

    this.turnStart = function(turn, player)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.day1Dialog();
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.day2Dialog();
        }
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Blow ye winds o' the mornin'! Blow ye winds hi ho."),
                    "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Drake! What are you doing all the way out here? You've got to get into position!"),
                    "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Morning, Eagle. Wouldn't you rather just sit and bask in the sun a while?"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you insane? The Orange Star Army is on the move!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, is that so?"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Is that so? Drake, are you..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Be cool - everything's under control. No need to rush. I've got everything set up. Now, doesn't this sun feel good? Oh... He's gone. Ah well. I guess I'd better get under way before Eagle gets his feathers any more ruffled."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Drake always finds some way to sidetrack me. He's absolutely peerless in naval combat, but sometimes I wonder..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("") + playername + qsTr("! Andy! Can you hear me?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Is that you, Nell? What is it?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We have an emergency! The Coral Fortress is being stormed by Drake of Green Earth! We've never had trouble with him before, so we didn't see them coming!"),
                    "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("There are a bunch of bad guys here!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This point is of vital importance to Orange Star. If the enemy seizes a lot of properties here, we'll lose control of the area. ou have to beat them to it and secure as many cities as possible. Including your bases and HQ, you must secure twelve properties to win."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Secure as many properties as possible? Ok, I got it!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("These bases are yours to use. ") + playername + qsTr(", deploy whatever units you see fit. I'm counting on you!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I've got it covered! Um... Hey, ") + playername + qsTr(" which units do we use to Capture with , again?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

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
        dialog14.queueAnimation(dialog15);
    };

    this.day1Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now's the time. I need to know if Orange Star was behind that blackhearted act. Anchors, away!"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
    };

    this.day2Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("By the way, what kind of CO is Drake?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I've heard that he's virtually unstoppable in naval combat. His armada is supposed to be a real powerhouse."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("There you go again, Sami! You're a warehouse of intel."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But I could swear there was something else... something big..."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

