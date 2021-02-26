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
        return qsTr("Survive for 14 days.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! You OK, son?"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grit!"),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Tsk! Time's up. All I'll gain now are more casualties. Guess it's time to disappear! Toodles!"),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Nice work, son. Holding out against a superior force that, I mean."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));            
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander Grit? Does this mean I'm worthy of being a commanding officer now?"),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What're you talkin' about? You are a grade-A certified top-of-the-line CO, son! And you can take that to the bank!"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir!"),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
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
        var bmList = campaignScript.getBMBuildList();
        map.getPlayer(0).setBuildList(bmList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
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
        if (turn === 15 && player === 0)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Colin! You're in command now. Grit and I are returning to HQ to plan our continued strategy."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir! Understood, sir! I'll guard these properties for the next 14 days."),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Mmm. No need to worry. According to our intel division, there's no chance of the enemy striking here. You're one-hundred percent safe."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But our intel division ain't the most reliable bunch of fellas. Do your best, son. We'll be back to get ya in 14 days."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I won't let you down, sir! I'm a Blue Moon CO after all!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander COlin!"),
                    "co_officier_bm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes?"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's an attack! The Black Hole Army's here!"),
                    "co_officier_bm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? Just like that? Stay calm. Stay calm. Must not panic. I... must... be... strong! OK! Tighten the defenses!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Helloooooo! Is anybody home? Looks deserted! Guess I'll just capture everything."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Not so fast. These properties... are under... my protection."),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Just you? Talk about easy."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uh... Reinforcements are on their way!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah, like in 14 days, right? I'll capture everything by then. C'mon, troops! Fire at will! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Keep strong, everyone! We just have to protect the HQ for 14 days!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));

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

