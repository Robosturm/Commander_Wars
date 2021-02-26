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
                        qsTr("How... What happened? My Md tank brigade... Routed? Hsss! Retreat! "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grit! Where are you, Grit? Forcing that villain Adder to retreat, it was quite... "),
                        "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hm? What was that? Something I can do for you, Frosty? "),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You... you impertinent fool! Is this the way you speak to your commander? Stand right there! I'm going to teach you a lesson! "),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Not in front of the boy. Hey, son, I'm gonna catch some shut-eye. You're in charge. "),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Um... Huh?! Me? Did you mean me? "),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Aarrgh! Grit! Hold it right there! "),
                        "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
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
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit! Grit! Curses, man! What are you doing?! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uhh? Who... What's all the ruckus? Even the chickens are still sleepin'. Oh, Olaf... Yaaaaawwwnn! "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Open your eyes and get up, you lazy scoundrel! I entrusted the defense of these mountains to you! Who do you think you are, lollygagging about like... "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Calm down there, O Bearded One. No need to get all riled up. If things get a bit dicey, we can always sound the retreat. I doubt the enemy's gonna do anything too rash. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You fool! Let me tell you something... "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? What is it? Don't attempt to divert my wrath with that dour look of yours! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I do believe that someone's payin' us a visit. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Shoot... Will you look at all them Md tanks! Goin' head-to-head with those boys makes no sense at all. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Very perceptive, Commander Grit. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Black Hole Army, huh? "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My name is Adder. I came here specifically to offer you a proposition. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A proposition? "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes. Let me cut to the chase. We'd like you to join the Black Hole Army. What do you say? "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Pardon?"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You possess much skill... Wasting it for Blue Moon is such a shame. You don't really buy into the whole 'protect and serve' business, right? Dying to save a country you care nothing for? Admit it. It's nonsense. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You can see with your own eyes, the might of our army. We passed through several cities on our way here... Heh heh... Crushing them beneath our boots took no time at all. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Be it Blue Moon or any other country, no one can stop us. Come, Grit. You know that joining us is the smart thing to do. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well now... You got one thing right. I never did care much which territory belonged to which country. I couldn't have cared less if y'all decided to set up shop here. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh heh... In that case... "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("However... "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm? "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("However... You've got no call to be treating the folks who live here any way you please. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What?!? "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You destroyed whole cities? You did just say that, right? Well, listen here, I don't care how powerful you say you are... You, Adder, are now my number-one enemy. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh... If that's the way you want it, I'll destroy you here as well! Md tanks, advance! Grind this fool to dust! "),
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
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? What're those things on the right of the map? Hey, Olaf! What in tarnation are they?"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Those are missile silos. Infantry and mech units can use them to launch a single missile. They can strike anything on the field of battle. "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well I'll be... A distance weapon... That's nice. I wonder which of us has the better range of fire. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Fool! It's a weapon, not a rival! Those missiles have a wide blast radius, so watch for your own troops."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Zippity doo dah! "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

