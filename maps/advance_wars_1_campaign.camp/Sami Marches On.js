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
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("My, oh, my! You're much stronger than you used to be, aren't you?"),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Of course I am!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("But your basic abilities haven't really changed at all, have they? In an area with lots of properties to capture, your skills are adequate. But you're still poor at direct combat. In straight-up battle, you'd move your infantry in and seize the enemy HQ. That's what you're best at, isn't it?"),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ha... You're as good as ever at breaking everything down and analyzing it. But what are you doing, Sonja? Why did you attack us?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I can't... I can't tell you, yet. You'd better look over there."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh? That's Andy! How did you...?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I told him I had some new tools for him, and he followed me like a lost puppy."),
                        "co_sonja", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("(sigh)..."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("He should be waking up anytime now. Bye-bye."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Come back... She's gone. Great. Sonja... You'll pay for this!"),
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

        // set building limit to 16
        map.getGameRules().addVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        turnLimit.setRuleValue(16, 0);

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
                    qsTr("") + playername + qsTr("! I finally found you! Max! What are you doing sleeping? Wake up, you big lug!"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're wasting your breath. I've seen to it that he'll be asleep for quite some time."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You... Sonja! I might've known! Splitting the three of us up... This was all part of your strategy, wasn't it!"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Of course. Brilliant deduction. I've got Andy secured in a nice, safe place, too. If you want him back, you just have to get past me. The first person to secure sixteen properties on this map wins. Got it? Good. Ready, set, go!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Come back here!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This is just perfect! Set up by that little brat again! ") + playername + qsTr(", she didn't get you, too, did she?"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));

        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, good. That's a relief. We'd be lost if you got knocked out, too."),
                    "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));

        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, great. Fog of War... This is Sonja's forte. Well, we have to cross the water here, so we must protect our landers. We'll have no chance of victory if we can't transport our troops. Let's try and use the reefs for cover as we go."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I think Sonja's forgotten that I'm an infantry troop specialist, though."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" If we need to capture properties to win this, then it's as good as done! Get ready, Sonja! We're comin' for you!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
        dialog7.queueAnimation(dialog8);
        dialog8.queueAnimation(dialog9);
    };

    this.day1Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nice to see you again, Sonja"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Grit! Did you discover anything?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nope. Not a darn thing. I was able to follow His Beardedness, but... I think Olaf himself has been tossed by the wayside."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh..."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Maybe Olaf'll realize that he's been used. That'd be nice. He ain't such a bad guy, you know? Let's move on to somethin' else now, Sonja. Don't you think you're taking things a tad bit too far? There's a whole mess of folks after you, darlin'. Like all of Orange Star."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Really? Actually, I don't think I've done enough. This is invaluable intel that I'm gathering. I must continue!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, if you're that determined, I guess I ain't nothin' else to say. I'll go see what else I can find out."),
                    "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK. Thanks, Grit!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

