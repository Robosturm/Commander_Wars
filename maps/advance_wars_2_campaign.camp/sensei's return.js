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
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You... You're an old man! For you to have defeated me... Who... Who are you? "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sensei! You did it! You are victorious! "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" Hm? I am? Oh, that's nice. "),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wait! Now I remember... It was in the reports... This country once boasted a CO who was said to be unbeatable... A man whom everyone feared... Impossible... It couldn't be! "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who, me? "),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Uh... Today was... unlucky. A bad day. We'll meet again, old... This is not finished! "),
                        "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Seemed to have pulled through in one piece. Sonja? Are you all right? "),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sensei? You were unbeatable? Everyone... feared you? "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who, me? Oh, that's so long ago, I'd almost forgotten. There might have been a rumor of that nature at some point, but... Come now, there are battles yet to be fought. Let's go. "),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes... Yes, sir!"),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
            dialog9.queueAnimation(dialog10);
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
        var ycList = campaignScript.getYCBuildList();
        map.getPlayer(0).setBuildList(ycList);
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
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Where are the supplies we've plundered? "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("They're en route to our factory even as we speak, sir! "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So quickly? Excellent... Heh heh. Our plans are moving right on schedule, eh? Even so, what a beautiful sea. Gazing upon it fills me with an overwhelming desire to see it all...... destroyed! "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We're under attack! It's Yellow Comet!"),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So much for 'me time' and tender thoughts... Send out the order for reinforcements! Prepare for battle! "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This is difficult terrain... The land routes are so narrow. Even if we move in numbers, we're sure to take heavy casualties. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... I think I'll take a crack at it. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sensei? "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Just sit back and watch. We'll see if my air units can't do some good. Even us old folks like to help out now and again. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What have we here? Have they run out of beds in Yellow Comet's infirmaries? "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So you're the Black Hole, eh? What do you want here? "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I have no reason to tell you, old man. In time, the entire world will kneel at our feet. Any who dare stand in our way will be ground beneath our heels. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, can't have that, now can we? I don't like war—it's tiring. But someone's got to stop you. Let me tell you a little secret. Did you know that when you join two damaged units together, and their combined HP is over 10, the surplus gets converted into funds? "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are you babbling about? Do you think some loose change is going to sway the course of battle? Heh heh heh... Or are you just worried about your pension? "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" If you don't grasp what I'm telling you, boy, then you've got no hope of defeating me. He who laughs last laughs best. So long. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" You're quite the orator. Still, you're a bit long in the tooth to challenge me. Since you're so tired, I'll make this quick and finish you in 10 days! "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
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
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

