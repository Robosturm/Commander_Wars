var ANIMATION_BUILDER = function(map)
{
    this.map = map;
    this.queuedAnimationBuilders = [];
    this.perAnimationFunction = (animation, map) => {};

    this.buildAnimation = function(parentAnimation = null)
    {
        var animation = GameAnimationFactory.createAnimation(this.map, 0, 0);
        animation.addSprite("white_pixel", 0, 0, 0, 0);
        if (parentAnimation !== null)
        {
            parentAnimation.queueAnimation(animation);
        }
        this.perAnimationFunction(animation, this.map);
        return animation;
    };

    this.displayAnimation = function(parentAnimation = null)
    {
        var animation = this.buildAnimation(parentAnimation);
        for (var i = 0; i < this.queuedAnimationBuilders.length; i++)
        {
            this.queuedAnimationBuilders[i].displayAnimation(animation);
        }
    };

    this.queueAnimationBuilder = function(animationBuilder)
    {
        this.queuedAnimationBuilders.push(animationBuilder);
    };

    this.setPerAnimationFunction = function(perAnimationFunction)
    {
        this.perAnimationFunction = perAnimationFunction;
        return this;
    };
};