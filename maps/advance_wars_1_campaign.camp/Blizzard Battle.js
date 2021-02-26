var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
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
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        // set building limit to 12
        map.getGameRules().addVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        turnLimit.setRuleValue(12, 0);

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
                    qsTr("Blast! We're running out of opportunities."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I don't see the need to get so worked up. It's not like this was Blue Moon property to begin with."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What did you say? After all my work and planning?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's common theft, man! Nothing more, nothing less."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Why, you insolent little..."),
                    "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Whoa! Hold on chief! Don't you think it's time to move out? Like you said, the Orange Star troops are knocking on our door."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit! You're pushing your luck! Don't go anywhere, you understand? I'll deal with you when I get back."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    playername + qsTr("! Andy! Max! Can you hear me?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What is it, Nell?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I've learned that this is a vital deployment point for Olaf. If we can drive him from the region, we may just cripple his army!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Really? This may be where we can end this conflict with Olaf!"),
                    "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's right! You just need to capture as much property as possible, and fast! The first army to secure twelve properties will win this battle! Stay alert and good luck!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's see... Yep, we've got bases for deploying troops, and..."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK! Understood. Let's go!"),
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
        dialog11.queueAnimation(dialog12);
        dialog12.queueAnimation(dialog13);
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

