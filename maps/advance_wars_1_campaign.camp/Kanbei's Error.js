var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
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
                        qsTr("By my sword! What a horribly useless base that was! And I was defeated in such a short time!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Was that base in a poor location?"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            dialog1.queueAnimation(dialog2);
            if (map.getCurrentDay() <= 12)
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

        // set building limit to 15
        map.getGameRules().addVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_BUILDINGLIMIT");
        turnLimit.setRuleValue(15, 0);

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
            gameScript.initDialog(mapmap);
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.day1Dialog(map);
        }
    };

    this.initDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("They're pretty good, aren't they? You can go on and on about their superior number of bases, but they really just outlasted you, Father."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("But Sonja..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Father! Stop that whining! You're the Yellow Comet commander for goodness' sake!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yes, you're right... I am. What should I do?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well, first of all, we obviously need to match their number of bases."),
                    "co_sonja", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Of course! Kanbei must have bases, too!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yes, but we also..."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("It is decided! Kanbei shall secure bases near the enemy!"),
                    "co_kanbei", GameEnums.COMood_Sad, PLAYER.getDefaultColor(3));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Father? Ohhhh... gone again. Doesn't he realize that the proper location of bases is just as important as numbers?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Kanbei's got us surrounded!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("On top of that, we have to seize properties to win. Looks like things are gonna get rough."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("But look! This time we have an airport!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What's an airport again?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Airports let us deploy air units! Plus, damaged planes and copters can go there to recover HP."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Oh, OK. Got it! That's cool, but isn't that Kanbei's base right there?"),
                    "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Don't worry about that one. Take a close look. That base deploys ground units, right? Well, it's out in the middle of nowhere. It's completely useless! Let's ignore that one and Capture some other bases and cities. We need to Capture fifteen properties to win."),
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
        dialog12.queueAnimation(dialog13);
        dialog13.queueAnimation(dialog14);
        dialog14.queueAnimation(dialog15);
    };

    this.day1Dialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Oh, ho ho ho!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Look what I have found! A base for Kanbei's troops!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I shall deploy a mountain of troops!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

