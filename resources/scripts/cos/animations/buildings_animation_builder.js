var BUILDINGS_ANIMATION_BUILDER = function(buildings, map) 
{
    ANIMATION_BUILDER.call(this, map);
    this.buildings = buildings;
    this.buildingFilter = (building, map) => true;
    this.randomize = true;
    this.minDelay = 135;
    this.maxDelay = 265;
    this.animationAmount = 5;
    this.sounds = ["power0.wav"];
    this.sprite = "power0";
    this.xOffset = 1.27;
    this.yOffset = 1.27;
    this.scale = 2;
    this.perAnimationFunction = (building, animation, map) => {};

    this.buildAnimation = function(parentAnimation = null) 
    {
        var animations = [];
        var counter = 0;
        if (this.randomize){
            this.buildings.randomize();
        }
        var soundIndex = 0;
        var lastAnimation = null;
        var lastDelay = 0;
        var delays = [];
        for (var i = 0; i < this.buildings.size(); i++)
        {
            var building = this.buildings.at(i);
            if (this.buildingFilter(building, this.map))
            {
                var animation = GameAnimationFactory.createAnimation(this.map, building.getX(), building.getY());
                var delay = globals.randInt(this.minDelay, this.maxDelay);
                if (animations.length < this.animationAmount)
                {
                    delay *= animations.length;
                }
                if (this.sounds.length > 0)
                {
                    animation.setSound(this.sounds[soundIndex], 1, delay);
                    soundIndex++;
                    if (soundIndex >= this.sounds.length)
                    {
                        soundIndex = 0;
                    }
                }
                animation.addSprite(this.sprite, -map.getImageSize() * this.xOffset, -map.getImageSize() * this.yOffset, 0, this.scale, delay);
                var totalDelay = 0;
                if (animations.length < this.animationAmount)
                {
                    if (parentAnimation !== null)
                    {
                        parentAnimation.queueAnimation(animation);
                    }
                    animations.push(animation);
                    delays.push(delay);
                    totalDelay = delay;
                }
                else
                {
                    animations[counter].queueAnimation(animation);
                    animations[counter] = animation;
                    delays[counter] += delay;
                    totalDelay = delays[counter];
                    counter++;
                    if (counter >= animations.length)
                    {
                        counter = 0;
                    }
                }
                if (totalDelay > lastDelay)
                {
                    lastAnimation = animation;
                    lastDelay = totalDelay;
                }
                this.perAnimationFunction(building, animation, this.map);
            }
        }
        return lastAnimation;
    };

    this.setBuildingFilter = function(buildingFilter)
    {
        this.buildingFilter = buildingFilter;
        return this;
    };

    this.setRandomize = function(randomize)
    {
        this.randomize = randomize;
        return this;
    }

    this.setMinDelay = function(minDelay)
    {
        this.minDelay = minDelay;
        return this;
    };

    this.setMaxDelay = function(maxDelay)
    {
        this.maxDelay = maxDelay;
        return this;
    };

    this.setAnimationAmount = function(animationAmount)
    {
        this.animationAmount = animationAmount;
        return this;
    };

    this.setSound = function(sound)
    {
        if (sound === null)
        {
            this.setSounds([]);
        }
        else
        {
            this.setSounds([sound]);
        }
        return this;
    };

    this.setSounds = function(sounds)
    {
        this.sounds = sounds;
        return this;
    };

    this.setSprite = function(sprite, updateOffset = true)
    {
        this.sprite = sprite;
        if (updateOffset)
        {
            var exceptionIndex = UNITS_ANIMATION_BUILDER.offsetExceptionSprites.indexOf(sprite);
            if (exceptionIndex >= 0)
            {
                this.setOffset(UNITS_ANIMATION_BUILDER.offsetExceptionValues[exceptionIndex]);
            }
        }
        return this;
    };

    this.setOffset = function(offset)
    {
        this.xOffset = offset;
        this.yOffset = offset;
        return this;
    };

    this.setXOffset = function(xOffset)
    {
        this.xOffset = xOffset;
        return this;
    };

    this.setYOffset = function(yOffset)
    {
        this.yOffset = yOffset;
        return this;
    };

    this.setScale = function(scale)
    {
        this.scale = scale;
        return this;
    }

    this.setPerAnimationFunction = function(perAnimationFunction)
    {
        this.perAnimationFunction = perAnimationFunction;
        return this;
    };
};
BUILDINGS_ANIMATION_BUILDER.prototype = new ANIMATION_BUILDER(null);

var OWN_BUILDINGS_ANIMATION_BUILDER = function(co, map)
{
    if (co === null)
    {
        BUILDINGS_ANIMATION_BUILDER.call(this, null, map);
    }
    else
    {
        BUILDINGS_ANIMATION_BUILDER.call(this, co.getOwner().getBuildings(), map);
    }
};
OWN_BUILDINGS_ANIMATION_BUILDER.prototype = new BUILDINGS_ANIMATION_BUILDER(null, null);

var ENEMY_BUILDINGS_ANIMATION_BUILDER = function(co, map)
{
    if (co === null)
    {
        BUILDINGS_ANIMATION_BUILDER.call(this, null, map);
    }
    else
    {
        BUILDINGS_ANIMATION_BUILDER.call(this, co.getOwner().getEnemyBuildings(), map);
    }
};
ENEMY_BUILDINGS_ANIMATION_BUILDER.prototype = new BUILDINGS_ANIMATION_BUILDER(null, null);