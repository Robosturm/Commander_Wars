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
            var playername = globals.getSettings().getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grrrr... Not again!"),
                        "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("He will not be pleased with me if this losing streak continues."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grit!"),
                        "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yep."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Get out there and stop that cursed Orange Star Army!"),
                        "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Easier said than done, Boss. You didn't leave me anything to work with."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't get saucy! Rally the troops and do something! I have to go and pay him my respects."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't fail me, Grit!"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well, I'll be a... Olaf just tucked his tail between his legs and ran away. Where does that leave me? Guess I'll mosey on down and clean up his mess."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
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
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar); // no fog of war or GameEnums.Fog_OfWar -> for on
        // map.getGameRules().changeWeatherChance("WEATHER_1SUN", 90); // sets the weather chance of sun to 90. The actual chance is the the value divided through the sum of all chances
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player

        var list = campaignScript.getBasicBuildList();
        map.getPlayer(0).setBuildList(list);
        map.getPlayer(1).setBuildList(list);
    };
    this.actionDone = function()
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
        var playername = globals.getSettings().getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("By Neptune's beard, that was a rough battle!"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Come on, Drake! We'll be beaten again if you don't get moving!"),
                    "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Quit your jabbering and look around, Eagle. Only fools sail onto uncharted waters blindfolded."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are you talking ab...?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Orange Star did not attack Green Earth."),
                    "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But, we saw..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We saw what we were meant to see! And this goes beyond the battles in Green Earth. This entire conflict has been orchestrated from the beginning."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But how..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Quiet! Now, if I'm right, we'll have to turn the tides on our mystery foe. The best way to test my theory is to battle Orange Star again."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You mean... that wasn't Andy?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Paul! I have a mission for you and Max!"),
                    "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A mission for me? You must need some serious smashin' done. Well, if that's the case, you know I'm ready to go!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's not that simple, Max. There's a catch. The main body of the Green Earth Army is closing in on this position."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? The entire army...?"),
                    "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));


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
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

