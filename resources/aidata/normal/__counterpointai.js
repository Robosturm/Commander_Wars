;var COUNTERPOINTAI =
{
    // Reject persisted state from earlier strategy revisions.
    STRATEGY_VERSION : 13,
    DOMAIN_GROUND : "ground",
    DOMAIN_AIR : "air",
    DOMAIN_NAVAL : "naval",
    DOMAIN_HOVER : "hover",
    ACTION_BUILD_UNITS : "ACTION_BUILD_UNITS",
    NON_WEAPON_ATTACK_ACTIONS : { ACTION_EXPLODE : true },
    PHASE_SPECIAL : "special",
    PHASE_ORDINARY : "ordinary",
    ORDER_MODE_DAMAGE : "damage",
    ORDER_MODE_INVERSE : "inverse",
    PLANNER_STATE_VARIABLE_ID : "COUNTERPOINT_STATE",
    PLANNER_STATE_SCHEMA_VERSION : 1,
    // Rejection sampling changes every seeded sequence.
    RNG_ALGORITHM_VERSION : 2,
    // Fallbacks paired with the tunable of the same name by _tunable, so an install carrying an
    // older tunables file reads these instead of undefined and quietly disabling the feature.
    // PERCENT_MAX is here because the planner rolls against it, where undefined would poison
    // every percent chance at once rather than one feature.
    PERCENT_MAX_DEFAULT : 100,
    TEMPERATURE_DEFAULT : 1.0,
    MAX_ISLAND_MAPS_DEFAULT : 6,
    FERRY_TARGET_SAMPLE_DEFAULT : 24,
    FERRY_URGENT_STRANDED_SHARE_DEFAULT : 0.5,
    FERRY_MAX_HULLS_DEFAULT : 2,
    FERRY_CAPPERS_PER_HULL_DEFAULT : 2,
    FERRY_SAVE_MAX_TURNS_DEFAULT : 1,
    COUNTER_SAVE_MAX_TURNS_DEFAULT : 1,
    COUNTER_SAVE_MIN_DAY_DEFAULT : 3,
    COUNTER_GAP_RATIO_DEFAULT : 0.5,
    COUNTER_WORTH_RATIO_DEFAULT : 1.5,
    ADJACENCY_DAMAGE_NORM_DEFAULT : 100,
    COVERAGE_QUALITY_DAMAGE_DEFAULT : 55,
    INDIRECT_SATURATION_FREE_COUNT_DEFAULT : 2,
    INDIRECT_SATURATION_COST_WEIGHT_DEFAULT : 60,
    HOLD_FOR_BETTER_RATIO_DEFAULT : 0.25,
    MAX_STRATEGIC_HOLDS_PER_TURN_DEFAULT : 3,
    PHANTOM_RETAL_WEIGHT_DEFAULT : 0.5,
    PHANTOM_OFFENSE_WEIGHT_DEFAULT : 0.5,
    AA_NO_AIR_DISCOUNT_DEFAULT : 0.35,
    AA_COVERAGE_URGENCY_DEFAULT : 4,
    AA_COVERAGE_BOOST_MAX_DEFAULT : 25,
    AA_ENEMY_AIR_PER_UNIT_DEFAULT : 2.5,
    AA_TARGET_RATIO_FLOOR : 0.001,
    SURPLUS_FUNDED_DIVISOR_DEFAULT : 3,
    SURPLUS_FUNDED_MIN_DEFAULT : 1,
    SURPLUS_FUNDED_JITTER_DEFAULT : 1,
    SURPLUS_ROTATION_SLOTS_DEFAULT : 3,
    VALUE_TARGET_MAX_TURNS_DEFAULT : 1,
    AA_SHARE_FLOOR : 0.1,
    MAX_INDIRECT_UNITS_DEFAULT : -1,
    MOBILITY_AIR_REACH_BONUS_DEFAULT : 1.25,
    MOBILITY_VALUE_REFERENCE_DEFAULT : 5,
    MOBILITY_VALUE_SLOPE_DEFAULT : 0.5,
    MOBILITY_VALUE_FLOOR_DEFAULT : 0.5,
    MOBILITY_VALUE_CEIL_DEFAULT : 1.5,
    CAPPER_VARIETY_MOVE_WEIGHT_DEFAULT : 1.0,
    CAPPER_VARIETY_MIN_SHARE_DEFAULT : 0.1,
    SURFACE_PRODUCTION_MIN_ISLAND_SHARE : 0.025,
    SURFACE_PRODUCTION_MIN_ISLAND_SIZE : 2,
    // A phantom threat represents a fresh unit.
    PHANTOM_RETAL_HP : 10,
    // A dying fielded unit must not suppress its replacement threat.
    PHANTOM_PRESENT_MIN_HP : 5,
    // Compiled, not tunable: below this the ground transport ramp cannot pay for itself, and an
    // undefined read here would disable ground transports outright rather than loosen them.
    GROUND_TRANSPORT_MIN_CAPPERS : 2,
    RNG_COUNTER_MULTIPLIER : 1831565813,
    // Eight redraws keep modulo bias below 1e-5.
    RNG_UNBIASED_ATTEMPTS : 8,
    RNG_LEFT_SHIFT_A : 13,
    RNG_RIGHT_SHIFT : 17,
    RNG_LEFT_SHIFT_B : 5,
    PLANNER_UINT32_MAX : 4294967295,
    PLANNER_VALUE_MAX : 2147483647,
    PLANNER_ID_LENGTH_HARD_LIMIT : 1024,
    PLANNER_KEY_LENGTH_HARD_LIMIT : 2048,
    // Ceilings the tunables cannot raise, keyed by the tunable each one bounds.
    PLANNER_HARD_LIMITS :
    {
        MAX_PLAN_COUNT : 512,
        MAX_PLAN_CANDIDATES : 512,
        MAX_TOTAL_PLAN_CANDIDATES : 32768,
        MAX_PLANNER_DRAW_COUNT : 1000000000,
        MAX_PLANNER_STATE_LENGTH : 8388608,
        MAX_CANDIDATE_COUNT : 65536,
        // Kept under the signed limit of the engine's bounded random API.
        MAX_RANDOM_WEIGHT_TOTAL : 1000000000
    },

    _finiteNumber : function(value, fallback)
    {
        var numeric = +value;
        return isFinite(numeric) ? numeric : fallback;
    },

    _clamp : function(value, minimum, maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    },

    // Funds, counts and indexes all arrive from engine calls or restored state, so they get read
    // as a whole number at or above zero rather than trusted.
    _wholeCount : function(value)
    {
        return Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(value, 0)));
    },

    _readNumber : function(source, key, fallback)
    {
        if (source === null || source === undefined || source[key] === undefined)
        {
            return fallback;
        }
        return COUNTERPOINTAI._finiteNumber(source[key], fallback);
    },

    // Pairs a tunable with its compiled fallback through the *_DEFAULT naming convention, so the
    // two cannot drift apart at a call site.
    _tunable : function(name)
    {
        return COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI[name], COUNTERPOINTAI[name + "_DEFAULT"]);
    },

    _readUnitNumber : function(source, unitId, fallback)
    {
        if (source === null || source === undefined)
        {
            return fallback;
        }
        var prefixedId = "#" + unitId;
        if (source[prefixedId] !== undefined)
        {
            return COUNTERPOINTAI._finiteNumber(source[prefixedId], fallback);
        }
        if (Object.prototype.hasOwnProperty.call(source, unitId))
        {
            return COUNTERPOINTAI._finiteNumber(source[unitId], fallback);
        }
        return fallback;
    },

    _unitId : function(unit)
    {
        if (unit === null || unit === undefined)
        {
            return "";
        }
        if (unit.id !== undefined)
        {
            return String(unit.id);
        }
        if (unit.unitId !== undefined)
        {
            return String(unit.unitId);
        }
        if (typeof unit.getUnitID === "function")
        {
            return String(unit.getUnitID());
        }
        return "";
    },

    _unitKey : function(unit)
    {
        if (unit !== null && unit !== undefined && unit.compositionKey !== undefined)
        {
            return String(unit.compositionKey);
        }
        if (unit !== null && unit !== undefined && unit.ownerId !== undefined)
        {
            return String(unit.ownerId) + ":" + COUNTERPOINTAI._unitId(unit);
        }
        return COUNTERPOINTAI._unitId(unit);
    },

    _sampleCompositionKey : function(unit)
    {
        var unitKey = COUNTERPOINTAI._unitKey(unit);
        var profileKey = unit !== null && unit !== undefined &&
            unit.weaponProfileKey !== undefined ? "@" + String(unit.weaponProfileKey) : "";
        if (unit !== null && unit !== undefined &&
            (unit.compositionKey !== undefined || unit.ownerId !== undefined))
        {
            return unitKey + profileKey;
        }
        return unitKey + "@" + COUNTERPOINTAI._strategicValue(unit) + profileKey;
    },

    _unitHp : function(unit)
    {
        if (unit === null || unit === undefined)
        {
            return 0;
        }
        if (unit.hp !== undefined)
        {
            return COUNTERPOINTAI._finiteNumber(unit.hp, 0);
        }
        if (typeof unit.getHp === "function")
        {
            return COUNTERPOINTAI._finiteNumber(unit.getHp(), 0);
        }
        return 0;
    },

    _collectionLength : function(collection)
    {
        if (collection === null || collection === undefined)
        {
            return 0;
        }
        if (collection.length !== undefined)
        {
            return COUNTERPOINTAI._wholeCount(collection.length);
        }
        if (typeof collection.size === "function")
        {
            return COUNTERPOINTAI._wholeCount(collection.size());
        }
        return 0;
    },

    _collectionAt : function(collection, index)
    {
        if (collection.length !== undefined)
        {
            return collection[index];
        }
        return collection.at(index);
    },

    _visitNonNullCollectionItems : function(collection, visitor)
    {
        var length = COUNTERPOINTAI._collectionLength(collection);
        for (var index = 0; index < length; ++index)
        {
            var item = COUNTERPOINTAI._collectionAt(collection, index);
            if (item !== null && item !== undefined)
            {
                visitor(item);
            }
        }
    },

    // Production scoring intentionally sees the whole enemy army. prepareProduction is already
    // handed that snapshot, but the ordinary build queue's vector is distance pruned by the
    // caller before it ever reaches a strategy.
    _fullEnemyUnits : function(ai, fallback)
    {
        if (ai === null || ai === undefined)
        {
            return fallback;
        }
        var player = ai.getPlayer();
        if (player === null || player === undefined)
        {
            return fallback;
        }
        var enemies = player.getEnemyUnits();
        return enemies === null || enemies === undefined ? fallback : enemies;
    },

    // Missing key falls back to the ceiling here, unlike _plannerLimit, which falls back to its
    // minimum. Both keep the behaviour they shipped with.
    _ceilingLimit : function(name)
    {
        var ceiling = COUNTERPOINTAI._readNumber(COUNTERPOINTAI.PLANNER_HARD_LIMITS, name, 1);
        var configured = Math.floor(COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI[name], ceiling));
        return COUNTERPOINTAI._clamp(configured, 1, ceiling);
    },

    _candidateLimit : function()
    {
        return COUNTERPOINTAI._ceilingLimit("MAX_CANDIDATE_COUNT");
    },

    _randomWeightLimit : function()
    {
        return COUNTERPOINTAI._ceilingLimit("MAX_RANDOM_WEIGHT_TOTAL");
    },

    _captureBaseChance : function()
    {
        var chance = COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI.CAPTURE_BASE_CHANCE, 0);
        if (chance <= 0)
        {
            return 0;
        }
        return Math.min(COUNTERPOINTAI._tunable("PERCENT_MAX"), chance);
    },

    _capperBuildsAllowed : function()
    {
        return COUNTERPOINTAI._captureBaseChance() > 0;
    },

    _safeFactor : function(factor)
    {
        var numeric = COUNTERPOINTAI._finiteNumber(factor, COUNTERPOINTAI.FACTOR_MIN);
        return COUNTERPOINTAI._clamp(
            numeric,
            COUNTERPOINTAI.FACTOR_MIN,
            COUNTERPOINTAI.FACTOR_MAX
        );
    },

    _flipFactor : function(score, factor)
    {
        var safeScore = COUNTERPOINTAI._finiteNumber(score, 0);
        var safeFactor = COUNTERPOINTAI._safeFactor(factor);
        return safeScore > 0 ? safeScore * safeFactor : safeScore / safeFactor;
    },

    _hpFrac : function(unit)
    {
        var hp = COUNTERPOINTAI._unitHp(unit);
        if (hp > COUNTERPOINTAI.NORMALIZED_HP_MAX)
        {
            hp /= COUNTERPOINTAI.PERCENT_HP_DIVISOR;
        }
        hp = COUNTERPOINTAI._clamp(hp, 0, COUNTERPOINTAI.NORMALIZED_HP_MAX);
        return hp / COUNTERPOINTAI.NORMALIZED_HP_MAX;
    },

    _sampleComposition : function(units)
    {
        var entries = [];
        var indexes = {};
        var length = Math.min(
            COUNTERPOINTAI._collectionLength(units),
            COUNTERPOINTAI._candidateLimit()
        );
        for (var index = 0; index < length; ++index)
        {
            var unit = COUNTERPOINTAI._collectionAt(units, index);
            var unitId = COUNTERPOINTAI._unitId(unit);
            if (unit === null || unit === undefined || unitId === "")
            {
                continue;
            }
            var compositionKey = COUNTERPOINTAI._sampleCompositionKey(unit);
            var lookupKey = "#" + compositionKey;
            var entryIndex = indexes[lookupKey];
            if (entryIndex === undefined)
            {
                entryIndex = entries.length;
                indexes[lookupKey] = entryIndex;
                entries.push({
                    id : unitId,
                    compositionKey : compositionKey,
                    count : 0,
                    hpSum : 0,
                    domain : unit.domain,
                    strategicValue : COUNTERPOINTAI._strategicValue(unit),
                    movement : COUNTERPOINTAI._readNumber(unit, "movement", 0),
                    minRange : COUNTERPOINTAI._readNumber(unit, "minRange", 1),
                    maxRange : COUNTERPOINTAI._readNumber(unit, "maxRange", 1),
                    canCapture : unit.canCapture === true,
                    isTransporter : unit.isTransporter === true,
                    canTransportTank : unit.canTransportTank === true,
                    armoredDamage : unit.armoredDamage,
                    maxDamageVsAir : unit.maxDamageVsAir,
                    hasAttackWeapon : COUNTERPOINTAI._hasAttackCapability(unit),
                    hasAttackAction : COUNTERPOINTAI._hasAttackAction(unit),
                    isAASpecialist : COUNTERPOINTAI._isAASpecialist(unit),
                    damageById : unit.damageById,
                    directDamageById : unit.directDamageById,
                    indirectDamageById : unit.indirectDamageById,
                    hasDirectChannel : COUNTERPOINTAI._hasDirectChannel(unit),
                    hasIndirectChannel : COUNTERPOINTAI._hasIndirectChannel(unit)
                });
            }
            entries[entryIndex].count += 1;
            entries[entryIndex].hpSum += COUNTERPOINTAI._hpFrac(unit);
        }
        return entries;
    },

    _hasValidComposition : function(composition)
    {
        var length = COUNTERPOINTAI._collectionLength(composition);
        for (var index = 0; index < length; ++index)
        {
            var entry = COUNTERPOINTAI._collectionAt(composition, index);
            if (entry !== null && entry !== undefined &&
                COUNTERPOINTAI._unitId(entry) !== "")
            {
                return true;
            }
        }
        return false;
    },

    _analyzeEnemyComp : function(enemyComposition)
    {
        var air = 0;
        var attackAir = 0;
        var naval = 0;
        var indirect = 0;
        var total = 0;
        var length = COUNTERPOINTAI._collectionLength(enemyComposition);
        for (var index = 0; index < length; ++index)
        {
            var enemy = COUNTERPOINTAI._collectionAt(enemyComposition, index);
            if (enemy === null || enemy === undefined ||
                COUNTERPOINTAI._unitId(enemy) === "")
            {
                continue;
            }
            var hp = Math.max(0, COUNTERPOINTAI._readNumber(enemy, "hpSum", 0));
            total += hp;
            if (enemy.domain === COUNTERPOINTAI.DOMAIN_AIR)
            {
                air += hp;
            }
            if (COUNTERPOINTAI._isAirThreat(enemy))
            {
                attackAir += hp;
            }
            if (enemy.domain === COUNTERPOINTAI.DOMAIN_NAVAL)
            {
                naval += hp;
            }
            if (COUNTERPOINTAI._isPureIndirect(enemy))
            {
                indirect += hp;
            }
        }
        if (total <= 0)
        {
            return {
                airShare : 0,
                attackAirShare : 0,
                navalShare : 0,
                indirectShare : 0,
                total : 0
            };
        }
        return {
            airShare : air / total,
            attackAirShare : attackAir / total,
            navalShare : naval / total,
            indirectShare : indirect / total,
            total : total
        };
    },

    _damageAgainst : function(attacker, defenderId)
    {
        if (attacker === null || attacker === undefined ||
            attacker.damageById === null || attacker.damageById === undefined)
        {
            return 0;
        }
        return Math.max(
            0,
            COUNTERPOINTAI._readUnitNumber(attacker.damageById, defenderId, 0)
        );
    },

    _channelDamageAgainst : function(attacker, mapName, defenderId)
    {
        if (attacker === null || attacker === undefined ||
            attacker[mapName] === null || attacker[mapName] === undefined)
        {
            return 0;
        }
        return Math.max(
            0,
            COUNTERPOINTAI._readUnitNumber(attacker[mapName], defenderId, 0)
        );
    },

    _directDamageAgainst : function(attacker, defenderId)
    {
        if ((attacker === null || attacker === undefined ||
             attacker.directDamageById === null ||
             attacker.directDamageById === undefined) &&
            COUNTERPOINTAI._hasDirectChannel(attacker))
        {
            return COUNTERPOINTAI._damageAgainst(attacker, defenderId);
        }
        return COUNTERPOINTAI._channelDamageAgainst(
            attacker,
            "directDamageById",
            defenderId
        );
    },

    _indirectDamageAgainst : function(attacker, defenderId)
    {
        if ((attacker === null || attacker === undefined ||
             attacker.indirectDamageById === null ||
             attacker.indirectDamageById === undefined) &&
            COUNTERPOINTAI._hasIndirectChannel(attacker))
        {
            return COUNTERPOINTAI._damageAgainst(attacker, defenderId);
        }
        return COUNTERPOINTAI._channelDamageAgainst(
            attacker,
            "indirectDamageById",
            defenderId
        );
    },

    _hasDirectChannel : function(unit)
    {
        if (unit !== null && unit !== undefined &&
            typeof unit.hasDirectChannel === "boolean")
        {
            return unit.hasDirectChannel;
        }
        return COUNTERPOINTAI._readNumber(unit, "minRange", 1) <= 1;
    },

    _hasIndirectChannel : function(unit)
    {
        if (unit !== null && unit !== undefined &&
            typeof unit.hasIndirectChannel === "boolean")
        {
            return unit.hasIndirectChannel;
        }
        return COUNTERPOINTAI._readNumber(unit, "minRange", 1) > 1;
    },

    _hasAttackCapability : function(unit)
    {
        if (unit === null || unit === undefined)
        {
            return false;
        }
        if (typeof unit.hasAttackWeapon === "boolean")
        {
            return unit.hasAttackWeapon;
        }
        var damageById = unit.damageById;
        if (damageById === null || damageById === undefined)
        {
            return false;
        }
        for (var key in damageById)
        {
            if (COUNTERPOINTAI._readNumber(damageById, key, 0) > 0)
            {
                return true;
            }
        }
        return false;
    },

    _hasAttackAction : function(unit)
    {
        if (unit === null || unit === undefined)
        {
            return false;
        }
        if (typeof unit.hasAttackAction === "boolean")
        {
            return unit.hasAttackAction;
        }
        if (typeof unit.getActionList !== "function")
        {
            return false;
        }
        for (var actionId in COUNTERPOINTAI.NON_WEAPON_ATTACK_ACTIONS)
        {
            if (COUNTERPOINTAI._hasAction(unit, actionId))
            {
                return true;
            }
        }
        return false;
    },

    _isAirThreat : function(unit)
    {
        return unit !== null && unit !== undefined &&
            unit.domain === COUNTERPOINTAI.DOMAIN_AIR &&
            (COUNTERPOINTAI._hasAttackCapability(unit) ||
             unit.isTransporter !== true && COUNTERPOINTAI._hasAttackAction(unit));
    },

    _isPureIndirect : function(unit)
    {
        return COUNTERPOINTAI._hasIndirectChannel(unit) &&
            !COUNTERPOINTAI._hasDirectChannel(unit);
    },

    _countPureIndirects : function(units)
    {
        var count = 0;
        var length = COUNTERPOINTAI._collectionLength(units);
        for (var index = 0; index < length; ++index)
        {
            var unit = COUNTERPOINTAI._collectionAt(units, index);
            if (unit !== null && unit !== undefined && COUNTERPOINTAI._isPureIndirect(unit))
            {
                count += 1;
            }
        }
        return count;
    },

    _indirectCapBlocks : function(context)
    {
        var cap = COUNTERPOINTAI._tunable("MAX_INDIRECT_UNITS");
        if (!(cap >= 0))
        {
            return false;
        }
        if (COUNTERPOINTAI.MAX_INDIRECT_IGNORE_INDIRECT_CO !== false &&
            context.indirectCo === true)
        {
            return false;
        }
        return COUNTERPOINTAI._readNumber(context, "fieldedIndirects", 0) >= cap;
    },

    _usableDamageAgainst : function(attacker, defenderId, banIndirects)
    {
        if (banIndirects === true && COUNTERPOINTAI._hasDirectChannel(attacker))
        {
            return COUNTERPOINTAI._directDamageAgainst(attacker, defenderId);
        }
        return COUNTERPOINTAI._damageAgainst(attacker, defenderId);
    },

    _phantomDamageTaken : function(phantom, candidate, mapName, indirectChannel)
    {
        var candidateId = COUNTERPOINTAI._unitId(candidate);
        if (phantom !== null && phantom !== undefined &&
            phantom[mapName] !== null && phantom[mapName] !== undefined)
        {
            return Math.max(
                0,
                COUNTERPOINTAI._readUnitNumber(phantom[mapName], candidateId, 0)
            );
        }
        var hasChannel = indirectChannel ?
            COUNTERPOINTAI._hasIndirectChannel(candidate) :
            COUNTERPOINTAI._hasDirectChannel(candidate);
        if (!hasChannel || phantom === null || phantom === undefined ||
            phantom.damageTakenById === null || phantom.damageTakenById === undefined)
        {
            return 0;
        }
        return Math.max(
            0,
            COUNTERPOINTAI._readUnitNumber(phantom.damageTakenById, candidateId, 0)
        );
    },

    _coverageDamage : function(damage, qualityDamage)
    {
        var safeDamage = Math.max(0, COUNTERPOINTAI._finiteNumber(damage, 0));
        return safeDamage * Math.min(1, safeDamage / Math.max(1, qualityDamage));
    },

    _computeOwnCoverage : function(ownComposition, enemyComposition)
    {
        var coverage = Object.create(null);
        var enemyLength = COUNTERPOINTAI._collectionLength(enemyComposition);
        var ownLength = COUNTERPOINTAI._collectionLength(ownComposition);
        var qualityDamage = Math.max(1, COUNTERPOINTAI._tunable("COVERAGE_QUALITY_DAMAGE"));
        for (var enemyIndex = 0; enemyIndex < enemyLength; ++enemyIndex)
        {
            var enemy = COUNTERPOINTAI._collectionAt(enemyComposition, enemyIndex);
            var enemyId = COUNTERPOINTAI._unitId(enemy);
            var enemyKey = COUNTERPOINTAI._unitKey(enemy);
            if (enemy === null || enemy === undefined || enemyId === "")
            {
                continue;
            }
            var total = 0;
            for (var ownIndex = 0; ownIndex < ownLength; ++ownIndex)
            {
                var own = COUNTERPOINTAI._collectionAt(ownComposition, ownIndex);
                var hpSum = Math.max(0, COUNTERPOINTAI._readNumber(own, "hpSum", 0));
                var damage = COUNTERPOINTAI._damageAgainst(own, enemyId);
                total += hpSum * COUNTERPOINTAI._coverageDamage(damage, qualityDamage);
            }
            coverage[enemyKey] = total;
        }
        return coverage;
    },

    _computeAirCoverage : function(ownComposition, enemyComposition)
    {
        var total = 0;
        var ownLength = COUNTERPOINTAI._collectionLength(ownComposition);
        var enemyLength = COUNTERPOINTAI._collectionLength(enemyComposition);
        var qualityDamage = Math.max(1, COUNTERPOINTAI._tunable("COVERAGE_QUALITY_DAMAGE"));
        for (var ownIndex = 0; ownIndex < ownLength; ++ownIndex)
        {
            var own = COUNTERPOINTAI._collectionAt(ownComposition, ownIndex);
            var bestDamage = 0;
            for (var enemyIndex = 0; enemyIndex < enemyLength; ++enemyIndex)
            {
                var enemy = COUNTERPOINTAI._collectionAt(enemyComposition, enemyIndex);
                if (COUNTERPOINTAI._isAirThreat(enemy))
                {
                    bestDamage = Math.max(
                        bestDamage,
                        COUNTERPOINTAI._damageAgainst(own, COUNTERPOINTAI._unitId(enemy))
                    );
                }
            }
            var hpSum = Math.max(0, COUNTERPOINTAI._readNumber(own, "hpSum", 0));
            total += hpSum * Math.min(
                COUNTERPOINTAI.COVERAGE_DAMAGE_SCALE,
                COUNTERPOINTAI._coverageDamage(bestDamage, qualityDamage)
            );
        }
        return total;
    },

    // Use engine damage for buildable phantoms absent from fielded-unit tables.
    _computePhantomCoverage : function(system, ownComposition, phantoms)
    {
        var coverage = Object.create(null);
        var phantomLength = COUNTERPOINTAI._collectionLength(phantoms);
        var ownLength = COUNTERPOINTAI._collectionLength(ownComposition);
        var qualityDamage = Math.max(1, COUNTERPOINTAI._tunable("COVERAGE_QUALITY_DAMAGE"));
        for (var phantomIndex = 0; phantomIndex < phantomLength; ++phantomIndex)
        {
            var phantom = COUNTERPOINTAI._collectionAt(phantoms, phantomIndex);
            var phantomId = COUNTERPOINTAI._unitId(phantom);
            var phantomKey = COUNTERPOINTAI._unitKey(phantom);
            if (phantom === null || phantom === undefined || phantomId === "")
            {
                continue;
            }
            var total = 0;
            for (var ownIndex = 0; ownIndex < ownLength; ++ownIndex)
            {
                var own = COUNTERPOINTAI._collectionAt(ownComposition, ownIndex);
                var ownId = COUNTERPOINTAI._unitId(own);
                if (own === null || own === undefined || ownId === "")
                {
                    continue;
                }
                var hpSum = Math.max(0, COUNTERPOINTAI._readNumber(own, "hpSum", 0));
                var damage = Math.max(
                    0,
                    COUNTERPOINTAI._finiteNumber(
                        system.getCounterpointBaseDamage(ownId, phantomId),
                        0
                    )
                );
                total += hpSum * COUNTERPOINTAI._coverageDamage(damage, qualityDamage);
            }
            coverage[phantomKey] = total;
        }
        return coverage;
    },

    // Share one coverage curve between real and phantom threats.
    _coverageGapFactor : function(coverage, threatNeed, candidateValue)
    {
        var candidateValueUnits = Math.max(1, candidateValue / COUNTERPOINTAI.COST_SCALE);
        var gapFloor = COUNTERPOINTAI.GAP_FLOOR_BASE +
            COUNTERPOINTAI.GAP_FLOOR_COST_FACTOR * candidateValueUnits;
        gapFloor = Math.min(COUNTERPOINTAI.GAP_FLOOR_MAX, gapFloor);
        if (!(threatNeed > 0))
        {
            return 1;
        }
        var coverageRatio = coverage / threatNeed;
        if (coverageRatio > COUNTERPOINTAI.COVERAGE_SATURATION_RATIO)
        {
            gapFloor = Math.max(
                COUNTERPOINTAI.SATURATED_GAP_FLOOR_MIN,
                gapFloor - COUNTERPOINTAI.SATURATED_GAP_REDUCTION
            );
        }
        return Math.max(
            gapFloor,
            Math.min(
                COUNTERPOINTAI.GAP_FACTOR_MAX,
                COUNTERPOINTAI.GAP_FACTOR_MAX - coverageRatio *
                    Math.max(0, COUNTERPOINTAI.GAP_FACTOR_MAX - 1)
            )
        );
    },

    _compositionHasAirThreat : function(composition)
    {
        var length = COUNTERPOINTAI._collectionLength(composition);
        for (var index = 0; index < length; ++index)
        {
            var unit = COUNTERPOINTAI._collectionAt(composition, index);
            if (COUNTERPOINTAI._isAirThreat(unit))
            {
                return true;
            }
        }
        return false;
    },

    _isIslandMode : function(mapContext)
    {
        if (mapContext === null || mapContext === undefined)
        {
            return false;
        }
        // No ground vehicle means shareIslandWithEnemy answered false for want of a probe, not water.
        return (mapContext.groundProbe === true && mapContext.shareIsland === false) ||
               COUNTERPOINTAI._readNumber(mapContext, "enemyIslandBuildings", 0) > 0;
    },

    _isAASpecialist : function(unit)
    {
        if (unit === null || unit === undefined ||
            unit.canCapture === true ||
            COUNTERPOINTAI._strategicValue(unit) < COUNTERPOINTAI.TANK_MIN_BASE_COST)
        {
            return false;
        }
        if (unit.maxDamageVsAir === undefined)
        {
            return unit.isAASpecialist === true;
        }
        var airDamage = COUNTERPOINTAI._readNumber(unit, "maxDamageVsAir", 0);
        var armoredDamage = COUNTERPOINTAI._readNumber(unit, "armoredDamage", 0);
        var qualityDamage = Math.max(
            1,
            COUNTERPOINTAI._tunable("COVERAGE_QUALITY_DAMAGE")
        );
        return airDamage >= qualityDamage &&
            (armoredDamage < COUNTERPOINTAI.TANK_AA_ARMORED_THRESHOLD ||
             airDamage - armoredDamage >= qualityDamage);
    },

    // Answers air without being air, so a gunship cannot stand in for the ground answer. Scoring
    // still treats it as anti-air; this gates only the build target.
    _countsAsAntiAir : function(unit)
    {
        return COUNTERPOINTAI._isAASpecialist(unit) &&
               unit.domain !== COUNTERPOINTAI.DOMAIN_AIR;
    },

    _isTankClass : function(unit)
    {
        if (unit === null || unit === undefined)
        {
            return false;
        }
        return unit.domain === COUNTERPOINTAI.DOMAIN_GROUND &&
               unit.canCapture !== true &&
               unit.isTransporter !== true &&
               !COUNTERPOINTAI._isPureIndirect(unit) &&
               COUNTERPOINTAI._readNumber(unit, "movement", 0) >= COUNTERPOINTAI.TANK_MIN_MOVEMENT &&
               COUNTERPOINTAI._strategicValue(unit) >= COUNTERPOINTAI.TANK_MIN_BASE_COST &&
               !COUNTERPOINTAI._isAASpecialist(unit);
    },

    _isTankCapableTransport : function(unit)
    {
        return unit !== null && unit !== undefined &&
               unit.isTransporter === true && unit.canTransportTank === true;
    },

    _classifyUnit : function(unit)
    {
        return {
            id : COUNTERPOINTAI._unitId(unit),
            domain : unit !== null && unit !== undefined ? unit.domain : "",
            canCapture : unit !== null && unit !== undefined && unit.canCapture === true,
            isTransporter : unit !== null && unit !== undefined && unit.isTransporter === true,
            isIndirect : COUNTERPOINTAI._isPureIndirect(unit),
            isAASpecialist : COUNTERPOINTAI._isAASpecialist(unit),
            isTank : COUNTERPOINTAI._isTankClass(unit),
            canTransportTank : COUNTERPOINTAI._isTankCapableTransport(unit)
        };
    },

    _softcapCapperHP : function(hpSum)
    {
        var hp = Math.max(0, COUNTERPOINTAI._finiteNumber(hpSum, 0));
        if (hp <= COUNTERPOINTAI.CAP_HP_SOFT_CAP)
        {
            return hp;
        }
        return COUNTERPOINTAI.CAP_HP_SOFT_CAP +
               Math.log(1 + hp - COUNTERPOINTAI.CAP_HP_SOFT_CAP) *
               COUNTERPOINTAI.CAP_HP_LOG_SCALE;
    },

    _unitHasOffenseVs : function(unit, enemyComposition, banIndirects)
    {
        var length = COUNTERPOINTAI._collectionLength(enemyComposition);
        if (length === 0)
        {
            return true;
        }
        var validEnemyFound = false;
        for (var index = 0; index < length; ++index)
        {
            var enemyId = COUNTERPOINTAI._unitId(
                COUNTERPOINTAI._collectionAt(enemyComposition, index)
            );
            if (enemyId === "")
            {
                continue;
            }
            validEnemyFound = true;
            if (COUNTERPOINTAI._usableDamageAgainst(unit, enemyId, banIndirects) > 0)
            {
                return true;
            }
        }
        return !validEnemyFound;
    },

    // Only hulls that can carry a ground unit are counted, so a fleet of copter carrying cruisers
    // cannot convince the ramp that the ferries are already built.
    _countOwnTransporters : function(units, groundIds)
    {
        var counts = { ground : 0, air : 0, naval : 0, hover : 0 };
        var length = COUNTERPOINTAI._collectionLength(units);
        for (var index = 0; index < length; ++index)
        {
            var unit = COUNTERPOINTAI._collectionAt(units, index);
            if (unit === null || unit === undefined || unit.isTransporter !== true ||
                !COUNTERPOINTAI._carriesAnyOf(unit.cargoIds, groundIds))
            {
                continue;
            }
            var domain = unit.domain;
            if (domain === COUNTERPOINTAI.DOMAIN_AIR)
            {
                counts.air += 1;
            }
            else if (domain === COUNTERPOINTAI.DOMAIN_NAVAL)
            {
                counts.naval += 1;
            }
            else if (domain === COUNTERPOINTAI.DOMAIN_HOVER)
            {
                counts.hover += 1;
            }
            else
            {
                counts.ground += 1;
            }
        }
        return counts;
    },

    _transportChance : function(ownCount, turn, domain, islandMode)
    {
        var count = COUNTERPOINTAI._wholeCount(ownCount);
        var currentTurn = Math.max(1, Math.floor(COUNTERPOINTAI._finiteNumber(turn, 1)));
        if (islandMode === true)
        {
            if (domain === COUNTERPOINTAI.DOMAIN_GROUND)
            {
                if (count === 0)
                {
                    return Math.min(
                        COUNTERPOINTAI.ISLAND_GROUND_TRANSPORT_MAX_CHANCE,
                        COUNTERPOINTAI.ISLAND_GROUND_TRANSPORT_BASE_CHANCE +
                        currentTurn * COUNTERPOINTAI.ISLAND_GROUND_TRANSPORT_TURN_STEP
                    );
                }
                return count === 1 ?
                    COUNTERPOINTAI.ISLAND_GROUND_TRANSPORT_ONE_CHANCE :
                    COUNTERPOINTAI.ISLAND_GROUND_TRANSPORT_MANY_CHANCE;
            }
            if (count === 0)
            {
                return Math.min(
                    COUNTERPOINTAI.ISLAND_TRANSPORT_MAX_CHANCE,
                    COUNTERPOINTAI.ISLAND_TRANSPORT_BASE_CHANCE +
                    currentTurn * COUNTERPOINTAI.ISLAND_TRANSPORT_TURN_STEP
                );
            }
            if (count === 1)
            {
                return COUNTERPOINTAI.ISLAND_TRANSPORT_ONE_CHANCE;
            }
            if (count === 2)
            {
                return COUNTERPOINTAI.ISLAND_TRANSPORT_TWO_CHANCE;
            }
            if (count === 3)
            {
                return COUNTERPOINTAI.ISLAND_TRANSPORT_THREE_CHANCE;
            }
            if (count === 4)
            {
                return COUNTERPOINTAI.ISLAND_TRANSPORT_FOUR_CHANCE;
            }
            return COUNTERPOINTAI.ISLAND_TRANSPORT_MANY_CHANCE;
        }
        if (count === 0)
        {
            return Math.min(
                COUNTERPOINTAI.CONNECTED_TRANSPORT_MAX_CHANCE,
                COUNTERPOINTAI.CONNECTED_TRANSPORT_BASE_CHANCE +
                currentTurn * COUNTERPOINTAI.CONNECTED_TRANSPORT_TURN_STEP
            );
        }
        return count === 1 ?
            COUNTERPOINTAI.CONNECTED_TRANSPORT_ONE_CHANCE :
            COUNTERPOINTAI.CONNECTED_TRANSPORT_MANY_CHANCE;
    },

    _strategicValue : function(unit)
    {
        var value = COUNTERPOINTAI._readNumber(unit, "strategicValue", 0);
        return Math.max(1, value);
    },

    _sumTopN : function(contributions, weights)
    {
        var sorted = contributions === null || contributions === undefined ?
            [] : contributions.slice();
        sorted.sort(function(left, right)
        {
            return right - left;
        });
        var total = 0;
        var length = Math.min(sorted.length, weights.length);
        for (var index = 0; index < length; ++index)
        {
            total += sorted[index] * weights[index];
        }
        return total;
    },

    _contextNumber : function(context, mapName, unitId, fallback)
    {
        if (context === null || context === undefined ||
            context[mapName] === null || context[mapName] === undefined)
        {
            return fallback;
        }
        return COUNTERPOINTAI._readUnitNumber(context[mapName], unitId, fallback);
    },

    _ownCount : function(context, unitId)
    {
        return Math.max(0, COUNTERPOINTAI._contextNumber(context, "ownCounts", unitId, 0));
    },

    _navalOffenseFactor : function(candidate, candidateId, targetDomain, scoreContext,
                                    indirectChannel)
    {
        if (candidate.domain !== COUNTERPOINTAI.DOMAIN_NAVAL ||
            targetDomain !== COUNTERPOINTAI.DOMAIN_GROUND ||
            scoreContext.islandMode === true)
        {
            return 1;
        }
        var effectiveRange = indirectChannel === false ? 1 :
            COUNTERPOINTAI._readNumber(candidate, "maxRange", 1) +
                COUNTERPOINTAI._contextNumber(
                    scoreContext,
                    "indirectRangeDeltas",
                    candidateId,
                    0
                );
        if (effectiveRange <= 1)
        {
            return COUNTERPOINTAI.NAVAL_VS_LAND_DIRECT_PENALTY;
        }
        if (effectiveRange < COUNTERPOINTAI.NAVAL_VS_LAND_RANGE_CUTOFF)
        {
            return COUNTERPOINTAI.NAVAL_VS_LAND_SHORT_PENALTY;
        }
        return 1;
    },

    _indirectChannelFactor : function(candidateId, ownCount, scoreContext)
    {
        var rangeDelta = COUNTERPOINTAI._contextNumber(
            scoreContext,
            "indirectRangeDeltas",
            candidateId,
            0
        );
        if (scoreContext.banIndirects === true || rangeDelta < 0)
        {
            return 0;
        }
        var factor = rangeDelta < 1 ? COUNTERPOINTAI.INDIRECT_TAX :
            COUNTERPOINTAI.INDIRECT_SPECIALIST_BONUS;
        if (rangeDelta < 1 && ownCount >= 1)
        {
            factor *= Math.pow(COUNTERPOINTAI.INDIRECT_STACK_PENALTY, ownCount);
        }
        return COUNTERPOINTAI._safeFactor(factor);
    },

    _offenseCounterFactor : function(damage, valueRatio)
    {
        if (damage < COUNTERPOINTAI.HARD_COUNTER_DAMAGE)
        {
            return 1;
        }
        return valueRatio > COUNTERPOINTAI.EXPENSIVE_COUNTER_COST_RATIO ?
            COUNTERPOINTAI.EXPENSIVE_COUNTER_FACTOR : COUNTERPOINTAI.HARD_COUNTER_FACTOR;
    },

    _defenseCounterFactor : function(damage)
    {
        return damage >= COUNTERPOINTAI.HARD_COUNTER_DAMAGE ?
            COUNTERPOINTAI.HARD_COUNTER_FACTOR : 1;
    },

    _enemyChannelDefense : function(enemy, candidateId, enemyHp, threatFactor,
                                     adjacencyScale, candidateIsTank)
    {
        var directDamage = COUNTERPOINTAI._directDamageAgainst(enemy, candidateId);
        var directDefense = enemyHp * adjacencyScale * directDamage * threatFactor *
            COUNTERPOINTAI._defenseCounterFactor(directDamage);
        var indirectDamage = COUNTERPOINTAI._indirectDamageAgainst(enemy, candidateId);
        var indirectDefense = enemyHp * indirectDamage * threatFactor *
            COUNTERPOINTAI._defenseCounterFactor(indirectDamage);
        if (candidateIsTank)
        {
            indirectDefense *= COUNTERPOINTAI.TANK_VS_INDIRECT_DEF_MUL;
        }
        return Math.max(directDefense, indirectDefense);
    },

    _hybridMatchupChoice : function(directOffense, indirectOffense, offenseScale,
                                     baseDefense, indirectFactor)
    {
        var directContribution = directOffense * offenseScale;
        var indirectContribution = indirectOffense * offenseScale;
        if (indirectOffense <= 0 || indirectFactor <= 0)
        {
            return {
                indirect : false,
                offense : directContribution,
                rawDefense : baseDefense,
                defense : baseDefense
            };
        }
        var indirectRawDefense = baseDefense;
        var indirectDefense = baseDefense * COUNTERPOINTAI.INDIRECT_DEFENSE_FACTOR;
        var directScore = directContribution - baseDefense;
        var indirectDiscountedScore = indirectContribution - indirectDefense;
        var indirectScore = indirectDiscountedScore > 0 ?
            indirectDiscountedScore : indirectContribution - indirectRawDefense;
        var contributionFactor = indirectScore > 0 ?
            indirectFactor : 1 / indirectFactor;
        indirectContribution *= contributionFactor;
        indirectRawDefense *= contributionFactor;
        indirectDefense *= contributionFactor;
        var indirect = COUNTERPOINTAI._flipFactor(indirectScore, indirectFactor) > directScore;
        return {
            indirect : indirect,
            offense : indirect ? indirectContribution : directContribution,
            rawDefense : indirect ? indirectRawDefense : baseDefense,
            defense : indirect ? indirectDefense : baseDefense
        };
    },

    _hybridChannelOffense : function(candidate, candidateId, enemy, damage, indirectChannel,
                                      valueRatio, ownCount, scoreContext)
    {
        if (damage <= 0)
        {
            return 0;
        }
        var factor = COUNTERPOINTAI._offenseCounterFactor(damage, valueRatio) *
            COUNTERPOINTAI._navalOffenseFactor(
                candidate,
                candidateId,
                enemy.domain,
                scoreContext,
                indirectChannel
            ) *
            COUNTERPOINTAI._offenseMobilityFactor(
                indirectChannel,
                Math.max(0, COUNTERPOINTAI._readNumber(candidate, "movement", 0)),
                Math.max(0, COUNTERPOINTAI._readNumber(enemy, "movement", 0))
            );
        return damage * factor;
    },

    // Keep absolute reach in _intrinsicMobilityFactor to avoid double counting.
    _offenseMobilityFactor : function(isIndirect, candidateMovement, targetMovement)
    {
        if (isIndirect || candidateMovement <= 0)
        {
            return 1;
        }
        if (targetMovement > candidateMovement)
        {
            return COUNTERPOINTAI.MOBILITY_BASE_FACTOR +
                COUNTERPOINTAI.MOBILITY_RATIO_FACTOR * candidateMovement / targetMovement;
        }
        return 1;
    },

    _intrinsicMobilityFactor : function(candidateMovement, reference)
    {
        var move = Math.max(0, COUNTERPOINTAI._finiteNumber(candidateMovement, 0));
        var ref = reference > 0 ? reference :
            COUNTERPOINTAI._tunable("MOBILITY_VALUE_REFERENCE");
        if (ref <= 0)
        {
            return 1;
        }
        if (move <= 0)
        {
            return COUNTERPOINTAI._tunable("MOBILITY_VALUE_FLOOR");
        }
        var factor = Math.pow(move / ref, COUNTERPOINTAI._tunable("MOBILITY_VALUE_SLOPE"));
        return COUNTERPOINTAI._clamp(
            factor,
            COUNTERPOINTAI._tunable("MOBILITY_VALUE_FLOOR"),
            COUNTERPOINTAI._tunable("MOBILITY_VALUE_CEIL")
        );
    },

    // Use the roster median so extreme units cannot distort the movement reference.
    _medianMovement : function(candidates)
    {
        var length = COUNTERPOINTAI._collectionLength(candidates);
        if (length === 0)
        {
            return 0;
        }
        var moves = [];
        for (var index = 0; index < length; ++index)
        {
            var move = Math.max(0, COUNTERPOINTAI._readNumber(
                COUNTERPOINTAI._collectionAt(candidates, index), "movement", 0));
            if (move > 0)
            {
                moves.push(move);
            }
        }
        if (moves.length === 0)
        {
            return 0;
        }
        moves.sort(function(left, right) { return left - right; });
        var mid = Math.floor(moves.length / 2);
        return moves.length % 2 === 0 ? (moves[mid - 1] + moves[mid]) / 2 : moves[mid];
    },

    _scoreUnitAgainstEnemies : function(candidate, enemyComposition, context)
    {
        var enemies = enemyComposition || [];
        var enemyLength = COUNTERPOINTAI._collectionLength(enemies);
        var scoreContext = context || {};
        var phantomLength = COUNTERPOINTAI._collectionLength(
            scoreContext.phantomThreats
        );
        if (candidate === null || candidate === undefined ||
            (enemyLength === 0 && phantomLength === 0))
        {
            return 0;
        }
        var candidateId = COUNTERPOINTAI._unitId(candidate);
        var candidateValue = COUNTERPOINTAI._strategicValue(candidate);
        var candidateMovement = Math.max(0, COUNTERPOINTAI._readNumber(candidate, "movement", 0));
        var hasDirectChannel = COUNTERPOINTAI._hasDirectChannel(candidate);
        var hasIndirectChannel = COUNTERPOINTAI._hasIndirectChannel(candidate);
        var isHybrid = hasDirectChannel && hasIndirectChannel;
        var isIndirect = hasIndirectChannel && !hasDirectChannel;
        var isTank = COUNTERPOINTAI._isTankClass(candidate);
        var ownCount = COUNTERPOINTAI._ownCount(scoreContext, candidateId);

        // A transporter that cannot take a ground unit is no ferry, whatever its cargo bay says, so
        // it falls through and is scored on its guns like any other warship.
        if (scoreContext.islandMode === true && candidate.isTransporter === true &&
            COUNTERPOINTAI._readFlag(scoreContext.groundCarriers, candidateId))
        {
            // Scored from a fixed reference rather than the hull's own price. A cost proportional
            // score cancels against the division by cost in _netDamageWeights, leaving every hull
            // with identical efficiency and the pick decided by the cost term alone, which rises.
            // That preferred a transport purely for being expensive.
            var transportScore = COUNTERPOINTAI.COST_SCALE *
                COUNTERPOINTAI.TRANSPORT_BASE_SCORE_FACTOR;
            if (candidate.domain === COUNTERPOINTAI.DOMAIN_NAVAL ||
                candidate.domain === COUNTERPOINTAI.DOMAIN_HOVER ||
                candidate.domain === COUNTERPOINTAI.DOMAIN_AIR)
            {
                transportScore *= COUNTERPOINTAI._safeFactor(
                    COUNTERPOINTAI.ISLAND_NAVAL_TRANSPORT_BONUS
                );
            }
            var ferryStats = scoreContext.tankFerryStats || {};
            if (COUNTERPOINTAI._isTankCapableTransport(candidate) &&
                COUNTERPOINTAI._readNumber(ferryStats, "tanks", 0) >
                COUNTERPOINTAI._readNumber(ferryStats, "capacity", 0))
            {
                transportScore *= COUNTERPOINTAI._safeFactor(
                    COUNTERPOINTAI.ISLAND_TANK_FERRY_DEMAND_BOOST
                );
            }
            if (ownCount >= 1)
            {
                transportScore *= Math.pow(
                    COUNTERPOINTAI._safeFactor(COUNTERPOINTAI.ISLAND_TRANSPORT_DIVERSITY),
                    ownCount
                );
            }
            return transportScore;
        }

        // Weak attackers must not dilute hard-counter retaliation.
        var adjacencyDamageNorm = Math.max(1, COUNTERPOINTAI._tunable("ADJACENCY_DAMAGE_NORM"));
        var directHp = 0;
        for (var prepassIndex = 0; prepassIndex < enemyLength; ++prepassIndex)
        {
            var prepassEnemy = COUNTERPOINTAI._collectionAt(enemies, prepassIndex);
            if (prepassEnemy === null || prepassEnemy === undefined ||
                COUNTERPOINTAI._unitId(prepassEnemy) === "")
            {
                continue;
            }
            if (!COUNTERPOINTAI._hasDirectChannel(prepassEnemy))
            {
                continue;
            }
            var prepassHp = Math.max(0, COUNTERPOINTAI._readNumber(prepassEnemy, "hpSum", 0));
            if (prepassEnemy.canCapture === true)
            {
                prepassHp = COUNTERPOINTAI._softcapCapperHP(prepassHp);
            }
            directHp += prepassHp * Math.min(
                1,
                COUNTERPOINTAI._directDamageAgainst(prepassEnemy, candidateId) /
                    adjacencyDamageNorm
            );
        }
        var adjacencyScale = directHp > COUNTERPOINTAI.ADJACENCY_RETAL_CAP ?
            COUNTERPOINTAI.ADJACENCY_RETAL_CAP / directHp : 1;
        var offenseContributions = [];
        var defenseContributions = [];
        var rawDefenseContributions = [];
        var threatProfile = scoreContext.threatProfile ||
            COUNTERPOINTAI._analyzeEnemyComp(enemies);
        var airNeedTotal = 0;
        var airCovTotal = Math.max(
            0,
            COUNTERPOINTAI._readNumber(scoreContext, "ownAirCoverage", 0)
        );

        for (var enemyIndex = 0; enemyIndex < enemyLength; ++enemyIndex)
        {
            var enemy = COUNTERPOINTAI._collectionAt(enemies, enemyIndex);
            var enemyId = COUNTERPOINTAI._unitId(enemy);
            var enemyKey = COUNTERPOINTAI._unitKey(enemy);
            if (enemy === null || enemy === undefined || enemyId === "")
            {
                continue;
            }
            var enemyHp = Math.max(0, COUNTERPOINTAI._readNumber(enemy, "hpSum", 0));
            if (enemy.canCapture === true)
            {
                enemyHp = COUNTERPOINTAI._softcapCapperHP(enemyHp);
            }
            var offense = COUNTERPOINTAI._damageAgainst(candidate, enemyId);
            var enemyValue = COUNTERPOINTAI._strategicValue(enemy);
            var threatFactor = Math.max(enemyValue, COUNTERPOINTAI.THREAT_VALUE_FLOOR) /
                COUNTERPOINTAI.COST_SCALE;
            if (enemy.canCapture === true && enemyValue <= COUNTERPOINTAI.CHEAP_CAPPER_MAX_COST)
            {
                threatFactor *= COUNTERPOINTAI.CAP_THREAT_MULTIPLIER;
            }
            var baseDefense = COUNTERPOINTAI._enemyChannelDefense(
                enemy,
                candidateId,
                enemyHp,
                threatFactor,
                adjacencyScale,
                isTank
            );
            var valueRatio = candidateValue / enemyValue;
            var hybridDirectOffense = 0;
            var hybridIndirectOffense = 0;
            var offenseCounterFactor = COUNTERPOINTAI._offenseCounterFactor(
                offense,
                valueRatio
            );
            var mobilityFactor = COUNTERPOINTAI._offenseMobilityFactor(
                isIndirect,
                candidateMovement,
                Math.max(0, COUNTERPOINTAI._readNumber(enemy, "movement", 0))
            );
            if (isHybrid)
            {
                hybridDirectOffense = COUNTERPOINTAI._hybridChannelOffense(
                    candidate,
                    candidateId,
                    enemy,
                    COUNTERPOINTAI._directDamageAgainst(candidate, enemyId),
                    false,
                    valueRatio,
                    ownCount,
                    scoreContext
                );
                hybridIndirectOffense = COUNTERPOINTAI._hybridChannelOffense(
                    candidate,
                    candidateId,
                    enemy,
                    COUNTERPOINTAI._indirectDamageAgainst(candidate, enemyId),
                    true,
                    valueRatio,
                    ownCount,
                    scoreContext
                );
                offense = Math.max(hybridDirectOffense, hybridIndirectOffense);
                offenseCounterFactor = 1;
                mobilityFactor = 1;
            }
            else if (offense > 0)
            {
                offense *= COUNTERPOINTAI._navalOffenseFactor(
                    candidate,
                    candidateId,
                    enemy.domain,
                    scoreContext,
                    isIndirect
                );
            }

            var roleFactor = 1;
            var roleCoefficient = scoreContext.islandMode === true ?
                COUNTERPOINTAI.ROLE_SHARE_FACTOR *
                COUNTERPOINTAI._safeFactor(COUNTERPOINTAI.ISLAND_ROLE_NUDGE_MULTIPLIER) :
                COUNTERPOINTAI.ROLE_SHARE_FACTOR;
            if (enemy.domain === COUNTERPOINTAI.DOMAIN_AIR)
            {
                roleFactor += roleCoefficient *
                    COUNTERPOINTAI._readNumber(threatProfile, "airShare", 0);
            }
            if (enemy.domain === COUNTERPOINTAI.DOMAIN_NAVAL)
            {
                roleFactor += roleCoefficient *
                    COUNTERPOINTAI._readNumber(threatProfile, "navalShare", 0);
            }
            var enemyIsIndirect = COUNTERPOINTAI._isPureIndirect(enemy);
            if (enemyIsIndirect)
            {
                roleFactor += COUNTERPOINTAI.INDIRECT_ROLE_SHARE_FACTOR *
                    COUNTERPOINTAI._readNumber(threatProfile, "indirectShare", 0);
            }

            var coverage = COUNTERPOINTAI._contextNumber(
                scoreContext,
                "ownCoverage",
                enemyKey,
                0
            );
            var threatNeed = enemyHp * COUNTERPOINTAI.COVERAGE_DAMAGE_SCALE;
            var gapFactor = COUNTERPOINTAI._coverageGapFactor(
                coverage,
                threatNeed,
                candidateValue
            );
            if (COUNTERPOINTAI._isAirThreat(enemy))
            {
                airNeedTotal += threatNeed;
            }

            var offenseContribution = 0;
            var defenseContribution = 0;
            var rawDefenseContribution = 0;
            if (isHybrid)
            {
                var indirectChannelFactor = COUNTERPOINTAI._indirectChannelFactor(
                    candidateId,
                    ownCount,
                    scoreContext
                );
                var hybridChoice = COUNTERPOINTAI._hybridMatchupChoice(
                    hybridDirectOffense,
                    hybridIndirectOffense,
                    enemyHp * threatFactor * roleFactor * gapFactor,
                    baseDefense,
                    indirectChannelFactor
                );
                offenseContribution = hybridChoice.offense;
                rawDefenseContribution = hybridChoice.rawDefense;
                defenseContribution = hybridChoice.defense;
            }
            else if (offense > 0)
            {
                offenseContribution = enemyHp * offense * threatFactor *
                    offenseCounterFactor * roleFactor * gapFactor * mobilityFactor;
            }
            offenseContributions.push(offenseContribution);

            if (!isHybrid && baseDefense > 0)
            {
                rawDefenseContribution = baseDefense;
                defenseContribution = rawDefenseContribution *
                    (isIndirect ? COUNTERPOINTAI.INDIRECT_DEFENSE_FACTOR : 1);
            }
            defenseContributions.push(defenseContribution);
            rawDefenseContributions.push(rawDefenseContribution);
        }

        // Phantoms prevent the current roster from hiding an affordable counter-buy.
        var phantoms = scoreContext.phantomThreats || [];
        var phantomWeight = COUNTERPOINTAI._tunable("PHANTOM_RETAL_WEIGHT");
        var phantomOffenseWeight = COUNTERPOINTAI._tunable("PHANTOM_OFFENSE_WEIGHT");
        if ((phantomWeight > 0 || phantomOffenseWeight > 0) && phantoms.length > 0)
        {
            var phantomWorst = 0;
            var phantomDiscountedWorst = 0;
            var phantomBest = 0;
            for (var phantomIndex = 0; phantomIndex < phantoms.length; ++phantomIndex)
            {
                var phantom = phantoms[phantomIndex];
                var phantomValue = COUNTERPOINTAI._strategicValue(phantom);
                var phantomThreat = Math.max(
                    phantomValue,
                    COUNTERPOINTAI.THREAT_VALUE_FLOOR
                ) / COUNTERPOINTAI.COST_SCALE;
                if (phantom.canCapture === true &&
                    phantomValue <= COUNTERPOINTAI.CHEAP_CAPPER_MAX_COST)
                {
                    phantomThreat *= COUNTERPOINTAI.CAP_THREAT_MULTIPLIER;
                }
                // Credit only phantom threats not already covered by the roster.
                var phantomGapFactor = COUNTERPOINTAI._coverageGapFactor(
                    COUNTERPOINTAI._contextNumber(
                        scoreContext,
                        "phantomCoverage",
                        COUNTERPOINTAI._unitKey(phantom),
                        0
                    ),
                    COUNTERPOINTAI.PHANTOM_RETAL_HP * COUNTERPOINTAI.COVERAGE_DAMAGE_SCALE,
                    candidateValue
                );
                var phantomDefense = COUNTERPOINTAI._enemyChannelDefense(
                    phantom,
                    candidateId,
                    COUNTERPOINTAI.PHANTOM_RETAL_HP,
                    phantomThreat,
                    1,
                    isTank
                );
                var phantomChoice = {
                    indirect : false,
                    offense : 0,
                    rawDefense : phantomDefense,
                    defense : phantomDefense
                };
                var phantomOffenseValue = 0;
                if (phantom.isTransporter !== true)
                {
                    var phantomDirectOffense = COUNTERPOINTAI._phantomDamageTaken(
                        phantom,
                        candidate,
                        "directDamageTakenById",
                        false
                    );
                    var phantomIndirectOffense = COUNTERPOINTAI._phantomDamageTaken(
                        phantom,
                        candidate,
                        "indirectDamageTakenById",
                        true
                    );
                    if (isHybrid)
                    {
                        var phantomDirectValue = COUNTERPOINTAI._hybridChannelOffense(
                            candidate,
                            candidateId,
                            phantom,
                            phantomDirectOffense,
                            false,
                            candidateValue / phantomValue,
                            ownCount,
                            scoreContext
                        );
                        var phantomIndirectValue = COUNTERPOINTAI._hybridChannelOffense(
                            candidate,
                            candidateId,
                            phantom,
                            phantomIndirectOffense,
                            true,
                            candidateValue / phantomValue,
                            ownCount,
                            scoreContext
                        );
                        phantomChoice = COUNTERPOINTAI._hybridMatchupChoice(
                            phantomDirectValue,
                            phantomIndirectValue,
                            COUNTERPOINTAI.PHANTOM_RETAL_HP * phantomThreat,
                            COUNTERPOINTAI._enemyChannelDefense(
                                phantom,
                                candidateId,
                                COUNTERPOINTAI.PHANTOM_RETAL_HP,
                                phantomThreat,
                                1,
                                isTank
                            ),
                            COUNTERPOINTAI._indirectChannelFactor(
                                candidateId,
                                ownCount,
                                scoreContext
                            )
                        );
                        phantomOffenseValue = phantomChoice.offense;
                    }
                    else
                    {
                        var phantomOffense = isIndirect ?
                            phantomIndirectOffense : phantomDirectOffense;
                        phantomOffenseValue = phantomOffense *
                            COUNTERPOINTAI._offenseCounterFactor(
                                phantomOffense,
                                candidateValue / phantomValue
                            ) *
                            COUNTERPOINTAI._navalOffenseFactor(
                                candidate,
                                candidateId,
                                phantom.domain,
                                scoreContext,
                                isIndirect
                            ) *
                            COUNTERPOINTAI._offenseMobilityFactor(
                                isIndirect,
                                candidateMovement,
                                Math.max(0, COUNTERPOINTAI._readNumber(phantom, "movement", 0))
                            );
                    }
                }
                if (isHybrid)
                {
                    phantomWorst = Math.max(phantomWorst, phantomChoice.rawDefense);
                    phantomDiscountedWorst = Math.max(
                        phantomDiscountedWorst,
                        phantomChoice.defense
                    );
                    phantomBest = Math.max(
                        phantomBest,
                        phantomOffenseValue * phantomGapFactor
                    );
                }
                else
                {
                    phantomWorst = Math.max(phantomWorst, phantomDefense);
                    phantomDiscountedWorst = Math.max(
                        phantomDiscountedWorst,
                        phantomDefense * (isIndirect ?
                            COUNTERPOINTAI.INDIRECT_DEFENSE_FACTOR : 1)
                    );
                    phantomBest = Math.max(
                        phantomBest,
                        COUNTERPOINTAI.PHANTOM_RETAL_HP * phantomThreat *
                            phantomOffenseValue * phantomGapFactor
                    );
                }
            }
            if (phantomWeight > 0 && phantomWorst > 0)
            {
                var phantomRaw = phantomWorst * phantomWeight;
                rawDefenseContributions.push(phantomRaw);
                defenseContributions.push(phantomDiscountedWorst * phantomWeight);
            }
            if (phantomOffenseWeight > 0 && phantomBest > 0)
            {
                offenseContributions.push(phantomBest * phantomOffenseWeight);
            }
        }

        var offenseScore = COUNTERPOINTAI._sumTopN(
            offenseContributions,
            COUNTERPOINTAI.TOP_N_WEIGHTS
        );
        var defenseScore = COUNTERPOINTAI._sumTopN(
            defenseContributions,
            COUNTERPOINTAI.TOP_N_WEIGHTS
        );
        var rawDefenseScore = COUNTERPOINTAI._sumTopN(
            rawDefenseContributions,
            COUNTERPOINTAI.TOP_N_WEIGHTS
        );
        var discountedScore = offenseScore - defenseScore;
        var score = discountedScore > 0 ? discountedScore : offenseScore - rawDefenseScore;
        var indirectRangeDelta = COUNTERPOINTAI._contextNumber(
            scoreContext,
            "indirectRangeDeltas",
            candidateId,
            0
        );
        if (isIndirect && indirectRangeDelta < 1)
        {
            score = COUNTERPOINTAI._flipFactor(score, COUNTERPOINTAI.INDIRECT_TAX);
        }
        else if (isIndirect)
        {
            score = COUNTERPOINTAI._flipFactor(
                score,
                COUNTERPOINTAI.INDIRECT_SPECIALIST_BONUS
            );
        }
        if (isTank)
        {
            score = COUNTERPOINTAI._flipFactor(score, COUNTERPOINTAI.TANK_BONUS);
        }
        // Apply absolute reach once here; per-enemy mobility already handles being outrun.
        var mobilityMovement = candidate.domain === COUNTERPOINTAI.DOMAIN_AIR ?
            candidateMovement * COUNTERPOINTAI._tunable("MOBILITY_AIR_REACH_BONUS") :
            candidateMovement;
        score = COUNTERPOINTAI._flipFactor(
            score,
            COUNTERPOINTAI._intrinsicMobilityFactor(
                mobilityMovement,
                COUNTERPOINTAI._readNumber(scoreContext, "mobilityReference", 0)
            )
        );
        // Demote AA when no fielded or buildable attack-air exists.
        if (COUNTERPOINTAI._isAASpecialist(candidate) &&
            COUNTERPOINTAI._readNumber(threatProfile, "attackAirShare", 0) <= 0 &&
            scoreContext.phantomAirPresent !== true)
        {
            score = COUNTERPOINTAI._flipFactor(
                score,
                COUNTERPOINTAI._tunable("AA_NO_AIR_DISCOUNT")
            );
        }
        if (COUNTERPOINTAI._isAASpecialist(candidate) && airNeedTotal > 0)
        {
            var airShortfall = COUNTERPOINTAI._clamp(1 - airCovTotal / airNeedTotal, 0, 1);
            if (airShortfall > 0)
            {
                var airShare = Math.max(
                    COUNTERPOINTAI.AA_SHARE_FLOOR,
                    COUNTERPOINTAI._readNumber(threatProfile, "attackAirShare", 0)
                );
                score = COUNTERPOINTAI._flipFactor(
                    score,
                    Math.min(
                        COUNTERPOINTAI._tunable("AA_COVERAGE_BOOST_MAX"),
                        1 + COUNTERPOINTAI._tunable("AA_COVERAGE_URGENCY") * airShortfall / airShare
                    )
                );
            }
        }
        if (scoreContext.islandMode === true)
        {
            if (candidate.domain === COUNTERPOINTAI.DOMAIN_GROUND &&
                candidate.canCapture !== true && candidate.isTransporter !== true)
            {
                score = COUNTERPOINTAI._flipFactor(
                    score,
                    isTank ? COUNTERPOINTAI.ISLAND_GROUND_TANK_PENALTY :
                        COUNTERPOINTAI.ISLAND_GROUND_SUPPORT_PENALTY
                );
            }
            else if (candidate.domain === COUNTERPOINTAI.DOMAIN_NAVAL &&
                     candidate.isTransporter !== true)
            {
                score = COUNTERPOINTAI._flipFactor(score, COUNTERPOINTAI.ISLAND_NAVAL_BONUS);
            }
            else if (candidate.domain === COUNTERPOINTAI.DOMAIN_AIR &&
                     candidate.isTransporter !== true)
            {
                score = COUNTERPOINTAI._flipFactor(score, COUNTERPOINTAI.ISLAND_AIR_BONUS);
            }
        }
        if (ownCount >= 1)
        {
            score = COUNTERPOINTAI._flipFactor(
                score,
                Math.pow(COUNTERPOINTAI.UNIT_DIVERSITY_FACTOR, ownCount)
            );
        }
        if (isIndirect && indirectRangeDelta < 1 && ownCount >= 1)
        {
            score = COUNTERPOINTAI._flipFactor(
                score,
                Math.pow(COUNTERPOINTAI.INDIRECT_STACK_PENALTY, ownCount)
            );
            // Deduct saturation so indirect scores can fall below negative alternatives.
            var surplusCopies = ownCount -
                COUNTERPOINTAI._tunable("INDIRECT_SATURATION_FREE_COUNT");
            if (surplusCopies > 0)
            {
                score -= COUNTERPOINTAI._tunable("INDIRECT_SATURATION_COST_WEIGHT") *
                    surplusCopies *
                    Math.max(1, candidateValue / COUNTERPOINTAI.COST_SCALE);
            }
        }
        return COUNTERPOINTAI._finiteNumber(score, 0);
    },

    _normalizeWeights : function(rawWeights)
    {
        var length = COUNTERPOINTAI._collectionLength(rawWeights);
        if (length === 0 || length > COUNTERPOINTAI._candidateLimit())
        {
            return { weights : [], total : 0 };
        }
        var sanitized = [];
        var largest = 0;
        var positiveCount = 0;
        var invalidCount = 0;
        for (var index = 0; index < length; ++index)
        {
            var rawValue = COUNTERPOINTAI._collectionAt(rawWeights, index);
            var raw = typeof rawValue === "number" ? rawValue : NaN;
            var value = raw;
            if (raw > 0 && !isFinite(raw))
            {
                value = Number.MAX_VALUE;
            }
            else if (!isFinite(raw) || raw < 0)
            {
                value = 0;
                invalidCount += 1;
            }
            sanitized.push(value);
            if (value > 0)
            {
                positiveCount += 1;
                largest = Math.max(largest, value);
            }
        }
        if (positiveCount === 0)
        {
            if (invalidCount !== length)
            {
                return { weights : sanitized, total : 0 };
            }
            var rankFallback = [];
            for (var fallbackIndex = 0; fallbackIndex < length; ++fallbackIndex)
            {
                rankFallback.push(length - fallbackIndex);
            }
            return COUNTERPOINTAI._normalizeWeights(rankFallback);
        }

        var relativeTotal = 0;
        var relative = [];
        for (var relativeIndex = 0; relativeIndex < length; ++relativeIndex)
        {
            var relativeWeight = sanitized[relativeIndex] > 0 ?
                sanitized[relativeIndex] / largest : 0;
            relative.push(relativeWeight);
            relativeTotal += relativeWeight;
        }
        var weightLimit = Math.max(positiveCount, COUNTERPOINTAI._randomWeightLimit());
        var bonusBudget = weightLimit - positiveCount;
        var remainingBonus = bonusBudget;
        var weights = [];
        var total = 0;
        for (var weightIndex = 0; weightIndex < length; ++weightIndex)
        {
            var weight = 0;
            if (relative[weightIndex] > 0)
            {
                var bonus = Math.floor(
                    relative[weightIndex] * bonusBudget / relativeTotal
                );
                bonus = Math.min(bonus, remainingBonus);
                remainingBonus -= bonus;
                weight = 1 + bonus;
            }
            weights.push(weight);
            total += weight;
        }
        return { weights : weights, total : total };
    },

    _costWeights : function(candidates)
    {
        var length = COUNTERPOINTAI._collectionLength(candidates);
        if (length > COUNTERPOINTAI._candidateLimit())
        {
            return { weights : [], total : 0 };
        }
        var rawWeights = [];
        for (var index = 0; index < length; ++index)
        {
            var candidate = COUNTERPOINTAI._collectionAt(candidates, index);
            var weight = Math.floor(Math.pow(
                COUNTERPOINTAI._strategicValue(candidate),
                COUNTERPOINTAI.COST_WEIGHT_EXPONENT
            ));
            rawWeights.push(Math.max(1, weight));
        }
        return COUNTERPOINTAI._normalizeWeights(rawWeights);
    },

    _inverseCostWeights : function(candidates, turn)
    {
        var length = COUNTERPOINTAI._collectionLength(candidates);
        if (length > COUNTERPOINTAI._candidateLimit())
        {
            return { weights : [], total : 0 };
        }
        if (length === 0)
        {
            return { weights : [], total : 0 };
        }
        if (length === 1)
        {
            return COUNTERPOINTAI._normalizeWeights([1]);
        }
        var cheapestIndex = 0;
        for (var index = 1; index < length; ++index)
        {
            if (COUNTERPOINTAI._strategicValue(
                    COUNTERPOINTAI._collectionAt(candidates, index)
                ) < COUNTERPOINTAI._strategicValue(
                    COUNTERPOINTAI._collectionAt(candidates, cheapestIndex)
                ))
            {
                cheapestIndex = index;
            }
        }

        var currentTurn = Math.max(1, Math.floor(COUNTERPOINTAI._finiteNumber(turn, 1)));
        var biasFraction = COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_TIGHT;
        if (currentTurn >= COUNTERPOINTAI.CHEAPEST_CAPPER_TURN_GATE)
        {
            if (length >= COUNTERPOINTAI.CAPPER_POOL_VERY_WIDE)
            {
                biasFraction = COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_FLOOD;
            }
            else if (length >= COUNTERPOINTAI.CAPPER_POOL_WIDE)
            {
                var highSpan = COUNTERPOINTAI.CAPPER_POOL_VERY_WIDE -
                    COUNTERPOINTAI.CAPPER_POOL_WIDE;
                var highProgress = (length - COUNTERPOINTAI.CAPPER_POOL_WIDE) / highSpan;
                biasFraction = COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_BROAD -
                    highProgress * (COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_BROAD -
                    COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_FLOOD);
            }
            else if (length > COUNTERPOINTAI.CAPPER_POOL_NARROW)
            {
                var lowSpan = COUNTERPOINTAI.CAPPER_POOL_WIDE -
                    COUNTERPOINTAI.CAPPER_POOL_NARROW;
                var lowProgress = (length - COUNTERPOINTAI.CAPPER_POOL_NARROW) / lowSpan;
                biasFraction = COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_TIGHT -
                    lowProgress * (COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_TIGHT -
                    COUNTERPOINTAI.CHEAPEST_CAPPER_BIAS_BROAD);
            }
        }
        biasFraction = COUNTERPOINTAI._clamp(biasFraction, 0, 1);
        biasFraction = Math.floor(biasFraction * COUNTERPOINTAI._tunable("PERCENT_MAX")) /
            COUNTERPOINTAI._tunable("PERCENT_MAX");
        // Return shed variety weight to the cheapest capper so the draw never loses weight.
        var otherWeight = (1 - biasFraction) / (length - 1);
        var cheapestUnit = COUNTERPOINTAI._collectionAt(candidates, cheapestIndex);
        var cheapestMovement = Math.max(0, COUNTERPOINTAI._readNumber(cheapestUnit, "movement", 0));
        var cheapestCost = Math.max(1, COUNTERPOINTAI._strategicValue(cheapestUnit));
        var moveWeightExponent = COUNTERPOINTAI._tunable("CAPPER_VARIETY_MOVE_WEIGHT");
        var minVarietyShare = COUNTERPOINTAI._tunable("CAPPER_VARIETY_MIN_SHARE");
        var rawWeights = new Array(length);
        var shedToCheapest = 0;
        for (var weightIndex = 0; weightIndex < length; ++weightIndex)
        {
            if (weightIndex === cheapestIndex)
            {
                continue;
            }
            var otherUnit = COUNTERPOINTAI._collectionAt(candidates, weightIndex);
            var otherMovement = Math.max(0, COUNTERPOINTAI._readNumber(otherUnit, "movement", 0));
            var otherCost = Math.max(1, COUNTERPOINTAI._strategicValue(otherUnit));
            // Missing cheapest movement falls back from reach per cost to pure cost.
            var usefulness = cheapestMovement > 0 ?
                (otherMovement * cheapestCost) / (otherCost * cheapestMovement) :
                cheapestCost / otherCost;
            var keepFraction = COUNTERPOINTAI._clamp(
                Math.pow(usefulness, moveWeightExponent),
                minVarietyShare,
                1
            );
            var share = otherWeight * keepFraction;
            shedToCheapest += otherWeight - share;
            rawWeights[weightIndex] = share;
        }
        rawWeights[cheapestIndex] = biasFraction + shedToCheapest;
        return COUNTERPOINTAI._normalizeWeights(rawWeights);
    },

    _netDamageWeights : function(candidates, enemyComposition, context)
    {
        var enemies = enemyComposition || [];
        var scoreContext = context || {};
        if (!COUNTERPOINTAI._hasValidComposition(enemies) &&
            COUNTERPOINTAI._collectionLength(scoreContext.phantomThreats) === 0)
        {
            return COUNTERPOINTAI._costWeights(candidates);
        }
        var length = COUNTERPOINTAI._collectionLength(candidates);
        if (length > COUNTERPOINTAI._candidateLimit())
        {
            return { weights : [], total : 0 };
        }
        // Share one roster-relative movement reference across this scoring pass.
        scoreContext.mobilityReference = COUNTERPOINTAI._medianMovement(candidates);
        var scores = [];
        for (var scoreIndex = 0; scoreIndex < length; ++scoreIndex)
        {
            scores.push(COUNTERPOINTAI._scoreUnitAgainstEnemies(
                COUNTERPOINTAI._collectionAt(candidates, scoreIndex),
                enemies,
                scoreContext
            ));
        }

        var ranks = [];
        var indexedScores = [];
        for (var rankIndex = 0; rankIndex < length; ++rankIndex)
        {
            indexedScores.push({ index : rankIndex, score : scores[rankIndex] });
        }
        indexedScores.sort(function(left, right)
        {
            if (right.score === left.score)
            {
                return left.index - right.index;
            }
            return right.score - left.score;
        });
        for (var sortedIndex = 0; sortedIndex < length; ++sortedIndex)
        {
            ranks[indexedScores[sortedIndex].index] = sortedIndex;
        }

        var temperature = COUNTERPOINTAI._clamp(
            COUNTERPOINTAI._tunable("TEMPERATURE"),
            COUNTERPOINTAI.TEMPERATURE_MIN,
            COUNTERPOINTAI.TEMPERATURE_MAX
        );
        var inverseTemperature = 1 / temperature;
        var applyTemperature = Math.abs(inverseTemperature - 1) >
            COUNTERPOINTAI.TEMPERATURE_EPSILON;
        // Distinct nonpositive ranks preserve the least-bad fallback order.
        var rankCeiling = Math.floor(Math.pow(
            length,
            COUNTERPOINTAI.SCORE_EFFICIENCY_EXPONENT
        )) + 1;
        var rawWeights = [];
        for (var weightIndex = 0; weightIndex < length; ++weightIndex)
        {
            var rawWeight = 1;
            if (scores[weightIndex] > 0)
            {
                var valueUnits = Math.max(
                    1,
                    COUNTERPOINTAI._strategicValue(
                        COUNTERPOINTAI._collectionAt(candidates, weightIndex)
                    ) / COUNTERPOINTAI.COST_SCALE
                );
                var efficiency = scores[weightIndex] / valueUnits;
                rawWeight = Math.max(1, Math.floor(
                    Math.pow(efficiency, COUNTERPOINTAI.SCORE_EFFICIENCY_EXPONENT) *
                    Math.pow(valueUnits, COUNTERPOINTAI.SCORE_VALUE_EXPONENT)
                )) * rankCeiling;
            }
            else
            {
                var rankScore = length - ranks[weightIndex];
                rawWeight = Math.floor(Math.pow(
                    rankScore,
                    COUNTERPOINTAI.SCORE_EFFICIENCY_EXPONENT
                ));
            }
            rawWeight = Math.max(1, rawWeight);
            if (applyTemperature)
            {
                rawWeight = Math.max(1, Math.floor(Math.pow(rawWeight, inverseTemperature)));
            }
            rawWeights.push(rawWeight);
        }
        return COUNTERPOINTAI._normalizeWeights(rawWeights);
    },

    _pickWeightedIndex : function(normalized, roll)
    {
        if (normalized === null || normalized === undefined ||
            normalized.weights === null || normalized.weights === undefined)
        {
            return -1;
        }
        var length = COUNTERPOINTAI._collectionLength(normalized.weights);
        var total = normalized.total;
        var maximumTotal = Math.max(length, COUNTERPOINTAI._randomWeightLimit());
        if (length === 0 || length > COUNTERPOINTAI._candidateLimit() ||
            typeof total !== "number" || !isFinite(total) ||
            total <= 0 || Math.floor(total) !== total || total > maximumTotal)
        {
            return -1;
        }
        var verifiedTotal = 0;
        for (var verifyIndex = 0; verifyIndex < length; ++verifyIndex)
        {
            var verifiedWeight = normalized.weights[verifyIndex];
            if (typeof verifiedWeight !== "number" || !isFinite(verifiedWeight) ||
                verifiedWeight < 0 || Math.floor(verifiedWeight) !== verifiedWeight)
            {
                return -1;
            }
            verifiedTotal += verifiedWeight;
        }
        if (verifiedTotal !== total)
        {
            return -1;
        }
        var boundedRoll = Math.floor(COUNTERPOINTAI._finiteNumber(roll, 0));
        boundedRoll = COUNTERPOINTAI._clamp(boundedRoll, 0, total - 1);
        var accumulated = 0;
        var lastPositive = -1;
        for (var index = 0; index < length; ++index)
        {
            var weight = normalized.weights[index];
            if (weight > 0)
            {
                lastPositive = index;
            }
            accumulated += weight;
            if (boundedRoll < accumulated)
            {
                return index;
            }
        }
        return lastPositive;
    },

    _plannerLimit : function(name, minimum)
    {
        var maximum = COUNTERPOINTAI._readNumber(COUNTERPOINTAI.PLANNER_HARD_LIMITS, name, minimum);
        var value = Math.floor(COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI[name], minimum));
        return COUNTERPOINTAI._clamp(value, minimum, maximum);
    },

    _validPlannerInteger : function(value, minimum, maximum)
    {
        return typeof value === "number" && isFinite(value) &&
            Math.floor(value) === value && value >= minimum && value <= maximum;
    },

    _validPlannerJitter : function(value)
    {
        return value === null || COUNTERPOINTAI._validPlannerInteger(
            value,
            -COUNTERPOINTAI.PLANNER_VALUE_MAX,
            COUNTERPOINTAI.PLANNER_VALUE_MAX
        );
    },

    _validPlannerIndexList : function(values, candidateCount)
    {
        if (!Array.isArray(values) || values.length > candidateCount)
        {
            return false;
        }
        for (var index = 0; index < values.length; ++index)
        {
            if (!COUNTERPOINTAI._validPlannerInteger(
                    values[index],
                    0,
                    candidateCount - 1
                ))
            {
                return false;
            }
        }
        return true;
    },

    _validPlannerCandidate : function(candidate, candidateCount)
    {
        if (candidate === null || typeof candidate !== "object" ||
            typeof candidate.id !== "string" || candidate.id.length === 0 ||
            candidate.id.length > COUNTERPOINTAI.PLANNER_ID_LENGTH_HARD_LIMIT ||
            !COUNTERPOINTAI._validPlannerInteger(
                candidate.ordinal,
                0,
                candidateCount - 1
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                candidate.transactionCost,
                -1,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                candidate.strategicValue,
                1,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            typeof candidate.enabled !== "boolean" ||
            typeof candidate.domain !== "string" ||
            typeof candidate.canCapture !== "boolean" ||
            typeof candidate.isTransporter !== "boolean" ||
            typeof candidate.isAA !== "boolean" ||
            typeof candidate.isIndirect !== "boolean" ||
            (candidate.orderScore !== undefined &&
             !COUNTERPOINTAI._validPlannerInteger(
                 candidate.orderScore,
                 -COUNTERPOINTAI.PLANNER_VALUE_MAX,
                 COUNTERPOINTAI.PLANNER_VALUE_MAX
             )))
        {
            return false;
        }
        return candidate.domain === "" ||
            COUNTERPOINTAI._isKnownDomain(candidate.domain);
    },

    _validPlannerPlan : function(plan)
    {
        var candidateLimit = COUNTERPOINTAI._plannerLimit("MAX_PLAN_CANDIDATES", 1);
        if (plan === null || typeof plan !== "object" ||
            typeof plan.key !== "string" ||
            plan.key.length > COUNTERPOINTAI.PLANNER_KEY_LENGTH_HARD_LIMIT ||
            typeof plan.actionId !== "string" || plan.actionId.length === 0 ||
            plan.actionId.length > COUNTERPOINTAI.PLANNER_ID_LENGTH_HARD_LIMIT ||
            !COUNTERPOINTAI._validPlannerInteger(
                plan.x,
                0,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                plan.y,
                0,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            plan.key !== COUNTERPOINTAI._planKey(plan.x, plan.y, plan.actionId) ||
            (plan.phase !== COUNTERPOINTAI.PHASE_SPECIAL &&
             plan.phase !== COUNTERPOINTAI.PHASE_ORDINARY) ||
            (plan.phase === COUNTERPOINTAI.PHASE_ORDINARY &&
             plan.actionId !== COUNTERPOINTAI.ACTION_BUILD_UNITS) ||
            (plan.phase === COUNTERPOINTAI.PHASE_SPECIAL &&
             plan.actionId === COUNTERPOINTAI.ACTION_BUILD_UNITS) ||
            !COUNTERPOINTAI._validPlannerInteger(
                plan.reservedBudget,
                0,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            typeof plan.freeOnly !== "boolean" ||
            typeof plan.hasPaid !== "boolean" ||
            typeof plan.hasFree !== "boolean" ||
            typeof plan.skipped !== "boolean" ||
            typeof plan.strategicHoldEligible !== "boolean" ||
            typeof plan.strategicHold !== "boolean" ||
            typeof plan.available !== "boolean" ||
            typeof plan.allowCapperBorrow !== "boolean" ||
            typeof plan.forcedBuild !== "boolean" ||
            typeof plan.randomSurplusFunded !== "boolean" ||
            typeof plan.complete !== "boolean" ||
            !Array.isArray(plan.candidates) ||
            plan.candidates.length === 0 || plan.candidates.length > candidateLimit ||
            !Array.isArray(plan.borrowed) ||
            plan.borrowed.length > COUNTERPOINTAI._plannerLimit("MAX_PLAN_COUNT", 1))
        {
            return false;
        }
        if (plan.strategicHold && (!plan.strategicHoldEligible || plan.skipped))
        {
            return false;
        }
        for (var candidateIndex = 0;
             candidateIndex < plan.candidates.length;
             ++candidateIndex)
        {
            if (!COUNTERPOINTAI._validPlannerCandidate(
                    plan.candidates[candidateIndex],
                    plan.candidates.length
                ))
            {
                return false;
            }
        }
        var costKinds = COUNTERPOINTAI._planCostKinds(plan.candidates);
        if (plan.hasPaid !== costKinds.hasPaid ||
            plan.hasFree !== costKinds.hasFree ||
            plan.freeOnly !== costKinds.freeOnly)
        {
            return false;
        }
        if (!COUNTERPOINTAI._validPlannerIndexList(
                plan.order,
                plan.candidates.length
            ) ||
            !COUNTERPOINTAI._validPlannerIndexList(
                plan.rejected,
                plan.candidates.length
            ) ||
            (plan.selected !== -1 &&
             !COUNTERPOINTAI._validPlannerInteger(
                 plan.selected,
                 0,
                 plan.candidates.length - 1
             )) ||
            (plan.selected !== -1 && plan.rejected.indexOf(plan.selected) >= 0))
        {
            return false;
        }
        for (var borrowedIndex = 0; borrowedIndex < plan.borrowed.length; ++borrowedIndex)
        {
            var borrowed = plan.borrowed[borrowedIndex];
            if (borrowed === null || typeof borrowed !== "object" ||
                typeof borrowed.key !== "string" ||
                borrowed.key.length > COUNTERPOINTAI.PLANNER_KEY_LENGTH_HARD_LIMIT ||
                !COUNTERPOINTAI._validPlannerInteger(
                    borrowed.amount,
                    1,
                    COUNTERPOINTAI.PLANNER_VALUE_MAX
                ))
            {
                return false;
            }
        }
        return true;
    },

    _validTurnTargets : function(targets)
    {
        return targets !== null && typeof targets === "object" &&
            COUNTERPOINTAI._validPlannerInteger(
                targets.aaPerTurn,
                -1,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) &&
            COUNTERPOINTAI._validPlannerInteger(
                targets.indirectRemaining,
                -1,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            );
    },

    _validPlannerState : function(state)
    {
        if (state === null || state === undefined ||
            state.schemaVersion !== COUNTERPOINTAI.PLANNER_STATE_SCHEMA_VERSION ||
            state.algorithmVersion !== COUNTERPOINTAI.RNG_ALGORITHM_VERSION ||
            state.strategyVersion !== COUNTERPOINTAI.STRATEGY_VERSION ||
            !Array.isArray(state.plans) ||
            state.plans.length > COUNTERPOINTAI._plannerLimit("MAX_PLAN_COUNT", 1) ||
            typeof state.specialPrepared !== "boolean" ||
            typeof state.ordinaryPrepared !== "boolean" ||
            !COUNTERPOINTAI._validPlannerJitter(state.specialRandomFundingJitter) ||
            !COUNTERPOINTAI._validPlannerJitter(state.ordinaryRandomFundingJitter) ||
            !COUNTERPOINTAI._validTurnTargets(state.turnTargets))
        {
            return false;
        }
        if (!COUNTERPOINTAI._validPlannerInteger(
                state.playerId,
                0,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                state.day,
                0,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                state.generation,
                1,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                state.seed,
                0,
                COUNTERPOINTAI.PLANNER_UINT32_MAX
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                state.drawCounter,
                0,
                COUNTERPOINTAI._plannerLimit("MAX_PLANNER_DRAW_COUNT", 1)
            ) ||
            !COUNTERPOINTAI._validPlannerInteger(
                state.heldFunds,
                0,
                COUNTERPOINTAI.PLANNER_VALUE_MAX
            ))
        {
            return false;
        }
        var totalCandidates = 0;
        var keys = Object.create(null);
        var plansByKey = Object.create(null);
        var totalCandidateLimit = COUNTERPOINTAI._plannerLimit(
            "MAX_TOTAL_PLAN_CANDIDATES",
            1
        );
        for (var planIndex = 0; planIndex < state.plans.length; ++planIndex)
        {
            var plan = state.plans[planIndex];
            if (!COUNTERPOINTAI._validPlannerPlan(plan) || keys[plan.key] === true)
            {
                return false;
            }
            keys[plan.key] = true;
            plansByKey[plan.key] = plan;
            totalCandidates += plan.candidates.length;
            if (totalCandidates > totalCandidateLimit)
            {
                return false;
            }
        }
        for (var borrowedPlanIndex = 0;
             borrowedPlanIndex < state.plans.length;
             ++borrowedPlanIndex)
        {
            var borrowedPlan = state.plans[borrowedPlanIndex];
            var borrowedRecords = borrowedPlan.borrowed;
            if (borrowedRecords.length > 0 &&
                (borrowedPlan.complete || borrowedPlan.skipped ||
                 borrowedPlan.selected < 0 || !borrowedPlan.allowCapperBorrow ||
                 !borrowedPlan.candidates[borrowedPlan.selected].canCapture))
            {
                return false;
            }
            var borrowedKeys = Object.create(null);
            var borrowedTotal = 0;
            for (var borrowedIndex = 0;
                 borrowedIndex < borrowedRecords.length;
                 ++borrowedIndex)
            {
                var borrowed = borrowedRecords[borrowedIndex];
                var donor = plansByKey[borrowed.key];
                if (donor === undefined || borrowed.key === borrowedPlan.key ||
                    borrowedKeys[borrowed.key] === true ||
                    donor.phase !== borrowedPlan.phase || donor.complete ||
                    donor.skipped || !donor.hasPaid)
                {
                    return false;
                }
                borrowedKeys[borrowed.key] = true;
                borrowedTotal += borrowed.amount;
                if (borrowedTotal > borrowedPlan.reservedBudget)
                {
                    return false;
                }
            }
        }
        return true;
    },

    _loadPlannerState : function(system)
    {
        var variable = system.getVariables().createVariable(
            COUNTERPOINTAI.PLANNER_STATE_VARIABLE_ID
        );
        var values = variable.readDataListString();
        if (COUNTERPOINTAI._collectionLength(values) !== 1)
        {
            return null;
        }
        var encoded = String(COUNTERPOINTAI._collectionAt(values, 0));
        if (encoded.length > COUNTERPOINTAI._plannerLimit("MAX_PLANNER_STATE_LENGTH", 1))
        {
            return null;
        }
        try
        {
            var state = JSON.parse(encoded);
            return COUNTERPOINTAI._validPlannerState(state) ? state : null;
        }
        catch (error)
        {
            return null;
        }
    },

    _plannerStateJson : function(state)
    {
        if (!COUNTERPOINTAI._validPlannerState(state))
        {
            return null;
        }
        var encoded;
        try
        {
            encoded = JSON.stringify(state);
        }
        catch (error)
        {
            return null;
        }
        if (encoded.length > COUNTERPOINTAI._plannerLimit("MAX_PLANNER_STATE_LENGTH", 1))
        {
            return null;
        }
        return encoded;
    },

    _savePlannerState : function(system, state)
    {
        var encoded = COUNTERPOINTAI._plannerStateJson(state);
        if (encoded === null)
        {
            return false;
        }
        system.getVariables().createVariable(
            COUNTERPOINTAI.PLANNER_STATE_VARIABLE_ID
        ).writeDataListString([encoded]);
        return true;
    },

    _savePlannerStateChange : function(system, state, previousStateJson)
    {
        var currentStateJson = COUNTERPOINTAI._plannerStateJson(state);
        if (currentStateJson === null ||
            !COUNTERPOINTAI._savePlannerState(system, state))
        {
            return null;
        }
        return currentStateJson !== previousStateJson;
    },

    _saveMenuSkipState : function(system, state, previousStateJson)
    {
        var stateChanged = COUNTERPOINTAI._savePlannerStateChange(
            system,
            state,
            previousStateJson
        );
        if (stateChanged === null)
        {
            return GameEnums.MenuSelection_Skip;
        }
        return stateChanged ?
            GameEnums.MenuSelection_Restart :
            GameEnums.MenuSelection_Skip;
    },

    _clonePlannerState : function(state)
    {
        try
        {
            return JSON.parse(JSON.stringify(state));
        }
        catch (error)
        {
            return null;
        }
    },

    _loadCurrentPlannerState : function(system, ai, map)
    {
        var state = COUNTERPOINTAI._loadPlannerState(system);
        if (state === null || state.playerId !== ai.getPlayer().getPlayerID() ||
            state.day !== map.getCurrentDay())
        {
            return null;
        }
        return state;
    },

    // Applies the change to a throwaway copy and asks whether the result would still serialize,
    // so the caller can refuse a move that would not fit rather than discover it after committing.
    _plannerStateFitsAfter : function(state, planIndex, apply)
    {
        var copy = COUNTERPOINTAI._clonePlannerState(state);
        if (copy === null)
        {
            return false;
        }
        if (planIndex < 0 || planIndex >= copy.plans.length)
        {
            return false;
        }
        apply(copy.plans);
        return COUNTERPOINTAI._plannerStateJson(copy) !== null;
    },

    _plannerSelectionFits : function(state, planIndex, candidateIndex, spent)
    {
        return COUNTERPOINTAI._plannerStateFitsAfter(state, planIndex, function(plans)
            {
                COUNTERPOINTAI._completePlan(plans, planIndex, spent);
            }) &&
            COUNTERPOINTAI._plannerStateFitsAfter(state, planIndex, function(plans)
            {
                COUNTERPOINTAI._rejectPlanCandidate(plans, planIndex, candidateIndex);
            });
    },

    _ensurePlannerState : function(system, ai, map)
    {
        var player = ai.getPlayer();
        var playerId = player.getPlayerID();
        var day = map.getCurrentDay();
        var previous = COUNTERPOINTAI._loadPlannerState(system);
        if (previous !== null && previous.playerId === playerId && previous.day === day)
        {
            return previous;
        }
        var generation = previous === null ? 1 : previous.generation + 1;
        // Random seeds stay behind the production-system seam and outside gameplay RNG.
        var seed = COUNTERPOINTAI.PLANNER_DETERMINISTIC_SEED === true ?
            system.deriveCounterpointSeed(
                COUNTERPOINTAI.RNG_ALGORITHM_VERSION,
                generation
            ) :
            system.randomCounterpointSeed();
        return {
            schemaVersion : COUNTERPOINTAI.PLANNER_STATE_SCHEMA_VERSION,
            algorithmVersion : COUNTERPOINTAI.RNG_ALGORITHM_VERSION,
            strategyVersion : COUNTERPOINTAI.STRATEGY_VERSION,
            playerId : playerId,
            day : day,
            generation : generation,
            seed : COUNTERPOINTAI._finiteNumber(seed, 0) >>> 0,
            drawCounter : 0,
            heldFunds : 0,
            specialPrepared : false,
            ordinaryPrepared : false,
            specialRandomFundingJitter : null,
            ordinaryRandomFundingJitter : null,
            turnTargets : { aaPerTurn : -1, indirectRemaining : -1 },
            plans : []
        };
    },

    _nextPlannerRoll : function(state, upperExclusive)
    {
        var upper = Math.floor(COUNTERPOINTAI._finiteNumber(upperExclusive, 0));
        if (upper <= 1)
        {
            return 0;
        }
        upper = Math.min(upper, COUNTERPOINTAI._randomWeightLimit());
        // Redraw the biased tail instead of favoring low modulo residues.
        var span = COUNTERPOINTAI.PLANNER_UINT32_MAX + 1;
        var limit = Math.floor(span / upper) * upper;
        var value = 0;
        for (var attempt = 0; attempt < COUNTERPOINTAI.RNG_UNBIASED_ATTEMPTS; ++attempt)
        {
            var counter = state.drawCounter >>> 0;
            value = (state.seed ^ Math.imul(
                (counter + 1) >>> 0,
                COUNTERPOINTAI.RNG_COUNTER_MULTIPLIER
            )) >>> 0;
            value ^= value << COUNTERPOINTAI.RNG_LEFT_SHIFT_A;
            value ^= value >>> COUNTERPOINTAI.RNG_RIGHT_SHIFT;
            value ^= value << COUNTERPOINTAI.RNG_LEFT_SHIFT_B;
            value = value >>> 0;
            state.drawCounter += 1;
            if (value < limit)
            {
                break;
            }
        }
        return value % upper;
    },

    _planKey : function(x, y, actionId)
    {
        return String(x) + "," + String(y) + ":" + String(actionId);
    },

    _planIndexByKey : function(plans, key)
    {
        for (var index = 0; index < plans.length; ++index)
        {
            if (plans[index].key === key)
            {
                return index;
            }
        }
        return -1;
    },

    _planByKey : function(plans, key)
    {
        var index = COUNTERPOINTAI._planIndexByKey(plans, key);
        return index < 0 ? null : plans[index];
    },

    _findPlan : function(state, x, y, actionId)
    {
        return COUNTERPOINTAI._planByKey(state.plans, COUNTERPOINTAI._planKey(x, y, actionId));
    },

    _sortPlans : function(plans)
    {
        plans.sort(function(left, right)
        {
            if (left.x !== right.x)
            {
                return left.x - right.x;
            }
            if (left.y !== right.y)
            {
                return left.y - right.y;
            }
            if (left.actionId < right.actionId)
            {
                return -1;
            }
            if (left.actionId > right.actionId)
            {
                return 1;
            }
            return 0;
        });
        return plans;
    },

    _domainFromUnitType : function(unitType)
    {
        if (unitType === GameEnums.UnitType_Air)
        {
            return COUNTERPOINTAI.DOMAIN_AIR;
        }
        if (unitType === GameEnums.UnitType_Naval)
        {
            return COUNTERPOINTAI.DOMAIN_NAVAL;
        }
        if (unitType === GameEnums.UnitType_Hovercraft)
        {
            return COUNTERPOINTAI.DOMAIN_HOVER;
        }
        if (unitType === GameEnums.UnitType_Ground ||
            unitType === GameEnums.UnitType_Infantry)
        {
            return COUNTERPOINTAI.DOMAIN_GROUND;
        }
        // Unknown unit types get no domain policy.
        return "";
    },

    _isKnownDomain : function(domain)
    {
        return domain === COUNTERPOINTAI.DOMAIN_GROUND ||
            domain === COUNTERPOINTAI.DOMAIN_AIR ||
            domain === COUNTERPOINTAI.DOMAIN_NAVAL ||
            domain === COUNTERPOINTAI.DOMAIN_HOVER;
    },

    _copyProductionCandidates : function(data)
    {
        var ids = data.getUnitIds();
        var costs = data.getTransactionCosts();
        var values = data.getStrategicValues();
        var enabled = data.getEnabledList();
        var length = Math.min(
            COUNTERPOINTAI._collectionLength(ids),
            COUNTERPOINTAI._collectionLength(costs),
            COUNTERPOINTAI._collectionLength(values),
            COUNTERPOINTAI._collectionLength(enabled),
            COUNTERPOINTAI._plannerLimit("MAX_PLAN_CANDIDATES", 1)
        );
        var ordinals = Object.create(null);
        var candidates = [];
        for (var index = 0; index < length; ++index)
        {
            var id = String(COUNTERPOINTAI._collectionAt(ids, index));
            var ordinalKey = "#" + id;
            var ordinal = ordinals[ordinalKey] || 0;
            ordinals[ordinalKey] = ordinal + 1;
            var transactionCost = Math.floor(COUNTERPOINTAI._finiteNumber(
                COUNTERPOINTAI._collectionAt(costs, index),
                -1
            ));
            candidates.push({
                id : id,
                ordinal : ordinal,
                transactionCost : transactionCost,
                strategicValue : Math.max(1, Math.floor(COUNTERPOINTAI._finiteNumber(
                    COUNTERPOINTAI._collectionAt(values, index),
                    1
                ))),
                enabled : COUNTERPOINTAI._collectionAt(enabled, index) === true,
                domain : "",
                canCapture : false,
                isTransporter : false,
                isAA : false,
                isIndirect : false
            });
        }
        return candidates;
    },

    _planCostKinds : function(candidates)
    {
        var hasPaid = false;
        var hasFree = false;
        for (var index = 0; index < candidates.length; ++index)
        {
            var candidate = candidates[index];
            if (candidate.transactionCost < 0)
            {
                continue;
            }
            if (candidate.transactionCost === 0)
            {
                hasFree = true;
            }
            else
            {
                hasPaid = true;
            }
        }
        return {
            hasPaid : hasPaid,
            hasFree : hasFree,
            freeOnly : hasFree && !hasPaid
        };
    },

    _refreshPlanCostKinds : function(plan)
    {
        var costKinds = COUNTERPOINTAI._planCostKinds(plan.candidates);
        plan.hasPaid = costKinds.hasPaid;
        plan.hasFree = costKinds.hasFree;
        plan.freeOnly = costKinds.freeOnly;
    },

    _newPlan : function(data, phase)
    {
        var candidates = COUNTERPOINTAI._copyProductionCandidates(data);
        var rejected = [];
        for (var index = 0; index < candidates.length; ++index)
        {
            var candidate = candidates[index];
            if (candidate.transactionCost < 0)
            {
                rejected.push(index);
            }
        }
        var x = data.getX();
        var y = data.getY();
        var actionId = String(data.getActionId());
        var plan = {
            key : COUNTERPOINTAI._planKey(x, y, actionId),
            x : x,
            y : y,
            actionId : actionId,
            phase : phase,
            reservedBudget : 0,
            freeOnly : false,
            hasPaid : false,
            hasFree : false,
            skipped : false,
            strategicHoldEligible : false,
            strategicHold : false,
            forcedBuild : false,
            randomSurplusFunded : false,
            available : data.getActionAvailable() === true,
            allowCapperBorrow : false,
            borrowed : [],
            candidates : candidates,
            order : [],
            rejected : rejected,
            selected : -1,
            complete : false
        };
        COUNTERPOINTAI._refreshPlanCostKinds(plan);
        return plan;
    },

    _discoverPlans : function(system, ai, buildings, phase, knownKeys)
    {
        var playerId = ai.getPlayer().getPlayerID();
        var buildingRecords = [];
        var buildingLength = COUNTERPOINTAI._collectionLength(buildings);
        for (var buildingIndex = 0; buildingIndex < buildingLength; ++buildingIndex)
        {
            var building = COUNTERPOINTAI._collectionAt(buildings, buildingIndex);
            if (building === null || building === undefined ||
                building.getOwnerID() !== playerId)
            {
                continue;
            }
            buildingRecords.push({
                building : building,
                x : building.getX(),
                y : building.getY()
            });
        }
        buildingRecords.sort(function(left, right)
        {
            return left.x !== right.x ? left.x - right.x : left.y - right.y;
        });

        var plans = [];
        var seen = Object.create(null);
        for (var knownIndex = 0;
             knownKeys !== null && knownKeys !== undefined && knownIndex < knownKeys.length;
             ++knownIndex)
        {
            seen[knownKeys[knownIndex]] = true;
        }
        var planLimit = COUNTERPOINTAI._plannerLimit("MAX_PLAN_COUNT", 1);
        for (var recordIndex = 0;
             recordIndex < buildingRecords.length && plans.length < planLimit;
             ++recordIndex)
        {
            var record = buildingRecords[recordIndex];
            var actions = record.building.getActionList();
            var actionLength = COUNTERPOINTAI._collectionLength(actions);
            for (var actionIndex = 0;
                 actionIndex < actionLength && plans.length < planLimit;
                 ++actionIndex)
            {
                var actionId = String(COUNTERPOINTAI._collectionAt(actions, actionIndex));
                var isOrdinary = actionId === COUNTERPOINTAI.ACTION_BUILD_UNITS;
                if ((phase === COUNTERPOINTAI.PHASE_ORDINARY) !== isOrdinary)
                {
                    continue;
                }
                var key = COUNTERPOINTAI._planKey(record.x, record.y, actionId);
                if (seen[key] === true)
                {
                    continue;
                }
                seen[key] = true;
                var data = system.getProductionActionData(record.building, actionId);
                if (data === null || data === undefined || data.getActionAvailable() !== true)
                {
                    continue;
                }
                var plan = COUNTERPOINTAI._newPlan(data, phase);
                if (plan.candidates.length > 0)
                {
                    plans.push(plan);
                }
            }
        }
        return COUNTERPOINTAI._sortPlans(plans);
    },

    _appendUniqueId : function(ids, seen, id)
    {
        var key = "#" + id;
        if (seen[key] !== true)
        {
            seen[key] = true;
            ids.push(id);
        }
    },

    // getAllUnitIDs is every unit the mods loaded; the match's bans live on each player's build list.
    _matchUnitIds : function(map, fallback)
    {
        var ids = [];
        var seen = Object.create(null);
        if (map !== null && map !== undefined && typeof map.getPlayerCount === "function")
        {
            var playerCount = COUNTERPOINTAI._finiteNumber(map.getPlayerCount(), 0);
            for (var playerIndex = 0; playerIndex < playerCount; ++playerIndex)
            {
                var player = map.getPlayer(playerIndex);
                if (player === null || player === undefined ||
                    typeof player.getBuildList !== "function")
                {
                    continue;
                }
                var buildList = player.getBuildList();
                var listLength = COUNTERPOINTAI._collectionLength(buildList);
                for (var listIndex = 0; listIndex < listLength; ++listIndex)
                {
                    COUNTERPOINTAI._appendUniqueId(
                        ids,
                        seen,
                        String(COUNTERPOINTAI._collectionAt(buildList, listIndex))
                    );
                }
            }
        }
        if (ids.length === 0)
        {
            return fallback;
        }
        ids.sort();
        return ids;
    },

    _visitPlanCandidates : function(plans, visitor)
    {
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var candidates = plans[planIndex].candidates;
            for (var candidateIndex = 0;
                 candidateIndex < candidates.length;
                 ++candidateIndex)
            {
                visitor(candidates[candidateIndex], planIndex, candidateIndex);
            }
        }
    },

    _unitIdsFromCollection : function(units)
    {
        var ids = [];
        var seen = Object.create(null);
        COUNTERPOINTAI._visitNonNullCollectionItems(units, function(unit)
        {
            var id = String(unit.getUnitID());
            COUNTERPOINTAI._appendUniqueId(ids, seen, id);
        });
        ids.sort();
        return ids;
    },

    // Price alone let an expensive mod foot unit in, and its MECH damage id skewed every reading.
    _isArmoredProbeUnit : function(unit, strategicValue)
    {
        return unit !== null && unit !== undefined &&
               unit.getUnitType() !== GameEnums.UnitType_Infantry &&
               COUNTERPOINTAI._domainFromUnitType(unit.getUnitType()) ===
                   COUNTERPOINTAI.DOMAIN_GROUND &&
               unit.canCapture() !== true &&
               unit.isTransporter() !== true &&
               unit.getBaseMinRange() <= 1 &&
               strategicValue >= COUNTERPOINTAI.TANK_MIN_BASE_COST;
    },

    _probeIds : function(system, candidateIds, units, enemyUnits, accepted)
    {
        var ids = [];
        var seen = Object.create(null);
        var collect = function(collection)
        {
            COUNTERPOINTAI._visitNonNullCollectionItems(collection, function(unit)
            {
                if (!accepted(unit))
                {
                    return;
                }
                var id = String(unit.getUnitID());
                COUNTERPOINTAI._appendUniqueId(ids, seen, id);
            });
        };
        collect(units);
        collect(enemyUnits);
        var rosterLength = COUNTERPOINTAI._collectionLength(candidateIds);
        for (var index = 0; index < rosterLength; ++index)
        {
            var candidateId = String(COUNTERPOINTAI._collectionAt(candidateIds, index));
            var dummy = system.getDummyUnit(candidateId);
            if (dummy === null || dummy === undefined)
            {
                continue;
            }
            if (accepted(dummy))
            {
                COUNTERPOINTAI._appendUniqueId(ids, seen, candidateId);
            }
        }
        ids.sort();
        return ids;
    },

    _armoredProbeIds : function(system, candidateIds, units, enemyUnits)
    {
        return COUNTERPOINTAI._probeIds(
            system,
            candidateIds,
            units,
            enemyUnits,
            function(unit)
            {
                return COUNTERPOINTAI._isArmoredProbeUnit(
                    unit,
                    Math.max(1, unit.getBaseCosts())
                );
            }
        );
    },

    _airProbeIds : function(system, candidateIds, units, enemyUnits)
    {
        return COUNTERPOINTAI._probeIds(
            system,
            candidateIds,
            units,
            enemyUnits,
            function(unit)
            {
                return COUNTERPOINTAI._domainFromUnitType(unit.getUnitType()) ===
                    COUNTERPOINTAI.DOMAIN_AIR;
            }
        );
    },

    _maxDamageAgainstIds : function(system, attackerId, defenderIds)
    {
        var maximum = 0;
        for (var index = 0; index < defenderIds.length; ++index)
        {
            maximum = Math.max(
                maximum,
                COUNTERPOINTAI._finiteNumber(
                    system.getCounterpointBaseDamage(attackerId, defenderIds[index]),
                    0
                )
            );
        }
        return maximum;
    },

    // Median, not max: the probe set is heuristic, and one soft member skewed every unit's reading.
    _typicalArmoredDamage : function(system, attackerId, armoredIds)
    {
        var readings = [];
        for (var index = 0; index < armoredIds.length; ++index)
        {
            if (armoredIds[index] === attackerId)
            {
                continue;
            }
            readings.push(COUNTERPOINTAI._finiteNumber(
                system.getCounterpointBaseDamage(attackerId, armoredIds[index]),
                0
            ));
        }
        if (readings.length === 0)
        {
            return 0;
        }
        readings.sort(function(left, right)
        {
            return left - right;
        });
        return Math.max(0, readings[Math.floor((readings.length - 1) / 2)]);
    },

    _transportCanCarryTank : function(system, transportIds, armoredIds, airIds)
    {
        for (var index = 0; index < transportIds.length; ++index)
        {
            var id = transportIds[index];
            var dummy = system.getDummyUnit(id);
            if (dummy === null || dummy === undefined)
            {
                continue;
            }
            var cargo = {
                id : id,
                domain : COUNTERPOINTAI._domainFromUnitType(dummy.getUnitType()),
                strategicValue : Math.max(1, dummy.getBaseCosts()),
                movement : dummy.getBaseMovementPoints(),
                minRange : dummy.getBaseMinRange(),
                canCapture : dummy.canCapture() === true,
                isTransporter : dummy.isTransporter() === true,
                armoredDamage : COUNTERPOINTAI._typicalArmoredDamage(
                    system,
                    id,
                    armoredIds
                ),
                maxDamageVsAir : COUNTERPOINTAI._maxDamageAgainstIds(system, id, airIds)
            };
            if (COUNTERPOINTAI._isTankClass(cargo))
            {
                return true;
            }
        }
        return false;
    },

    _weaponTypeChannels : function(weaponType, minRange, maxRange, hasExplicitDirect)
    {
        if (typeof GameEnums === "undefined")
        {
            return null;
        }
        if (weaponType === GameEnums.WeaponType_Direct)
        {
            return { direct : true, indirect : false };
        }
        if (weaponType === GameEnums.WeaponType_Indirect)
        {
            return { direct : false, indirect : true };
        }
        if (weaponType === GameEnums.WeaponType_Both)
        {
            return {
                direct : minRange <= 1 || hasExplicitDirect === true,
                indirect : maxRange > 1
            };
        }
        return null;
    },

    _legacyDamageProfile : function(minRange, damageById)
    {
        var directDamageById = Object.create(null);
        var indirectDamageById = Object.create(null);
        var legacyIndirect = minRange > 1;
        for (var key in damageById)
        {
            if (legacyIndirect)
            {
                indirectDamageById[key] = damageById[key];
            }
            else
            {
                directDamageById[key] = damageById[key];
            }
        }
        return {
            damageById : damageById,
            directDamageById : directDamageById,
            indirectDamageById : indirectDamageById,
            hasDirectChannel : !legacyIndirect,
            hasIndirectChannel : legacyIndirect
        };
    },

    _weaponData : function(unit, minRange, maxRange)
    {
        var safeMinRange = Math.max(1, minRange);
        var safeMaxRange = Math.max(safeMinRange, maxRange);
        try
        {
            var weapon1 = unit.getWeapon1ID();
            var weapon2 = unit.getWeapon2ID();
            var type1 = unit.getTypeOfWeapon1();
            var type2 = unit.getTypeOfWeapon2();
            return {
                ids : [
                    weapon1 === null || weapon1 === undefined ? "" : String(weapon1),
                    weapon2 === null || weapon2 === undefined ? "" : String(weapon2)
                ],
                types : [type1, type2],
                hasExplicitDirect : typeof GameEnums !== "undefined" &&
                    (type1 === GameEnums.WeaponType_Direct ||
                     type2 === GameEnums.WeaponType_Direct),
                minRange : safeMinRange,
                maxRange : safeMaxRange
            };
        }
        catch (error)
        {
            return {
                ids : [],
                types : [],
                hasExplicitDirect : false,
                minRange : safeMinRange,
                maxRange : safeMaxRange
            };
        }
    },

    _weaponDataHasWeapon : function(weaponData)
    {
        for (var index = 0; index < weaponData.ids.length; ++index)
        {
            if (weaponData.ids[index] !== "")
            {
                return true;
            }
        }
        return false;
    },

    _damageCacheKey : function(attackerId, weaponData)
    {
        return JSON.stringify([
            attackerId,
            weaponData.ids,
            weaponData.types,
            weaponData.minRange,
            weaponData.maxRange,
            weaponData.hasExplicitDirect
        ]);
    },

    _damageProfile : function(system, attackerId, defenderIds, weaponData, damageCache)
    {
        var damageById = Object.create(null);
        for (var defenderIndex = 0; defenderIndex < defenderIds.length; ++defenderIndex)
        {
            var defenderId = defenderIds[defenderIndex];
            damageById["#" + defenderId] = Math.max(
                0,
                COUNTERPOINTAI._finiteNumber(
                    system.getCounterpointBaseDamage(attackerId, defenderId),
                    0
                )
            );
        }
        var directDamageById = Object.create(null);
        var indirectDamageById = Object.create(null);
        var minRange = weaponData.minRange;
        var maxRange = weaponData.maxRange;
        var hasDirectChannel = false;
        var hasIndirectChannel = false;
        var slots = [];
        for (var weaponIndex = 0; weaponIndex < weaponData.ids.length; ++weaponIndex)
        {
            var weaponId = weaponData.ids[weaponIndex];
            if (weaponId === "")
            {
                continue;
            }
            var channels = COUNTERPOINTAI._weaponTypeChannels(
                weaponData.types[weaponIndex],
                minRange,
                maxRange,
                weaponData.hasExplicitDirect
            );
            if (channels === null || typeof Global === "undefined" ||
                Global[weaponId] === null || Global[weaponId] === undefined ||
                typeof Global[weaponId].getBaseDamage !== "function")
            {
                return COUNTERPOINTAI._legacyDamageProfile(minRange, damageById);
            }
            hasDirectChannel = hasDirectChannel || channels.direct;
            hasIndirectChannel = hasIndirectChannel || channels.indirect;
            slots.push({
                weapon : Global[weaponId],
                direct : channels.direct,
                indirect : channels.indirect
            });
        }
        if (!hasDirectChannel && !hasIndirectChannel)
        {
            return COUNTERPOINTAI._legacyDamageProfile(minRange, damageById);
        }
        var cache = damageCache || Object.create(null);
        var cacheKey = COUNTERPOINTAI._damageCacheKey(attackerId, weaponData);
        if (cache[cacheKey] === undefined)
        {
            cache[cacheKey] = Object.create(null);
        }
        var profileCache = cache[cacheKey];
        for (var damageIndex = 0; damageIndex < defenderIds.length; ++damageIndex)
        {
            var defenderId = defenderIds[damageIndex];
            var defenderKey = "#" + defenderId;
            if (profileCache[defenderKey] !== undefined)
            {
                var cached = profileCache[defenderKey];
                directDamageById[defenderKey] = cached.direct;
                indirectDamageById[defenderKey] = cached.indirect;
                continue;
            }
            var defender = system.getDummyUnit(defenderId);
            if (defender === null || defender === undefined)
            {
                return COUNTERPOINTAI._legacyDamageProfile(minRange, damageById);
            }
            var directDamage = 0;
            var indirectDamage = 0;
            for (var slotIndex = 0; slotIndex < slots.length; ++slotIndex)
            {
                var slot = slots[slotIndex];
                var damage;
                try
                {
                    damage = Math.max(
                        0,
                        COUNTERPOINTAI._finiteNumber(
                            slot.weapon.getBaseDamage(defender),
                            0
                        )
                    );
                }
                catch (error)
                {
                    return COUNTERPOINTAI._legacyDamageProfile(minRange, damageById);
                }
                if (slot.direct)
                {
                    directDamage = Math.max(directDamage, damage);
                }
                if (slot.indirect)
                {
                    indirectDamage = Math.max(indirectDamage, damage);
                }
            }
            directDamageById[defenderKey] = directDamage;
            indirectDamageById[defenderKey] = indirectDamage;
            profileCache[defenderKey] = {
                direct : directDamage,
                indirect : indirectDamage
            };
        }
        for (var aggregateIndex = 0; aggregateIndex < defenderIds.length; ++aggregateIndex)
        {
            var aggregateKey = "#" + defenderIds[aggregateIndex];
            damageById[aggregateKey] = Math.max(
                COUNTERPOINTAI._readNumber(directDamageById, aggregateKey, 0),
                COUNTERPOINTAI._readNumber(indirectDamageById, aggregateKey, 0)
            );
        }
        return {
            damageById : damageById,
            directDamageById : directDamageById,
            indirectDamageById : indirectDamageById,
            hasDirectChannel : hasDirectChannel,
            hasIndirectChannel : hasIndirectChannel
        };
    },

    _snapshotUnit : function(system, unit, id, strategicValue, damageTargets, armoredIds,
                             airIds, damageCache)
    {
        if (unit === null || unit === undefined)
        {
            return null;
        }
        var transportIds = unit.isTransporter() === true
            ? COUNTERPOINTAI._stringList(unit.getTransportUnits())
            : [];
        var unitType = unit.getUnitType();
        var movement = unit.getBaseMovementPoints();
        var minRange = unit.getBaseMinRange();
        var maxRange = unit.getBaseMaxRange();
        var canCapture = unit.canCapture() === true;
        var isTransporter = unit.isTransporter() === true;
        var loadingPlace = isTransporter ? unit.getLoadingPlace() : 0;
        var weaponData = COUNTERPOINTAI._weaponData(unit, minRange, maxRange);
        var damageProfile = COUNTERPOINTAI._damageProfile(
            system,
            id,
            damageTargets,
            weaponData,
            damageCache
        );
        var snapshot = {
            id : id,
            strategicValue : Math.max(1, strategicValue),
            domain : COUNTERPOINTAI._domainFromUnitType(unitType),
            movement : movement,
            minRange : minRange,
            maxRange : maxRange,
            canCapture : canCapture,
            isTransporter : isTransporter,
            cargoIds : transportIds,
            loadingPlace : loadingPlace,
            armoredDamage : COUNTERPOINTAI._typicalArmoredDamage(
                system,
                id,
                armoredIds
            ),
            maxDamageVsAir : COUNTERPOINTAI._maxDamageAgainstIds(system, id, airIds),
            damageById : damageProfile.damageById,
            directDamageById : damageProfile.directDamageById,
            indirectDamageById : damageProfile.indirectDamageById,
            hasDirectChannel : damageProfile.hasDirectChannel,
            hasIndirectChannel : damageProfile.hasIndirectChannel,
            hasAttackWeapon : COUNTERPOINTAI._weaponDataHasWeapon(weaponData) ||
                COUNTERPOINTAI._hasAttackCapability(damageProfile),
            hasAttackAction : COUNTERPOINTAI._hasAttackAction(unit),
            weaponProfileKey : COUNTERPOINTAI._damageCacheKey(id, weaponData)
        };
        snapshot.isAASpecialist = COUNTERPOINTAI._isAASpecialist(snapshot);
        snapshot.canTransportTank = snapshot.isTransporter &&
            COUNTERPOINTAI._transportCanCarryTank(system, transportIds, armoredIds, airIds);
        return snapshot;
    },

    _enemyStrategicValue : function(unit)
    {
        var baseValue = Math.max(1, unit.getBaseCosts());
        var owner = unit.getOwner();
        if (owner === null || owner === undefined)
        {
            return baseValue;
        }
        return Math.max(
            1,
            COUNTERPOINTAI._finiteNumber(unit.getUnitCosts(), baseValue)
        );
    },

    _snapshotCollection : function(system, units, damageTargets, armoredIds, airIds, enemy,
                                   damageCache)
    {
        var snapshots = [];
        COUNTERPOINTAI._visitNonNullCollectionItems(units, function(unit)
        {
            var id = String(unit.getUnitID());
            var strategicValue = enemy === true ?
                COUNTERPOINTAI._enemyStrategicValue(unit) :
                Math.max(1, unit.getBaseCosts());
            var snapshot = COUNTERPOINTAI._snapshotUnit(
                system,
                unit,
                id,
                strategicValue,
                damageTargets,
                armoredIds,
                airIds,
                damageCache
            );
            if (snapshot === null)
            {
                return;
            }
            snapshot.hp = unit.getHp();
            var owner = unit.getOwner();
            if (owner !== null && owner !== undefined)
            {
                snapshot.ownerId = owner.getPlayerID();
            }
            snapshots.push(snapshot);
        });
        return snapshots;
    },

    _candidateRejected : function(plan, candidateIndex)
    {
        return plan.rejected.indexOf(candidateIndex) >= 0;
    },

    _rejectCandidate : function(plan, candidateIndex)
    {
        if (!COUNTERPOINTAI._candidateRejected(plan, candidateIndex) &&
            plan.rejected.length < COUNTERPOINTAI._plannerLimit("MAX_PLAN_CANDIDATES", 1))
        {
            plan.rejected.push(candidateIndex);
        }
        if (plan.selected === candidateIndex)
        {
            plan.selected = -1;
        }
    },

    _enrichPlanCandidates : function(system, plans, enemyIds, armoredIds, airIds, damageCache)
    {
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var plan = plans[planIndex];
            for (var candidateIndex = 0;
                 candidateIndex < plan.candidates.length;
                 ++candidateIndex)
            {
                if (COUNTERPOINTAI._candidateRejected(plan, candidateIndex))
                {
                    continue;
                }
                var candidate = plan.candidates[candidateIndex];
                var dummy = system.getDummyUnit(candidate.id);
                var snapshot = COUNTERPOINTAI._snapshotUnit(
                    system,
                    dummy,
                    candidate.id,
                    candidate.strategicValue,
                    enemyIds,
                    armoredIds,
                    airIds,
                    damageCache
                );
                if (snapshot === null)
                {
                    COUNTERPOINTAI._rejectCandidate(plan, candidateIndex);
                    continue;
                }
                candidate.domain = snapshot.domain;
                candidate.canCapture = snapshot.canCapture;
                candidate.isTransporter = snapshot.isTransporter;
                candidate.scoreData = snapshot;
                candidate.planIndex = candidateIndex;
            }
        }
    },

    _filterTrappedSurfaceCandidates : function(system, ai, plans)
    {
        var created = Object.create(null);
        var islandSizes = Object.create(null);
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var plan = plans[planIndex];
            var surface = [];
            var totalIslandSize = 0;
            for (var candidateIndex = 0;
                 candidateIndex < plan.candidates.length;
                 ++candidateIndex)
            {
                if (COUNTERPOINTAI._candidateRejected(plan, candidateIndex))
                {
                    continue;
                }
                var candidate = plan.candidates[candidateIndex];
                if (candidate.domain !== COUNTERPOINTAI.DOMAIN_GROUND &&
                    candidate.domain !== COUNTERPOINTAI.DOMAIN_HOVER &&
                    candidate.domain !== COUNTERPOINTAI.DOMAIN_NAVAL)
                {
                    continue;
                }
                var dummy = system.getDummyUnit(candidate.id);
                if (dummy === null || dummy === undefined)
                {
                    continue;
                }
                var movementType = dummy.getMovementType();
                try
                {
                    if (!COUNTERPOINTAI._createIslandMapFor(
                            ai,
                            created,
                            movementType,
                            candidate.id
                        ))
                    {
                        surface = [];
                        break;
                    }
                    var islandKey = movementType + "@" + plan.x + "," + plan.y;
                    if (islandSizes[islandKey] === undefined)
                    {
                        islandSizes[islandKey] = Math.max(
                            0,
                            COUNTERPOINTAI._finiteNumber(
                                ai.getIslandSize(dummy, plan.x, plan.y),
                                0
                            )
                        );
                    }
                    var islandSize = islandSizes[islandKey];
                    surface.push({ index : candidateIndex, islandSize : islandSize });
                    totalIslandSize += islandSize;
                }
                catch (islandError)
                {
                    surface = [];
                    break;
                }
            }
            if (surface.length === 0)
            {
                continue;
            }
            var minimumRelativeSize = totalIslandSize / surface.length *
                COUNTERPOINTAI.SURFACE_PRODUCTION_MIN_ISLAND_SHARE;
            for (var surfaceIndex = 0; surfaceIndex < surface.length; ++surfaceIndex)
            {
                var entry = surface[surfaceIndex];
                if (entry.islandSize < COUNTERPOINTAI.SURFACE_PRODUCTION_MIN_ISLAND_SIZE ||
                    entry.islandSize < minimumRelativeSize)
                {
                    COUNTERPOINTAI._rejectCandidate(plan, entry.index);
                }
            }
        }
    },

    _ownCounts : function(ownComposition)
    {
        var counts = Object.create(null);
        for (var index = 0; index < ownComposition.length; ++index)
        {
            var entry = ownComposition[index];
            var key = "#" + entry.id;
            counts[key] = (counts[key] || 0) + entry.count;
        }
        return counts;
    },

    _tankFerryStats : function(ownSnapshots)
    {
        var stats = { tanks : 0, tankTrans : 0, capacity : 0 };
        for (var index = 0; index < ownSnapshots.length; ++index)
        {
            var unit = ownSnapshots[index];
            if (COUNTERPOINTAI._isTankClass(unit))
            {
                stats.tanks += 1;
            }
            if (COUNTERPOINTAI._isTankCapableTransport(unit))
            {
                stats.tankTrans += 1;
                stats.capacity += Math.max(1, COUNTERPOINTAI._readNumber(
                    unit,
                    "loadingPlace",
                    1
                ));
            }
        }
        return stats;
    },

    _indirectRangeDeltas : function(system, ai, plans)
    {
        var deltas = Object.create(null);
        var seen = Object.create(null);
        var player = ai.getPlayer();
        var offMap = Qt.point(-1, -1);
        COUNTERPOINTAI._visitPlanCandidates(plans, function(candidate)
        {
            var key = "#" + candidate.id;
            if (seen[key] === true || candidate.scoreData === undefined ||
                !COUNTERPOINTAI._hasIndirectChannel(candidate.scoreData))
            {
                return;
            }
            seen[key] = true;
            var dummy = system.getDummyUnit(candidate.id);
            if (dummy === null || dummy === undefined)
            {
                return;
            }
            var delta = 0;
            for (var coIndex = 0; coIndex < player.getMaxCoCount(); ++coIndex)
            {
                var co = player.getCO(coIndex);
                if (co !== null && co !== undefined)
                {
                    delta += COUNTERPOINTAI._finiteNumber(
                        co.getFirerangeModifier(dummy, offMap),
                        0
                    );
                }
            }
            deltas[key] = delta;
        });
        return deltas;
    },

    // The unit kinds shareIslandWithEnemy actually walks; without one its answer carries no meaning.
    _hasGroundVehicle : function(units)
    {
        var found = false;
        COUNTERPOINTAI._visitNonNullCollectionItems(units, function(unit)
        {
            if (unit.getUnitType() === GameEnums.UnitType_Ground)
            {
                found = true;
            }
        });
        return found;
    },

    _mapPlanningContext : function(ai, buildings, enemyBuildings, units)
    {
        var context = {
            shareIsland : true,
            enemyIslandBuildings : 0,
            groundProbe : COUNTERPOINTAI._hasGroundVehicle(units)
        };
        try
        {
            context.shareIsland = ai.shareIslandWithEnemy(
                units,
                buildings,
                enemyBuildings
            );
        }
        catch (shareError)
        {
            context.shareIsland = true;
        }
        try
        {
            context.enemyIslandBuildings = ai.getBuildingCountsOnEnemyIslands(
                units,
                enemyBuildings
            );
        }
        catch (buildingError)
        {
            context.enemyIslandBuildings = 0;
        }
        return context;
    },

    _transportContext : function(ownComposition, availableDomains, islandMode)
    {
        var capperCount = 0;
        for (var index = 0; index < ownComposition.length; ++index)
        {
            if (ownComposition[index].canCapture === true)
            {
                capperCount += ownComposition[index].count;
            }
        }
        return {
            ground : capperCount >= COUNTERPOINTAI.GROUND_TRANSPORT_MIN_CAPPERS,
            air : islandMode && availableDomains.air,
            naval : islandMode && availableDomains.naval,
            hover : islandMode && availableDomains.hover
        };
    },

    // Ground unit ids this match can actually produce. A transport is only worth its ferry score if
    // it can carry one of these: a cruiser is a transporter too, but it takes nothing but copters.
    _readFlag : function(source, id)
    {
        return source !== null && source !== undefined && source["#" + id] === true;
    },

    _carriesAnyOf : function(cargoIds, wantedIds)
    {
        var length = COUNTERPOINTAI._collectionLength(cargoIds);
        for (var index = 0; index < length; ++index)
        {
            if (wantedIds["#" + String(COUNTERPOINTAI._collectionAt(cargoIds, index))] === true)
            {
                return true;
            }
        }
        return false;
    },

    _planningContext : function(system, ai, plans, buildings, units, enemyUnits,
                                enemyBuildings, map)
    {
        var damageCache = Object.create(null);
        var enemyIds = COUNTERPOINTAI._unitIdsFromCollection(enemyUnits);
        var roster = COUNTERPOINTAI._productionRoster(system, ai, buildings);
        // Every facility's roster, not just this batch's, so an enemy snapshot carries the same
        // damage table whichever factories happened to be planned first.
        var candidateIds = roster.unitIds;
        var allUnitIds = COUNTERPOINTAI._matchUnitIds(map, candidateIds);
        var armoredIds = COUNTERPOINTAI._armoredProbeIds(
            system,
            allUnitIds,
            units,
            enemyUnits
        );
        var airIds = COUNTERPOINTAI._airProbeIds(system, allUnitIds, units, enemyUnits);
        COUNTERPOINTAI._enrichPlanCandidates(
            system,
            plans,
            enemyIds,
            armoredIds,
            airIds,
            damageCache
        );
        COUNTERPOINTAI._filterTrappedSurfaceCandidates(system, ai, plans);
        var ownSnapshots = COUNTERPOINTAI._snapshotCollection(
            system,
            units,
            enemyIds,
            armoredIds,
            airIds,
            false,
            damageCache
        );
        var enemySnapshots = COUNTERPOINTAI._snapshotCollection(
            system,
            enemyUnits,
            candidateIds,
            armoredIds,
            airIds,
            true,
            damageCache
        );
        var ownComposition = COUNTERPOINTAI._sampleComposition(ownSnapshots);
        var enemyComposition = COUNTERPOINTAI._sampleComposition(enemySnapshots);
        var mapContext = COUNTERPOINTAI._mapPlanningContext(
            ai,
            buildings,
            enemyBuildings,
            units
        );
        var islandMode = COUNTERPOINTAI._isIslandMode(mapContext);
        var indirectRangeDeltas = COUNTERPOINTAI._indirectRangeDeltas(
            system,
            ai,
            plans
        );
        var banIndirects = false;
        var indirectCo = false;
        for (var deltaId in indirectRangeDeltas)
        {
            if (indirectRangeDeltas[deltaId] < 0)
            {
                banIndirects = true;
            }
            else if (indirectRangeDeltas[deltaId] > 0)
            {
                indirectCo = true;
            }
        }
        var ownTransporters = COUNTERPOINTAI._countOwnTransporters(
            ownSnapshots,
            roster.groundIds
        );
        var phantomThreatList = COUNTERPOINTAI._phantomThreats(
            system,
            enemyBuildings,
            candidateIds,
            armoredIds,
            airIds,
            COUNTERPOINTAI._fieldedHpById(enemySnapshots),
            damageCache
        );
        return {
            plans : plans,
            roster : roster,
            groundCarriers : roster.carriers,
            enemyComposition : enemyComposition,
            ownComposition : ownComposition,
            ownSnapshots : ownSnapshots,
            ownCounts : COUNTERPOINTAI._ownCounts(ownComposition),
            ownCoverage : COUNTERPOINTAI._computeOwnCoverage(
                ownComposition,
                enemyComposition
            ),
            ownAirCoverage : COUNTERPOINTAI._computeAirCoverage(
                ownComposition,
                enemyComposition
            ),
            threatProfile : COUNTERPOINTAI._analyzeEnemyComp(enemyComposition),
            phantomThreats : phantomThreatList,
            phantomCoverage : COUNTERPOINTAI._computePhantomCoverage(
                system,
                ownComposition,
                phantomThreatList
            ),
            phantomAirPresent : COUNTERPOINTAI._compositionHasAirThreat(phantomThreatList),
            indirectRangeDeltas : indirectRangeDeltas,
            tankFerryStats : COUNTERPOINTAI._tankFerryStats(ownSnapshots),
            islandMode : islandMode,
            transportContext : COUNTERPOINTAI._transportContext(
                ownComposition,
                roster.domains,
                islandMode
            ),
            ownTransporters : ownTransporters,
            ferry : COUNTERPOINTAI._ferryContext(
                system,
                ai,
                plans,
                roster,
                enemyBuildings,
                map,
                islandMode,
                ownSnapshots,
                ownTransporters
            ),
            banIndirects : banIndirects,
            indirectCo : indirectCo,
            fieldedIndirects : COUNTERPOINTAI._countPureIndirects(ownSnapshots)
        };
    },

    // Which transports can actually deliver a ground capturer to somewhere we cannot already walk.
    // Air needs no shore, so it is asked directly; sea and hover need a mutually passable tile.
    _ferryContext : function(system, ai, plans, roster, enemyBuildings, map, islandMode,
                             ownSnapshots, ownTransporters)
    {
        var ferry = { strandedShare : 0, stranded : 0, urgent : false, measured : false,
                      cost : 0, want : 0, deliverable : Object.create(null),
                      needed : Object.create(null), plans : Object.create(null),
                      unitsByPlan : Object.create(null) };
        if (islandMode !== true)
        {
            return ferry;
        }
        var capper = roster.capper;
        var homes = roster.homes;
        if (capper === null || homes.length === 0)
        {
            return ferry;
        }
        var capperDummy = system.getDummyUnit(String(capper.id));
        if (capperDummy === null || capperDummy === undefined)
        {
            return ferry;
        }
        var capperMove = String(capperDummy.getMovementType());
        var created = Object.create(null);
        if (!COUNTERPOINTAI._ensureIslandMapFor(ai, created, capperMove, capper.id))
        {
            return ferry;
        }
        var targets = COUNTERPOINTAI._captureTargets(map, enemyBuildings);
        if (targets.length === 0)
        {
            return ferry;
        }
        var stranded = [];
        for (var targetIndex = 0; targetIndex < targets.length; ++targetIndex)
        {
            var target = targets[targetIndex];
            var walkable = false;
            for (var homeIndex = 0; homeIndex < homes.length && !walkable; ++homeIndex)
            {
                walkable = ai.onSameIsland(
                    capperMove,
                    homes[homeIndex].x,
                    homes[homeIndex].y,
                    target.x,
                    target.y
                );
            }
            if (!walkable)
            {
                stranded.push(target);
            }
        }
        ferry.stranded = stranded.length;
        ferry.strandedShare = stranded.length / targets.length;
        if (stranded.length === 0)
        {
            return ferry;
        }

        // How many hulls the urgent path may buy outright. Stranded targets do not fall when a boat
        // is bought, so without a demand cap urgency has no end and the port builds nothing else.
        ferry.want = COUNTERPOINTAI._ferryHullsWanted(ownSnapshots);
        var counts = ownTransporters || {};
        var domains = [COUNTERPOINTAI.DOMAIN_GROUND, COUNTERPOINTAI.DOMAIN_AIR,
                       COUNTERPOINTAI.DOMAIN_NAVAL, COUNTERPOINTAI.DOMAIN_HOVER];
        for (var domainIndex = 0; domainIndex < domains.length; ++domainIndex)
        {
            ferry.needed[domains[domainIndex]] =
                COUNTERPOINTAI._readNumber(counts, domains[domainIndex], 0) < ferry.want;
        }

        var capperIds = roster.capperIds;

        // Keyed on movement type and dock rather than domain: a lander and a black boat are
        // different movement types, so a verdict for one says nothing about the other.
        // Read from the roster, not the batch. A port whose hulls are all unaffordable reports no
        // build action, so it never becomes a plan, so a batch-derived scan saw no transport, so
        // nothing was ever urgent and nothing was ever saved, so the funds never arrived. The port
        // has to be visible while it is still unbuildable for that loop to be broken.
        var verdicts = Object.create(null);
        for (var index = 0; index < roster.transports.length; ++index)
        {
            var transport = roster.transports[index];
            if (!COUNTERPOINTAI._carriesAnyOf(transport.cargoIds, capperIds))
            {
                continue;
            }
            var key = transport.move + "@" + transport.x + "," + transport.y;
            if (verdicts[key] === undefined)
            {
                verdicts[key] = COUNTERPOINTAI._ensureIslandMapFor(
                        ai,
                        created,
                        transport.move,
                        transport.id
                    ) &&
                    COUNTERPOINTAI._deliversToStranded(
                        ai,
                        map,
                        transport.domain,
                        transport,
                        transport.move,
                        capperMove,
                        homes,
                        stranded
                    );
            }
            if (verdicts[key] === true)
            {
                ferry.deliverable[transport.domain] = true;
                // Only a domain still short of hulls gets first call on the surplus, and only its
                // price is worth saving towards.
                if (ferry.needed[transport.domain] === true)
                {
                    ferry.plans[transport.key] = true;
                    if (ferry.unitsByPlan[transport.key] === undefined)
                    {
                        ferry.unitsByPlan[transport.key] = Object.create(null);
                    }
                    ferry.unitsByPlan[transport.key]["#" + transport.id] = true;
                    if (transport.cost > 0 &&
                        (ferry.cost <= 0 || transport.cost < ferry.cost))
                    {
                        ferry.cost = transport.cost;
                    }
                }
            }
        }
        // measured stays false when nothing at all can deliver, which is the difference between
        // "this transport is useless" and "we could not tell". Only the former may refuse a build.
        ferry.measured = COUNTERPOINTAI._countKeys(ferry.deliverable) > 0;
        ferry.urgent = ferry.measured &&
            ferry.strandedShare >= COUNTERPOINTAI._tunable("FERRY_URGENT_STRANDED_SHARE");
        return ferry;
    },

    // Hulls wanted at once, sized by the capturers there are to carry. One is always wanted, since
    // a map that strands its targets needs a first boat before it can have anything to ferry.
    _ferryHullsWanted : function(ownSnapshots)
    {
        var cappers = 0;
        var length = COUNTERPOINTAI._collectionLength(ownSnapshots);
        for (var index = 0; index < length; ++index)
        {
            var unit = COUNTERPOINTAI._collectionAt(ownSnapshots, index);
            if (unit !== null && unit !== undefined && unit.canCapture === true &&
                unit.domain === COUNTERPOINTAI.DOMAIN_GROUND)
            {
                cappers += 1;
            }
        }
        var perHull = Math.max(1, Math.floor(COUNTERPOINTAI._tunable("FERRY_CAPPERS_PER_HULL")));
        return COUNTERPOINTAI._clamp(
            Math.floor(cappers / perHull),
            1,
            Math.max(1, Math.floor(COUNTERPOINTAI._tunable("FERRY_MAX_HULLS")))
        );
    },

    // On an island map a transport that cannot deliver a capturer anywhere new is dead weight, so
    // it is refused outright rather than left to the turn ramp. One that can is offered straight
    // away once enough of the map is unwalkable, instead of waiting for the ramp to climb.
    _admitTransports : function(context, state, domain, turn)
    {
        var ferry = context.ferry;
        if (context.islandMode === true && ferry !== null && ferry !== undefined &&
            ferry.stranded > 0 && ferry.measured === true)
        {
            if (ferry.deliverable[domain] !== true)
            {
                return false;
            }
            // Having measured the demand, "enough hulls already" is an answer, not a gap in what we
            // know, so it refuses outright. Leaving the turn ramp to decide meant a satisfied fleet
            // still drew for another boat every turn, and at 40 percent a draw it kept winning.
            if (ferry.needed[domain] !== true)
            {
                return false;
            }
            if (ferry.urgent === true)
            {
                return true;
            }
        }
        return context.transportContext[domain] === true &&
            COUNTERPOINTAI._nextPlannerRoll(state, COUNTERPOINTAI._tunable("PERCENT_MAX")) <
                COUNTERPOINTAI._transportChance(
                    context.ownTransporters[domain],
                    turn,
                    domain,
                    context.islandMode
                );
    },

    _reachesAny : function(ai, capperMove, x, y, spots)
    {
        for (var index = 0; index < spots.length; ++index)
        {
            if (ai.onSameIsland(capperMove, x, y, spots[index].x, spots[index].y))
            {
                return true;
            }
        }
        return false;
    },

    // Loading is symmetric to unloading: the capturer walks onto the transport to board, so a real
    // ferry needs terrain it shares with the transport at BOTH ends. The two tiles may differ, one
    // beach to load and another to unload. The scan carries no sample cap because capping it by
    // hits while walking the map in column order only ever collects our own coastline.
    _deliversToStranded : function(ai, map, domain, plan, transportMove, capperMove, homes, stranded)
    {
        var index = 0;
        if (domain === COUNTERPOINTAI.DOMAIN_AIR)
        {
            // Air unloads anywhere on the target's landmass, so there is no shore question to ask.
            for (index = 0; index < stranded.length; ++index)
            {
                if (ai.onSameIsland(transportMove, plan.x, plan.y,
                                    stranded[index].x, stranded[index].y))
                {
                    return true;
                }
            }
            return false;
        }
        var width = map.getMapWidth();
        var height = map.getMapHeight();
        var canLoad = false;
        var canUnload = false;
        for (var x = 0; x < width; ++x)
        {
            for (var y = 0; y < height; ++y)
            {
                if (!ai.onSameIsland(transportMove, plan.x, plan.y, x, y))
                {
                    continue;
                }
                if (!canLoad)
                {
                    canLoad = COUNTERPOINTAI._reachesAny(ai, capperMove, x, y, homes);
                }
                if (!canUnload)
                {
                    canUnload = COUNTERPOINTAI._reachesAny(ai, capperMove, x, y, stranded);
                }
                if (canLoad && canUnload)
                {
                    return true;
                }
            }
        }
        return false;
    },

    _countKeys : function(table)
    {
        var count = 0;
        for (var key in table)
        {
            if (Object.prototype.hasOwnProperty.call(table, key))
            {
                ++count;
            }
        }
        return count;
    },

    _createIslandMapFor : function(ai, created, movementType, unitId)
    {
        if (movementType === "" || movementType === null || movementType === undefined)
        {
            return false;
        }
        if (created[movementType] === true)
        {
            return true;
        }
        ai.createIslandMap(movementType, String(unitId));
        created[movementType] = true;
        return true;
    },

    // Ferry analysis caps its map count because modded rosters may expose many movement types.
    _ensureIslandMapFor : function(ai, created, movementType, unitId)
    {
        if (movementType === "" || movementType === null || movementType === undefined)
        {
            return false;
        }
        if (created[movementType] === true)
        {
            return true;
        }
        if (COUNTERPOINTAI._countKeys(created) >=
            Math.max(1, Math.floor(COUNTERPOINTAI._tunable("MAX_ISLAND_MAPS"))))
        {
            return false;
        }
        return COUNTERPOINTAI._createIslandMapFor(
            ai,
            created,
            movementType,
            unitId
        );
    },

    _appendBuildingPositions : function(targets, collection, limit)
    {
        var length = COUNTERPOINTAI._collectionLength(collection);
        for (var index = 0; index < length && targets.length < limit; ++index)
        {
            var building = COUNTERPOINTAI._collectionAt(collection, index);
            if (building === null || building === undefined)
            {
                continue;
            }
            targets.push({ x : building.getX(), y : building.getY() });
        }
    },

    // Both matter: an enemy town is a target, and so is an unclaimed neutral one. Each side gets
    // its own budget, otherwise a map with many enemy buildings crowds the neutrals out entirely.
    _captureTargets : function(map, enemyBuildings)
    {
        var limit = Math.max(1, Math.floor(COUNTERPOINTAI._tunable("FERRY_TARGET_SAMPLE")));
        var targets = [];
        COUNTERPOINTAI._appendBuildingPositions(targets, enemyBuildings, limit);
        limit += targets.length;
        var neutral = null;
        try
        {
            neutral = map.getBuildings(null);
        }
        catch (neutralError)
        {
            neutral = null;
        }
        COUNTERPOINTAI._appendBuildingPositions(targets, neutral, limit);
        return targets;
    },

    _spreadBudget : function(targets, amount)
    {
        var value = COUNTERPOINTAI._wholeCount(amount);
        if (value === 0 || targets.length === 0)
        {
            return;
        }
        var perPlan = Math.floor(value / targets.length);
        var remainder = value - perPlan * targets.length;
        for (var index = 0; index < targets.length; ++index)
        {
            targets[index].reservedBudget += perPlan;
            if (remainder > 0)
            {
                targets[index].reservedBudget += 1;
                remainder -= 1;
            }
        }
    },

    _spreadBudgetWithinCap : function(targets, amount, cap)
    {
        var remaining = COUNTERPOINTAI._wholeCount(amount);
        var active = [];
        for (var targetIndex = 0; targetIndex < targets.length; ++targetIndex)
        {
            if (targets[targetIndex].reservedBudget < cap)
            {
                active.push(targets[targetIndex]);
            }
        }
        while (remaining > 0 && active.length > 0)
        {
            var perPlan = Math.floor(remaining / active.length);
            var extra = remaining - perPlan * active.length;
            var next = [];
            var allocated = 0;
            for (var index = 0; index < active.length; ++index)
            {
                var share = perPlan + (index < extra ? 1 : 0);
                var capacity = Math.max(0, cap - active[index].reservedBudget);
                var addition = Math.min(share, capacity);
                active[index].reservedBudget += addition;
                remaining -= addition;
                allocated += addition;
                if (capacity > addition)
                {
                    next.push(active[index]);
                }
            }
            if (allocated === 0)
            {
                break;
            }
            active = next;
        }
        return remaining;
    },

    _remainingBudgetPlans : function(plans, planIndex)
    {
        var source = plans[planIndex];
        var targets = [];
        for (var index = 0; index < plans.length; ++index)
        {
            if (index === planIndex)
            {
                continue;
            }
            var candidate = plans[index];
            if (candidate.phase === source.phase && candidate.hasPaid &&
                !candidate.complete && !candidate.skipped &&
                (COUNTERPOINTAI.RANDOM_SURPLUS_FUNDING !== true ||
                 candidate.randomSurplusFunded === true))
            {
                targets.push(candidate);
            }
        }
        return targets;
    },

    // Never leave a factory that can produce a capturer empty: its floor is that capturer's own
    // menu cost, so a roster whose cheapest foot soldier is 1500 needs no retuning. A plan with no
    // capturer, which is an airport or port on most rosters, reserves nothing and stays skippable,
    // and that is what leaves room for an expensive transport.
    // Only ground capturers set a floor. Naval capturers exist (a gunboat carries ACTION_CAPTURE),
    // but reserving a harbour's gunboat cost would starve the ground factories that actually do the
    // capturing, and ports are meant to stay skippable.
    // Deliberately not gated on candidate.enabled: the engine sets that from whether the player can
    // afford the unit right now (resources/scripts/actions/ACTION_BUILD_UNITS.js), so gating on it
    // would collapse the floor to 0 exactly when funds are tight, which is when it matters most.
    _planFloor : function(plan)
    {
        var allDomains = COUNTERPOINTAI.FLOOR_ALL_DOMAINS === true;
        var bestCapper = -1;
        var bestUnit = -1;
        for (var index = 0; plan.candidates && index < plan.candidates.length; ++index)
        {
            var candidate = plan.candidates[index];
            if (!COUNTERPOINTAI._isKnownDomain(candidate.domain) ||
                (!allDomains && candidate.domain !== COUNTERPOINTAI.DOMAIN_GROUND) ||
                candidate.isTransporter === true ||
                typeof candidate.transactionCost !== "number" ||
                candidate.transactionCost < 0 ||
                COUNTERPOINTAI._candidateRejected(plan, index))
            {
                continue;
            }
            if (bestUnit < 0 || candidate.transactionCost < bestUnit)
            {
                bestUnit = candidate.transactionCost;
            }
            if (candidate.canCapture === true &&
                (bestCapper < 0 || candidate.transactionCost < bestCapper))
            {
                bestCapper = candidate.transactionCost;
            }
        }
        var floor = bestCapper >= 0 ? bestCapper : bestUnit;
        if (floor < 0)
        {
            return 0;
        }
        return Math.max(floor, COUNTERPOINTAI._wholeCount(COUNTERPOINTAI.FACTORY_FLOOR));
    },

    // Ceiling a saving target may cost, so an unreachable bomber stops outranking every factory.
    _planValueReach : function(ai, roster, funds)
    {
        var spare = COUNTERPOINTAI._spareThisTurn(ai, roster, funds);
        var horizon = Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(
            COUNTERPOINTAI._tunable("VALUE_TARGET_MAX_TURNS"),
            0
        )));
        return Math.max(0, spare.surplus) + Math.max(0, spare.perTurn) * horizon;
    },

    _planValueTarget : function(plan, reach)
    {
        if (COUNTERPOINTAI.DYNAMIC_FLOOR === false)
        {
            return { value : 0, target : 0 };
        }
        // A negative reach turns the horizon off, which is what callers without one pass.
        var affordable = COUNTERPOINTAI._finiteNumber(reach, -1);
        var valuedCost = -1;
        var valuedScore = 0;
        for (var index = 0; plan.candidates && index < plan.candidates.length; ++index)
        {
            var candidate = plan.candidates[index];
            if (candidate.isTransporter === true ||
                typeof candidate.transactionCost !== "number" ||
                candidate.transactionCost < 0 ||
                (affordable >= 0 && candidate.transactionCost > affordable) ||
                COUNTERPOINTAI._candidateRejected(plan, index))
            {
                continue;
            }
            var score = typeof candidate.orderScore === "number" ? candidate.orderScore : 0;
            if (score > 0 && (score > valuedScore ||
                (score === valuedScore && candidate.transactionCost < valuedCost)))
            {
                valuedScore = score;
                valuedCost = candidate.transactionCost;
            }
        }
        return valuedCost >= 0 ?
            { value : valuedScore, target : valuedCost } : { value : 0, target : 0 };
    },

    _isGroundFactoryPlan : function(plan)
    {
        for (var index = 0; plan.candidates && index < plan.candidates.length; ++index)
        {
            var candidate = plan.candidates[index];
            if (candidate.domain === COUNTERPOINTAI.DOMAIN_GROUND &&
                candidate.isTransporter !== true)
            {
                return true;
            }
        }
        return false;
    },

    // Choose the cheapest enabled rescue to preserve sibling budgets.
    _cheapestBuildableCandidate : function(plan, plans, turnTargets)
    {
        var best = -1;
        var bestCost = -1;
        for (var index = 0; plan.candidates && index < plan.candidates.length; ++index)
        {
            if (COUNTERPOINTAI._candidateRejected(plan, index))
            {
                continue;
            }
            var candidate = plan.candidates[index];
            if (candidate.enabled !== true ||
                candidate.domain !== COUNTERPOINTAI.DOMAIN_GROUND ||
                candidate.isTransporter === true ||
                typeof candidate.transactionCost !== "number" ||
                candidate.transactionCost < 0 ||
                COUNTERPOINTAI._turnSaturatedSkipped(candidate, plans, turnTargets))
            {
                continue;
            }
            if (best < 0 || candidate.transactionCost < bestCost)
            {
                best = index;
                bestCost = candidate.transactionCost;
            }
        }
        return best;
    },

    _hasPlanKey : function(plans, key)
    {
        return COUNTERPOINTAI._planByKey(plans, key) !== null;
    },

    // Candidates are not enriched yet at allocation time for a building with no plan, so read the
    // dummy directly rather than relying on candidate.canCapture.
    _cheapestGroundCapperCost : function(system, data)
    {
        var ids = data.getUnitIds();
        var costs = data.getTransactionCosts();
        var length = COUNTERPOINTAI._collectionLength(ids);
        var best = -1;
        for (var index = 0; index < length; ++index)
        {
            var cost = COUNTERPOINTAI._collectionAt(costs, index);
            if (typeof cost !== "number" || cost < 0 || (best >= 0 && cost >= best))
            {
                continue;
            }
            var dummy = system.getDummyUnit(String(COUNTERPOINTAI._collectionAt(ids, index)));
            if (dummy === null || dummy === undefined || dummy.canCapture() !== true ||
                COUNTERPOINTAI._domainFromUnitType(dummy.getUnitType()) !==
                    COUNTERPOINTAI.DOMAIN_GROUND)
            {
                continue;
            }
            best = cost;
        }
        return best < 0 ? 0 : best;
    },

    // Everything this match can produce, read from the player's own facilities once per planning
    // pass. Plan batches are partial by nature: a factory with a unit parked on it reports no build
    // action and is planned in a later batch, so a batch can hold nothing but a harbour. Any fact
    // about what can be built therefore has to come from here. Deriving one from the batch in hand
    // silently changes the answer depending on which factories happened to be free that turn.
    _fieldedHpById : function(snapshots)
    {
        var totals = Object.create(null);
        for (var index = 0; index < snapshots.length; ++index)
        {
            var key = "#" + snapshots[index].id;
            totals[key] = (totals[key] || 0) +
                Math.max(0, COUNTERPOINTAI._readNumber(snapshots[index], "hp", 0));
        }
        return totals;
    },

    _phantomProductionFloors : function(system, enemyBuildings)
    {
        var result = { totals : Object.create(null), facilities : Object.create(null) };
        var groundCappers = Object.create(null);
        var length = COUNTERPOINTAI._collectionLength(enemyBuildings);
        for (var index = 0; index < length; ++index)
        {
            var building = COUNTERPOINTAI._collectionAt(enemyBuildings, index);
            if (building === null || building === undefined)
            {
                continue;
            }
            var owner = building.getOwner();
            if (owner === null || owner === undefined)
            {
                continue;
            }
            var best = -1;
            var ids = building.getConstructionList();
            var idLength = COUNTERPOINTAI._collectionLength(ids);
            for (var idIndex = 0; idIndex < idLength; ++idIndex)
            {
                var id = String(COUNTERPOINTAI._collectionAt(ids, idIndex));
                var key = "#" + id;
                if (groundCappers[key] === undefined)
                {
                    var dummy = system.getDummyUnit(id);
                    groundCappers[key] = dummy !== null && dummy !== undefined &&
                        dummy.canCapture() === true &&
                        COUNTERPOINTAI._domainFromUnitType(dummy.getUnitType()) ===
                            COUNTERPOINTAI.DOMAIN_GROUND;
                }
                if (groundCappers[key] !== true)
                {
                    continue;
                }
                var cost = COUNTERPOINTAI._finiteNumber(
                    owner.getCosts(id, building.getPosition()),
                    -1
                );
                if (cost >= 0 && (best < 0 || cost < best))
                {
                    best = cost;
                }
            }
            var floor = best < 0 ? 0 : Math.max(
                best,
                COUNTERPOINTAI._wholeCount(COUNTERPOINTAI.FACTORY_FLOOR)
            );
            var ownerKey = "#" + owner.getPlayerID();
            var facilityKey = ownerKey + ":" + building.getX() + "," + building.getY();
            result.facilities[facilityKey] = floor;
            result.totals[ownerKey] = (result.totals[ownerKey] || 0) + floor;
        }
        return result;
    },

    _phantomFitsProductionBudget : function(cost, budget, totalFloor, sourceFloor)
    {
        return cost + Math.max(0, totalFloor - sourceFloor) <= budget;
    },

    // Credible reinforcements preserve every other enemy factory's production floor.
    _phantomThreats : function(system, enemyBuildings, candidateIds, armoredIds, airIds,
                               fieldedHp, damageCache)
    {
        var threats = [];
        if (!(COUNTERPOINTAI._tunable("PHANTOM_RETAL_WEIGHT") > 0) &&
            !(COUNTERPOINTAI._tunable("PHANTOM_OFFENSE_WEIGHT") > 0))
        {
            return threats;
        }
        var budgets = Object.create(null);
        var taken = Object.create(null);
        var knownUnits = Object.create(null);
        var candidateWeaponData = Object.create(null);
        var productionFloors = COUNTERPOINTAI._phantomProductionFloors(
            system,
            enemyBuildings
        );
        var length = COUNTERPOINTAI._collectionLength(enemyBuildings);
        for (var index = 0; index < length; ++index)
        {
            var building = COUNTERPOINTAI._collectionAt(enemyBuildings, index);
            if (building === null || building === undefined)
            {
                continue;
            }
            var owner = building.getOwner();
            if (owner === null || owner === undefined)
            {
                continue;
            }
            var ownerKey = "#" + owner.getPlayerID();
            if (budgets[ownerKey] === undefined)
            {
                budgets[ownerKey] = COUNTERPOINTAI._wholeCount(owner.getFunds()) +
                    COUNTERPOINTAI._wholeCount(owner.calcIncome());
            }
            var facilityKey = ownerKey + ":" + building.getX() + "," + building.getY();
            var totalFloor = COUNTERPOINTAI._readNumber(
                productionFloors.totals,
                ownerKey,
                0
            );
            var sourceFloor = COUNTERPOINTAI._readNumber(
                productionFloors.facilities,
                facilityKey,
                0
            );
            var ids = building.getConstructionList();
            var idLength = COUNTERPOINTAI._collectionLength(ids);
            for (var idIndex = 0; idIndex < idLength; ++idIndex)
            {
                var id = String(COUNTERPOINTAI._collectionAt(ids, idIndex));
                var key = "#" + id;
                if (taken[key] === true ||
                    COUNTERPOINTAI._readUnitNumber(fieldedHp, id, 0) >=
                        COUNTERPOINTAI.PHANTOM_PRESENT_MIN_HP)
                {
                    continue;
                }
                var probe = null;
                if (knownUnits[key] === undefined)
                {
                    probe = system.getDummyUnit(id);
                    knownUnits[key] = probe !== null && probe !== undefined;
                }
                if (knownUnits[key] !== true)
                {
                    taken[key] = true;
                    continue;
                }
                var transactionCost = COUNTERPOINTAI._finiteNumber(
                    owner.getCosts(id, building.getPosition()),
                    -1
                );
                if (transactionCost < 0 ||
                    !COUNTERPOINTAI._phantomFitsProductionBudget(
                        transactionCost,
                        budgets[ownerKey],
                        totalFloor,
                        sourceFloor
                    ))
                {
                    continue;
                }
                taken[key] = true;
                var dummy = probe !== null && probe !== undefined ?
                    probe : system.getDummyUnit(id);
                if (dummy === null || dummy === undefined)
                {
                    continue;
                }
                var snapshot = COUNTERPOINTAI._snapshotUnit(
                    system,
                    dummy,
                    id,
                    Math.max(1, transactionCost),
                    candidateIds,
                    armoredIds,
                    airIds,
                    damageCache
                );
                if (snapshot !== null)
                {
                    // Reverse damage credits candidates that can fight the expected buy.
                    snapshot.damageTakenById = Object.create(null);
                    snapshot.directDamageTakenById = Object.create(null);
                    snapshot.indirectDamageTakenById = Object.create(null);
                    for (var takenIndex = 0; takenIndex < candidateIds.length; ++takenIndex)
                    {
                        var candidateId = candidateIds[takenIndex];
                        var candidateKey = "#" + candidateId;
                        if (candidateWeaponData[candidateKey] === undefined)
                        {
                            var candidateDummy = system.getDummyUnit(candidateId);
                            candidateWeaponData[candidateKey] = candidateDummy === null ||
                                candidateDummy === undefined ? null : COUNTERPOINTAI._weaponData(
                                    candidateDummy,
                                    candidateDummy.getBaseMinRange(),
                                    candidateDummy.getBaseMaxRange()
                                );
                        }
                        var reverseProfile = null;
                        if (candidateWeaponData[candidateKey] !== null)
                        {
                            reverseProfile = COUNTERPOINTAI._damageProfile(
                                system,
                                candidateId,
                                [id],
                                candidateWeaponData[candidateKey],
                                damageCache
                            );
                        }
                        var reverseKey = "#" + id;
                        if (reverseProfile === null)
                        {
                            snapshot.damageTakenById[candidateKey] = Math.max(
                                0,
                                COUNTERPOINTAI._finiteNumber(
                                    system.getCounterpointBaseDamage(candidateId, id),
                                    0
                                )
                            );
                            continue;
                        }
                        snapshot.damageTakenById[candidateKey] =
                            COUNTERPOINTAI._readNumber(reverseProfile.damageById, reverseKey, 0);
                        snapshot.directDamageTakenById[candidateKey] = COUNTERPOINTAI._readNumber(
                            reverseProfile.directDamageById,
                            reverseKey,
                            0
                        );
                        snapshot.indirectDamageTakenById[candidateKey] =
                            COUNTERPOINTAI._readNumber(
                                reverseProfile.indirectDamageById,
                                reverseKey,
                                0
                            );
                    }
                    threats.push(snapshot);
                }
            }
        }
        return threats;
    },

    _productionRoster : function(system, ai, buildings)
    {
        var playerId = ai.getPlayer().getPlayerID();
        var length = COUNTERPOINTAI._collectionLength(buildings);
        var roster = {
            unitIds : [],
            groundIds : Object.create(null),
            capperIds : Object.create(null),
            carriers : Object.create(null),
            cheapestCost : Object.create(null),
            domains : { ground : false, air : false, naval : false, hover : false },
            capper : null,
            homes : [],
            transports : [],
            floorTotal : 0
        };
        var floor = COUNTERPOINTAI._wholeCount(COUNTERPOINTAI.FACTORY_FLOOR);
        var seen = Object.create(null);
        var checked = Object.create(null);
        for (var index = 0; index < length; ++index)
        {
            var building = COUNTERPOINTAI._collectionAt(buildings, index);
            if (building === null || building === undefined ||
                building.getOwnerID() !== playerId ||
                !COUNTERPOINTAI._hasAction(building, COUNTERPOINTAI.ACTION_BUILD_UNITS))
            {
                continue;
            }
            var data = system.getProductionActionData(
                building,
                COUNTERPOINTAI.ACTION_BUILD_UNITS
            );
            if (data === null || data === undefined)
            {
                continue;
            }
            var ids = data.getUnitIds();
            var costs = data.getTransactionCosts();
            var idLength = COUNTERPOINTAI._collectionLength(ids);
            var facilityCapper = -1;
            for (var idIndex = 0; idIndex < idLength; ++idIndex)
            {
                var cost = COUNTERPOINTAI._collectionAt(costs, idIndex);
                if (typeof cost !== "number" || cost < 0)
                {
                    continue;
                }
                var id = String(COUNTERPOINTAI._collectionAt(ids, idIndex));
                var key = "#" + id;
                // Memoised across facilities: the roster repeats at every one, and each miss costs
                // a dummy unit.
                if (checked[key] === undefined)
                {
                    var dummy = system.getDummyUnit(id);
                    checked[key] = dummy === null || dummy === undefined ? null : {
                        domain : COUNTERPOINTAI._domainFromUnitType(dummy.getUnitType()),
                        canCapture : dummy.canCapture() === true,
                        isTransporter : dummy.isTransporter() === true,
                        move : String(dummy.getMovementType()),
                        cargoIds : dummy.isTransporter() === true ?
                            COUNTERPOINTAI._stringList(dummy.getTransportUnits()) : []
                    };
                }
                var info = checked[key];
                if (info === null)
                {
                    continue;
                }
                if (seen[key] !== true)
                {
                    seen[key] = true;
                    roster.unitIds.push(id);
                }
                if (roster.cheapestCost[key] === undefined || cost < roster.cheapestCost[key])
                {
                    roster.cheapestCost[key] = cost;
                }
                roster.domains[info.domain] = true;
                // Every dock that can build a hull, whether or not the engine will let it today.
                // The whole point is to know a ferry is wanted while the port is still unbuildable.
                if (info.isTransporter === true)
                {
                    roster.transports.push({
                        id : id,
                        cost : cost,
                        domain : info.domain,
                        move : info.move,
                        cargoIds : info.cargoIds,
                        x : building.getX(),
                        y : building.getY(),
                        key : COUNTERPOINTAI._planKey(
                            building.getX(),
                            building.getY(),
                            COUNTERPOINTAI.ACTION_BUILD_UNITS
                        )
                    });
                }
                if (info.domain !== COUNTERPOINTAI.DOMAIN_GROUND)
                {
                    continue;
                }
                roster.groundIds[key] = true;
                if (info.canCapture !== true)
                {
                    continue;
                }
                roster.capperIds[key] = true;
                if (facilityCapper < 0 || cost < facilityCapper)
                {
                    facilityCapper = cost;
                }
                if (roster.capper === null || cost < roster.capper.cost)
                {
                    roster.capper = { id : id, cost : cost };
                }
            }
            if (facilityCapper >= 0)
            {
                roster.homes.push({ x : building.getX(), y : building.getY() });
                // Same rule as _planFloor, so what saving treats as committed matches what the
                // allocation will actually reserve once every factory has a plan.
                roster.floorTotal += Math.max(facilityCapper, floor);
            }
        }
        // Second pass, because a hull is only a ferry relative to the full ground roster and the
        // ground ids are not complete until every facility has been read.
        for (var transportIndex = 0;
             transportIndex < roster.transports.length;
             ++transportIndex)
        {
            var transport = roster.transports[transportIndex];
            if (COUNTERPOINTAI._carriesAnyOf(transport.cargoIds, roster.groundIds))
            {
                roster.carriers["#" + transport.id] = true;
            }
        }
        return roster;
    },

    _stringList : function(collection)
    {
        var list = [];
        var length = COUNTERPOINTAI._collectionLength(collection);
        for (var index = 0; index < length; ++index)
        {
            list.push(String(COUNTERPOINTAI._collectionAt(collection, index)));
        }
        return list;
    },

    _hasAction : function(building, actionId)
    {
        var actions = building.getActionList();
        var length = COUNTERPOINTAI._collectionLength(actions);
        for (var index = 0; index < length; ++index)
        {
            if (String(COUNTERPOINTAI._collectionAt(actions, index)) === actionId)
            {
                return true;
            }
        }
        return false;
    },

    _blockedProductionStatus : function(system, ai, buildings, state, pendingPlans)
    {
        var playerId = ai.getPlayer().getPlayerID();
        var length = COUNTERPOINTAI._collectionLength(buildings);
        var status = { count : 0, capperReserve : 0 };
        for (var index = 0; index < length; ++index)
        {
            var building = COUNTERPOINTAI._collectionAt(buildings, index);
            if (building === null || building === undefined ||
                building.getOwnerID() !== playerId ||
                !COUNTERPOINTAI._hasAction(building, COUNTERPOINTAI.ACTION_BUILD_UNITS) ||
                building.getTerrain().getUnit() === null)
            {
                continue;
            }
            var key = COUNTERPOINTAI._planKey(
                building.getX(),
                building.getY(),
                COUNTERPOINTAI.ACTION_BUILD_UNITS
            );
            if (COUNTERPOINTAI._hasPlanKey(state.plans, key) ||
                COUNTERPOINTAI._hasPlanKey(pendingPlans, key))
            {
                continue;
            }
            ++status.count;
            var data = system.getProductionActionData(
                building,
                COUNTERPOINTAI.ACTION_BUILD_UNITS
            );
            if (data === null || data === undefined ||
                COUNTERPOINTAI.RESERVE_BLOCKED_FACTORIES === false)
            {
                continue;
            }
            status.capperReserve += COUNTERPOINTAI._cheapestGroundCapperCost(system, data);
        }
        return status;
    },

    _spendableFunds : function(state, ai)
    {
        var funds = COUNTERPOINTAI._wholeCount(ai.getPlayer().getFunds());
        var held = COUNTERPOINTAI._wholeCount(state.heldFunds) +
            COUNTERPOINTAI._strategicHeldFunds(state.plans);
        return Math.max(0, funds - Math.min(funds, held));
    },

    _strategicHeldFunds : function(plans)
    {
        var held = 0;
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var plan = plans[planIndex];
            if (plan.complete && plan.strategicHold)
            {
                held += COUNTERPOINTAI._wholeCount(plan.reservedBudget);
            }
        }
        return held;
    },

    // What a turn can spare above the capturer floors, which the hold must never eat into.
    // Money left unallocated stays in the bank into the next turn, which is the whole saving
    // mechanism: a purchase too dear for one turn's spare money is reached by holding some back.
    // Nothing is held once the spare money already covers it, or when it can never be reached, so
    // an unaffordable target never turns into a permanent hoard. Between those, only what has to
    // survive the turn is held, since next turn's income pays the rest and idling the whole surplus
    // would park money that could have bought something now. Short of that amount everything spare
    // is held so it accumulates, which is what a horizon past one turn is for.
    _savingHold : function(surplus, perTurn, cost, turns)
    {
        var horizon = Math.max(1, Math.floor(COUNTERPOINTAI._finiteNumber(turns, 1)));
        if (surplus <= 0 || cost <= 0 || surplus >= cost || perTurn <= 0 ||
            surplus + perTurn * horizon < cost)
        {
            return 0;
        }
        var needed = cost - perTurn;
        return needed <= 0 ? 0 : Math.min(surplus, needed);
    },

    // Spare money is what is left after every facility's capturer bill, counted across all of them
    // rather than the batch in hand: a batch holding only a harbour reported no floors at all, so
    // the whole treasury looked free to bank.
    _spareThisTurn : function(ai, roster, funds)
    {
        var floors = Math.max(0, COUNTERPOINTAI._finiteNumber(roster.floorTotal, 0));
        return {
            surplus : COUNTERPOINTAI._wholeCount(funds) - floors,
            perTurn : COUNTERPOINTAI._finiteNumber(ai.getPlayer().calcIncome(), 0) - floors
        };
    },

    _ferrySaving : function(ai, roster, ferry, funds)
    {
        if (COUNTERPOINTAI.SAVE_FOR_FERRY === false || ferry === null || ferry === undefined ||
            ferry.urgent !== true || ferry.cost <= 0)
        {
            return 0;
        }
        var spare = COUNTERPOINTAI._spareThisTurn(ai, roster, funds);
        return COUNTERPOINTAI._savingHold(
            spare.surplus,
            spare.perTurn,
            ferry.cost,
            COUNTERPOINTAI._tunable("FERRY_SAVE_MAX_TURNS")
        );
    },

    // Future counters are enabled purchases above the surplus left after production floors.
    _futureCounterAnswers : function(context, enemyId, surplus, reach)
    {
        var answers = [];
        var plans = context.plans || [];
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var plan = plans[planIndex];
            if (plan.actionId !== COUNTERPOINTAI.ACTION_BUILD_UNITS ||
                !plan.hasPaid || plan.complete || plan.skipped)
            {
                continue;
            }
            for (var candidateIndex = 0;
                 candidateIndex < plan.candidates.length;
                 ++candidateIndex)
            {
                var candidate = plan.candidates[candidateIndex];
                var cost = candidate.transactionCost;
                if (candidate.scoreData === undefined || !candidate.enabled ||
                    cost <= surplus || cost > reach || cost < 0 ||
                    COUNTERPOINTAI._candidateRejected(plan, candidateIndex) ||
                    (!COUNTERPOINTAI._capperBuildsAllowed() && candidate.canCapture) ||
                    (context.banIndirects &&
                     COUNTERPOINTAI._isPureIndirect(candidate.scoreData)))
                {
                    continue;
                }
                var damage = COUNTERPOINTAI._usableDamageAgainst(
                    candidate.scoreData,
                    enemyId,
                    context.banIndirects
                );
                if (damage > 0)
                {
                    answers.push({ damage : damage, cost : cost });
                }
            }
        }
        return answers;
    },

    _currentCounterOpportunity : function(context, enemyId, surplus)
    {
        var plans = context.plans || [];
        var paidPlans = [];
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            if (plans[planIndex].hasPaid && !plans[planIndex].complete &&
                !plans[planIndex].skipped &&
                plans[planIndex].actionId === COUNTERPOINTAI.ACTION_BUILD_UNITS)
            {
                paidPlans.push(plans[planIndex]);
            }
        }
        var opportunity = { best : 0, total : 0 };
        if (paidPlans.length === 0)
        {
            return opportunity;
        }
        var fairSurplus = Math.ceil(Math.max(0, surplus) / paidPlans.length);
        for (var paidIndex = 0; paidIndex < paidPlans.length; ++paidIndex)
        {
            var plan = paidPlans[paidIndex];
            var budget = COUNTERPOINTAI._planFloor(plan) + fairSurplus;
            var bestDamage = 0;
            for (var candidateIndex = 0;
                 candidateIndex < plan.candidates.length;
                 ++candidateIndex)
            {
                var candidate = plan.candidates[candidateIndex];
                if (candidate.scoreData === undefined || !candidate.enabled ||
                    candidate.transactionCost < 0 ||
                    candidate.transactionCost > budget ||
                    COUNTERPOINTAI._candidateRejected(plan, candidateIndex) ||
                    (!COUNTERPOINTAI._capperBuildsAllowed() && candidate.canCapture) ||
                    (context.banIndirects &&
                     COUNTERPOINTAI._isPureIndirect(candidate.scoreData)))
                {
                    continue;
                }
                bestDamage = Math.max(
                    bestDamage,
                    COUNTERPOINTAI._usableDamageAgainst(
                        candidate.scoreData,
                        enemyId,
                        context.banIndirects
                    )
                );
            }
            opportunity.best = Math.max(opportunity.best, bestDamage);
            opportunity.total += bestDamage;
        }
        return opportunity;
    },

    _counterRequiredDamage : function(bestDamage, totalDamage, baseRatio, held, surplus)
    {
        var base = bestDamage * Math.max(1, COUNTERPOINTAI._finiteNumber(baseRatio, 1));
        var full = Math.max(base, totalDamage);
        var share = surplus > 0 ? COUNTERPOINTAI._clamp(held / surplus, 0, 1) : 0;
        return base + (full - base) * share;
    },

    // Pick the cheapest future answer that clears its own production opportunity cost.
    _counterTarget : function(system, context, surplus, perTurn, turns)
    {
        var enemies = context.enemyComposition;
        var length = COUNTERPOINTAI._collectionLength(enemies);
        var reach = surplus + perTurn * Math.max(1, Math.floor(turns));
        var gapRatio = COUNTERPOINTAI._tunable("COUNTER_GAP_RATIO");
        var worthRatio = COUNTERPOINTAI._tunable("COUNTER_WORTH_RATIO");
        var best = 0;
        for (var index = 0; index < length; ++index)
        {
            var enemy = COUNTERPOINTAI._collectionAt(enemies, index);
            var enemyId = COUNTERPOINTAI._unitId(enemy);
            if (enemyId === "")
            {
                continue;
            }
            var enemyHp = Math.max(0, COUNTERPOINTAI._readNumber(enemy, "hpSum", 0));
            if (enemy.canCapture === true)
            {
                enemyHp = COUNTERPOINTAI._softcapCapperHP(enemyHp);
            }
            // Same ratio the scoring uses for its gap factor, so "under covered" means one thing.
            var threatNeed = enemyHp * COUNTERPOINTAI.COVERAGE_DAMAGE_SCALE;
            if (threatNeed <= 0)
            {
                continue;
            }
            var coverage = COUNTERPOINTAI._contextNumber(
                context,
                "ownCoverage",
                COUNTERPOINTAI._unitKey(enemy),
                0
            );
            if (coverage / threatNeed >= gapRatio)
            {
                continue;
            }
            var answers = COUNTERPOINTAI._futureCounterAnswers(
                context,
                enemyId,
                surplus,
                reach
            );
            var current = COUNTERPOINTAI._currentCounterOpportunity(
                context,
                enemyId,
                surplus
            );
            var remainingNeed = Math.max(0, threatNeed - coverage);
            if (current.total >= remainingNeed)
            {
                continue;
            }
            for (var answerIndex = 0; answerIndex < answers.length; ++answerIndex)
            {
                var answer = answers[answerIndex];
                var held = COUNTERPOINTAI._savingHold(
                    surplus,
                    perTurn,
                    answer.cost,
                    turns
                );
                var requiredDamage = COUNTERPOINTAI._counterRequiredDamage(
                    current.best,
                    current.total,
                    worthRatio,
                    held,
                    surplus
                );
                if (held > 0 && answer.damage >= requiredDamage &&
                    (best <= 0 || answer.cost < best))
                {
                    best = answer.cost;
                }
            }
        }
        return best;
    },

    // Bank for a counter the way a player does: a threat we are not equipped for, and a unit that
    // answers it which this turn cannot afford but next turn can.
    // A ferry outranks a counter: on a map that strands its targets there is nothing to counter
    // with until the capturers can get out. Only one hold at a time either way, since banking for
    // both at once would idle more than either purchase needs. A zero ferry hold does not mean
    // there is no ferry, though: it also means the hull is affordable right now. Reading the two
    // the same let a counter bank exactly the money the hull was about to be bought with, and the
    // port passed the turn with nothing built.
    _savingDecision : function(system, ai, context, funds, day, blockedCount)
    {
        var ferry = context.ferry;
        var held = COUNTERPOINTAI._ferrySaving(ai, context.roster, ferry, funds);
        if (held > 0)
        {
            return held;
        }
        if (ferry !== null && ferry !== undefined && ferry.urgent === true && ferry.cost > 0 &&
            COUNTERPOINTAI._spareThisTurn(ai, context.roster, funds).surplus >= ferry.cost)
        {
            return 0;
        }
        if (blockedCount > 0)
        {
            return 0;
        }
        return COUNTERPOINTAI._counterSaving(system, ai, context, funds, day);
    },

    _counterSaving : function(system, ai, context, funds, day)
    {
        // Early on the enemy army is barely on the board, so the coverage ratio is mostly noise and
        // a hold costs a unit of opening presence to answer a threat that is not there yet.
        if (COUNTERPOINTAI.SAVE_FOR_COUNTERS === false ||
            day < Math.max(1, Math.floor(COUNTERPOINTAI._tunable("COUNTER_SAVE_MIN_DAY"))))
        {
            return 0;
        }
        var spare = COUNTERPOINTAI._spareThisTurn(ai, context.roster, funds);
        if (spare.surplus <= 0 || spare.perTurn <= 0)
        {
            return 0;
        }
        var turns = Math.max(1, Math.floor(COUNTERPOINTAI._tunable("COUNTER_SAVE_MAX_TURNS")));
        return COUNTERPOINTAI._savingHold(
            spare.surplus,
            spare.perTurn,
            COUNTERPOINTAI._counterTarget(
                system,
                context,
                spare.surplus,
                spare.perTurn,
                turns
            ),
            turns
        );
    },

    // Short of covering every floor, fund the cheapest floors first. Spreading the shortfall
    // proportionally instead would leave every factory below its own floor, so none could build and
    // none could lend, and the turn would pass with the money still in the bank.
    _fundCheapestFloorsFirst : function(paidPlans, floors, safeFunds)
    {
        var order = [];
        for (var index = 0; index < paidPlans.length; ++index)
        {
            paidPlans[index].reservedBudget = 0;
            order.push(index);
        }
        order.sort(function(left, right)
        {
            return floors[left] !== floors[right] ?
                floors[left] - floors[right] : left - right;
        });
        var remaining = safeFunds;
        for (var orderIndex = 0; orderIndex < order.length; ++orderIndex)
        {
            var planIndex = order[orderIndex];
            if (floors[planIndex] > 0 && floors[planIndex] <= remaining)
            {
                paidPlans[planIndex].reservedBudget = floors[planIndex];
                remaining -= floors[planIndex];
            }
        }
        // Whatever is left over goes to the cheapest floor rather than being stranded.
        if (remaining > 0)
        {
            paidPlans[order[0]].reservedBudget += remaining;
        }
    },

    _threatDistance : function(plan)
    {
        return Math.max(0, COUNTERPOINTAI._finiteNumber(
            plan.enemyDistance,
            COUNTERPOINTAI.PLANNER_VALUE_MAX
        ));
    },

    // Threat alone would keep one factory at the head all match, so the closest few take turns.
    _rotateFrontSlots : function(order, day)
    {
        var slots = Math.min(
            order.length,
            Math.max(1, Math.floor(COUNTERPOINTAI._tunable("SURPLUS_ROTATION_SLOTS")))
        );
        if (slots < 2)
        {
            return order;
        }
        var offset = COUNTERPOINTAI._wholeCount(day) % slots;
        return order.slice(offset, slots).concat(
            order.slice(0, offset),
            order.slice(slots)
        );
    },

    _surplusOrder : function(paidPlans, ferry, values, day)
    {
        var first = [];
        var holders = [];
        var rest = [];
        var urgent = ferry !== null && ferry !== undefined && ferry.urgent === true;
        for (var index = 0; index < paidPlans.length; ++index)
        {
            if (urgent && ferry.plans[paidPlans[index].key] === true)
            {
                first.push(index);
            }
            else if (paidPlans[index].strategicHoldEligible === true)
            {
                holders.push(index);
            }
            else
            {
                rest.push(index);
            }
        }
        if (values !== null && values !== undefined)
        {
            // Identical roster scores leave the distance key as the only real separator here.
            rest.sort(function(left, right)
            {
                if (values[left] !== values[right])
                {
                    return values[right] - values[left];
                }
                var leftDistance = COUNTERPOINTAI._threatDistance(paidPlans[left]);
                var rightDistance = COUNTERPOINTAI._threatDistance(paidPlans[right]);
                return leftDistance !== rightDistance ?
                    leftDistance - rightDistance : left - right;
            });
            rest = COUNTERPOINTAI._rotateFrontSlots(rest, day);
        }
        return first.concat(rest, holders);
    },

    _fundUrgentFerry : function(paidPlans, remaining, ferry)
    {
        if (remaining <= 0 || ferry === null || ferry === undefined ||
            ferry.urgent !== true || ferry.cost <= 0)
        {
            return remaining;
        }
        var bestPlan = null;
        var bestCost = -1;
        var bestNeeded = 0;
        for (var planIndex = 0; planIndex < paidPlans.length; ++planIndex)
        {
            var plan = paidPlans[planIndex];
            if (plan.skipped === true || plan.available !== true ||
                ferry.plans[plan.key] !== true)
            {
                continue;
            }
            var knownUnits = ferry.unitsByPlan === undefined ?
                undefined : ferry.unitsByPlan[plan.key];
            for (var candidateIndex = 0;
                 candidateIndex < plan.candidates.length;
                 ++candidateIndex)
            {
                var candidate = plan.candidates[candidateIndex];
                if (candidate.enabled !== true || candidate.isTransporter !== true ||
                    ferry.needed[candidate.domain] !== true ||
                    candidate.transactionCost <= 0 ||
                    (knownUnits === undefined && candidate.transactionCost !== ferry.cost) ||
                    (knownUnits !== undefined &&
                     !COUNTERPOINTAI._readFlag(knownUnits, candidate.id)) ||
                    COUNTERPOINTAI._candidateRejected(plan, candidateIndex))
                {
                    continue;
                }
                var needed = Math.max(
                    0,
                    candidate.transactionCost - plan.reservedBudget
                );
                if (needed <= remaining &&
                    (bestPlan === null || candidate.transactionCost < bestCost))
                {
                    bestPlan = plan;
                    bestCost = candidate.transactionCost;
                    bestNeeded = needed;
                }
            }
        }
        if (bestPlan !== null)
        {
            bestPlan.reservedBudget += bestNeeded;
            return remaining - bestNeeded;
        }
        return remaining;
    },

    _cheapestEnabledPlanCost : function(plan)
    {
        var best = -1;
        for (var index = 0; index < plan.candidates.length; ++index)
        {
            var candidate = plan.candidates[index];
            if (candidate.enabled !== true || candidate.transactionCost < 0 ||
                COUNTERPOINTAI._candidateRejected(plan, index))
            {
                continue;
            }
            if (best < 0 || candidate.transactionCost < best)
            {
                best = candidate.transactionCost;
            }
        }
        return best;
    },

    _takeRandomFundingRequests : function(state, source, count, target)
    {
        var take = Math.min(source.length, Math.max(0, count));
        for (var pick = 0; pick < take; ++pick)
        {
            var swap = pick + COUNTERPOINTAI._nextPlannerRoll(
                state,
                source.length - pick
            );
            var held = source[pick];
            source[pick] = source[swap];
            source[swap] = held;
            target.push(source[pick]);
        }
        return take;
    },

    _minimumFundingRequestCost : function(source, start, count)
    {
        if (count <= 0)
        {
            return 0;
        }
        var costs = [];
        for (var index = start; index < source.length; ++index)
        {
            costs.push(source[index].needed);
        }
        if (costs.length < count)
        {
            return -1;
        }
        costs.sort(function(left, right) { return left - right; });
        var total = 0;
        for (var costIndex = 0; costIndex < count; ++costIndex)
        {
            total += costs[costIndex];
        }
        return total;
    },

    // Persist random recipients so late rescans cannot choose a second subset.
    _fundRandomFactories : function(state, paidPlans, remaining, dynamicCap)
    {
        if (remaining <= 0)
        {
            return;
        }
        var eligible = [];
        for (var index = 0; index < paidPlans.length; ++index)
        {
            if (paidPlans[index].skipped !== true)
            {
                eligible.push(index);
            }
        }
        var owned = eligible.length;
        if (owned === 0)
        {
            return;
        }
        var phase = paidPlans[0].phase;
        var previousEligible = 0;
        var previousFunded = 0;
        for (var previousIndex = 0; previousIndex < state.plans.length; ++previousIndex)
        {
            var previous = state.plans[previousIndex];
            if (previous.phase === phase && previous.hasPaid && previous.skipped !== true &&
                previous.available === true)
            {
                previousEligible += 1;
                if (previous.randomSurplusFunded === true)
                {
                    previousFunded += 1;
                }
            }
        }
        var divisor = Math.max(1, COUNTERPOINTAI._tunable("SURPLUS_FUNDED_DIVISOR"));
        var minimum = Math.max(1, Math.floor(COUNTERPOINTAI._tunable("SURPLUS_FUNDED_MIN")));
        var totalOwned = previousEligible + owned;
        var totalMinimum = Math.min(totalOwned, minimum);
        var jitterKey = phase === COUNTERPOINTAI.PHASE_SPECIAL ?
            "specialRandomFundingJitter" : "ordinaryRandomFundingJitter";
        var jitter = state[jitterKey];
        if (jitter === null)
        {
            var jitterLimit = Math.floor(
                (COUNTERPOINTAI._randomWeightLimit() - 1) / 2
            );
            var jitterMax = Math.min(
                jitterLimit,
                Math.max(
                    0,
                    Math.floor(COUNTERPOINTAI._tunable("SURPLUS_FUNDED_JITTER"))
                )
            );
            jitter = jitterMax > 0 ?
                COUNTERPOINTAI._nextPlannerRoll(state, 2 * jitterMax + 1) - jitterMax : 0;
            state[jitterKey] = jitter;
        }
        var desiredFunded = COUNTERPOINTAI._clamp(
            Math.round(totalOwned / divisor) + jitter,
            totalMinimum,
            totalOwned
        );
        var funded = Math.min(owned, Math.max(0, desiredFunded - previousFunded));
        if (funded <= 0)
        {
            return;
        }
        var activatable = [];
        for (var eligibleIndex = 0; eligibleIndex < eligible.length; ++eligibleIndex)
        {
            var plan = paidPlans[eligible[eligibleIndex]];
            var targetCost = COUNTERPOINTAI._cheapestEnabledPlanCost(plan);
            var needed = targetCost < 0 ? -1 :
                Math.max(0, targetCost - plan.reservedBudget);
            var request = { plan : plan, needed : needed };
            if (needed >= 0 && needed <= remaining)
            {
                activatable.push(request);
            }
        }
        if (activatable.length === 0)
        {
            return;
        }
        // Size against all live factories; only actual activations consume cumulative slots.
        var activationCosts = [];
        for (var costIndex = 0; costIndex < activatable.length; ++costIndex)
        {
            activationCosts.push(activatable[costIndex].needed);
        }
        activationCosts.sort(function(left, right) { return left - right; });
        var activationCount = 0;
        var activationCost = 0;
        while (activationCount < funded && activationCount < activationCosts.length &&
               activationCost + activationCosts[activationCount] <= remaining)
        {
            activationCost += activationCosts[activationCount];
            activationCount += 1;
        }
        if (activationCount === 0)
        {
            return;
        }
        var randomized = [];
        COUNTERPOINTAI._takeRandomFundingRequests(
            state,
            activatable,
            activatable.length,
            randomized
        );
        var viable = [];
        var allocationRemaining = remaining;
        for (var randomIndex = 0; randomIndex < randomized.length; ++randomIndex)
        {
            var randomizedRequest = randomized[randomIndex];
            var slotsAfter = activationCount - viable.length - 1;
            var minimumAfter = COUNTERPOINTAI._minimumFundingRequestCost(
                randomized,
                randomIndex + 1,
                slotsAfter
            );
            if (viable.length < activationCount &&
                randomizedRequest.needed <= allocationRemaining &&
                minimumAfter >= 0 &&
                minimumAfter <= allocationRemaining - randomizedRequest.needed)
            {
                randomizedRequest.plan.reservedBudget += randomizedRequest.needed;
                allocationRemaining -= randomizedRequest.needed;
                viable.push(randomizedRequest.plan);
            }
        }
        for (var viableIndex = 0; viableIndex < viable.length; ++viableIndex)
        {
            viable[viableIndex].randomSurplusFunded = true;
        }
        COUNTERPOINTAI._spreadBudgetWithinCap(
            viable,
            allocationRemaining,
            dynamicCap
        );
    },

    _completeUnfundedRandomPlans : function(paidPlans)
    {
        for (var index = 0; index < paidPlans.length; ++index)
        {
            var plan = paidPlans[index];
            var cheapest = COUNTERPOINTAI._cheapestEnabledPlanCost(plan);
            var outsideSubset = plan.randomSurplusFunded !== true &&
                plan.hasFree !== true && plan.reservedBudget === 0;
            var fundedButUnaffordable = plan.randomSurplusFunded === true &&
                (cheapest < 0 || cheapest > plan.reservedBudget);
            if (plan.skipped !== true && (outsideSubset || fundedButUnaffordable))
            {
                plan.complete = true;
            }
        }
    },

    _allocatePhaseBudgets : function(state, plans, funds, ferry, reach)
    {
        var paidPlans = [];
        var safeFunds = COUNTERPOINTAI._wholeCount(funds);
        var skipChance = COUNTERPOINTAI._clamp(
            COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI.RANDOM_BASE_SKIP_CHANCE, 0),
            0,
            COUNTERPOINTAI._tunable("PERCENT_MAX")
        );
        var maySkip = state.day >= COUNTERPOINTAI.RANDOM_BASE_SKIP_MIN_DAY &&
            safeFunds >= COUNTERPOINTAI.RANDOM_BASE_SKIP_MIN_FUNDS;
        var dynamicCap = COUNTERPOINTAI.FACTORY_BUDGET_CAP_FLOOR;
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var plan = plans[planIndex];
            plan.reservedBudget = 0;
            if (!plan.hasPaid)
            {
                continue;
            }
            if (!plan.hasFree && maySkip && skipChance > 0 &&
                COUNTERPOINTAI._nextPlannerRoll(state, COUNTERPOINTAI._tunable("PERCENT_MAX")) < skipChance)
            {
                plan.skipped = true;
            }
            paidPlans.push(plan);
            for (var candidateIndex = 0;
                 candidateIndex < plan.candidates.length;
                 ++candidateIndex)
            {
                dynamicCap = Math.max(
                    dynamicCap,
                    plan.candidates[candidateIndex].strategicValue,
                    plan.candidates[candidateIndex].transactionCost
                );
            }
        }
        if (paidPlans.length === 0)
        {
            return;
        }

        var floors = [];
        var values = [];
        var targets = [];
        var totalFloor = 0;
        for (var totalIndex = 0; totalIndex < paidPlans.length; ++totalIndex)
        {
            floors.push(COUNTERPOINTAI._planFloor(paidPlans[totalIndex]));
            var valueTarget = COUNTERPOINTAI._planValueTarget(paidPlans[totalIndex], reach);
            values.push(valueTarget.value);
            targets.push(valueTarget.target);
            totalFloor += floors[totalIndex];
        }
        if (safeFunds < totalFloor)
        {
            COUNTERPOINTAI._fundCheapestFloorsFirst(paidPlans, floors, safeFunds);
        }
        else
        {
            for (var floorIndex = 0; floorIndex < paidPlans.length; ++floorIndex)
            {
                paidPlans[floorIndex].reservedBudget = floors[floorIndex];
            }
            var remaining = safeFunds - totalFloor;
            if (COUNTERPOINTAI.RANDOM_SURPLUS_FUNDING === true)
            {
                COUNTERPOINTAI._fundRandomFactories(state, paidPlans, remaining, dynamicCap);
                COUNTERPOINTAI._completeUnfundedRandomPlans(paidPlans);
            }
            else
            {
                var order = COUNTERPOINTAI._surplusOrder(
                    paidPlans,
                    ferry,
                    values,
                    state === null || state === undefined ? 0 : state.day
                );
                remaining = COUNTERPOINTAI._fundUrgentFerry(
                    paidPlans,
                    remaining,
                    ferry
                );
                for (var valueIndex = 0;
                     valueIndex < order.length && remaining > 0;
                     ++valueIndex)
                {
                    var valuePlanIndex = order[valueIndex];
                    if (values[valuePlanIndex] <= 0)
                    {
                        continue;
                    }
                    var reserved = paidPlans[valuePlanIndex].reservedBudget;
                    var wanted = targets[valuePlanIndex];
                    // Settle for the best pick the money covers rather than funding nothing at all,
                    // which used to leave every factory at its floor and the whole surplus on one.
                    if (wanted - reserved > remaining)
                    {
                        wanted = COUNTERPOINTAI._planValueTarget(
                            paidPlans[valuePlanIndex],
                            reserved + remaining
                        ).target;
                    }
                    var toTarget = Math.min(
                        dynamicCap,
                        Math.max(0, wanted - reserved)
                    );
                    if (toTarget <= 0 || toTarget > remaining)
                    {
                        continue;
                    }
                    paidPlans[valuePlanIndex].reservedBudget += toTarget;
                    remaining -= toTarget;
                }
                // Leftovers reach plans with something to spend them on first, then everyone else.
                // Skipping plans already at target stops the head padding past what it was funded for.
                for (var capPass = 0; capPass < 2 && remaining > 0; ++capPass)
                {
                    for (var capIndex = 0;
                         capIndex < order.length && remaining > 0;
                         ++capIndex)
                    {
                        var planIndex = order[capIndex];
                        if (capPass === 0 &&
                            (values[planIndex] <= 0 ||
                             paidPlans[planIndex].reservedBudget >= targets[planIndex]))
                        {
                            continue;
                        }
                        var capacity = Math.max(
                            0,
                            dynamicCap - paidPlans[planIndex].reservedBudget
                        );
                        var addition = Math.min(remaining, capacity);
                        paidPlans[planIndex].reservedBudget += addition;
                        remaining -= addition;
                    }
                }
                if (remaining > 0)
                {
                    paidPlans[0].reservedBudget += remaining;
                }
            }
        }

        var reclaimed = 0;
        var survivors = [];
        for (var paidIndex = 0; paidIndex < paidPlans.length; ++paidIndex)
        {
            if (paidPlans[paidIndex].skipped)
            {
                var freed = paidPlans[paidIndex].reservedBudget;
                // The skip roll stays blind, but skipping is banking, so money that was funded for
                // an urgently wanted hull stays banked instead of turning into ground units. Only
                // the hull's price is held; anything above it spreads as usual. Ferries only: a
                // ferry purchase is bound to the dock named in ferry.plans, while a counter can be
                // bought at any surviving factory, where the spread is what funds it.
                if (ferry !== null && ferry !== undefined && ferry.urgent === true &&
                    ferry.cost > 0 && ferry.plans[paidPlans[paidIndex].key] === true &&
                    state !== null && state !== undefined)
                {
                    var banked = Math.min(freed, ferry.cost);
                    state.heldFunds = COUNTERPOINTAI._wholeCount(state.heldFunds) + banked;
                    freed -= banked;
                }
                reclaimed += freed;
                paidPlans[paidIndex].reservedBudget = 0;
                paidPlans[paidIndex].complete = true;
            }
            else if (!paidPlans[paidIndex].complete &&
                     (COUNTERPOINTAI.RANDOM_SURPLUS_FUNDING !== true ||
                      paidPlans[paidIndex].randomSurplusFunded === true))
            {
                survivors.push(paidPlans[paidIndex]);
            }
        }
        if (COUNTERPOINTAI.RANDOM_SURPLUS_FUNDING === true)
        {
            COUNTERPOINTAI._spreadBudgetWithinCap(survivors, reclaimed, dynamicCap);
        }
        else
        {
            COUNTERPOINTAI._spreadBudget(survivors, reclaimed);
        }
    },

    _borrowForCandidate : function(plans, planIndex, transactionCost)
    {
        var plan = plans[planIndex];
        var needed = Math.max(0, transactionCost - plan.reservedBudget);
        if (needed === 0)
        {
            return true;
        }
        var borrowed = [];
        for (var index = 0; index < plans.length && needed > 0; ++index)
        {
            if (index === planIndex)
            {
                continue;
            }
            var donor = plans[index];
            if (donor.phase !== plan.phase || donor.complete || donor.skipped || !donor.hasPaid)
            {
                continue;
            }
            // Lend only what the donor holds above its own capturer floor, so borrowing can never
            // be the reason a ground factory sits idle.
            var available = Math.max(0, donor.reservedBudget - COUNTERPOINTAI._planFloor(donor));
            var amount = Math.min(needed, available);
            if (amount > 0)
            {
                donor.reservedBudget -= amount;
                plan.reservedBudget += amount;
                needed -= amount;
                borrowed.push({ key : donor.key, amount : amount });
            }
        }
        if (needed > 0)
        {
            COUNTERPOINTAI._returnBorrowedBudget(plans, plan, borrowed);
            return false;
        }
        plan.borrowed = plan.borrowed.concat(borrowed);
        return true;
    },

    // Hands each borrowed amount back to the plan it came from. The clamp only ever bites on a
    // restore, since a rollback is undoing increments made moments earlier.
    _returnBorrowedBudget : function(plans, plan, borrowed)
    {
        for (var index = 0; index < borrowed.length; ++index)
        {
            var entry = borrowed[index];
            var donor = COUNTERPOINTAI._planByKey(plans, entry.key);
            if (donor !== null)
            {
                donor.reservedBudget += entry.amount;
            }
            plan.reservedBudget = Math.max(0, plan.reservedBudget - entry.amount);
        }
    },

    _restoreBorrowedBudget : function(plans, plan)
    {
        COUNTERPOINTAI._returnBorrowedBudget(plans, plan, plan.borrowed);
        plan.borrowed = [];
    },

    _releasePlanBudget : function(plans, planIndex, spent)
    {
        var plan = plans[planIndex];
        var used = COUNTERPOINTAI._wholeCount(spent);
        var remainder = Math.max(0, plan.reservedBudget - used);
        plan.reservedBudget = used;
        plan.borrowed = [];
        if (COUNTERPOINTAI.RECYCLE_UNUSED_BUDGET !== false)
        {
            COUNTERPOINTAI._spreadBudget(
                COUNTERPOINTAI._remainingBudgetPlans(plans, planIndex),
                remainder
            );
        }
    },

    _completePlan : function(plans, planIndex, spent)
    {
        var plan = plans[planIndex];
        plan.complete = true;
        if (plan.strategicHold)
        {
            COUNTERPOINTAI._restoreBorrowedBudget(plans, plan);
            plan.reservedBudget = Math.max(
                0,
                plan.reservedBudget - COUNTERPOINTAI._wholeCount(spent)
            );
            plan.borrowed = [];
            return;
        }
        COUNTERPOINTAI._releasePlanBudget(plans, planIndex, spent);
    },

    _abandonPlan : function(plans, planIndex)
    {
        var plan = plans[planIndex];
        COUNTERPOINTAI._restoreBorrowedBudget(plans, plan);
        plan.strategicHold = false;
        plan.strategicHoldEligible = false;
        plan.forcedBuild = false;
        plan.randomSurplusFunded = false;
        plan.available = false;
        plan.selected = -1;
        COUNTERPOINTAI._completePlan(plans, planIndex, 0);
    },

    _rejectPlanCandidate : function(plans, planIndex, candidateIndex)
    {
        var plan = plans[planIndex];
        COUNTERPOINTAI._restoreBorrowedBudget(plans, plan);
        COUNTERPOINTAI._rejectCandidate(plan, candidateIndex);
    },

    _scoreContext : function(context)
    {
        return {
            ownCounts : context.ownCounts,
            ownCoverage : context.ownCoverage,
            ownAirCoverage : context.ownAirCoverage,
            threatProfile : context.threatProfile,
            phantomThreats : context.phantomThreats,
            phantomCoverage : context.phantomCoverage,
            phantomAirPresent : context.phantomAirPresent,
            indirectRangeDeltas : context.indirectRangeDeltas,
            banIndirects : context.banIndirects,
            tankFerryStats : context.tankFerryStats,
            groundCarriers : context.groundCarriers,
            islandMode : context.islandMode
        };
    },

    _setCandidateOrderScore : function(candidate, context)
    {
        if (context.islandMode === true && candidate.isTransporter === true &&
            COUNTERPOINTAI._readFlag(context.groundCarriers, candidate.id))
        {
            return;
        }
        var rawScore = COUNTERPOINTAI._scoreUnitAgainstEnemies(
            candidate.scoreData,
            context.enemyComposition,
            COUNTERPOINTAI._scoreContext(context)
        );
        var roundedScore = Math.round(rawScore);
        if (rawScore < 0 && roundedScore === 0)
        {
            roundedScore = -1;
        }
        candidate.orderScore = COUNTERPOINTAI._clamp(
            roundedScore,
            -COUNTERPOINTAI.PLANNER_VALUE_MAX,
            COUNTERPOINTAI.PLANNER_VALUE_MAX
        );
    },

    _scorePlanCandidates : function(plans, context, turn)
    {
        var dynamic = COUNTERPOINTAI.DYNAMIC_FLOOR !== false;
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            var plan = plans[planIndex];
            for (var index = 0; plan.candidates && index < plan.candidates.length; ++index)
            {
                if (COUNTERPOINTAI._candidateRejected(plan, index))
                {
                    continue;
                }
                var candidate = plan.candidates[index];
                var data = candidate.scoreData;
                candidate.isAA = data !== undefined && data !== null && COUNTERPOINTAI._countsAsAntiAir(data);
                candidate.isIndirect = data !== undefined && data !== null && COUNTERPOINTAI._isPureIndirect(data);
                if (dynamic && !(turn <= 1 && candidate.canCapture === true))
                {
                    COUNTERPOINTAI._setCandidateOrderScore(candidate, context);
                }
            }
        }
    },

    _enemyAirThreatCount : function(enemyComposition)
    {
        var length = COUNTERPOINTAI._collectionLength(enemyComposition);
        var total = 0;
        for (var index = 0; index < length; ++index)
        {
            var enemy = COUNTERPOINTAI._collectionAt(enemyComposition, index);
            if (COUNTERPOINTAI._isAirThreat(enemy))
            {
                total += Math.max(0, COUNTERPOINTAI._readNumber(enemy, "count", 0));
            }
        }
        return total;
    },

    _ownAACount : function(ownComposition)
    {
        var length = COUNTERPOINTAI._collectionLength(ownComposition);
        var total = 0;
        for (var index = 0; index < length; ++index)
        {
            var own = COUNTERPOINTAI._collectionAt(ownComposition, index);
            if (COUNTERPOINTAI._countsAsAntiAir(own))
            {
                total += Math.max(0, COUNTERPOINTAI._readNumber(own, "count", 0));
            }
        }
        return total;
    },

    _computeTurnTargets : function(context)
    {
        var airUnits = COUNTERPOINTAI._enemyAirThreatCount(context.enemyComposition);
        var ratio = Math.max(
            COUNTERPOINTAI.AA_TARGET_RATIO_FLOOR,
            COUNTERPOINTAI._tunable("AA_ENEMY_AIR_PER_UNIT")
        );
        // A fleet ratio net of anti-air already owned, not a fresh allowance every turn. Negative
        // means uncapped, matching indirectRemaining; no enemy air is the scoring discount's job.
        var aaPerTurn = -1;
        if (airUnits > 0)
        {
            aaPerTurn = Math.max(
                0,
                Math.max(1, Math.round(airUnits / ratio)) -
                    COUNTERPOINTAI._ownAACount(context.ownComposition)
            );
        }
        var cap = COUNTERPOINTAI._tunable("MAX_INDIRECT_UNITS");
        var indirectExempt = COUNTERPOINTAI.MAX_INDIRECT_IGNORE_INDIRECT_CO !== false &&
            context.indirectCo === true;
        var indirectRemaining = (!(cap >= 0) || indirectExempt) ? -1 :
            Math.max(0, cap - COUNTERPOINTAI._readNumber(context, "fieldedIndirects", 0));
        return { aaPerTurn : aaPerTurn, indirectRemaining : indirectRemaining };
    },

    _countCompletedBuilds : function(plans, flag)
    {
        var total = 0;
        for (var index = 0; index < plans.length; ++index)
        {
            var plan = plans[index];
            if (plan.complete === true && plan.selected >= 0 &&
                plan.candidates && plan.candidates[plan.selected] &&
                plan.candidates[plan.selected][flag] === true)
            {
                total += 1;
            }
        }
        return total;
    },

    _turnSaturatedSkipped : function(candidate, plans, turnTargets)
    {
        if (turnTargets === undefined || turnTargets === null)
        {
            return false;
        }
        if (candidate.isAA === true && turnTargets.aaPerTurn >= 0 &&
            COUNTERPOINTAI._countCompletedBuilds(plans, "isAA") >= turnTargets.aaPerTurn)
        {
            return true;
        }
        if (candidate.isIndirect === true && turnTargets.indirectRemaining >= 0 &&
            COUNTERPOINTAI._countCompletedBuilds(plans, "isIndirect") >= turnTargets.indirectRemaining)
        {
            return true;
        }
        return false;
    },

    _weightedCandidateOrder : function(candidates, mode, state, context, turn)
    {
        var remaining = candidates.slice();
        var order = [];
        while (remaining.length > 0)
        {
            var scoreCandidates = [];
            for (var scoreIndex = 0; scoreIndex < remaining.length; ++scoreIndex)
            {
                scoreCandidates.push(remaining[scoreIndex].scoreData);
            }
            var normalized;
            if (mode === COUNTERPOINTAI.ORDER_MODE_INVERSE)
            {
                normalized = COUNTERPOINTAI._inverseCostWeights(scoreCandidates, turn);
            }
            else if (mode === COUNTERPOINTAI.ORDER_MODE_DAMAGE)
            {
                normalized = COUNTERPOINTAI._netDamageWeights(
                    scoreCandidates,
                    context.enemyComposition,
                    COUNTERPOINTAI._scoreContext(context)
                );
            }
            else
            {
                normalized = COUNTERPOINTAI._costWeights(scoreCandidates);
            }
            var pickIndex = COUNTERPOINTAI._pickWeightedIndex(
                normalized,
                COUNTERPOINTAI._nextPlannerRoll(state, normalized.total)
            );
            if (pickIndex < 0 || pickIndex >= remaining.length)
            {
                pickIndex = 0;
            }
            order.push(remaining[pickIndex]);
            remaining.splice(pickIndex, 1);
        }
        return order;
    },

    _appendCandidateIndexes : function(target, seen, candidates)
    {
        for (var index = 0; index < candidates.length; ++index)
        {
            var candidateIndex = candidates[index].planIndex;
            var key = "#" + candidateIndex;
            if (seen[key] !== true)
            {
                seen[key] = true;
                target.push(candidateIndex);
            }
        }
    },

    _candidateValueSort : function(left, right)
    {
        if (left.strategicValue !== right.strategicValue)
        {
            return left.strategicValue - right.strategicValue;
        }
        if (left.id !== right.id)
        {
            return left.id < right.id ? -1 : 1;
        }
        return left.ordinal - right.ordinal;
    },

    _openingCandidatePool : function(candidates, context, turn)
    {
        if (turn > 1 || COUNTERPOINTAI.TURN1_FORCE_CAPPERS === false ||
            !COUNTERPOINTAI._capperBuildsAllowed())
        {
            return candidates;
        }
        var opening = [];
        for (var index = 0; index < candidates.length; ++index)
        {
            var candidate = candidates[index];
            if (candidate.canCapture ||
                (context.islandMode && candidate.isTransporter &&
                 COUNTERPOINTAI._readFlag(context.groundCarriers, candidate.id)))
            {
                opening.push(candidate);
            }
        }
        return opening.length > 0 ? opening : candidates;
    },

    _planCandidateOrder : function(plan, state, context, turn)
    {
        var domains = [
            COUNTERPOINTAI.DOMAIN_GROUND,
            COUNTERPOINTAI.DOMAIN_AIR,
            COUNTERPOINTAI.DOMAIN_NAVAL,
            COUNTERPOINTAI.DOMAIN_HOVER
        ];
        var pools = Object.create(null);
        var fallbackPool = [];
        for (var domainIndex = 0; domainIndex < domains.length; ++domainIndex)
        {
            pools[domains[domainIndex]] = { combat : [], transport : [] };
        }
        for (var candidateIndex = 0;
             candidateIndex < plan.candidates.length;
             ++candidateIndex)
        {
            if (COUNTERPOINTAI._candidateRejected(plan, candidateIndex))
            {
                continue;
            }
            var candidate = plan.candidates[candidateIndex];
            var scoreData = candidate.scoreData;
            if (scoreData === undefined ||
                ((context.banIndirects || COUNTERPOINTAI._indirectCapBlocks(context)) &&
                 COUNTERPOINTAI._isPureIndirect(scoreData)))
            {
                COUNTERPOINTAI._rejectCandidate(plan, candidateIndex);
                continue;
            }
            var pool = pools[candidate.domain];
            if (pool === undefined)
            {
                fallbackPool.push(candidate);
                continue;
            }
            // Only ferries belong in the transport pool. Pooling every transporter there put a
            // cruiser behind the transport gate, which both hid it as a warship and let it be
            // bought as though it were a ferry.
            if (candidate.isTransporter &&
                COUNTERPOINTAI._readFlag(context.groundCarriers, candidate.id))
            {
                pool.transport.push(candidate);
            }
            else
            {
                pool.combat.push(candidate);
            }
        }

        var available = COUNTERPOINTAI._openingCandidatePool(
            fallbackPool,
            context,
            turn
        );
        for (var poolIndex = 0; poolIndex < domains.length; ++poolIndex)
        {
            var domain = domains[poolIndex];
            var domainPool = pools[domain].combat.slice();
            if (pools[domain].transport.length > 0 &&
                COUNTERPOINTAI._admitTransports(context, state, domain, turn))
            {
                domainPool = domainPool.concat(pools[domain].transport);
            }
            domainPool = COUNTERPOINTAI._openingCandidatePool(
                domainPool,
                context,
                turn
            );
            available = available.concat(domainPool);
        }

        var cappers = [];
        var nonCappers = [];
        for (var availableIndex = 0; availableIndex < available.length; ++availableIndex)
        {
            var entry = available[availableIndex];
            if (turn > 1 || !entry.canCapture)
            {
                COUNTERPOINTAI._setCandidateOrderScore(entry, context);
            }
            if (entry.canCapture)
            {
                cappers.push(entry);
            }
            else if (entry.isTransporter || COUNTERPOINTAI._unitHasOffenseVs(
                    entry.scoreData,
                    context.enemyComposition,
                    context.banIndirects
                ))
            {
                nonCappers.push(entry);
            }
        }

        var affordableCappers = [];
        for (var capperIndex = 0; capperIndex < cappers.length; ++capperIndex)
        {
            if (cappers[capperIndex].transactionCost <= plan.reservedBudget)
            {
                affordableCappers.push(cappers[capperIndex]);
            }
        }
        var borrowEnabled = COUNTERPOINTAI.RECYCLE_UNUSED_BUDGET !== false &&
            COUNTERPOINTAI.CAPPER_BORROW_FROM_RESERVE !== false;
        var capperRollPool = borrowEnabled ? cappers : affordableCappers;
        var capperRollHit = capperRollPool.length > 0 &&
            plan.reservedBudget <= COUNTERPOINTAI.CAP_ROLL_MAX_BUDGET &&
            COUNTERPOINTAI._nextPlannerRoll(state, COUNTERPOINTAI._tunable("PERCENT_MAX")) <
                COUNTERPOINTAI._captureBaseChance();
        plan.allowCapperBorrow = capperRollHit && borrowEnabled;

        var firstPool = capperRollHit ? capperRollPool : nonCappers;
        var secondPool = capperRollHit ? nonCappers : cappers;
        var firstMode = capperRollHit ?
            COUNTERPOINTAI.ORDER_MODE_INVERSE : COUNTERPOINTAI.ORDER_MODE_DAMAGE;
        var secondMode = capperRollHit ?
            COUNTERPOINTAI.ORDER_MODE_DAMAGE : COUNTERPOINTAI.ORDER_MODE_INVERSE;
        var ordered = [];
        var seen = Object.create(null);
        COUNTERPOINTAI._appendCandidateIndexes(
            ordered,
            seen,
            COUNTERPOINTAI._weightedCandidateOrder(
                firstPool,
                firstMode,
                state,
                context,
                turn
            )
        );
        if (!(secondPool === cappers && !COUNTERPOINTAI._capperBuildsAllowed()))
        {
            COUNTERPOINTAI._appendCandidateIndexes(
                ordered,
                seen,
                COUNTERPOINTAI._weightedCandidateOrder(
                    secondPool,
                    secondMode,
                    state,
                    context,
                    turn
                )
            );
        }

        var cheapCappers = cappers.slice();
        cheapCappers.sort(COUNTERPOINTAI._candidateValueSort);
        if (COUNTERPOINTAI._capperBuildsAllowed())
        {
            COUNTERPOINTAI._appendCandidateIndexes(ordered, seen, cheapCappers);
        }
        var cheapNonCappers = nonCappers.slice();
        cheapNonCappers.sort(COUNTERPOINTAI._candidateValueSort);
        COUNTERPOINTAI._appendCandidateIndexes(ordered, seen, cheapNonCappers);
        if (COUNTERPOINTAI._capperBuildsAllowed())
        {
            COUNTERPOINTAI._appendCandidateIndexes(ordered, seen, cheapCappers);
        }
        plan.order = ordered;
        COUNTERPOINTAI._ferryFirstOrder(plan, context);
    },

    // Urgency decides funding, not what the funding buys, so a harbour handed the money for a hull
    // it needs could still draw a warship and spend it. Where a wanted ferry can actually be built,
    // it goes to the front of the order, which makes it the pick as soon as the budget covers it and
    // otherwise falls straight through to the rest of the list.
    _ferryFirstOrder : function(plan, context)
    {
        var ferry = context.ferry;
        if (context.islandMode !== true || ferry === null || ferry === undefined ||
            ferry.urgent !== true || ferry.plans[plan.key] !== true)
        {
            return;
        }
        var front = [];
        var rest = [];
        for (var index = 0; index < plan.order.length; ++index)
        {
            var candidate = plan.candidates[plan.order[index]];
            if (candidate.isTransporter === true &&
                ferry.needed[candidate.domain] === true &&
                COUNTERPOINTAI._readFlag(context.groundCarriers, candidate.id))
            {
                front.push(plan.order[index]);
            }
            else
            {
                rest.push(plan.order[index]);
            }
        }
        plan.order = front.concat(rest);
    },

    _canUseUnreservedFunds : function(plan, transactionCost, funds)
    {
        var safeFunds = COUNTERPOINTAI._wholeCount(funds);
        return COUNTERPOINTAI.AVOID_BUDGET_BASE_SKIPS === true &&
            (COUNTERPOINTAI.RANDOM_SURPLUS_FUNDING !== true ||
             plan.randomSurplusFunded === true) &&
            plan.reservedBudget < safeFunds && transactionCost <= safeFunds;
    },

    // Combat score does not gate cappers or ferries bought for economic value.
    _scoredCombatCandidate : function(candidate)
    {
        return candidate.canCapture !== true &&
               candidate.isTransporter !== true &&
               typeof candidate.orderScore === "number";
    },

    _negativeScorerSkipped : function(candidate, affordableAlternative)
    {
        if (COUNTERPOINTAI.SKIP_NEGATIVE_SCORERS === false)
        {
            return false;
        }
        return affordableAlternative === true &&
               COUNTERPOINTAI._scoredCombatCandidate(candidate) &&
               candidate.orderScore < 0;
    },

    // Only scored fallback transports are eligible for rejection.
    _unwantedTransporterSkipped : function(candidate, affordableAlternative)
    {
        if (COUNTERPOINTAI.SKIP_UNWANTED_TRANSPORTERS === false)
        {
            return false;
        }
        return affordableAlternative === true &&
               candidate.isTransporter === true &&
               typeof candidate.orderScore === "number" &&
               candidate.orderScore < 0;
    },

    // Never defer an economic capper for a higher combat score.
    _holdForBetterCandidate : function(candidate, bestMissedScore)
    {
        var ratio = COUNTERPOINTAI._tunable("HOLD_FOR_BETTER_RATIO");
        if (!(ratio > 0) || bestMissedScore <= 0 ||
            !COUNTERPOINTAI._scoredCombatCandidate(candidate))
        {
            return false;
        }
        return candidate.orderScore < bestMissedScore * ratio;
    },

    _bestPlanOpportunityScore : function(plan, budget)
    {
        var best = 0;
        for (var orderIndex = 0; orderIndex < plan.order.length; ++orderIndex)
        {
            var candidateIndex = plan.order[orderIndex];
            if (COUNTERPOINTAI._candidateRejected(plan, candidateIndex))
            {
                continue;
            }
            var candidate = plan.candidates[candidateIndex];
            if (candidate.enabled === true && candidate.transactionCost >= 0 &&
                candidate.transactionCost <= budget &&
                typeof candidate.orderScore === "number")
            {
                best = Math.max(best, candidate.orderScore);
            }
        }
        return Math.max(0, best);
    },

    _productionLineupScore : function(plans, planIndex, candidateIndex)
    {
        var plan = plans[planIndex];
        var candidate = plan.candidates[candidateIndex];
        var score = Math.max(0, candidate.orderScore);
        var targets = COUNTERPOINTAI._remainingBudgetPlans(plans, planIndex);
        if (targets.length === 0)
        {
            return score;
        }
        var remainder = Math.max(0, plan.reservedBudget - candidate.transactionCost);
        var perPlan = Math.floor(remainder / targets.length);
        var extra = remainder - perPlan * targets.length;
        for (var targetIndex = 0; targetIndex < targets.length; ++targetIndex)
        {
            var addition = perPlan + (targetIndex < extra ? 1 : 0);
            score += COUNTERPOINTAI._bestPlanOpportunityScore(
                targets[targetIndex],
                targets[targetIndex].reservedBudget + addition
            );
        }
        return score;
    },

    _opportunityAwareCandidate : function(plans, planIndex, candidateIndex, turnTargets)
    {
        var plan = plans[planIndex];
        var candidate = plan.candidates[candidateIndex];
        if (plan.phase !== COUNTERPOINTAI.PHASE_ORDINARY ||
            plan.strategicHoldEligible === true || plan.strategicHold === true ||
            plan.allowCapperBorrow === true ||
            COUNTERPOINTAI.RECYCLE_UNUSED_BUDGET === false ||
            typeof candidate.orderScore !== "number" || candidate.orderScore <= 0)
        {
            return candidateIndex;
        }
        var bestIndex = candidateIndex;
        var bestLineupScore = COUNTERPOINTAI._productionLineupScore(
            plans,
            planIndex,
            candidateIndex
        );
        for (var orderIndex = 0; orderIndex < plan.order.length; ++orderIndex)
        {
            var alternativeIndex = plan.order[orderIndex];
            if (alternativeIndex === candidateIndex ||
                COUNTERPOINTAI._candidateRejected(plan, alternativeIndex))
            {
                continue;
            }
            var alternative = plan.candidates[alternativeIndex];
            if (alternative.enabled !== true || alternative.transactionCost < 0 ||
                alternative.transactionCost >= candidate.transactionCost ||
                alternative.transactionCost > plan.reservedBudget ||
                typeof alternative.orderScore !== "number" || alternative.orderScore <= 0 ||
                COUNTERPOINTAI._turnSaturatedSkipped(alternative, plans, turnTargets))
            {
                continue;
            }
            var lineupScore = COUNTERPOINTAI._productionLineupScore(
                plans,
                planIndex,
                alternativeIndex
            );
            if (lineupScore > bestLineupScore)
            {
                bestIndex = alternativeIndex;
                bestLineupScore = lineupScore;
            }
        }
        return bestIndex;
    },

    _nearestEnemyDistance : function(plan, enemyUnits)
    {
        var nearest = COUNTERPOINTAI.PLANNER_VALUE_MAX;
        var length = COUNTERPOINTAI._collectionLength(enemyUnits);
        for (var index = 0; index < length; ++index)
        {
            var enemy = COUNTERPOINTAI._collectionAt(enemyUnits, index);
            if (enemy === null || enemy === undefined)
            {
                continue;
            }
            nearest = Math.min(
                nearest,
                Math.abs(plan.x - enemy.getX()) + Math.abs(plan.y - enemy.getY())
            );
        }
        return nearest;
    },

    _designateStrategicHolders : function(plans, enemyUnits, blockedCount)
    {
        var indexes = [];
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            plans[planIndex].strategicHoldEligible = false;
            // Measured once per turn here because the surplus order reads it too.
            plans[planIndex].enemyDistance = COUNTERPOINTAI._nearestEnemyDistance(
                plans[planIndex],
                enemyUnits
            );
            if (plans[planIndex].hasPaid && !plans[planIndex].complete)
            {
                indexes.push(planIndex);
            }
        }
        indexes.sort(function(leftIndex, rightIndex)
        {
            var left = plans[leftIndex];
            var right = plans[rightIndex];
            if (left.enemyDistance !== right.enemyDistance)
            {
                return right.enemyDistance - left.enemyDistance;
            }
            if (left.x !== right.x)
            {
                return right.x - left.x;
            }
            return right.y - left.y;
        });
        var limit = Math.min(
            indexes.length,
            COUNTERPOINTAI._wholeCount(
                COUNTERPOINTAI._tunable("MAX_STRATEGIC_HOLDS_PER_TURN"))
        );
        if (blockedCount > 0)
        {
            limit = 0;
        }
        for (var holderIndex = 0; holderIndex < limit; ++holderIndex)
        {
            plans[indexes[holderIndex]].strategicHoldEligible = true;
        }
    },

    _canStartStrategicHold : function(plans, planIndex)
    {
        if (plans[planIndex].strategicHold === true)
        {
            return true;
        }
        if (plans[planIndex].strategicHoldEligible !== true)
        {
            return false;
        }
        var limit = COUNTERPOINTAI._wholeCount(
            COUNTERPOINTAI._tunable("MAX_STRATEGIC_HOLDS_PER_TURN"));
        var held = 0;
        for (var heldIndex = 0; heldIndex < plans.length; ++heldIndex)
        {
            if (plans[heldIndex].strategicHold === true)
            {
                ++held;
            }
        }
        return held < limit;
    },

    _selectPlanCandidate : function(plans, planIndex, funds, turnTargets)
    {
        var plan = plans[planIndex];
        plan.selected = -1;
        // Reset forcedBuild so its reserve bypass cannot leak across retries.
        plan.forcedBuild = false;
        var retryable = false;
        var fallbackCandidateIndexes = [];
        var bestMissedScore = 0;
        var heldAffordable = false;
        var heldCandidateIndex = -1;
        var canStartStrategicHold = COUNTERPOINTAI._canStartStrategicHold(plans, planIndex);
        var safeFunds = COUNTERPOINTAI._wholeCount(funds);
        var affordableAlternative = false;
        for (var missedIndex = 0; missedIndex < plan.order.length; ++missedIndex)
        {
            var missedCandidateIndex = plan.order[missedIndex];
            if (COUNTERPOINTAI._candidateRejected(plan, missedCandidateIndex))
            {
                continue;
            }
            var missedCandidate = plan.candidates[missedCandidateIndex];
            if (missedCandidate.enabled === true &&
                missedCandidate.transactionCost >= 0 &&
                (missedCandidate.transactionCost <= plan.reservedBudget ||
                 COUNTERPOINTAI._canUseUnreservedFunds(
                     plan,
                     missedCandidate.transactionCost,
                     safeFunds
                 )) &&
                !COUNTERPOINTAI._turnSaturatedSkipped(
                    missedCandidate,
                    plans,
                    turnTargets
                ) &&
                ((COUNTERPOINTAI._scoredCombatCandidate(missedCandidate) &&
                  missedCandidate.orderScore >= 0) ||
                 missedCandidate.canCapture === true))
            {
                affordableAlternative = true;
            }
            if (missedCandidate.transactionCost < 0 ||
                typeof missedCandidate.orderScore !== "number" ||
                missedCandidate.orderScore <= 0)
            {
                continue;
            }
            var missedByBudget = missedCandidate.enabled === true &&
                missedCandidate.transactionCost > plan.reservedBudget;
            if (missedByBudget && missedCandidate.orderScore > bestMissedScore)
            {
                bestMissedScore = missedCandidate.orderScore;
            }
        }
        for (var orderIndex = 0; orderIndex < plan.order.length; ++orderIndex)
        {
            var candidateIndex = plan.order[orderIndex];
            if (COUNTERPOINTAI._candidateRejected(plan, candidateIndex))
            {
                continue;
            }
            var candidate = plan.candidates[candidateIndex];
            if (!candidate.enabled || candidate.transactionCost < 0)
            {
                retryable = true;
                continue;
            }
            if (COUNTERPOINTAI._negativeScorerSkipped(candidate, affordableAlternative) ||
                COUNTERPOINTAI._unwantedTransporterSkipped(candidate, affordableAlternative) ||
                COUNTERPOINTAI._turnSaturatedSkipped(candidate, plans, turnTargets))
            {
                continue;
            }
            if (candidate.transactionCost === 0)
            {
                COUNTERPOINTAI._restoreBorrowedBudget(plans, plan);
                if (canStartStrategicHold &&
                    COUNTERPOINTAI._holdForBetterCandidate(candidate, bestMissedScore))
                {
                    plan.strategicHold = true;
                }
                plan.selected = candidateIndex;
                return true;
            }
            if (candidate.transactionCost <= plan.reservedBudget)
            {
                // Keep walking after a hold so a stronger affordable candidate can still win.
                if ((canStartStrategicHold &&
                     COUNTERPOINTAI._holdForBetterCandidate(candidate, bestMissedScore)) ||
                    (heldAffordable && typeof candidate.orderScore !== "number"))
                {
                    heldAffordable = true;
                    if (heldCandidateIndex < 0 || candidate.transactionCost <
                        plan.candidates[heldCandidateIndex].transactionCost)
                    {
                        heldCandidateIndex = candidateIndex;
                    }
                    continue;
                }
                plan.selected = COUNTERPOINTAI._opportunityAwareCandidate(
                    plans,
                    planIndex,
                    candidateIndex,
                    turnTargets
                );
                return true;
            }
            if (candidate.canCapture && plan.allowCapperBorrow &&
                COUNTERPOINTAI._borrowForCandidate(
                    plans,
                    planIndex,
                    candidate.transactionCost
                ))
            {
                plan.selected = candidateIndex;
                return true;
            }
            if (COUNTERPOINTAI._canUseUnreservedFunds(
                    plan,
                    candidate.transactionCost,
                    funds
                ))
            {
                fallbackCandidateIndexes.push(candidateIndex);
            }
            retryable = true;
        }
        // A fallback may buy the high scorer that established the hold threshold.
        for (var fallbackIndex = 0;
             fallbackIndex < fallbackCandidateIndexes.length;
             ++fallbackIndex)
        {
            var fallbackCandidate = plan.candidates[fallbackCandidateIndexes[fallbackIndex]];
            if ((canStartStrategicHold &&
                 COUNTERPOINTAI._holdForBetterCandidate(fallbackCandidate, bestMissedScore)) ||
                (heldAffordable && typeof fallbackCandidate.orderScore !== "number"))
            {
                heldAffordable = true;
                if (heldCandidateIndex < 0 || fallbackCandidate.transactionCost <
                    plan.candidates[heldCandidateIndex].transactionCost)
                {
                    heldCandidateIndex = fallbackCandidateIndexes[fallbackIndex];
                }
                continue;
            }
            plan.selected = fallbackCandidateIndexes[fallbackIndex];
            return true;
        }
        // Build cheaply while keeping the holder's surplus committed for the stronger unit.
        if (heldAffordable)
        {
            plan.strategicHold = true;
            plan.selected = heldCandidateIndex;
            return true;
        }
        // Rescue only policy-deferred ground builds; unavailable rosters still skip.
        if (plan.skipped !== true && plan.phase === COUNTERPOINTAI.PHASE_ORDINARY &&
            COUNTERPOINTAI._isGroundFactoryPlan(plan))
        {
            var rescueIndex = COUNTERPOINTAI._cheapestBuildableCandidate(
                plan,
                plans,
                turnTargets
            );
            if (rescueIndex >= 0)
            {
                plan.forcedBuild = true;
                plan.selected = rescueIndex;
                return true;
            }
        }
        if (retryable)
        {
            return false;
        }
        COUNTERPOINTAI._abandonPlan(plans, planIndex);
        return false;
    },

    _stripTransientPlanData : function(plans)
    {
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            // Planner state is serialized under a length cap, so per-turn scratch is dropped here.
            delete plans[planIndex].enemyDistance;
            for (var candidateIndex = 0;
                 candidateIndex < plans[planIndex].candidates.length;
                 ++candidateIndex)
            {
                delete plans[planIndex].candidates[candidateIndex].scoreData;
                delete plans[planIndex].candidates[candidateIndex].planIndex;
            }
        }
    },

    _preparePlanPhase : function(system, ai, buildings, units, enemyUnits,
                                 enemyBuildings, map, state, phase)
    {
        COUNTERPOINTAI._preparePlans(
            system,
            ai,
            COUNTERPOINTAI._discoverPlans(
                system,
                ai,
                buildings,
                phase,
                COUNTERPOINTAI._planKeys(state.plans)
            ),
            buildings,
            units,
            enemyUnits,
            enemyBuildings,
            map,
            state,
            phase
        );
    },

    _planKeys : function(plans)
    {
        var keys = [];
        for (var index = 0; index < plans.length; ++index)
        {
            keys.push(plans[index].key);
        }
        return keys;
    },

    _hasPendingOrdinaryPlan : function(state)
    {
        for (var index = 0; index < state.plans.length; ++index)
        {
            if (state.plans[index].phase === COUNTERPOINTAI.PHASE_ORDINARY &&
                !state.plans[index].complete)
            {
                return true;
            }
        }
        return false;
    },

    // A factory with a unit parked on it reports no build action, so the once per turn scan drops
    // it for the rest of the turn. Look again once the planned factories are done, by which point
    // the occupying units have usually moved off.
    _rescanFreedFactories : function(system, ai, buildings, units, enemyUnits,
                                     enemyBuildings, map, state)
    {
        if (COUNTERPOINTAI.LATE_FACTORY_RESCAN === false)
        {
            return false;
        }
        var plans = COUNTERPOINTAI._discoverPlans(
            system,
            ai,
            buildings,
            COUNTERPOINTAI.PHASE_ORDINARY,
            COUNTERPOINTAI._planKeys(state.plans)
        );
        if (plans.length === 0)
        {
            return false;
        }
        COUNTERPOINTAI._preparePlans(
            system,
            ai,
            plans,
            buildings,
            units,
            COUNTERPOINTAI._fullEnemyUnits(ai, enemyUnits),
            enemyBuildings,
            map,
            state,
            COUNTERPOINTAI.PHASE_ORDINARY
        );
        COUNTERPOINTAI._savePlannerState(system, state);
        return true;
    },

    _preparePlans : function(system, ai, plans, buildings, units, enemyUnits,
                             enemyBuildings, map, state, phase)
    {
        var room = Math.max(
            0,
            COUNTERPOINTAI._plannerLimit("MAX_PLAN_COUNT", 1) - state.plans.length
        );
        if (plans.length > room)
        {
            plans = plans.slice(0, room);
        }
        var candidateRoom = COUNTERPOINTAI._plannerLimit(
            "MAX_TOTAL_PLAN_CANDIDATES",
            1
        );
        for (var existingIndex = 0;
             existingIndex < state.plans.length;
             ++existingIndex)
        {
            candidateRoom -= state.plans[existingIndex].candidates.length;
        }
        var boundedPlans = [];
        for (var boundedIndex = 0;
             boundedIndex < plans.length && candidateRoom > 0;
             ++boundedIndex)
        {
            if (plans[boundedIndex].candidates.length > candidateRoom)
            {
                continue;
            }
            boundedPlans.push(plans[boundedIndex]);
            candidateRoom -= plans[boundedIndex].candidates.length;
        }
        plans = boundedPlans;
        var context = COUNTERPOINTAI._planningContext(
            system,
            ai,
            plans,
            buildings,
            units,
            enemyUnits,
            enemyBuildings,
            map
        );
        var blocked = COUNTERPOINTAI._blockedProductionStatus(
            system,
            ai,
            buildings,
            state,
            plans
        );
        var ordinaryPhase = phase === COUNTERPOINTAI.PHASE_ORDINARY;
        var available = Math.max(0, ai.getPlayer().getFunds() -
            blocked.capperReserve -
            COUNTERPOINTAI._strategicHeldFunds(state.plans));
        // Preserve the first hold decision after spending changes the treasury.
        if (ordinaryPhase && state.ordinaryPrepared !== true)
        {
            // Measured against raw funds, because the roster's floor total already covers every
            // facility's capturer bill, including the blocked ones reserved for separately below.
            state.heldFunds = COUNTERPOINTAI._savingDecision(
                system,
                ai,
                context,
                ai.getPlayer().getFunds(),
                state.day,
                blocked.count
            );
        }
        COUNTERPOINTAI._designateStrategicHolders(
            plans,
            enemyUnits,
            ordinaryPhase ? blocked.count : 0
        );
        COUNTERPOINTAI._scorePlanCandidates(plans, context, map.getCurrentDay());
        if (state.specialPrepared !== true && state.ordinaryPrepared !== true)
        {
            state.turnTargets = COUNTERPOINTAI._computeTurnTargets(context);
        }
        var spendable = Math.max(0, available -
            Math.max(0, COUNTERPOINTAI._finiteNumber(state.heldFunds, 0)));
        COUNTERPOINTAI._allocatePhaseBudgets(
            state,
            plans,
            spendable,
            context.ferry,
            COUNTERPOINTAI._planValueReach(ai, context.roster, spendable)
        );
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
            if (plans[planIndex].complete)
            {
                continue;
            }
            COUNTERPOINTAI._planCandidateOrder(
                plans[planIndex],
                state,
                context,
                map.getCurrentDay()
            );
        }
        COUNTERPOINTAI._stripTransientPlanData(plans);
        state.plans = state.plans.concat(plans);
        COUNTERPOINTAI._sortPlans(state.plans);
    },

    initializeSimpleProductionSystem : function(system, ai, map)
    {
        return true;
    },

    prepareProduction : function(system, ai, buildings, units, enemyUnits,
                                 enemyBuildings, map)
    {
        var state = COUNTERPOINTAI._ensurePlannerState(system, ai, map);
        if (!state.specialPrepared)
        {
            COUNTERPOINTAI._preparePlanPhase(
                system,
                ai,
                buildings,
                units,
                enemyUnits,
                enemyBuildings,
                map,
                state,
                COUNTERPOINTAI.PHASE_SPECIAL
            );
            state.specialPrepared = true;
        }
        return COUNTERPOINTAI._savePlannerState(system, state);
    },

    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits,
                               enemyBuildings, map, groupDistribution)
    {
        system.resetForcedProduction();
        var state = COUNTERPOINTAI._ensurePlannerState(system, ai, map);
        if (!state.ordinaryPrepared)
        {
            COUNTERPOINTAI._preparePlanPhase(
                system,
                ai,
                buildings,
                units,
                COUNTERPOINTAI._fullEnemyUnits(ai, enemyUnits),
                enemyBuildings,
                map,
                state,
                COUNTERPOINTAI.PHASE_ORDINARY
            );
            state.ordinaryPrepared = true;
        }
        return COUNTERPOINTAI._savePlannerState(system, state);
    },

    _buildingAt : function(map, x, y)
    {
        if (!map.onMap(x, y))
        {
            return null;
        }
        var terrain = map.getTerrain(x, y);
        return terrain === null || terrain === undefined ? null : terrain.getBuilding();
    },

    _matchingCandidateIndex : function(candidate, ids)
    {
        var ordinal = 0;
        var length = COUNTERPOINTAI._collectionLength(ids);
        for (var index = 0; index < length; ++index)
        {
            if (String(COUNTERPOINTAI._collectionAt(ids, index)) !== candidate.id)
            {
                continue;
            }
            if (ordinal === candidate.ordinal)
            {
                return index;
            }
            ordinal += 1;
        }
        return -1;
    },

    _updatePlanFromActionData : function(plan, data)
    {
        plan.available = data !== null && data !== undefined &&
            data.getActionAvailable() === true;
        if (data === null || data === undefined)
        {
            for (var missingIndex = 0;
                 missingIndex < plan.candidates.length;
                 ++missingIndex)
            {
                plan.candidates[missingIndex].enabled = false;
                plan.candidates[missingIndex].transactionCost = -1;
            }
            COUNTERPOINTAI._refreshPlanCostKinds(plan);
            return;
        }
        var live = COUNTERPOINTAI._copyProductionCandidates(data);
        for (var candidateIndex = 0;
             candidateIndex < plan.candidates.length;
             ++candidateIndex)
        {
            var candidate = plan.candidates[candidateIndex];
            var liveIndex = -1;
            for (var index = 0; index < live.length; ++index)
            {
                if (live[index].id === candidate.id &&
                    live[index].ordinal === candidate.ordinal)
                {
                    liveIndex = index;
                    break;
                }
            }
            if (liveIndex < 0)
            {
                candidate.enabled = false;
                candidate.transactionCost = -1;
            }
            else
            {
                candidate.transactionCost = live[liveIndex].transactionCost;
                candidate.strategicValue = live[liveIndex].strategicValue;
                candidate.enabled = live[liveIndex].enabled;
            }
        }
        COUNTERPOINTAI._refreshPlanCostKinds(plan);
    },

    _refreshSpecialSiblings : function(system, state, building)
    {
        for (var planIndex = 0; planIndex < state.plans.length; ++planIndex)
        {
            var plan = state.plans[planIndex];
            if (plan.phase !== COUNTERPOINTAI.PHASE_SPECIAL || plan.complete ||
                plan.x !== building.getX() || plan.y !== building.getY())
            {
                continue;
            }
            COUNTERPOINTAI._updatePlanFromActionData(
                plan,
                system.getProductionActionData(building, plan.actionId)
            );
            if (!plan.available)
            {
                COUNTERPOINTAI._abandonPlan(state.plans, planIndex);
            }
        }
    },

    _releaseSpecialPlansAt : function(state, x, y)
    {
        for (var planIndex = 0; planIndex < state.plans.length; ++planIndex)
        {
            var plan = state.plans[planIndex];
            if (plan.phase === COUNTERPOINTAI.PHASE_SPECIAL && !plan.complete &&
                plan.x === x && plan.y === y)
            {
                COUNTERPOINTAI._abandonPlan(state.plans, planIndex);
            }
        }
    },

    _validateLiveCandidate : function(candidate, ids, costs, enabled)
    {
        var liveIndex = COUNTERPOINTAI._matchingCandidateIndex(candidate, ids);
        if (liveIndex < 0 || liveIndex >= COUNTERPOINTAI._collectionLength(costs) ||
            liveIndex >= COUNTERPOINTAI._collectionLength(enabled) ||
            COUNTERPOINTAI._collectionAt(enabled, liveIndex) !== true)
        {
            return { index : -1, cost : -1 };
        }
        var cost = Math.floor(COUNTERPOINTAI._finiteNumber(
            COUNTERPOINTAI._collectionAt(costs, liveIndex),
            -1
        ));
        if (cost < 0)
        {
            return { index : -1, cost : cost };
        }
        return { index : liveIndex, cost : cost };
    },

    _planIndex : function(plans, plan)
    {
        return COUNTERPOINTAI._planIndexByKey(plans, plan.key);
    },

    _resolveLivePlanCandidate : function(plans, planIndex, ids, costs, enabled, funds, turnTargets)
    {
        var plan = plans[planIndex];
        while (!plan.complete)
        {
            if (plan.selected < 0 &&
                !COUNTERPOINTAI._selectPlanCandidate(plans, planIndex, funds, turnTargets))
            {
                return null;
            }
            var candidateIndex = plan.selected;
            var candidate = plan.candidates[candidateIndex];
            var live = COUNTERPOINTAI._validateLiveCandidate(
                candidate,
                ids,
                costs,
                enabled
            );
            if (live.index < 0 || live.cost > funds)
            {
                if (live.index < 0)
                {
                    candidate.enabled = false;
                }
                else
                {
                    candidate.transactionCost = live.cost;
                    candidate.enabled = true;
                }
                COUNTERPOINTAI._rejectPlanCandidate(
                    plans,
                    planIndex,
                    candidateIndex
                );
                COUNTERPOINTAI._refreshPlanCostKinds(plan);
                continue;
            }
            candidate.transactionCost = live.cost;
            candidate.enabled = true;
            COUNTERPOINTAI._refreshPlanCostKinds(plan);
            if (live.cost > plan.reservedBudget)
            {
                if (plan.forcedBuild === true ||
                    COUNTERPOINTAI._canUseUnreservedFunds(
                        plan,
                        live.cost,
                        funds
                    ))
                {
                    return {
                        candidate : candidate,
                        candidateIndex : candidateIndex,
                        index : live.index,
                        cost : live.cost
                    };
                }
                COUNTERPOINTAI._restoreBorrowedBudget(plans, plan);
                plan.selected = -1;
                continue;
            }
            return {
                candidate : candidate,
                candidateIndex : candidateIndex,
                index : live.index,
                cost : live.cost
            };
        }
        return null;
    },

    getFactoryMenuItem : function(ai, action, ids, costs, enabled, units,
                                  buildings, owner, map)
    {
        var system = ai.getSimpleProductionSystem();
        var state = COUNTERPOINTAI._loadCurrentPlannerState(system, ai, map);
        if (state === null)
        {
            return false;
        }
        var previousStateJson = COUNTERPOINTAI._plannerStateJson(state);
        if (previousStateJson === null)
        {
            return false;
        }
        var building = action.getTargetBuilding();
        if (building === null || building === undefined)
        {
            return false;
        }
        COUNTERPOINTAI._refreshSpecialSiblings(system, state, building);
        var plan = COUNTERPOINTAI._findPlan(
            state,
            building.getX(),
            building.getY(),
            action.getActionID()
        );
        if (plan === null || plan.phase !== COUNTERPOINTAI.PHASE_SPECIAL)
        {
            return false;
        }
        var planIndex = COUNTERPOINTAI._planIndex(state.plans, plan);
        if (plan.complete)
        {
            return COUNTERPOINTAI._saveMenuSkipState(
                system,
                state,
                previousStateJson
            );
        }
        var resolved = COUNTERPOINTAI._resolveLivePlanCandidate(
            state.plans,
            planIndex,
            ids,
            costs,
            enabled,
            COUNTERPOINTAI._spendableFunds(state, ai),
            state.turnTargets
        );
        if (resolved !== null &&
            COUNTERPOINTAI._plannerSelectionFits(
                state,
                planIndex,
                resolved.candidateIndex,
                resolved.cost
            ) &&
            COUNTERPOINTAI._savePlannerState(system, state))
        {
            return resolved.index;
        }
        if (resolved === null)
        {
            return COUNTERPOINTAI._saveMenuSkipState(
                system,
                state,
                previousStateJson
            );
        }
        return GameEnums.MenuSelection_Skip;
    },

    onBuildingMenuItemResult : function(ai, action, succeeded, x, y, actionId, map)
    {
        var system = ai.getSimpleProductionSystem();
        var state = COUNTERPOINTAI._loadCurrentPlannerState(system, ai, map);
        if (state === null)
        {
            return false;
        }
        var plan = COUNTERPOINTAI._findPlan(
            state,
            x,
            y,
            actionId
        );
        if (plan === null || plan.phase !== COUNTERPOINTAI.PHASE_SPECIAL ||
            plan.complete || plan.selected < 0)
        {
            return false;
        }
        var planIndex = COUNTERPOINTAI._planIndex(state.plans, plan);
        var candidateIndex = plan.selected;
        if (succeeded === true)
        {
            COUNTERPOINTAI._completePlan(
                state.plans,
                planIndex,
                plan.candidates[candidateIndex].transactionCost
            );
            if (!COUNTERPOINTAI._savePlannerState(system, state))
            {
                return false;
            }
            var building = COUNTERPOINTAI._buildingAt(map, x, y);
            if (building !== null && building.getOwnerID() === state.playerId)
            {
                COUNTERPOINTAI._refreshSpecialSiblings(system, state, building);
            }
            else
            {
                COUNTERPOINTAI._releaseSpecialPlansAt(state, x, y);
            }
            COUNTERPOINTAI._savePlannerState(system, state);
            return false;
        }
        COUNTERPOINTAI._rejectPlanCandidate(
            state.plans,
            planIndex,
            candidateIndex
        );
        if (!COUNTERPOINTAI._savePlannerState(system, state))
        {
            return false;
        }
        var retry = COUNTERPOINTAI._selectPlanCandidate(
            state.plans,
            planIndex,
            COUNTERPOINTAI._spendableFunds(state, ai),
            state.turnTargets
        );
        if (!retry)
        {
            COUNTERPOINTAI._savePlannerState(system, state);
            return GameEnums.MenuSelection_Restart;
        }
        if (!COUNTERPOINTAI._plannerSelectionFits(
                state,
                planIndex,
                plan.selected,
                plan.candidates[plan.selected].transactionCost
            ) ||
            !COUNTERPOINTAI._savePlannerState(system, state))
        {
            return GameEnums.MenuSelection_Restart;
        }
        return true;
    },

    buildUnitSimpleProductionSystem : function(system, ai, buildings, units,
                                               enemyUnits, enemyBuildings, map)
    {
        var state = COUNTERPOINTAI._loadCurrentPlannerState(system, ai, map);
        if (state === null || !state.ordinaryPrepared)
        {
            return false;
        }
        if (!COUNTERPOINTAI._hasPendingOrdinaryPlan(state))
        {
            COUNTERPOINTAI._rescanFreedFactories(
                system,
                ai,
                buildings,
                units,
                enemyUnits,
                enemyBuildings,
                map,
                state
            );
        }
        var remainingPlanScanRestarts = 0;
        for (var limitIndex = 0; limitIndex < state.plans.length; ++limitIndex)
        {
            var limitPlan = state.plans[limitIndex];
            if (limitPlan.phase === COUNTERPOINTAI.PHASE_ORDINARY &&
                !limitPlan.complete)
            {
                remainingPlanScanRestarts += limitPlan.candidates.length + 1;
            }
        }
        var restartPlanScan;
        do
        {
            restartPlanScan = false;
            for (var planIndex = 0; planIndex < state.plans.length; ++planIndex)
            {
                var plan = state.plans[planIndex];
                if (plan.phase !== COUNTERPOINTAI.PHASE_ORDINARY || plan.complete)
                {
                    continue;
                }
                var previousStateJson = COUNTERPOINTAI._plannerStateJson(state);
                if (previousStateJson === null)
                {
                    return false;
                }
                var building = COUNTERPOINTAI._buildingAt(map, plan.x, plan.y);
                if (building === null || building.getOwnerID() !== state.playerId)
                {
                    COUNTERPOINTAI._abandonPlan(state.plans, planIndex);
                }
                else
                {
                    var data = system.getProductionActionData(building, plan.actionId);
                    COUNTERPOINTAI._updatePlanFromActionData(plan, data);
                    if (!plan.available)
                    {
                        COUNTERPOINTAI._abandonPlan(state.plans, planIndex);
                    }
                    else
                    {
                        var ids = data.getUnitIds();
                        var costs = data.getTransactionCosts();
                        var enabled = data.getEnabledList();
                        while (!plan.complete)
                        {
                            var resolved = COUNTERPOINTAI._resolveLivePlanCandidate(
                                state.plans,
                                planIndex,
                                ids,
                                costs,
                                enabled,
                                COUNTERPOINTAI._spendableFunds(state, ai),
                                state.turnTargets
                            );
                            if (resolved === null)
                            {
                                break;
                            }
                            if (!COUNTERPOINTAI._plannerSelectionFits(
                                    state,
                                    planIndex,
                                    resolved.candidateIndex,
                                    resolved.cost
                                ) ||
                                !COUNTERPOINTAI._savePlannerState(system, state))
                            {
                                return false;
                            }
                            if (system.executeCounterpointBuild(
                                    plan.x,
                                    plan.y,
                                    resolved.candidate.id,
                                    resolved.candidate.ordinal,
                                    resolved.cost
                                ))
                            {
                                COUNTERPOINTAI._completePlan(
                                    state.plans,
                                    planIndex,
                                    resolved.cost
                                );
                                COUNTERPOINTAI._savePlannerState(system, state);
                                return true;
                            }
                            COUNTERPOINTAI._rejectPlanCandidate(
                                state.plans,
                                planIndex,
                                resolved.candidateIndex
                            );
                            if (!COUNTERPOINTAI._savePlannerState(system, state))
                            {
                                return false;
                            }
                        }
                    }
                }
                var stateChanged = COUNTERPOINTAI._savePlannerStateChange(
                    system,
                    state,
                    previousStateJson
                );
                if (stateChanged === null)
                {
                    return false;
                }
                if (stateChanged && remainingPlanScanRestarts > 0)
                {
                    --remainingPlanScanRestarts;
                    restartPlanScan = true;
                    break;
                }
            }
        }
        while (restartPlanScan);
        COUNTERPOINTAI._savePlannerState(system, state);
        return false;
    }
};

;(function()
{
    // Derived, not listed: the tunables file loads after this one, so every non function property
    // declared above is a constant it must not be able to overwrite. A hand written list only
    // drifts, and the names it missed were silently writable.
    var names = Object.keys(COUNTERPOINTAI);
    for (var index = 0; index < names.length; ++index)
    {
        var name = names[index];
        var value = COUNTERPOINTAI[name];
        if (typeof value === "function")
        {
            continue;
        }
        if (value !== null && typeof value === "object")
        {
            Object.freeze(value);
        }
        Object.defineProperty(COUNTERPOINTAI, name, {
            value : value,
            writable : false,
            configurable : false,
            enumerable : true
        });
    }
}());
