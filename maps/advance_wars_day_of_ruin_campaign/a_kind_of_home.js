var Constructor = function() { // scriptStart
    this.immediateStart = function() { // immediateStart
        return true;
    }; // immediateStart
    this.getVictoryInfo = function() // victoryInfo
    {
        var variables = map.getGameScript().getVariables();
        var textData = variables.createVariable("victory_info");
        return textData.readDataString();
    }; // victoryInfo
    this.victory = function(team) { // victory
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (team === 0) { // 0 Victory
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("Gya! Those Rubes may have won the day, but we shall fight again!Retreat!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("We drove them away. For now... The enemy's been routed. Will, I want you to make sure the wounded are taken care of."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes, sir!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("Thanks for the assist, brother! Who's the runt? You got yourself some sort of cabin boy?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Cabin boy?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("He's one of us. We've been patrolling the region looking for survivors who might need our help."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr(""Helping" survivors? Helping ourself to their money, I hope. Ha ha ha...whew...Wait, you serious?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("Of course. We'd be more effective with air support, though. Why don't we join forces and--"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("Time out, Captain Courageous! You want Waylon to join forces with YOU? Listen to me now. I think you're a good man. I'll even give you a standing ovation. Clap, clap. But if you think I'm going to do charity work, you need to think again! I didn't survive the meteors just to risk myself for a bunch of chumps!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("What in the hell are you babbling about?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are you truly so blind, Brenner? There is no more central command! No orders, no missions... I do what i want! Got it?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, I don't think I do. And you've got it wrong. The world's been broken. But that doesn't mean you're free to do whatever you want. The ties that bind people together can be reestablished. We have to do that."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("And why would I want to do that? You want to clip my wings, brother? That it? I won't live in a cage! You can do what you want, but just leave me be. By the way, you know a fat cat named Greyfield?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("Admiral Greyfield? Yes, I served under him for a while. What about him?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("He's set up shop in the capital. Word is, he wants all surviving Rubinelle soldiers to come join him. Wants to start what he's calling the "New Rubinelle Army." Creative genius, he ain't. Now I ain't hip on takin' orders, but I could make a pretty penny as a mercenary. I'd have all the food I could eat, and the ladies love a man in uniform. Know what I'm saying?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("There's no accounting for taste."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aww, that's just cold, brother man. Ain't no call for that. Looks like you and I ain't gonna be friends. And with that, the sky calls. Good luck with the charity work."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("Wait a minute! What about your wounded?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("What's done is done. Wounded ain't dead, is it? I got places to go, things to do. You feel like playing nursemaid, go right ahead."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("You'd abandon your own men?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("You think you can guilt-trip me? You got another thing coming, brother man. My only responsibility is survivin' to eat, drink, and be merry another day."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is that so?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Uh-huh. Know what responsibility's gonna get YOU, Brenner? A broken heart and an early grave."), "CO_WAYLON", GameEnums.COMood_Normal, "#e88613"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("... ... ... ..."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_general.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are we letting him go, sir?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("We can't force him to stay, and we can't force him to care."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Is that guy really a soldier? He's nothing like you, Captain. He's nothing like any of us."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Soldiers are people, Will. You have to take the good with the bad. Each of us has to choose how to make it in this world, and Waylon's made his choice. Try not to let it bother you."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog27.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("We located and extracted. Waylon's downed air units. His men are resting comfortably, sir. We've also found some airports. Perhaps the men can be convinced to join us."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog28.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("What do they want? Have you heard any talk about them rejoining their captain?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog29.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Not a peep, sir. Apparently they want nothing more to do with him. It seems he's used them poorly, risking their lives for nothing more than his own amusement."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog30.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("I can't believe him!"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog31.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr(" Let Waylon's men know they're free to join us if they wish. Now let's head out for that nuclear shelter. It may give us reason to hope for a better day."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_camp_night_outside.png"); // 1 ScriptEventDialogItem
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
            dialog30.queueAnimation(dialog31);
            dialog31.queueAnimation(dialog32);
            // Dialog
        } // Victory End
    }; // victory
    this.turnStart = function(turn, player) { // turnStart
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
    // preconditionend
        if (turn === 1 && player === 0) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("So we've hit a snag in our supply-procurement plan. Got any suggestions what we do now, Doc?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog0.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! Yes...um...no. No, I don't. We're in trouble, no doubt about it."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog1.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(qsTr("Are there any other factories with the systems we need?"), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog2.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(qsTr("No, not to my knowledge."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog3.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(qsTr("Automated...crop factories?"), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog4.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you know something, Isabella?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog5.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes...I do."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog6.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog7.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_normal_road.png"); // 1 ScriptEventDialogItem
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're absolutely certain about this?"), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog8.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog9.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is no joking matter, young lady! I...I can't believe I just said that."), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog10.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(qsTr("Yes. It is located at latitude 25.345 degrees north and longitude 131.035 degrees west."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog11.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh...um... Well, as long as you're sure!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog12.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(qsTr("That's in the Seratta mountain range."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog13.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(qsTr("There is an underground nuclear shelter at the coordinates I have given you. It contains a subterranean farm and small-scale factory capable of feeding 1,000 people."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog14.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(qsTr("Do you really expect us to believe that? I've never heard of such a place!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog15.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(qsTr("Nor have I."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog16.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(qsTr("It was constructed for Rubinelle's senior cabinet minister and captain of industry. In the event of an all-out nuclear war, they would be able to live in the shelter for decades. Its existence is not known to the general public. Access requires a grade 4 security clearance."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog17.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(qsTr("Oh ho! Now we know! You must be the president's secret daughter! Oh ho! Oh ho ho ho!"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog18.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(qsTr("The construction contract for the shelter was awarded to a private military firm...IDS...IDS...IDS...IDS...IDS...ID..."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog19.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(qsTr("What is it, Isabella? What's wrong?"), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog20.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(qsTr("I'm sorry... It's my head. It's pounding so hard... I can't think."), "CO_ISABELLA", GameEnums.COMood_Normal, "#ff0000"); dialog21.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain Brenner! I'll take care of Isabella."), "CO_WILL", GameEnums.COMood_Normal, "#ff0000"); dialog22.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(qsTr("You do that, Will."), "CO_BRENNER", GameEnums.COMood_Normal, "#0098f8"); dialog23.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(qsTr("That girl may be a complete mystery, but I feel strangely confident she's not lying. Perhaps those coordinates will lead us to the answer to our dilemma, hmmm?"), "co_dr_morris", GameEnums.COMood_Normal, "#0098f8"); dialog24.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(qsTr("Could be, Doc. It's worth checking out, that's for sure."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog25.loadBackground("maps/advance_wars_day_of_ruin_campaign/cutscenes/bg_tent_inside.png"); // 1 ScriptEventDialogItem
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain, sir! One of our recon units just pulled in. It's reporting a firefight in progress! There's a lone unit running or its life. It's a Rubinelle unit, sir!"), "co_officier_ti", GameEnums.COMood_Normal, "#ff0000"); dialog26.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(qsTr("Would somebody tell me why these Lazurians are up in my business? Did they not see the meteors? The fires, the floods, the dead people? They act like there's still a damn war goin' on, and... Hey! Who are you?"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog27.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(qsTr("This is captain Brenner of the 12th Battalion."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog28.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(qsTr("Aw, yeah! Now, we're talkin'! The cavalry arrives!"), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog29.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(qsTr("Name and rank, soldier. Where's the rest of your company?"), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog30.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(qsTr("Name and rank? Listen up, brother man. That's all ancient history! I ain't heard a peep from the big brass since the sky fell in. How 'bout you? Nothin', right? Yeah, I thought so."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog31.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(qsTr("You're right. We've been operating on our own. It seems Rubinelle's government is gone for good."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog32.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(qsTr("Cheer up, Brenner! Ain't nothing wrong with that. The name's Waylon. Oh, right... Captain Waylon of the 13th Airborne! Ha ha ha! Lighten up, will ya? We're survivors! We're like lottery winners! Ain't much left of the old world. Least we can do is enjoy the new one."), "CO_WAYLON", GameEnums.COMood_Normal, "#d08000"); dialog33.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(qsTr("Captain! The enemy has spotted us and is moving to engage."), "CO_LIN", GameEnums.COMood_Normal, "#0098f8"); dialog34.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(qsTr("Understood. Get 'em ready, Lin."), "CO_BRENNER", GameEnums.COMood_Normal, "#ff0000"); dialog35.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(qsTr("Attention, all units! The yellow fighter is a friendly. Do not repeat, DO NOT use it for target practice."), "CO_LIN", GameEnums.COMood_Normal, "#ff0000"); dialog36.loadBackground(""); // 1 ScriptEventDialogItem
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(qsTr("Right kindly of you, baby. I owe you one!"), "CO_WAYLON", GameEnums.COMood_Normal, "#e88613"); dialog37.loadBackground(""); // 1 ScriptEventDialogItem
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
            dialog30.queueAnimation(dialog31);
            dialog31.queueAnimation(dialog32);
            dialog32.queueAnimation(dialog33);
            dialog33.queueAnimation(dialog34);
            dialog34.queueAnimation(dialog35);
            dialog35.queueAnimation(dialog36);
            dialog36.queueAnimation(dialog37);
            // Dialog
        } // Start Of Turn End
        if (turn === 1 && player === 1) { // 0 Start Of Turn
            // Dialog
            var dialog0 = GameAnimationFactory.createGameAnimationDialog(qsTr("More Rube forces... My prayers have been answered! Rejoice, brothers and sisters of Lazuria! Our enemy is before us! In the names of our ancestors, let us sow the soil with their bones!"), "CO_TASHA", GameEnums.COMood_Normal, "#0098f8"); dialog0.loadBackground(""); // 1 ScriptEventDialogItem
            // Dialog
        } // Start Of Turn End
    }; // turnStart
    this.actionDone = function(action) { // actionConditions
    // precondition
        var variables = map.getGameScript().getVariables();
        var campaignVariables;
        if (map.getCampaign() !== null){
            var campaignVariables = map.getCampaign().getVariables();
        }
        var variable1 = variables.createVariable("variable1");
    // preconditionend
        if (map.getPlayer(2).getIsDefeated() === true && variable1.readDataBool() === false) {// 0 Player Defeated
            map.getPlayer(0).defeatPlayer(null); // 0 Defeat Player
            variable1.writeDataBool(true);
        } // Player Defeated End
    }; // actionConditions
// scriptEnd
};
Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
