var RuleSelectionScript =
{
    getWeatherLabelNames : function()
    {
        let map = currentMenu.getMap();
        let rules = map.getGameRules();
        let count = rules.getWeatherCount();
        let i = 0;
        let array = [];
        for (i = 0; i < count; ++i)
        {
            let weather = rules.getWeather(i);
            array.push(weather.getWeatherName() + ":");
        }
        return array;
    },
    getWeatherNames : function()
    {
        let map = currentMenu.getMap();
        let rules = map.getGameRules();
        let count = rules.getWeatherCount();
        let i = 0;
        let array = [];
        for (i = 0; i < count; ++i)
        {
            let weather = rules.getWeather(i);
            array.push(weather.getWeatherName());
        }
        return array;
    },
    getWeatherChances : function()
    {
        let map = currentMenu.getMap();
        let rules = map.getGameRules();
        let count = rules.getWeatherCount();
        let i = 0;
        let array = [];
        for (i = 0; i < count; ++i)
        {
            let weather = rules.getWeather(i);
            array.push(rules.getWeatherChance(weather.getWeatherId()));
        }
        return array;
    },
    getCoZoneOptions : function()
    {
        return [qsTr("Global"), qsTr("Global and CO Zone Bonus"), qsTr("Only CO Zone")];
    },
    getCoZoneMode : function()
    {
        return [qsTr("Money"), qsTr("Money only attacker"), qsTr("HP"), qsTr("HP only attacker")];
    },
    getFogOfWarRule : function()
    {
        return [qsTr("Off"), qsTr("Mist of War"), qsTr("Fog of War"), qsTr("Shroud of War")];
    },
    getFogValue : function()
    {
        let fogMode = currentMenu.getMap().getGameRules().getFogMode();
        if (fogMode === GameEnums.Fog_OfMist)
        {
            return 1;
        }
        else if (fogMode === GameEnums.Fog_Off)
        {
            return 0;
        }
        else
        {
            return fogMode + 1;
        }
    },
    setFogValue : function(input)
    {
        if (input === 1)
        {
            currentMenu.getMap().getGameRules().setFogMode(GameEnums.Fog_OfMist);
        }
        else if (input > 1)
        {
            currentMenu.getMap().getGameRules().setFogMode(value - 1);
        }
        else
        {
            currentMenu.getMap().getGameRules().setFogMode(value);
        }
    },
    getDayScreen : function()
    {
        return [qsTr("Default"), qsTr("Permanent")];
    },
    showScriptSelection : function()
    {
        return !currentMenu.getIsEditorMode() && currentMenu.getRuleChangeEabled();
    },
    currentRuleIndex : 0,
    getRuleElementCount : function(loopIdx)
    {
        RuleSelectionScript.currentRuleIndex = loopIdx;
        return currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(loopIdx)).getRuleType().length;
    },
    getRuleElementType : function(loopIdx)
    {
        return currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentRuleIndex)).getRuleType()[loopIdx];
    },
    getIsRuleCheckboxType : function(loopIdx)
    {
        return RuleSelectionScript.getRuleElementType(loopIdx) === "checkbox";
    },
    getIsRuleSpinboxType : function(loopIdx)
    {
        return RuleSelectionScript.getRuleElementType(loopIdx) === "spinbox";
    },
    getRulename : function(loopIdx)
    {
        let rule = currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentRuleIndex));
        return rule.getRuleName(loopIdx);
    },
    getRuleDescription : function(loopIdx)
    {
        let rule = currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentRuleIndex));
        return rule.getRuleDescription(loopIdx);
    },
    getRuleValue : function(loopIdx)
    {
        let rule = currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentRuleIndex));
        return rule.getRuleValue(loopIdx);
    },
    getInfiniteValue : function(loopIdx)
    {
        let rule = currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentRuleIndex));
        return rule.getInfiniteValue(loopIdx);
    },
    setRuleValue : function(input, loopIdx)
    {
        let rule = currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentRuleIndex));
        return rule.setRuleValue(input, loopIdx);
    },
    getRuleX : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return 5;
        }
        else
        {
            return lastX + 300 + 10;
        }
    },
    getRuleY : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return lastY + lastHeight + 10;
        }
        else
        {
            return lastY;
        }
    },
};
