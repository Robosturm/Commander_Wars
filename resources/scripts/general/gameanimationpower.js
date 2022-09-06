var GAMEANIMATIONPOWER =
{
    loadAnimation : function(animation, color, powerMode, co, map)
    {
        // load main background
        if (powerMode === GameEnums.PowerMode_Superpower ||
            powerMode === GameEnums.PowerMode_Tagpower)
        {
            animation.createRotatingBackground("superpower_background", color);
        }
        else
        {
            animation.createRotatingBackground("power_background", color);
        }
        // load co activating the power
        var frameTime = animation.getFrameTime();
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
        GAMEANIMATIONPOWER.loadMovingText(animation, frameTime, text)
        var timeMs = text.length * frameTime * 4;
        if (timeMs < frameTime * coMovingDuration)
        {
            timeMs = frameTime * coMovingDuration;
        }
        animation.setDuration(timeMs);
        animation.addSound("power_activation.wav");
    },

    loadMovingText : function(animation, frameTime, text)
    {
        var screenHeight = settings.getHeight();
        var fontHeight = 72;
        var font = "powerFont72";
        var totalWidth = animation.getFontWidth(font, text)
        if (totalWidth > settings.getWidth() - 20)
        {
            font = "powerFont48";
            totalWidth = animation.getFontWidth(font, text)
            fontHeight = 48;
        }
        var length = text.length;
        var xPos = 10;
        var endY = screenHeight / 2 - fontHeight / 2;
        for (var i = 0; i < length; ++i)
        {
            var startY = 0;
            if (globals.isEven(i))
            {
                startY = screenHeight;
            }
            var delay = frameTime * 2 * i + 1;
            var duration = frameTime * 4;
            var subtext = text[i];
            animation.createMovingText(font, subtext, delay, Qt.point(xPos, startY), Qt.point(xPos, endY), duration, GameEnums.QEasingCurve_OutQuad);
            var letterWidth = animation.getFontWidth(font, subtext);
            if (letterWidth <= 0)
            {
                letterWidth = (xPos - 10) / (i + 1);
            }
            xPos += letterWidth;
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
            scale = (settings.getHeight() - 60) / size.y;
        }
        else
        {
            scale = (settings.getHeight() - 300) / size.y;
        }
        var startPos = Qt.point(settings.getWidth() - size.x * scale * offsetXMult - 20 * offsetXMult, settings.getHeight());
        if (startAtBottom === false)
        {
            startPos = Qt.point(settings.getWidth() - size.x * scale * offsetXMult - 20 * offsetXMult, - size.y * scale);
        }

        animation.addMovingCoSprite(sprite, scale, startPos, Qt.point(startPos.x, settings.getHeight() / 2 - size.y * scale / 2), frameTime * coMovingDuration, 0, GameEnums.QEasingCurve_OutQuad);

    }
};
