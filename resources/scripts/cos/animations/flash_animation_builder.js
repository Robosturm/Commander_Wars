var FLASH_ANIMATION_BUILDER = function(map)
{
    ANIMATION_BUILDER.call(this, map);
    this.duration = 3000;
    this.startColor = "#00FFFFFF";
    this.endColor = "#FFFFFFFF";
    this.twoSided = true;
    this.sound = null;
    this.delay = 0;
    this.perAnimationFunction = (animation, map) => {};

    this.buildAnimation = function(parentAnimation = null)
    {
        var animation = GameAnimationFactory.createAnimation(this.map, 0, 0);
        animation.addSprite2("white_pixel", 0, 0, this.duration + this.delay + 200, this.map.getMapWidth(), this.map.getMapHeight());
        animation.addTweenColor(0, this.startColor, this.endColor, this.duration, this.twoSided, this.delay);
        if (this.sound !== null)
        {
            animation.setSound(this.sound);
        }
        if (parentAnimation !== null)
        {
            parentAnimation.queueAnimation(animation);
        }
        this.perAnimationFunction(animation, this.map);
        return animation;
    };

    this.setDuration = function(duration)
    {
        this.duration = duration;
        return this;
    };

    this.setStartColor = function(startColor)
    {
        this.startColor = startColor;
        return this;
    };

    this.setEndColor = function(endColor)
    {
        this.endColor = endColor;
        return this;
    };

    this.setTwoSided = function(twoSided)
    {
        this.twoSided = twoSided;
        return this;
    };

    this.setSound = function(sound)
    {
        this.sound = sound;
        return this;
    };

    this.setDelay = function(delay)
    {
        this.delay = delay;
        return this;
    };

    this.setPerAnimationFunction = function(perAnimationFunction)
    {
        this.perAnimationFunction = perAnimationFunction;
        return this;
    };
};
FLASH_ANIMATION_BUILDER.prototype = new ANIMATION_BUILDER(null);