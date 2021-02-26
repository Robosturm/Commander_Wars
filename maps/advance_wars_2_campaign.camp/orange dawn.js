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
                        qsTr("How's that, ape-man?!? Told you not to mess with me!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("...Hah!"),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hah? Don't hah me! You just got beaten!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("This was just a test. Next time is for real."),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Like I've never heard that before! I'm gonna make you regret ever showing your ugly mug around here!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Max, or whatever you said your name was, you're mine! Before this is over, I'll crush you like a bug. So long."),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wait! Wait, you dirtbag! Shoot! He got away!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Black Hole... Deplorable characters. All of them."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You said it!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("By the way, there's something else I wanted to talk to you about. After the battle, you'll see some points and a rank displayed, right? The points you can use in Battle Maps to buy things from Hachi. I'll explain how to get high points."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("OK! I definitely want to win with the highest ranking possible."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" I thought as much! You know, battling for higher rankings is fun when you've got free time. First, let me tell you what the scores all mean... Speed is the length of time you take to complete the mission. Next, Power is the number of enemy units you destroy. Finally, Technique counts the number of units you lost. However, victory is your top priority. Doing silly things to earn more points is not acceptable!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Points or no points, I'm always at full throttle! I won't stop! "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(true); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        var t_copter = map.getTerrain(8, 8).getUnit();
        var b_copter1 = map.getTerrain(8, 7).getUnit();
        var b_copter2 = map.getTerrain(7, 8).getUnit();
        var art = map.getTerrain(12, 8).getUnit();
        if (b_copter1 === null || b_copter1.getHasMoved() === true ||
            b_copter2 === null || b_copter2.getHasMoved() === true)
        {
            // read the tower variable or create it
            var b_copterDialog = variables.createVariable("b_copterDialog");
            if (b_copterDialog.readDataBool() === false)
            {
                gameScript.b_copterDialog();
                b_copterDialog.writeDataBool(true);
            }
        }
        if (art === null || art.getHasMoved() === true)
        {
            // read the tower variable or create it
            var artDialog = variables.createVariable("artDialog");
            if (artDialog.readDataBool() === false)
            {
                gameScript.artDialog();
                artDialog.writeDataBool(true);
            }
        }
        if (t_copter === null || t_copter.getHasMoved() === true)
        {
            // read the tower variable or create it
            var t_copterDialog = variables.createVariable("t_copterDialog");
            if (t_copterDialog.readDataBool() === false)
            {
                gameScript.t_copterDialog();
                t_copterDialog.writeDataBool(true);
            }
        }
    };

    this.b_copterDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What you had there is a battle copter. It's a direct-combat unit. Air units are easy to deploy because their movement isn't limited by terrain. Of course, they don't receive any defensive cover, either. B copter firepower is at least equal to a tank's... and in dogfights with other copters, they're as tough as can be."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("All right! I'm gonna knock some heads!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
    };

    this.t_copterDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's a transport copter, right? Like APCs, T copters can carry infantry and mech units. Another similarity is that they lack weapons. T copters cannot fire. On a map like this, where you need to circle around to reach the enemy HQ, T copters can shorten the distance. Even seas are no obstacle."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now that's useful!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, it is! I'll leave it to you on how best to utilize them. Loading and unloading with T copters is handled the same as with APCs. Not being able to drop units off in certain terrain is the same, too. One other thing: T copters can't unload or supply troops when flying over seas. Don't forget that."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };

    this.artDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We talked about this earlier, right? Your difficulty with indirect combat?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh... yeah... I thought we'd covered that."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Just hear me out, Max. To see your units' ranges of fire, press the B (default) or Right Mouse Button. In your case, the ranges are a little small, so make sure you take a look."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah, I know... You don't have to keep reminding me about it. But yeah, I'll admit it: knowing your weak points is important."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Wow! Max, you've come a long way. I didn't think I'd hear you say that."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Oh, well... Hey! It's about time to show these goons we mean business!"),
                            "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
    };

    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You know, this Max guy kinda reminds me of me... except... I'm me. Hmmm... This should be fun."),
                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Orange Star dogs! Can you hear me?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Um... Lord Flak... The microphone?"),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Shut up! Too complicated! Anyone there? Or are you too scared to face me?!?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What's with the racket? You lonely or something?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hah! Look! A gorilla!"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What did you say?!? If anyone here's a gorilla, it's you, ape-man!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm no ape! I'm Flak!!!"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Flak? What's that, some sort of code name? Listen up, Flak. My name's Max. Now tell me what you jerks are doin' rampaging through our country?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("YOUR country? Orange Star belongs to the Black Hole Army now! You complain, you get hurt!"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, you think so!? Give it your best shot! "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Max! Calm down."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nell?!?"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("To lose your head is to lose the fight. I taught you that, remember?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, but... I... It's just..."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I understand your anger, Max. No one likes seeing his homeland ravaged. All the more reason to proceed calmly and rationally. Use your head, and drive these fiends from our borders! OK?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You got it! Let's roll!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now, Max, I know that you're a combat veteran, but... medium tanks, battle copters, and transport copters are tough to handle if you're a bit rusty."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You deployed copters?!? Well, it has been a while since I've dealt with air units. But Md tanks?!? C'mon, it's a tank! Bigger and stronger, but still a tank!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK, I'll give you that one. In the knock-down-drag-out world of direct combat, you're the champ. When it comes to firing, air units act the same as ground units. I'll bet that your tank expertise makes you handy with B copters, too!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ha ha... yeah. You're probably right. In a head-to-head battle, I can't be beaten!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("However, indirect combat is another story, isn't it?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I'll admit that your power has overcome that particular obstacle, though."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You know it! Indirect-combat units? Who needs 'em?!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I can't fault your confidence. Try not to overdo it!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

