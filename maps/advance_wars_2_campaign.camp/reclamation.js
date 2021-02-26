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
                        qsTr("Boo! Nothing's going right! That's enough. I'm going home! "),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Mis... Mistress Lash? Wait for us! "),
                        "co_officier_bh", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Did they really retreat? "),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No doubt about it. We've secured the enemy HQ, sir. "),
                        "co_officier_bm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I accomplished my mission! Wow, is that a relief! "),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Your command was... Audacious, sir. "),
                        "co_officier_bm", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Thank you. I'm glad you think so. I've just got to keep this up and drive Black Hole out as soon as possible. "),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
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
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var bmList = campaignScript.getBMBuildList();
        map.getPlayer(0).setBuildList(bmList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {

    };

    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What did you say? Black Hole's on our northern frontier? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir. They show every indication of attacking right away. "),
                    "co_officier_bm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm. But why? There's nothing of importance there. "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Maybe that's the reason. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What do you mean? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, there's nothing there, so maybe they figure the defenses are weak. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));        
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How should we respond? We could get there soon enough, but it may be a trap. We can't be careless. "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That might be part of their plan, too. Ha ha, that's downright crafty. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We're not here to admire them! Someone has to go... Me or Grit... "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'll go! "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You and Commander Grit should remain here, correct? Well, that leaves me! "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now there's a good idea. Let's send the youngster. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit, you lazy... The boy's as green as can be. "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That may be true, but he's been fulfilling his CO duties without a hitch. I say it's high time we start treating him like a full-fledged CO. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... I see what you're saying, but... "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's all settled then. Colin, head up north, and defeat the enemy! "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir! "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Easy, son. Relax a bit, and do what you can. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uh... and troop morale? If I'm not paying attention..."),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't mistake what I said now. Being relaxed and being careless aren't the same thing. The lives of your troops hang on the decisions you make out there. I know you're excited, and that's not bad. Just remember moderation. OK? "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sir! Yes, sir! Thank you, sir! "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Mistress Lash, Blue Moon troops have been spotted. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, it's about time. Who's in command? "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's... A young boy CO named Colin. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? That kid? How boring is that? I wanted the bearded geezer, or at least the scarecrow. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are your orders? Do you want the troops to attack? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's not going to be any fun of neither of 'em shows up. Sound the retreat. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog29 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Er... ma'am? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog30 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, OK. Let's just attack and wipe 'em all out. Tee hee hee! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
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
        dialog17.queueAnimation(dialog18);
        dialog18.queueAnimation(dialog19);
        dialog19.queueAnimation(dialog20);
        dialog20.queueAnimation(dialog21);
        dialog21.queueAnimation(dialog22);
        dialog22.queueAnimation(dialog23);
        dialog23.queueAnimation(dialog24);
        dialog24.queueAnimation(dialog25);
        dialog25.queueAnimation(dialog26);
        dialog26.queueAnimation(dialog27);
        dialog27.queueAnimation(dialog28);
        dialog28.queueAnimation(dialog29);
        dialog29.queueAnimation(dialog30);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

