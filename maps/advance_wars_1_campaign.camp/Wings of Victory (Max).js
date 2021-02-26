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
                        qsTr("Now, I want some answers! Why're you hounding Andy?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You demand an answer. Very well. Here's your answer, Max. Your compatriot, Andy, viciously attacked my homeland."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The look on his face during the assault... I'll never forget! Never!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! Hold up a minute, Eagle. There's no way Andy would do something like that. C'mon! He couldn't have planned all that out. He's just a kid!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Um... Excuse me?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What I mean is, he does things for a reason. And there's no reason for him to attack Green Earth. He didn't do it!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hmm... Very well. I'll take you at your word... for the moment. But don't believe for a moment that I trust any of you."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Andy, if I find that you're lying to me, you'll curse the day you were born!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
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
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Aye, like I said, that's a rough opponent. Doesn't this old sea dog deserve some smooth sailing now and then?"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Drake! Listen... About our earlier conversation..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ahoy there, Eagle. It's time for you to test the waters. Battle Orange Star yourself and see if my theory holds any water."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I think you're right."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah, those are Eagle's boys. It's just like him to deploy such a well-balanced battalion. But the key here is a strong frontal assault, and at that, I'm the king!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You know, I gotta find out what his beef with Andy's all about. Yeah, I owe it to the kid to get this guy off his back. OK, Paul, let's do this!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };

    this.day1Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Despite Drake's conviction, I find it hard to credit his hypothesis."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

