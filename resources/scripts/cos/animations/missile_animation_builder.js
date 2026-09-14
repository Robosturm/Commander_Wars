var MISSILE_ANIMATION_BUILDER = function(x, y, map) {
    ANIMATION_BUILDER.call(this, map);
    this.x = x;
    this.y = y;
    this.sprite = "explosion+silo";
    this.spriteSize = 2;
    this.spriteOffsetX = -1/4;
    this.spriteOffsetY = -1/2;
    this.spriteScale = 2;
    this.sound = "missle_explosion.wav";
    this.sleepAfterFinish = 0;
    this.delay = 0;
    this.perAnimationFunction = (animation, map) => {};

    this.buildAnimation = function(parentAnimation = null)
    {
        var animation = GameAnimationFactory.createAnimation(this.map,
            this.x - this.spriteSize, this.y - this.spriteSize);
        animation.addSprite(this.sprite,
            this.map.getImageSize() * this.spriteOffsetX * this.spriteScale,
            this.map.getImageSize() * this.spriteOffsetY * this.spriteScale,
            this.sleepAfterFinish, this.spriteScale, this.delay);
        if (this.sound !== null)
        {
            animation.setSound(this.sound, 1);
        }
        var functionName = "centerMap" + this.x + "_" + this.y;
        var functionX = this.x;
        var functionY = this.y;
        MISSILE_ANIMATION_BUILDER[functionName] = function(animation, map)
        {
            MISSILE_ANIMATION_BUILDER.centerMap(functionX, functionY, map);
        }
        animation.setStartOfAnimationCall("MISSILE_ANIMATION_BUILDER", functionName);
        parentAnimation.queueAnimation(animation);
        this.perAnimationFunction(animation, this.map);
        return animation;
    };

    this.setX = function(x)
    {
        this.x = x;
        return this;
    };

    this.setY = function(y)
    {
        this.y = y;
        return this;
    };

    this.setPos = function(pos)
    {
        this.setX(pos.x);
        this.setY(pos.y);
        return this;
    };

    this.setSprite = function(sprite)
    {
        this.sprite = sprite;
        return this;
    };

    this.setSpriteSize = function(spriteSize)
    {
        this.spriteSize = spriteSize;
        return this;
    };

    this.setSpriteOffsetX = function(spriteOffsetX)
    {
        this.spriteOffsetX = spriteOffsetX;
        return this;
    };

    this.setSpriteOffsetY = function(spriteOffsetY)
    {
        this.spriteOffsetY = spriteOffsetY;
        return this;
    };

    this.setSpriteScale = function(spriteScale)
    {
        this.spriteScale = spriteScale;
        return this;
    };

    this.setSound = function(sound)
    {
        this.sound = sound;
        return this;
    };

    this.setSleepAfterFinish = function(sleepAfterFinish)
    {
        this.sleepAfterFinish = sleepAfterFinish;
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

MISSILE_ANIMATION_BUILDER.centerMap = function(x, y, map)
{
    map.centerMap(x, y);
    // GameConsole.print("Camera X: " + x + " Y: " + y, 1);
};

MISSILE_ANIMATION_BUILDER.prototype = new ANIMATION_BUILDER(null);

var METEOR_ANIMATION_BUILDER = function(x, y, map)
{
    MISSILE_ANIMATION_BUILDER.call(this, x, y, map);
    this.setSprite("meteor");
    this.setSpriteSize(2);
    this.setSpriteOffsetX(0);
    this.setSpriteOffsetY(0);
    this.setSpriteScale(2.6);
    this.setSound("meteorFall.wav");
    this.setSleepAfterFinish(2500);
    this.startOffsetX = 4;
    this.startOffsetY = -2;
    this.startSpriteScale = 4;
    this.fallTime = 1000;
    this.fadeTime = 1200;

    this.buildAnimation = function(parentAnimation = null)
    {
        // create cool meteor animation :)
        var animation = GameAnimationFactory.createAnimation(this.map,
            this.x - this.spriteSize + this.startOffsetX, this.x - this.spriteSize + this.startOffsetY);
        animation.addSprite(this.sprite, this.spriteOffsetX, this.spriteOffsetY, this.sleepAfterFinish, this.startSpriteScale);
        animation.addTweenPosition(
            Qt.point((this.x - this.spriteSize) * this.map.getImageSize(), (this.y - this.spriteSize) * this.map.getImageSize()),
            this.fallTime
        );
        animation.addTweenScale(this.spriteScale / this.startSpriteScale, this.fallTime);
        animation.addTweenColor(0, "#FFFFFFFF", "#00FFFFFF", this.fallTime, false, this.fadeTime);
        if (this.sound !== null)
        {
            animation.addSound(this.sound);
        }
        
        var functionName = "centerMap" + this.x + "_" + this.y;
        var functionX = this.x;
        var functionY = this.y;
        MISSILE_ANIMATION_BUILDER[functionName] = function(animation, map)
        {
            MISSILE_ANIMATION_BUILDER.centerMap(functionX, functionY, map);
        }
        animation.setStartOfAnimationCall("MISSILE_ANIMATION_BUILDER", functionName);
        parentAnimation.queueAnimation(animation);
        this.perAnimationFunction(animation, this.map);
        return animation;
    };

    this.setStartOffsetX = function(startOffsetX)
    {
        this.startOffsetX = startOffsetX;
        return this;
    };

    this.setStartOffsetY = function(startOffsetY)
    {
        this.startOffsetY = startOffsetY;
        return this;
    };

    this.setStartSpriteScale = function(startSpriteScale)
    {
        this.startSpriteScale = startSpriteScale;
        return this;
    };

    this.setFallTime = function(fallTime)
    {
        this.fallTime = fallTime;
        return this;
    };

    this.setFadeTime = function(fadeTime)
    {
        this.fadeTime = fadeTime;
        return this;
    }
};

METEOR_ANIMATION_BUILDER.prototype = new MISSILE_ANIMATION_BUILDER(0, 0, null);