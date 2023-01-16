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
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("This is beyond comprehension!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Regardless of the odds, Kanbei should never lose this many units! Withdraw!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            dialog1.queueAnimation(dialog2);

            if (map.getCurrentDay() <= 10)
            {
                var campaignVariables = map.getCampaign().getVariables();
                var sonjaCounter = campaignVariables.createVariable("sonjaCounter");
                sonjaCounter.writeDataInt32(sonjaCounter.readDataInt32() + 1);
            }
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
                    qsTr("What shall we do, Sonja?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well, what are the current battle conditions, Father?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I asked about the conditions, Father."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well, I... Uhm..."),
                    "co_kanbei", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Oh, Father. This is so typical of you. Gathering thorough intel is the key to victory! What were you planning to do without even basic information?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Basic information... well, yes..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Hopeless. Absolutely hopeless. I suppose I'll share my intel with you, Father."),
                    "co_sonja", GameEnums.COMood_Sad, PLAYER.getDefaultColor(3));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("The enemy troops are deployed here. It doesn't appear to be a very large force."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Is that so? Then I'll just crush them with superior numbers!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well, yes. You could do that. But first..."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Wait! Wait! Father! You've got to remember to deploy in an area with secure bases. If the enemy seizes them before you arrive... Father? He's gone. Oh, Father. What are you trying to prove?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));


        var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Hey! Where's the rest of the army?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I'm sorry, Andy. There was an error, and preparations weren't completed. But there are bases for deployment. You know how to use them, right?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr(" Sure, I'm OK!"),
                    "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well, then, there's no problem! I'm counting on you, Andy! You too, ") + playername + qsTr("!"),
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
        dialog14.queueAnimation(dialog15);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

