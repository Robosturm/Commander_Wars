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
            var playername = settings.getUsername();
            // called when a player wins
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I, Eagle, beaten by Andy again?"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who are you calling Andy?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Eh?"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I think I'll take offense at your mistake. Do I look like a boy to you?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who are you?"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The name's Sami. I'm an Orange Star CO."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I... I lost to... a girl?"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You're careless. That's why you keep losing! I don't think you're ready for war. You need to go back to basic training!"),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Excuse me, Orange Star CO... Sami?"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes?"),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I would like to apologize for my behavior. It was inappropriate."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Oh, OK..."),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I vow by my lucky goggles that the next time we meet, I will do better."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Now I will take my leave. Farewell."),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wait, wait a minute! No one said you could leave. Come... He's gone!"),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("He seems like a pretty decent guy, after all."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sorry I'm late. Andy, Max... and special advisor... ") + playername + qsTr(", wasn't it?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Who are you?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My name is Sami. I'm the Special Forces commander. I was supposed to join you earlier, but my last mission ran long. Well, better late than never, right? I'm an infantry and mech unit specialist. My units can seize and secure new bases faster than any other CO's. My transport units are top of the line, too."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't hesitate to deploy me and my troops as you see fit. I'm ready for action!"),
                    "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Looking forward to working with you, Sami!"),
                    "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's the spirit! Oh, I've got some information, too. Word is that Olaf's forces are marching toward Yellow Comet."),
                    "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's go get 'em! ...Wait a minute... who's that?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... I think I've seen those guys before. Do you remember them, ") + playername + qsTr("?"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Those look like Green Earth troops."),
                    "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I've heard they have an independent battalion around. Maybe this is it. They're supposed to be really tough. Especially the air units."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Outstanding! ") + playername + qsTr(", Sami, you both know your stuff!"),
                    "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But how did you get this intel, Andy?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I ran into 'em once, and..."),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey, you survived, didn't you? That's pretty impressive, Andy."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Even with ") + playername + qsTr("'s help, that's good work! Way to go, kid! All right, let's roll!"),
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
    };

    this.day1Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The Orange Star Army... Andy's troops. "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This time, I won't be beaten by Andy and his ilk. I shall crush them with one stroke!"),
                    "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));

        dialog0.queueAnimation(dialog1);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

