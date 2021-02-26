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
                        qsTr("All right, Sonja! Now I want some answers! Why have you been attacking us?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("All in due time, Sami. But first, Andy."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You don't have any weak points."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well, of course not!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("But you don't have any special strengths, either."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I told him I had some new tools for him, and he followed me like a lost puppy."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ha, ha. But that in itself is your strength. Don't you think Andy's easy to work with, ") + playername + qsTr("? And if things start getting rough, Max and Sami are there to help out. All you need to do is have ") + playername + qsTr(" choose who fights each battle. If you do that, there's no one who can best you on the field of battle."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! Don't ignore me!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sami."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yeah?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Very soon you and your friends are going to face an unimaginable foe. Take care of ") + playername + qsTr(", and Andy, too."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("How do you...?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("That's not your concern. See you again, Sami."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! Hold it! She's gone again! She didn't really tell us a doggone thing!"),
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
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Andy! Are you okay?"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" ...hmm..."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Andy!"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));

        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm... so... groggy..."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Wake up!"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? Oh... morning, Sami."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Unbelievable! Here I am, working my tail off, and you're sleeping! Why am I the only one who's taking these things seriously?"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sonja's been leading us around by our noses like a bunch of sheep! Oooh...! She makes me so mad! Ever since we were classmates, she's been the Queen of Strategy."),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now here I am, losing units left and right, and my allies are all sleeping! Get up and get moving, Andy! It's up to you. You've got to go!"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I've got to battle Sonja?  I really don't want to."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If you mention that she gave you new tools again, I'm gonna thump you!"),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yikes!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you have any idea what I had to go through? New tools! Hah! Oh, and don't expect any sympathy from me if you lose!"),
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
        dialog9.queueAnimation(dialog10);
        dialog10.queueAnimation(dialog11);
        dialog11.queueAnimation(dialog12);
    };

    this.day1Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This is the last test... Show me what you do, Andy!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
    };

    this.day2Dialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Um, ") + playername + qsTr("? What's the deal with Sonja and Sami? Do they not get along?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Really? I wonder why Sami always gets so mad when she hears Sonja's name."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well anyway, let's do our best, ") + playername + qsTr("."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah, I thought so. In that case, who knows what'll happen if I lose."),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's do it, ") + playername + qsTr("! "),
                    "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

