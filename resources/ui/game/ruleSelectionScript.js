var RuleSelectionScript =
{
    getWeatherLabelNames : function()
    {
        var map = currentMenu.getMap();
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        var i = 0;
        var array = [];
        for (i = 0; i < count; ++i)
        {
            var weather = rules.getWeather(i);
            array.push(weather.getWeatherName() + ":");
        }
        return array;
    },
    getWeatherNames : function()
    {
        var map = currentMenu.getMap();
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        var i = 0;
        var array = [];
        for (i = 0; i < count; ++i)
        {
            var weather = rules.getWeather(i);
            array.push(weather.getWeatherName());
        }
        return array;
    },
    getWeatherChances : function()
    {
        var map = currentMenu.getMap();
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        var i = 0;
        var array = [];
        for (i = 0; i < count; ++i)
        {
            var weather = rules.getWeather(i);
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
    getDamageFormulaRule : function()
    {
        return [qsTr("Advance Wars 4"), qsTr("Advance Wars  1-3")];
    },
    getFogValue : function()
    {
        var fogMode = currentMenu.getMap().getGameRules().getFogMode();
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
            currentMenu.getMap().getGameRules().setFogMode(input - 1);
        }
        else
        {
            currentMenu.getMap().getGameRules().setFogMode(input);
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
    currentVictoryRuleIndex : 0,

    getVictoryRule : function()
    {
        return currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentVictoryRuleIndex))
    },
    getVictoryRuleElementCount : function(loopIdx)
    {
        RuleSelectionScript.currentVictoryRuleIndex = loopIdx;
        return RuleSelectionScript.getVictoryRule().getRuleType().length;
    },
    getVictoryRuleElementType : function(loopIdx)
    {
        return RuleSelectionScript.getVictoryRule().getRuleType()[loopIdx];
    },
    getIsVictoryRuleCheckboxType : function(loopIdx)
    {
        return RuleSelectionScript.getVictoryRuleElementType(loopIdx) === "checkbox";
    },
    getIsVictoryRuleSpinboxType : function(loopIdx)
    {
        return RuleSelectionScript.getVictoryRuleElementType(loopIdx) === "spinbox";
    },
    getVictoryRulename : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getRuleName(loopIdx);
    },
    getVictoryRuleMaxValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getMaxValue(loopIdx);
    },
    getVictoryRuleDescription : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getRuleDescription(loopIdx);
    },
    getVictoryRuleValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getRuleValue(loopIdx);
    },
    getVictoryRuleInfiniteValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getInfiniteValue(loopIdx);
    },
    setVictoryRuleValue : function(input, loopIdx)
    {
        var rule = currentMenu.getMap().getGameRules().getVictoryRule(objectId);
        return rule.setRuleValue(input, loopIdx);
    },
    getVictoryRuleX : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return 5;
        }
        else
        {
            return lastX + 200 + 10;
        }
    },
    getVictoryRuleY : function(loopIdx)
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


    currentGameRuleIndex : 0,
    getGameRule : function()
    {
        return currentMenu.getMap().getGameRules().getGameRule(gameRuleManager.getGameRuleID(RuleSelectionScript.currentGameRuleIndex))
    },

    getGameRuleElementCount : function(loopIdx)
    {
        RuleSelectionScript.currentGameRuleIndex = loopIdx;
        return RuleSelectionScript.getGameRule().getRuleType().length;
    },
    getGameRuleElementType : function(loopIdx)
    {
        return RuleSelectionScript.getGameRule().getRuleType()[loopIdx];
    },
    getIsGameRuleCheckboxType : function(loopIdx)
    {
        return RuleSelectionScript.getGameRuleElementType(loopIdx) === "checkbox";
    },
    getIsGameRuleSpinboxType : function(loopIdx)
    {
        return RuleSelectionScript.getGameRuleElementType(loopIdx) === "spinbox";
    },
    getGameRulename : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleName(loopIdx);
    },
    getGameRuleDescription : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleDescription(loopIdx);
    },
    getGameRuleValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleValue(loopIdx);
    },
    getGameRuleInfiniteValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getInfiniteValue(loopIdx);
    },
    setGameRuleValue : function(input, loopIdx)
    {
        var rule = currentMenu.getMap().getGameRules().getGameRule(objectId);
        return rule.setRuleValue(input, loopIdx);
    },
    getGameRuleX : function(loopIdx)
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
    getGameRuleY : function(loopIdx)
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
    getGatewayOptionEnabled : function()
    {
        return currentMenu.getRuleChangeEabled() && !currentMenu.getIsMultiplayerOnlyGatewayMode();
    },
    getGatewayDefaultValue : function()
    {
        if (currentMenu.getIsMultiplayerOnlyGatewayMode())
        {
            return true;
        }
        else
        {
            return currentMenu.getMap().getGameRules().getGatewayHosting()
        }
    }
};
