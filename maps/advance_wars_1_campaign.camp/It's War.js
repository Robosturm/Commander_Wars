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
                        qsTr("I lost again! Who in the world are you?"),
                        "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("My name's Andy! I'm the CO. This is my advisor, ") + playername + qsTr("."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Advisor? We needed no advisors when I was in the Orange Star Army! Hrumph!"),
                        "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Be that as it may. ") + playername + qsTr("! Andy! It takes a lot to best me. I'll remember your names. Mark my word!"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey, grandpa. Who are you?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who am I, you ask? Throughout Orange Star I was known as..."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Nell's papa?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" That's right, Nell's... What? No! That's what I get for being nice to children! No respect! Grrr... You just watch yourself, little boy!"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
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
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Really? Continents? Oh right, yeah, I forgot! Sorry."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Orange Star Forces? Here so soon? But, wait... something's not right here. Nell wouldn't leave Orange Star's capital city unguarded. Which means... A new commanding officer? Ha, ha, ha! Excellent! I've lost a few battles lately, but no more! An Orange Star Army without Nell is no match for me! Time to teach this raw recruit what war's all about!"),
                    "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));

        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("") + playername + qsTr("! Andy! Do you read me?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nell? Is that you? Is this a transceiver?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's right, Andy. I'll be using it to contact you from here on out. As this is your first real command, I wanted to check up on you."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No worries here! This is going to be easy!"),
                    "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, Andy! You're nothing if not enthusiastic. Let me give you a few words of advice. First, let's talk about your CO Power."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" You can use your CO Power when the Power Meter is full. The meter fills up gradually over the course of battle. Once it's full, you... well, I'll cover that when the time comes. Oh, I also have to tell you about the Terms of Victory. The Terms of Victory explain what you need to accomplish to fulfill a mission. Do you see Intel on the Map Menu? Select it and you'll see three new menu items: Terms, Status, and CO. If you select Terms, I'll explain what you must do to win on that map. You should also take a look at the other two items when you have the time. There's a lot of useful intel in there."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

