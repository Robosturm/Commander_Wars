var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediately without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team, map)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I lost again. You're a tough one."),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("That's 'cause it's not just me! ") + playername + qsTr("'s my strategic planner!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Oh, that's right. Orange Star is using strategic advisors. That way, the commander can just concentrate on the combat itself. That's an impressive system. It's why I couldn't match you in battle."),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I don't know why, but when we work together, we're really strong!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yes, indeed. This was your day in the sun. But..."),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Hey! Let's fight again some day."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yes, let's do that. Until we meet again, Andy. See you again, ") + playername + qsTr(". Next time, I won't lose!"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
        }
    };
    this.gameStart = function(map)
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
    this.actionDone = function(action, map)
    {

    };

    this.turnStart = function(turn, player, map)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog(map);
        }
    };

    this.initDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Andy! I wanted to face youz one more time. Let's go!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I'm ready!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("There aren't many units here, are there?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("That's right. Do you know what to do?"),
                    "co_nell", GameEnums.COMood_Hapy, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Sure! I just use these bases to deploy whatever troops I want!"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I guess there's nothing left for me to teach you."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

