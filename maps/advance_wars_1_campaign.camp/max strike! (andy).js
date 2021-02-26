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
                        qsTr("See? I told you!"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! Not bad, kid. Not bad at all. 'Course, I woulda finished that up a bit quicker, but..."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What did you say?"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));

            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh? You wanna piece of me?"),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));

            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Knock it off, both of you! Max, this is hardly the mature behaviour I was expecting from you!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" But he started..."),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Max!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hee, hee!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" If you keep this up, you're going to have your heads handed to you!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hooray!"),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
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
                    qsTr("Sorry to keep you waiting!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Who are you?"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm Max, a CO in the Orange Star army. I was ordered to join you and ") + playername + qsTr(", so here I am."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A new CO, huh? What's so special about you?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You get right to the point, don't you, pal? I'm the king of direct combat. Need someone to mix it up? I'm the best! When I really get rolling, Max Force boosts my firepower even higher."),
                    "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Cool!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I want to check this place out!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Cripes! I can't believe I have to be nanny to this little kid..."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm not a little kid! I'm a CO in the Orange Star Army! And I don't need a nanny!... Whatever that is."),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uh-huh. OK, little man, why don't you tell me how to use the bases on this map?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No problem! You use the naval base to deploy naval units and the air base to deploy air units!"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Not bad! I'm surprised you know that much! But if a CO doesn't have guts, he's got nothin'. Show me what you can do, kid."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };

    this.day1Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ha ha ha ha ha! Behold! Gaze upon my beautiful army!"),
                    "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Orange Star may match me in numbers, but I have so much more power!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But you're not very well balanced... What if the enemy send its subs under? That'll stop you cold."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Battleships at sea! Md Tanks on land! Is there anything more I need? No! Nothing at all! Attack! Attack! Attack!"),
                    "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, as long as you're happy, Boss."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

