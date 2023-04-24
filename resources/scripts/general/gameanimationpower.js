var GAMEANIMATIONPOWER =
        {
    loadAnimation : function(animation, color, powerMode, co, map)
    {
        // load main background
        if (powerMode === GameEnums.PowerMode_Tagpower)
        {
            animation.createRotatingBackground("tagpower_background", color);
        }
        else if (powerMode === GameEnums.PowerMode_Superpower)

        {
            animation.createRotatingBackground("superpower_background", color);
        }
        else
        {
            animation.createRotatingBackground("power_background", color);
        }
        // load co activating the power
        var frameTime = 100;
        var coMovingDuration = 30;
        var offsetXMult = 1;
        GAMEANIMATIONPOWER.loadMovingCo(animation, co, offsetXMult, coMovingDuration, frameTime, false);

        var text = "";
        if (powerMode === GameEnums.PowerMode_Tagpower)
        {
            // load tag power related information if needed
            var currentPlayer = map.getCurrentPlayer();
            var co2 = currentPlayer.getCO(1);
            animation.createPowerDescription(co, powerMode, true);
            animation.createPowerDescription(co2, powerMode, false);
            text = TAGPOWER.getTagname(co, co2);
            var synergy = 100 + TAGPOWER.getTagpower(co, co2);
            text += " " + qsTr("Synergy:") + " " + synergy.toString() + " %";

            offsetXMult = 2;
            GAMEANIMATIONPOWER.loadMovingCo(animation, co2, offsetXMult, coMovingDuration, frameTime, true);
        }
        else if (powerMode === GameEnums.PowerMode_Superpower)
        {
            text = co.getSuperPowerName();
            animation.createPowerDescription(co, powerMode, false);
        }
        else
        {
            text = co.getPowerName();
            animation.createPowerDescription(co, powerMode, false);
        }

        // Short names throw the animation's timing off, so I added this bit as a temporary fix.
        var textLength = text.length;
        if (textLength < 15)
        {
            var extraSpaces = 15 - textLength;
            for (var i = 0; i < extraSpaces; i++)
            {
                text = text + " ";
            }
        }

        var timeMs = text.length * frameTime * 4;
        if (timeMs < frameTime * coMovingDuration)
        {
            timeMs = frameTime * coMovingDuration;
        }
        GAMEANIMATIONPOWER.loadMovingText(animation, frameTime, text, timeMs)
        animation.setDuration(timeMs);
        animation.addSound("power_activation.wav");
    },

    randomDirections : function()
    {
        var width = settings.getStageWidth();
        var height = settings.getStageHeight();
        return [Qt.point(-100, -100), Qt.point(-100, height),
                Qt.point(width, -100), Qt.point(width, height)]
    },

    loadMovingText : function(animation, frameTime, text, timeMs)
    {
        var screenHeight = settings.getStageHeight();
        var screenWidth = settings.getStageWidth();
        var fontHeight = 72;
        var font = "main72";
        var totalWidth = animation.getFontWidth(font, text)
        if (totalWidth > screenWidth - 20)
        {
            font = "main48";
            totalWidth = animation.getFontWidth(font, text)
            fontHeight = 48;
        }
        var length = text.length;
        var burstDelay = timeMs * 0.9;
        var endX = 10;
        var endY = screenHeight / 2 - fontHeight / 2;
        var randomDirections = GAMEANIMATIONPOWER.randomDirections(); // There's probably a better way to do this too...
        for (var i = 0; i < length; ++i)
        {
            var startX = -40;
            var startY = -40;
            if (globals.isEven(i))
            {
                startX = endX;
                if (i % 4 === 0)
                {
                    startY = screenHeight;
                }
            }
            else
            {
                startY = endY;
                if (i % 4 === 3)
                {
                    startX = screenWidth;
                }
            }
            var delay = (frameTime * 2 * i);
            var duration = frameTime * 2;
            var subtext = text[i];
            animation.createMovingText(font, subtext, delay, Qt.point(startX, startY), Qt.point(endX, endY), duration, GameEnums.QEasingCurve_Linear);
            animation.addTweenWaitToLastQueue(burstDelay - delay, GameEnums.QEasingCurve_Linear);
            animation.addTweenPositionToLastQueue(randomDirections[globals.randInt(0, 3)], frameTime * 4, GameEnums.QEasingCurve_OutQuad);
            if (subtext !== " ")
            {
                animation.addSound("co_power_letter.wav", 1, delay + duration - frameTime)
            }
            if (i === 0)
            {
                animation.addSound("superpower_burst.wav", 1, burstDelay);
            }
            var letterWidth = animation.getFontWidth(font, subtext);
            if (letterWidth <= 0)
            {
                letterWidth = (endX - 10) / (i + 1);
            }
            endX += letterWidth;
        }
    },

    loadMovingCo : function(animation, co, offsetXMult, coMovingDuration, frameTime, startAtBottom)
    {
        var coid = co.getCoID();
        var sprite = coid.toLowerCase() + "+nrm";
        var scale = 1.0;
        var size = animation.getCoSpriteSize(sprite);
        if (settings.getSmallScreenDevice())
        {
            scale = (settings.getStageHeight() - 60) / size.y;
        }
        else
        {
            scale = (settings.getStageHeight() - 300) / size.y;
        }
        var startPos = Qt.point(settings.getStageWidth() - size.x * scale * offsetXMult - 20 * offsetXMult, settings.getStageHeight());
        if (startAtBottom === false)
        {
            startPos = Qt.point(settings.getStageWidth() - size.x * scale * offsetXMult - 20 * offsetXMult, - size.y * scale);
        }

        animation.addMovingCoSprite(sprite, scale, startPos, Qt.point(startPos.x, settings.getStageHeight() / 2 - size.y * scale / 2), frameTime * coMovingDuration, 0, GameEnums.QEasingCurve_OutQuad);
    }
};
