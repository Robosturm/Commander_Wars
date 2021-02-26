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
        return qsTr("Survive for 5 rounds.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ya...Yahoo!"),
                        "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You're awful beat up to be celebratin'."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well..."),
                        "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yeah, exactly. You know, Maxie, you never were any good at indirect combat. Why, if you'd have just used those big boys a little more, you wouldn't have taken such a poundin'."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));

            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Um..."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("And long as I'm givin' you what-for, what's up with you and the kid? He ain't got your particular shortcomings, so give him a break! He's pretty darn good, and you know it. C'mon, Maxie, what do you say?"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yeah... I guess you're right. Hey! Andy! Sorry for being such a jerk."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh? As long as you're cool, I'm cool. Hey, Grit!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What's up, Junior?"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));

            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("How many times do I have to tell you to stop calling me Junior?"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Whoa! I hear you, I hear you. My apologies. Well now, look at the time. Gotta go, boys! See y'all later! Take care, Junior."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("...."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

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
        else if (turn === 6 && player === 0)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Junior!"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Stop calling me Junior!"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're just a regular fireball, ain't you? By the way, how are you and Big Maxie getting along?"),
                    "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit? In the Blue Moon Army?"),
                    "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yep. Things happen, you know. Blue Moon helped me out of some trouble, and I always repay my debts."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But that's neither here nor there, big fella. You must be getting old if they've sent you out to test Junior here."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? Why you..."),
                    "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I tell you what, why don't I just lend you a hand testing him out? You boys survive the next five days and you'll know what he's made of. Think you can take me, old dog? Bring it on boys!"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit, you're one arrogant piece of work!"),
                    "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Blast! Grit's troops are all around me! If I stay put, I'll be blasted to smithereens. If I retreat, our HQ will be captured... How am I gonna get out of this?"),
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
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

