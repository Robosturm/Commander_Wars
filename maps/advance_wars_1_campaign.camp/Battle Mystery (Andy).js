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
                        qsTr("Arrgh, you got me! I didn't think you'd find me here, and I let my guard down. "),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander Drake, would you please tell us why you've been attacking us?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Haven't you sensed him? The person behind all of this fighting? Sonja and Grit have been trying to lure him out of hiding."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The person behind all this?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We've been trying to keep Sonja's movements a secret. So, we staged these battles to draw his attention away from her. But I think we're finished now. It seems Sonja's found something. I'll be joining her as soon as I'm able, but... I've got things to take care of in Green Earth. This is a difficult thing from me to ask..."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You don't have to! We'll help in any way we can!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);

            var campaignVariables = map.getCampaign().getVariables();
            var drakeCounter = campaignVariables.createVariable("drakeCounter");
            drakeCounter.writeDataInt32(drakeCounter.readDataInt32() + 1);
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
        var lander = map.getTerrain(11, 12).getUnit();
        if (lander === null)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
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
                    qsTr("") + playername + qsTr("! Andy! Can you hear me?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What is it this time, Nell?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We've been able to outsmart CO Drake! One of his landers is carrying vital cargo, and it's hiding in this area. It's up to you to flush it out and destroy it!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What is it this time, Nell?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Got it! I just need to find the lander and sink it, right?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's it! I think it's safe to assume that Drake's hiding in the reefs somewhere. Go find him!"),
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
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Barnacles! I was so worried about Eagle that I got trapped... Ahh, I'll think of something."),
                    "co_drake", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
    };

};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

