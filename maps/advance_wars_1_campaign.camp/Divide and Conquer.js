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
                        qsTr("Max, you have underdeveloped indirect-combat skills. You compensate for this weakness with a predilection for direct combat. In areas where you can attack with multiple units, you're a titan."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wha...?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("In tight quarters, though, you tend to become a target for indirect fire. Try pulling your foe out into wider terrain: you'll like the results. Or is strategy your department, ") + playername + qsTr("?"),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who... Who are you?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's a pleasure to meet you. My name is Sonja. I'm a Yellow Comet CO. I've taken the liberty of separating you from your companions. So, now..."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! (yawn) What the...? So... sleepy... Zzz..."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sweet dreams, Max."),
                        "co_sonja", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
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
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father, you're impossible! Do you know why this keeps happening?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You never let me finish what I'm trying to tell you!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uh..."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I will admit that your Imperial Forces don't generally lose, but... Let's just blame the enemy for being too strong this time."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmm..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This time, I'll take command."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sonja! Kanbei will not allow his daughter to be placed in harm's way! Why, if the unthinkable were to happen..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father, you hush! There's something I need to investigate. And, I need first-hand intel to formulate a strategy."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But, but..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ok, Father, take it easy. How's this? At the first sign of danger, I'll retreat. Ok?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You must promise to avoid unnecessary risks. Understood?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Of course, Father. I'll be back."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Where's is everybody?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sami? Andy?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey, ") + playername + qsTr("! I can't find Andy or Sami! The two of them have disappeared!"),
                    "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? What the...? I'm surrounded! Are those Yellow Comet troops? Man, I've been set up!"),
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
        dialog13.queueAnimation(dialog14);
        dialog14.queueAnimation(dialog15);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

