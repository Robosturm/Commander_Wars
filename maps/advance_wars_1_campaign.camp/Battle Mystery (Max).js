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
                        qsTr("Same old shanty! I just can't beat you guys."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander Drake, would you please tell us why you've been attacking us?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Haven't you sensed him? The person behind all of htis fighting? Sonja and Grit have tried to lure him out of hiding."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The person behind all this?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We've been trying to keep Sonja's movements a secret. So, we staged these battles to draw his attention away from her. But I think we're finished now. It seems Sonja's found something. I'll be joining her as soon as I'm able, but... I've got things to take care of in Green Earth. This is a difficult thing from me to ask..."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What is it?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I can't leave Green Earth unprotected. I promised Eagle. You understand? So... Would you go in my stead? They need your help."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Of course! We'll do all we can!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
                    qsTr("I saw his face! That had to be Andy, but...."),
                    "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hello, Eagle."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You... you're Sonja from Yellow Comet. What are you doing here?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It wasn't Andy who attacked you. It was someone else."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you know something about all of this?"),
                    "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, I do. I'll tell you who's pulling all of our strings if you'll follow me."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But I..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I know what you're worried about. Go ahead and go with her."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you sure it's OK?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'll take care of Green Earth for you, don't worry. That's what shipmates are for, right?"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You have my gratitude, Drake."),
                    "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I like the idea of getting a closer look at the Orange Star Army. I'll meet you again later. Watch yourself, Eagle!"),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Drake means boats! Just watching is fun!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I bet Drake has some reason for challenging us, just like Eagle."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If he's trying to find something out, I wish he'd just ask. But it's never that easy, is it? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Another mission that revolves around landers. Drake and his darn boats! It's safe to say that he's not going to go out of his way to make this easy. But again, I think he has his reasons."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah, you're probably right. Well, here I go!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I think they're ignoring me..."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

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
        dialog15.queueAnimation(dialog16);
        dialog16.queueAnimation(dialog17);
    };

    this.day1Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Time to make a big splasg abd give Eagle some cover."),
                    "co_drake", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
