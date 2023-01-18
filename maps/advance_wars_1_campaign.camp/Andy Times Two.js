var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediately without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.victory = function(team, map)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("We've won... for the time being."),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Here they come again!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Cripes! There's no end in sight!"),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("You don't have time to waste around here. Leave this to me and go!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I'm staying, too!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("No, Sami! You have to go as well!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("No! Even you can't stand against this horde!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("You have no idea how powerful a foe awaits your friends. They can't face him alone! Go, Sami! Go!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I understand. Watch your back!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Ha! They'll soon find Eagle's no easy prey!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
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
        map.getPlayer(2).setBuildList(list);
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
        else if (turn === 1 && player === 1)
        {
            gameScript.day1Dialog(map);
        }
        else if (turn === 1 && player === 2)
        {
            gameScript.day1BHDialog(map);
        }
    };

    this.initDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Who is that?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Olaf's invasion of Orange Star is where all of this began. During the attack, Andy was seen leading sorties on several countries. You saw him yourself, didn't you? So... doesn't all this strike you as exceedingly odd?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well, now that you mention it..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("You see what I'm getting at? It would've been impossible for Orange Star to launch multiple offensives while trying to repel Olaf's attack. That's the anomaly I wanted to show you."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Impossible! I'm getting angrier by the moment!"),
                    "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("It seems that our mystery foe has the ability to create human clones. I think it's safe to say that we've encountered several copies of Andy."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Blast! This is terrible!"),
                    "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I have some more stones to overturn, so I'll be going. What are your plans?"),
                    "co_sonja", GameEnums.COMood_Sad, PLAYER.getDefaultColor(3));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I won't leave this madman on the loose! I'll find him and crush him!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("It's up to you then, Eagle! Hail the Prince of the Skies! Good luck!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Eagle! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Andy. Please accept my apologies. I'm sorry I doubted you."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What the...? It's another me!"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Ah, yes. That's the clone of you our enigmatic foe somehow constructed. Starting with Green Earth, he's attacked several countries."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Are you serious?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr(" I've caused a lot of trouble for you, but I'd like to battle by your side. The green units are mine! Let me handle all of the enemy air units. It'll give you time to push straight ahead and storm the foe's HQ."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
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


    this.day1Dialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I'll make him suffer for fooling eagle."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
    };
    this.day1BHDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Maelstrom come... All who oppose me..."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

