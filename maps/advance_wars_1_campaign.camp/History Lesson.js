var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team, map)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("OK, Grit! Now you're going to answer some questions! Why in blazes did you go over to Blue Moon?"),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Grit!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr(" Nell? What are you doing here?"),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));

            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Hey there, Nell. It's been a long time."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Max. Would you please leave us alone for a minute?"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yeah... Sure..."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("You look well. I was worried."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yeah, I'm doin' all right. How about you? You and Max getting along all right?"),
                        "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("So that's the reason you left Orange Star. You sensed Max's feelings, and because he's your friend, you..."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Whoa! Hold on, now. Let me set you straight. I did what I did 'cause that's who I am, darlin'. I go where I want to go, and stay where I want to stay. That's it."),
                        "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Oh, I..."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("See you later, Nell. Take care."),
                        "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Grit... Goodbye."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
        }
    };
    this.gameStart = function(map)
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
                    qsTr("It looks like we'll be facing Grit next."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Grit... Is he the guy who keeps calling me junior? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Do you know him or something?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yeah, you could say that. He, Nell and I used to be friends. He was always a tough one to figure out, but this Blue Moon stuff..."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("You get right to the point, don't you, pal? I'm the king of direct combat. Need someone to mix it up? I'm the best! When I really get rolling, Max Force boosts my firepower even higher."),
                    "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Nell knows him, too? Why is he working for Blue Moon?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("That's something I'd like to ask him myself."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("That's why you've got to let me face him, Andy!"),
                    "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Reefs here, woods there, hiding places everywhere..."),
                    "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));

        // doing the queueing of the dialog
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
        dialog7.queueAnimation(dialog8);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

