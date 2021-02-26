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
                        qsTr("Howdy Max. Long time, no see."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grit? Is that you? I was wondering why I hadn't seen you around lately, and now I know. You're working for Blue Moon! You traitorous piece of gar--"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Aww, Maxie. Don't be like that. I got my reasons for doin' what I do, you know?"),
                        "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Man, I'm getting out of here before you get started preachin' again. See y'all down the road!"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hold it, Grit! Dang! He got away. But what's he doing with Blue Moon?"),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
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
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit! Where is that fool?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm right here. What are you bellowing about now?"),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Bellowing? Let me tell you something, you..."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My apologies, exalted commander. I am at your service."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's better. You just remember your place. Now, listen closely. Those units we saw earlier have advanced further into our territory. This is your chance to redeem yourself. Do what needs to be done."),
                    "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Riiight. So, I should wait here and..."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Wait? What do you mean wait? ") + playername + qsTr("'s troops are..."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, OK. Understood. If duty calls, your humble servant Grit'll answer!"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If I didn't know better, I might think you weren't taking this seriously."),
                    "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Fog of War..."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What's 'Fog of War'?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Please tell me you're joking."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("See that gray stuff blocking your vision? That's Fog of War."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now, I wonder where the enemy troops are hidden."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How do we find 'em?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, recon units have a good vision range, so let's send them out first. But if we leave them out in the open, they'll be sitting ducks. Our safest bet is to leapfrog from wood to wood. Units hidden in woods are almost impossible to spot."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Boy, that seems pretty simple."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You just don't know enough to realize the danger, Andy. If we can hide units in the woods, then so can our opponent. If we're not careful, hidden units will blow us away! All right. Let's move out!"),
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
        dialog15.queueAnimation(dialog16);
        dialog16.queueAnimation(dialog17);
    };

    this.initDialog = function()
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, here I am again."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This whole thing don't sit well with me, but I guess I gotta stop foolin' around. I figure the best way to stop these boys is to take out their lander. Yeah, that'll keep them from movin' those ground troops around. Reckon I'd best get started."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog0.queueAnimation(dialog1);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

