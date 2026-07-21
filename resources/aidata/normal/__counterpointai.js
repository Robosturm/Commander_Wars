;var COUNTERPOINTAI =
{
    STRATEGY_VERSION : 1,
    DOMAIN_GROUND : "ground",
    DOMAIN_AIR : "air",
    DOMAIN_NAVAL : "naval",
    DOMAIN_HOVER : "hover",
    ACTION_BUILD_UNITS : "ACTION_BUILD_UNITS",
    MENU_SELECTION_SKIP : -2,
    MENU_SELECTION_RESTART : -3,
    PHASE_SPECIAL : "special",
    PHASE_ORDINARY : "ordinary",
    PLANNER_STATE_VARIABLE_ID : "COUNTERPOINT_STATE",
    PLANNER_STATE_SCHEMA_VERSION : 1,
    RNG_ALGORITHM_VERSION : 1,
    RNG_COUNTER_MULTIPLIER : 1831565813,
    RNG_LEFT_SHIFT_A : 13,
    RNG_RIGHT_SHIFT : 17,
    RNG_LEFT_SHIFT_B : 5,
    PLANNER_UINT32_MAX : 4294967295,
    PLANNER_VALUE_MAX : 2147483647,
    PLANNER_ID_LENGTH_HARD_LIMIT : 1024,
    PLANNER_KEY_LENGTH_HARD_LIMIT : 2048,
    PLANNER_PLAN_COUNT_HARD_LIMIT : 512,
    PLANNER_CANDIDATE_COUNT_HARD_LIMIT : 512,
    PLANNER_TOTAL_CANDIDATE_HARD_LIMIT : 32768,
    PLANNER_DRAW_COUNT_HARD_LIMIT : 1000000000,
    PLANNER_STATE_LENGTH_HARD_LIMIT : 8388608,

    _finiteNumber : function(value, fallback)
    {
        var numeric = +value;
        return isFinite(numeric) ? numeric : fallback;
    },

    _clamp : function(value, minimum, maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    },

    _readNumber : function(source, key, fallback)
    {
        if (source === null || source === undefined || source[key] === undefined)
        {
            return fallback;
        }
        return COUNTERPOINTAI._finiteNumber(source[key], fallback);
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
        if (unit !== null && unit !== undefined &&
            (unit.compositionKey !== undefined || unit.ownerId !== undefined))
        {
            return unitKey;
        }
        return unitKey + "@" + COUNTERPOINTAI._strategicValue(unit);
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
            return Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(collection.length, 0)));
        }
        if (typeof collection.size === "function")
        {
            return Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(collection.size(), 0)));
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

    _candidateLimit : function()
    {
        var CANDIDATE_HARD_LIMIT = 65536;
        var configured = Math.floor(COUNTERPOINTAI._finiteNumber(
            COUNTERPOINTAI.MAX_CANDIDATE_COUNT,
            CANDIDATE_HARD_LIMIT
        ));
        return COUNTERPOINTAI._clamp(configured, 1, CANDIDATE_HARD_LIMIT);
    },

    _randomWeightLimit : function()
    {
        var RANDOM_WEIGHT_HARD_LIMIT = 1000000000;
        var configured = Math.floor(COUNTERPOINTAI._finiteNumber(
            COUNTERPOINTAI.MAX_RANDOM_WEIGHT_TOTAL,
            RANDOM_WEIGHT_HARD_LIMIT
        ));
        return COUNTERPOINTAI._clamp(configured, 1, RANDOM_WEIGHT_HARD_LIMIT);
    },

    _captureBaseChance : function()
    {
        var chance = COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI.CAPTURE_BASE_CHANCE, 0);
        if (chance <= 0)
        {
            return 0;
        }
        return Math.min(COUNTERPOINTAI.PERCENT_MAX, chance);
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
                    maxDamageVsArmored : unit.maxDamageVsArmored,
                    isAASpecialist : unit.isAASpecialist === true,
                    damageById : unit.damageById
                });
            }
            entries[entryIndex].count += 1;
            entries[entryIndex].hpSum += COUNTERPOINTAI._hpFrac(unit);
        }
        return entries;
    },

    _sampleEnemyComp : function(units)
    {
        return COUNTERPOINTAI._sampleComposition(units);
    },

    _sampleOwnComp : function(units)
    {
        return COUNTERPOINTAI._sampleComposition(units);
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
            if (enemy.domain === COUNTERPOINTAI.DOMAIN_NAVAL)
            {
                naval += hp;
            }
            if (COUNTERPOINTAI._readNumber(enemy, "minRange", 1) > 1)
            {
                indirect += hp;
            }
        }
        if (total <= 0)
        {
            return { airShare : 0, navalShare : 0, indirectShare : 0, total : 0 };
        }
        return {
            airShare : air / total,
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

    _computeOwnCoverage : function(ownComposition, enemyComposition)
    {
        var coverage = Object.create(null);
        var enemyLength = COUNTERPOINTAI._collectionLength(enemyComposition);
        var ownLength = COUNTERPOINTAI._collectionLength(ownComposition);
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
                total += hpSum * COUNTERPOINTAI._damageAgainst(own, enemyId);
            }
            coverage[enemyKey] = total;
        }
        return coverage;
    },

    _isIslandMode : function(mapContext)
    {
        if (mapContext === null || mapContext === undefined)
        {
            return false;
        }
        return mapContext.shareIsland === false ||
               COUNTERPOINTAI._readNumber(mapContext, "enemyIslandBuildings", 0) > 0;
    },

    _isAASpecialist : function(unit)
    {
        if (unit === null || unit === undefined ||
            unit.domain !== COUNTERPOINTAI.DOMAIN_GROUND ||
            unit.canCapture === true ||
            unit.isTransporter === true ||
            COUNTERPOINTAI._readNumber(unit, "minRange", 1) > 1 ||
            COUNTERPOINTAI._strategicValue(unit) < COUNTERPOINTAI.TANK_MIN_BASE_COST)
        {
            return false;
        }
        if (unit.maxDamageVsArmored === undefined)
        {
            return unit.isAASpecialist === true;
        }
        return COUNTERPOINTAI._readNumber(unit, "maxDamageVsArmored", 0) <
               COUNTERPOINTAI.TANK_AA_ARMORED_THRESHOLD;
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
               COUNTERPOINTAI._readNumber(unit, "minRange", 1) <= 1 &&
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
            isIndirect : COUNTERPOINTAI._readNumber(unit, "minRange", 1) > 1,
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

    _unitHasOffenseVs : function(unit, enemyComposition)
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
            if (COUNTERPOINTAI._damageAgainst(unit, enemyId) > 0)
            {
                return true;
            }
        }
        return !validEnemyFound;
    },

    _countOwnTransporters : function(units)
    {
        var counts = { ground : 0, air : 0, naval : 0, hover : 0 };
        var length = COUNTERPOINTAI._collectionLength(units);
        for (var index = 0; index < length; ++index)
        {
            var unit = COUNTERPOINTAI._collectionAt(units, index);
            if (unit === null || unit === undefined || unit.isTransporter !== true)
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
        var count = Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(ownCount, 0)));
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
            if (count === COUNTERPOINTAI.ISLAND_TRANSPORT_THREE_COUNT)
            {
                return COUNTERPOINTAI.ISLAND_TRANSPORT_THREE_CHANCE;
            }
            if (count === COUNTERPOINTAI.ISLAND_TRANSPORT_FOUR_COUNT)
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

    _scoreUnitAgainstEnemies : function(candidate, enemyComposition, context)
    {
        var enemies = enemyComposition || [];
        var enemyLength = COUNTERPOINTAI._collectionLength(enemies);
        if (candidate === null || candidate === undefined || enemyLength === 0)
        {
            return 0;
        }
        var scoreContext = context || {};
        var candidateId = COUNTERPOINTAI._unitId(candidate);
        var candidateValue = COUNTERPOINTAI._strategicValue(candidate);
        var candidateMovement = Math.max(0, COUNTERPOINTAI._readNumber(candidate, "movement", 0));
        var candidateMinRange = Math.max(1, COUNTERPOINTAI._readNumber(candidate, "minRange", 1));
        var isIndirect = candidateMinRange > 1;
        var isTank = COUNTERPOINTAI._isTankClass(candidate);
        var ownCount = COUNTERPOINTAI._ownCount(scoreContext, candidateId);

        if (scoreContext.islandMode === true && candidate.isTransporter === true)
        {
            var transportScore = candidateValue * COUNTERPOINTAI.TRANSPORT_BASE_SCORE_FACTOR;
            if (candidate.domain === COUNTERPOINTAI.DOMAIN_NAVAL ||
                candidate.domain === COUNTERPOINTAI.DOMAIN_HOVER)
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

        var directHp = 0;
        for (var prepassIndex = 0; prepassIndex < enemyLength; ++prepassIndex)
        {
            var prepassEnemy = COUNTERPOINTAI._collectionAt(enemies, prepassIndex);
            if (prepassEnemy === null || prepassEnemy === undefined ||
                COUNTERPOINTAI._unitId(prepassEnemy) === "")
            {
                continue;
            }
            if (COUNTERPOINTAI._readNumber(prepassEnemy, "minRange", 1) > 1)
            {
                continue;
            }
            var prepassHp = Math.max(0, COUNTERPOINTAI._readNumber(prepassEnemy, "hpSum", 0));
            if (prepassEnemy.canCapture === true)
            {
                prepassHp = COUNTERPOINTAI._softcapCapperHP(prepassHp);
            }
            directHp += prepassHp;
        }
        var adjacencyScale = directHp > COUNTERPOINTAI.ADJACENCY_RETAL_CAP ?
            COUNTERPOINTAI.ADJACENCY_RETAL_CAP / directHp : 1;
        var offenseContributions = [];
        var defenseContributions = [];
        var rawDefenseContributions = [];
        var threatProfile = scoreContext.threatProfile ||
            COUNTERPOINTAI._analyzeEnemyComp(enemies);

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
            var defense = COUNTERPOINTAI._damageAgainst(enemy, candidateId);
            if (offense > 0 &&
                candidate.domain === COUNTERPOINTAI.DOMAIN_NAVAL &&
                enemy.domain === COUNTERPOINTAI.DOMAIN_GROUND &&
                scoreContext.islandMode !== true)
            {
                var effectiveRange = COUNTERPOINTAI._readNumber(candidate, "maxRange", 1) +
                    COUNTERPOINTAI._contextNumber(
                        scoreContext,
                        "indirectRangeDeltas",
                        candidateId,
                        0
                    );
                if (effectiveRange <= 1)
                {
                    offense *= COUNTERPOINTAI.NAVAL_VS_LAND_DIRECT_PENALTY;
                }
                else if (effectiveRange < COUNTERPOINTAI.NAVAL_VS_LAND_RANGE_CUTOFF)
                {
                    offense *= COUNTERPOINTAI.NAVAL_VS_LAND_SHORT_PENALTY;
                }
            }

            var enemyValue = COUNTERPOINTAI._strategicValue(enemy);
            var threatFactor = Math.max(enemyValue, COUNTERPOINTAI.THREAT_VALUE_FLOOR) /
                COUNTERPOINTAI.COST_SCALE;
            if (enemy.canCapture === true && enemyValue <= COUNTERPOINTAI.CHEAP_CAPPER_MAX_COST)
            {
                threatFactor *= COUNTERPOINTAI.CAP_THREAT_MULTIPLIER;
            }
            var defenseCounterFactor = defense >= COUNTERPOINTAI.HARD_COUNTER_DAMAGE ?
                COUNTERPOINTAI.HARD_COUNTER_FACTOR : 1;
            var valueRatio = candidateValue / enemyValue;
            var offenseCounterFactor = 1;
            if (offense >= COUNTERPOINTAI.HARD_COUNTER_DAMAGE)
            {
                offenseCounterFactor = valueRatio > COUNTERPOINTAI.EXPENSIVE_COUNTER_COST_RATIO ?
                    COUNTERPOINTAI.EXPENSIVE_COUNTER_FACTOR :
                    COUNTERPOINTAI.HARD_COUNTER_FACTOR;
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
            var enemyIsIndirect = COUNTERPOINTAI._readNumber(enemy, "minRange", 1) > 1;
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
            var candidateValueUnits = Math.max(1, candidateValue / COUNTERPOINTAI.COST_SCALE);
            var gapFloor = COUNTERPOINTAI.GAP_FLOOR_BASE +
                COUNTERPOINTAI.GAP_FLOOR_COST_FACTOR * candidateValueUnits;
            gapFloor = Math.min(COUNTERPOINTAI.GAP_FLOOR_MAX, gapFloor);
            var gapFactor = 1;
            if (threatNeed > 0)
            {
                var coverageRatio = coverage / threatNeed;
                if (coverageRatio > COUNTERPOINTAI.COVERAGE_SATURATION_RATIO)
                {
                    gapFloor = Math.max(
                        COUNTERPOINTAI.SATURATED_GAP_FLOOR_MIN,
                        gapFloor - COUNTERPOINTAI.SATURATED_GAP_REDUCTION
                    );
                }
                gapFactor = Math.max(
                    gapFloor,
                    Math.min(
                        COUNTERPOINTAI.GAP_FACTOR_MAX,
                        COUNTERPOINTAI.GAP_FACTOR_MAX - coverageRatio
                    )
                );
            }

            var mobilityFactor = 1;
            if (!isIndirect && candidateMovement > 0)
            {
                var enemyMovement = Math.max(0, COUNTERPOINTAI._readNumber(enemy, "movement", 0));
                if (enemyMovement > candidateMovement)
                {
                    mobilityFactor = COUNTERPOINTAI.MOBILITY_BASE_FACTOR +
                        COUNTERPOINTAI.MOBILITY_RATIO_FACTOR * candidateMovement / enemyMovement;
                }
                else if (candidateMovement >= COUNTERPOINTAI.FAST_MOVEMENT_THRESHOLD)
                {
                    mobilityFactor = 1 + Math.min(
                        COUNTERPOINTAI.FAST_MOVEMENT_BONUS_MAX,
                        (candidateMovement - COUNTERPOINTAI.FAST_MOVEMENT_BASE) *
                        COUNTERPOINTAI.FAST_MOVEMENT_BONUS_STEP
                    );
                }
            }

            var offenseContribution = 0;
            if (offense > 0)
            {
                offenseContribution = enemyHp * offense * threatFactor *
                    offenseCounterFactor * roleFactor * gapFactor * mobilityFactor;
            }
            offenseContributions.push(offenseContribution);

            var defenseContribution = 0;
            var rawDefenseContribution = 0;
            if (defense > 0)
            {
                var adjacencyFactor = enemyIsIndirect ? 1 : adjacencyScale;
                var baseDefense = enemyHp * adjacencyFactor * defense *
                    threatFactor * defenseCounterFactor;
                var tankIndirectFactor = isTank && enemyIsIndirect ?
                    COUNTERPOINTAI.TANK_VS_INDIRECT_DEF_MUL : 1;
                rawDefenseContribution = baseDefense * tankIndirectFactor;
                defenseContribution = rawDefenseContribution *
                    (isIndirect ? COUNTERPOINTAI.INDIRECT_DEFENSE_FACTOR : 1);
            }
            defenseContributions.push(defenseContribution);
            rawDefenseContributions.push(rawDefenseContribution);
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
        biasFraction = Math.floor(biasFraction * COUNTERPOINTAI.PERCENT_MAX) /
            COUNTERPOINTAI.PERCENT_MAX;
        var otherWeight = (1 - biasFraction) / (length - 1);
        var rawWeights = [];
        for (var weightIndex = 0; weightIndex < length; ++weightIndex)
        {
            rawWeights.push(weightIndex === cheapestIndex ? biasFraction : otherWeight);
        }
        return COUNTERPOINTAI._normalizeWeights(rawWeights);
    },

    _netDamageWeights : function(candidates, enemyComposition, context)
    {
        var enemies = enemyComposition || [];
        if (!COUNTERPOINTAI._hasValidComposition(enemies))
        {
            return COUNTERPOINTAI._costWeights(candidates);
        }
        var length = COUNTERPOINTAI._collectionLength(candidates);
        if (length > COUNTERPOINTAI._candidateLimit())
        {
            return { weights : [], total : 0 };
        }
        var scores = [];
        var anyPositive = false;
        for (var scoreIndex = 0; scoreIndex < length; ++scoreIndex)
        {
            var score = COUNTERPOINTAI._scoreUnitAgainstEnemies(
                COUNTERPOINTAI._collectionAt(candidates, scoreIndex),
                enemies,
                context
            );
            scores.push(score);
            anyPositive = anyPositive || score > 0;
        }

        var ranks = [];
        if (!anyPositive)
        {
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
        }

        var temperature = COUNTERPOINTAI._clamp(
            COUNTERPOINTAI._finiteNumber(
                COUNTERPOINTAI.TEMPERATURE,
                COUNTERPOINTAI.DEFAULT_TEMPERATURE
            ),
            COUNTERPOINTAI.TEMPERATURE_MIN,
            COUNTERPOINTAI.TEMPERATURE_MAX
        );
        var inverseTemperature = 1 / temperature;
        var applyTemperature = Math.abs(inverseTemperature - 1) >
            COUNTERPOINTAI.TEMPERATURE_EPSILON;
        var rawWeights = [];
        for (var weightIndex = 0; weightIndex < length; ++weightIndex)
        {
            var rawWeight = 1;
            if (anyPositive)
            {
                var positiveScore = Math.max(1, scores[weightIndex]);
                var valueUnits = Math.max(
                    1,
                    COUNTERPOINTAI._strategicValue(
                        COUNTERPOINTAI._collectionAt(candidates, weightIndex)
                    ) / COUNTERPOINTAI.COST_SCALE
                );
                var efficiency = positiveScore / valueUnits;
                rawWeight = Math.floor(
                    Math.pow(efficiency, COUNTERPOINTAI.SCORE_EFFICIENCY_EXPONENT) *
                    Math.pow(valueUnits, COUNTERPOINTAI.SCORE_VALUE_EXPONENT)
                );
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
        var value = Math.floor(COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI[name], minimum));
        var maximum = minimum;
        if (name === "MAX_PLAN_COUNT")
        {
            maximum = COUNTERPOINTAI.PLANNER_PLAN_COUNT_HARD_LIMIT;
        }
        else if (name === "MAX_PLAN_CANDIDATES")
        {
            maximum = COUNTERPOINTAI.PLANNER_CANDIDATE_COUNT_HARD_LIMIT;
        }
        else if (name === "MAX_TOTAL_PLAN_CANDIDATES")
        {
            maximum = COUNTERPOINTAI.PLANNER_TOTAL_CANDIDATE_HARD_LIMIT;
        }
        else if (name === "MAX_PLANNER_DRAW_COUNT")
        {
            maximum = COUNTERPOINTAI.PLANNER_DRAW_COUNT_HARD_LIMIT;
        }
        else if (name === "MAX_PLANNER_STATE_LENGTH")
        {
            maximum = COUNTERPOINTAI.PLANNER_STATE_LENGTH_HARD_LIMIT;
        }
        return COUNTERPOINTAI._clamp(value, minimum, maximum);
    },

    _validPlannerInteger : function(value, minimum, maximum)
    {
        return typeof value === "number" && isFinite(value) &&
            Math.floor(value) === value && value >= minimum && value <= maximum;
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
            typeof candidate.isTransporter !== "boolean")
        {
            return false;
        }
        return candidate.domain === "" ||
            candidate.domain === COUNTERPOINTAI.DOMAIN_GROUND ||
            candidate.domain === COUNTERPOINTAI.DOMAIN_AIR ||
            candidate.domain === COUNTERPOINTAI.DOMAIN_NAVAL ||
            candidate.domain === COUNTERPOINTAI.DOMAIN_HOVER;
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
            typeof plan.available !== "boolean" ||
            typeof plan.allowCapperBorrow !== "boolean" ||
            typeof plan.complete !== "boolean" ||
            !Array.isArray(plan.candidates) ||
            plan.candidates.length === 0 || plan.candidates.length > candidateLimit ||
            !Array.isArray(plan.borrowed) ||
            plan.borrowed.length > COUNTERPOINTAI._plannerLimit("MAX_PLAN_COUNT", 1))
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

    _validPlannerState : function(state)
    {
        if (state === null || state === undefined ||
            state.schemaVersion !== COUNTERPOINTAI.PLANNER_STATE_SCHEMA_VERSION ||
            state.algorithmVersion !== COUNTERPOINTAI.RNG_ALGORITHM_VERSION ||
            state.strategyVersion !== COUNTERPOINTAI.STRATEGY_VERSION ||
            !Array.isArray(state.plans) ||
            state.plans.length > COUNTERPOINTAI._plannerLimit("MAX_PLAN_COUNT", 1) ||
            typeof state.specialPrepared !== "boolean" ||
            typeof state.ordinaryPrepared !== "boolean")
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
            return COUNTERPOINTAI.MENU_SELECTION_SKIP;
        }
        return stateChanged ?
            COUNTERPOINTAI.MENU_SELECTION_RESTART :
            COUNTERPOINTAI.MENU_SELECTION_SKIP;
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

    _settledPlannerStateFits : function(state, planIndex, spent)
    {
        var settled = COUNTERPOINTAI._clonePlannerState(state);
        if (settled === null)
        {
            return false;
        }
        if (planIndex < 0 || planIndex >= settled.plans.length)
        {
            return false;
        }
        COUNTERPOINTAI._completePlan(settled.plans, planIndex, spent);
        return COUNTERPOINTAI._plannerStateJson(settled) !== null;
    },

    _rejectedPlannerStateFits : function(state, planIndex, candidateIndex)
    {
        var rejected = COUNTERPOINTAI._clonePlannerState(state);
        if (rejected === null)
        {
            return false;
        }
        if (planIndex < 0 || planIndex >= rejected.plans.length)
        {
            return false;
        }
        COUNTERPOINTAI._rejectPlanCandidate(
            rejected.plans,
            planIndex,
            candidateIndex
        );
        return COUNTERPOINTAI._plannerStateJson(rejected) !== null;
    },

    _plannerSelectionFits : function(state, planIndex, candidateIndex, spent)
    {
        return COUNTERPOINTAI._settledPlannerStateFits(state, planIndex, spent) &&
            COUNTERPOINTAI._rejectedPlannerStateFits(
                state,
                planIndex,
                candidateIndex
            );
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
        var seed = system.deriveCounterpointSeed(
            COUNTERPOINTAI.RNG_ALGORITHM_VERSION,
            generation
        );
        return {
            schemaVersion : COUNTERPOINTAI.PLANNER_STATE_SCHEMA_VERSION,
            algorithmVersion : COUNTERPOINTAI.RNG_ALGORITHM_VERSION,
            strategyVersion : COUNTERPOINTAI.STRATEGY_VERSION,
            playerId : playerId,
            day : day,
            generation : generation,
            seed : COUNTERPOINTAI._finiteNumber(seed, 0) >>> 0,
            drawCounter : 0,
            specialPrepared : false,
            ordinaryPrepared : false,
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
        var counter = state.drawCounter >>> 0;
        var value = (state.seed ^ Math.imul(
            (counter + 1) >>> 0,
            COUNTERPOINTAI.RNG_COUNTER_MULTIPLIER
        )) >>> 0;
        value ^= value << COUNTERPOINTAI.RNG_LEFT_SHIFT_A;
        value ^= value >>> COUNTERPOINTAI.RNG_RIGHT_SHIFT;
        value ^= value << COUNTERPOINTAI.RNG_LEFT_SHIFT_B;
        state.drawCounter += 1;
        return (value >>> 0) % upper;
    },

    _planKey : function(x, y, actionId)
    {
        return String(x) + "," + String(y) + ":" + String(actionId);
    },

    _findPlan : function(state, x, y, actionId)
    {
        var key = COUNTERPOINTAI._planKey(x, y, actionId);
        for (var index = 0; index < state.plans.length; ++index)
        {
            if (state.plans[index].key === key)
            {
                return state.plans[index];
            }
        }
        return null;
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
        return COUNTERPOINTAI.DOMAIN_GROUND;
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
                isTransporter : false
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

    _discoverPlans : function(system, ai, buildings, phase)
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

    _uniqueUnitIds : function(plans)
    {
        var ids = [];
        var seen = Object.create(null);
        COUNTERPOINTAI._visitPlanCandidates(plans, function(candidate)
        {
            COUNTERPOINTAI._appendUniqueId(ids, seen, candidate.id);
        });
        ids.sort();
        return ids;
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

    _isArmoredProbeUnit : function(unit, strategicValue)
    {
        return unit !== null && unit !== undefined &&
               COUNTERPOINTAI._domainFromUnitType(unit.getUnitType()) ===
                   COUNTERPOINTAI.DOMAIN_GROUND &&
               unit.canCapture() !== true &&
               unit.isTransporter() !== true &&
               unit.getBaseMinRange() <= 1 &&
               strategicValue >= COUNTERPOINTAI.TANK_MIN_BASE_COST;
    },

    _armoredProbeIds : function(units, enemyUnits)
    {
        var ids = [];
        var seen = Object.create(null);
        var collect = function(collection)
        {
            COUNTERPOINTAI._visitNonNullCollectionItems(collection, function(unit)
            {
                var value = Math.max(1, unit.getBaseCosts());
                if (!COUNTERPOINTAI._isArmoredProbeUnit(unit, value))
                {
                    return;
                }
                var id = String(unit.getUnitID());
                COUNTERPOINTAI._appendUniqueId(ids, seen, id);
            });
        };
        collect(units);
        collect(enemyUnits);
        ids.sort();
        return ids;
    },

    _maxArmoredDamage : function(system, attackerId, armoredIds)
    {
        var maximum = Math.max(
            0,
            COUNTERPOINTAI._finiteNumber(
                system.getCounterpointBaseDamage(attackerId, attackerId),
                0
            )
        );
        for (var index = 0; index < armoredIds.length; ++index)
        {
            maximum = Math.max(
                maximum,
                COUNTERPOINTAI._finiteNumber(
                    system.getCounterpointBaseDamage(attackerId, armoredIds[index]),
                    0
                )
            );
        }
        return maximum;
    },

    _transportCanCarryTank : function(system, transportIds, armoredIds)
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
                maxDamageVsArmored : COUNTERPOINTAI._maxArmoredDamage(
                    system,
                    id,
                    armoredIds
                )
            };
            if (COUNTERPOINTAI._isTankClass(cargo))
            {
                return true;
            }
        }
        return false;
    },

    _snapshotUnit : function(system, unit, id, strategicValue, damageTargets, armoredIds)
    {
        if (unit === null || unit === undefined)
        {
            return null;
        }
        var transportIds = [];
        if (unit.isTransporter() === true)
        {
            var sourceTransportIds = unit.getTransportUnits();
            var transportLength = COUNTERPOINTAI._collectionLength(sourceTransportIds);
            for (var transportIndex = 0; transportIndex < transportLength; ++transportIndex)
            {
                transportIds.push(String(COUNTERPOINTAI._collectionAt(
                    sourceTransportIds,
                    transportIndex
                )));
            }
        }
        var damageById = Object.create(null);
        for (var damageIndex = 0; damageIndex < damageTargets.length; ++damageIndex)
        {
            var defenderId = damageTargets[damageIndex];
            damageById["#" + defenderId] = Math.max(
                0,
                COUNTERPOINTAI._finiteNumber(
                    system.getCounterpointBaseDamage(id, defenderId),
                    0
                )
            );
        }
        var snapshot = {
            id : id,
            strategicValue : Math.max(1, strategicValue),
            domain : COUNTERPOINTAI._domainFromUnitType(unit.getUnitType()),
            movement : unit.getBaseMovementPoints(),
            minRange : unit.getBaseMinRange(),
            maxRange : unit.getBaseMaxRange(),
            canCapture : unit.canCapture() === true,
            isTransporter : unit.isTransporter() === true,
            loadingPlace : unit.isTransporter() === true ? unit.getLoadingPlace() : 0,
            maxDamageVsArmored : COUNTERPOINTAI._maxArmoredDamage(
                system,
                id,
                armoredIds
            ),
            damageById : damageById
        };
        snapshot.canTransportTank = snapshot.isTransporter &&
            COUNTERPOINTAI._transportCanCarryTank(system, transportIds, armoredIds);
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
        var modifier = 0;
        var offMap = Qt.point(-1, -1);
        for (var coIndex = 0; coIndex < owner.getMaxCoCount(); ++coIndex)
        {
            var co = owner.getCO(coIndex);
            if (co !== null && co !== undefined)
            {
                modifier += COUNTERPOINTAI._finiteNumber(
                    co.getCostModifier(unit.getUnitID(), baseValue, offMap),
                    0
                );
            }
        }
        return Math.max(1, baseValue + modifier);
    },

    _snapshotCollection : function(system, units, damageTargets, armoredIds, enemy)
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
                armoredIds
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

    _enrichPlanCandidates : function(system, plans, enemyIds, armoredIds)
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
                    armoredIds
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
                candidate.scoreData.minRange <= 1)
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

    _mapPlanningContext : function(ai, buildings, enemyBuildings, units)
    {
        var context = { shareIsland : true, enemyIslandBuildings : 0 };
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

    _availableDomains : function(plans)
    {
        var domains = {
            ground : false,
            air : false,
            naval : false,
            hover : false
        };
        COUNTERPOINTAI._visitPlanCandidates(plans, function(candidate)
        {
            var domain = candidate.domain;
            if (domains[domain] !== undefined)
            {
                domains[domain] = true;
            }
        });
        return domains;
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
            ground : capperCount >= 2,
            air : islandMode && availableDomains.air,
            naval : islandMode && availableDomains.naval,
            hover : islandMode && availableDomains.hover
        };
    },

    _planningContext : function(system, ai, plans, buildings, units, enemyUnits, enemyBuildings)
    {
        var enemyIds = COUNTERPOINTAI._unitIdsFromCollection(enemyUnits);
        var candidateIds = COUNTERPOINTAI._uniqueUnitIds(plans);
        var armoredIds = COUNTERPOINTAI._armoredProbeIds(units, enemyUnits);
        COUNTERPOINTAI._enrichPlanCandidates(system, plans, enemyIds, armoredIds);
        var ownSnapshots = COUNTERPOINTAI._snapshotCollection(
            system,
            units,
            enemyIds,
            armoredIds,
            false
        );
        var enemySnapshots = COUNTERPOINTAI._snapshotCollection(
            system,
            enemyUnits,
            candidateIds,
            armoredIds,
            true
        );
        var ownComposition = COUNTERPOINTAI._sampleOwnComp(ownSnapshots);
        var enemyComposition = COUNTERPOINTAI._sampleEnemyComp(enemySnapshots);
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
        for (var deltaId in indirectRangeDeltas)
        {
            if (indirectRangeDeltas[deltaId] < 0)
            {
                banIndirects = true;
                break;
            }
        }
        return {
            enemyComposition : enemyComposition,
            ownComposition : ownComposition,
            ownSnapshots : ownSnapshots,
            ownCounts : COUNTERPOINTAI._ownCounts(ownComposition),
            ownCoverage : COUNTERPOINTAI._computeOwnCoverage(
                ownComposition,
                enemyComposition
            ),
            threatProfile : COUNTERPOINTAI._analyzeEnemyComp(enemyComposition),
            indirectRangeDeltas : indirectRangeDeltas,
            tankFerryStats : COUNTERPOINTAI._tankFerryStats(ownSnapshots),
            islandMode : islandMode,
            transportContext : COUNTERPOINTAI._transportContext(
                ownComposition,
                COUNTERPOINTAI._availableDomains(plans),
                islandMode
            ),
            ownTransporters : COUNTERPOINTAI._countOwnTransporters(ownSnapshots),
            banIndirects : banIndirects
        };
    },

    _spreadBudget : function(targets, amount)
    {
        var value = Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(amount, 0)));
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
                !candidate.complete && !candidate.skipped)
            {
                targets.push(candidate);
            }
        }
        return targets;
    },

    _allocatePhaseBudgets : function(state, plans, funds)
    {
        var paidPlans = [];
        var safeFunds = Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(funds, 0)));
        var skipChance = COUNTERPOINTAI._clamp(
            COUNTERPOINTAI._finiteNumber(COUNTERPOINTAI.BASE_SKIP_CHANCE, 0),
            0,
            COUNTERPOINTAI.PERCENT_MAX
        );
        var maySkip = state.day >= COUNTERPOINTAI.BASE_SKIP_MIN_DAY &&
            safeFunds >= COUNTERPOINTAI.BASE_SKIP_MIN_FUNDS;
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
                COUNTERPOINTAI._nextPlannerRoll(state, COUNTERPOINTAI.PERCENT_MAX) < skipChance)
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

        var floor = Math.max(0, Math.floor(COUNTERPOINTAI.FACTORY_FLOOR));
        if (safeFunds < floor * paidPlans.length)
        {
            var split = Math.floor(safeFunds / paidPlans.length);
            var splitRemainder = safeFunds - split * paidPlans.length;
            for (var splitIndex = 0; splitIndex < paidPlans.length; ++splitIndex)
            {
                paidPlans[splitIndex].reservedBudget = split;
                if (splitRemainder > 0)
                {
                    paidPlans[splitIndex].reservedBudget += 1;
                    splitRemainder -= 1;
                }
            }
        }
        else
        {
            for (var floorIndex = 0; floorIndex < paidPlans.length; ++floorIndex)
            {
                paidPlans[floorIndex].reservedBudget = floor;
            }
            var remaining = safeFunds - floor * paidPlans.length;
            var capacity = Math.max(0, dynamicCap - floor);
            for (var capIndex = 0;
                 capIndex < paidPlans.length && remaining > 0;
                 ++capIndex)
            {
                var addition = Math.min(remaining, capacity);
                paidPlans[capIndex].reservedBudget += addition;
                remaining -= addition;
            }
            if (remaining > 0)
            {
                paidPlans[0].reservedBudget += remaining;
            }
        }

        var reclaimed = 0;
        var survivors = [];
        for (var paidIndex = 0; paidIndex < paidPlans.length; ++paidIndex)
        {
            if (paidPlans[paidIndex].skipped)
            {
                reclaimed += paidPlans[paidIndex].reservedBudget;
                paidPlans[paidIndex].reservedBudget = 0;
                paidPlans[paidIndex].complete = true;
            }
            else
            {
                survivors.push(paidPlans[paidIndex]);
            }
        }
        COUNTERPOINTAI._spreadBudget(survivors, reclaimed);
    },

    _borrowForCandidate : function(plans, planIndex, transactionCost)
    {
        var plan = plans[planIndex];
        var needed = Math.max(0, transactionCost - plan.reservedBudget);
        if (needed === 0)
        {
            return true;
        }
        var floor = Math.max(0, Math.floor(COUNTERPOINTAI.FACTORY_FLOOR));
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
            var available = Math.max(0, donor.reservedBudget - floor);
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
            for (var restoreIndex = 0; restoreIndex < borrowed.length; ++restoreIndex)
            {
                var restore = borrowed[restoreIndex];
                for (var donorIndex = 0; donorIndex < plans.length; ++donorIndex)
                {
                    if (plans[donorIndex].key === restore.key)
                    {
                        plans[donorIndex].reservedBudget += restore.amount;
                        break;
                    }
                }
                plan.reservedBudget -= restore.amount;
            }
            return false;
        }
        plan.borrowed = plan.borrowed.concat(borrowed);
        return true;
    },

    _restoreBorrowedBudget : function(plans, plan)
    {
        for (var index = 0; index < plan.borrowed.length; ++index)
        {
            var borrowed = plan.borrowed[index];
            for (var donorIndex = 0; donorIndex < plans.length; ++donorIndex)
            {
                if (plans[donorIndex].key === borrowed.key)
                {
                    plans[donorIndex].reservedBudget += borrowed.amount;
                    break;
                }
            }
            plan.reservedBudget = Math.max(0, plan.reservedBudget - borrowed.amount);
        }
        plan.borrowed = [];
    },

    _releasePlanBudget : function(plans, planIndex, spent)
    {
        var plan = plans[planIndex];
        var used = Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(spent, 0)));
        var remainder = Math.max(0, plan.reservedBudget - used);
        plan.reservedBudget = used;
        plan.borrowed = [];
        if (COUNTERPOINTAI.RECYCLE_UNUSED_BUDGET)
        {
            COUNTERPOINTAI._spreadBudget(
                COUNTERPOINTAI._remainingBudgetPlans(plans, planIndex),
                remainder
            );
        }
    },

    _completePlan : function(plans, planIndex, spent)
    {
        plans[planIndex].complete = true;
        COUNTERPOINTAI._releasePlanBudget(plans, planIndex, spent);
    },

    _abandonPlan : function(plans, planIndex)
    {
        COUNTERPOINTAI._restoreBorrowedBudget(plans, plans[planIndex]);
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
            threatProfile : context.threatProfile,
            indirectRangeDeltas : context.indirectRangeDeltas,
            tankFerryStats : context.tankFerryStats,
            islandMode : context.islandMode
        };
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
            if (mode === "inverse")
            {
                normalized = COUNTERPOINTAI._inverseCostWeights(scoreCandidates, turn);
            }
            else if (mode === "damage")
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

    _planCandidateOrder : function(plan, state, context, turn)
    {
        var domains = [
            COUNTERPOINTAI.DOMAIN_GROUND,
            COUNTERPOINTAI.DOMAIN_AIR,
            COUNTERPOINTAI.DOMAIN_NAVAL,
            COUNTERPOINTAI.DOMAIN_HOVER
        ];
        var pools = Object.create(null);
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
                (context.banIndirects && scoreData.minRange > 1))
            {
                COUNTERPOINTAI._rejectCandidate(plan, candidateIndex);
                continue;
            }
            var pool = pools[candidate.domain] || pools[COUNTERPOINTAI.DOMAIN_GROUND];
            if (candidate.isTransporter)
            {
                pool.transport.push(candidate);
            }
            else
            {
                pool.combat.push(candidate);
            }
        }

        var available = [];
        for (var poolIndex = 0; poolIndex < domains.length; ++poolIndex)
        {
            var domain = domains[poolIndex];
            var domainPool = pools[domain].combat.slice();
            if (pools[domain].transport.length > 0 &&
                context.transportContext[domain] === true &&
                COUNTERPOINTAI._nextPlannerRoll(state, COUNTERPOINTAI.PERCENT_MAX) <
                    COUNTERPOINTAI._transportChance(
                        context.ownTransporters[domain],
                        turn,
                        domain,
                        context.islandMode
                    ))
            {
                domainPool = domainPool.concat(pools[domain].transport);
            }
            if (turn <= 1 && COUNTERPOINTAI.TURN1_FORCE_CAPPERS &&
                COUNTERPOINTAI._capperBuildsAllowed())
            {
                var opening = [];
                for (var openingIndex = 0; openingIndex < domainPool.length; ++openingIndex)
                {
                    if (domainPool[openingIndex].canCapture ||
                        (context.islandMode && domainPool[openingIndex].isTransporter))
                    {
                        opening.push(domainPool[openingIndex]);
                    }
                }
                if (opening.length > 0)
                {
                    domainPool = opening;
                }
            }
            available = available.concat(domainPool);
        }

        var cappers = [];
        var nonCappers = [];
        for (var availableIndex = 0; availableIndex < available.length; ++availableIndex)
        {
            var entry = available[availableIndex];
            if (entry.canCapture)
            {
                cappers.push(entry);
            }
            else if (entry.isTransporter || COUNTERPOINTAI._unitHasOffenseVs(
                entry.scoreData,
                context.enemyComposition
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
        var borrowEnabled = COUNTERPOINTAI.RECYCLE_UNUSED_BUDGET &&
            COUNTERPOINTAI.CAPPER_BORROW_FROM_RESERVE;
        var capperRollPool = borrowEnabled ? cappers : affordableCappers;
        var capperRollHit = capperRollPool.length > 0 &&
            plan.reservedBudget <= COUNTERPOINTAI.CAP_ROLL_MAX_BUDGET &&
            COUNTERPOINTAI._nextPlannerRoll(state, COUNTERPOINTAI.PERCENT_MAX) <
                COUNTERPOINTAI._captureBaseChance();
        plan.allowCapperBorrow = capperRollHit && borrowEnabled;

        var firstPool = capperRollHit ? capperRollPool : nonCappers;
        var secondPool = capperRollHit ? nonCappers : cappers;
        var firstMode = capperRollHit ? "inverse" : "damage";
        var secondMode = capperRollHit ? "damage" : "inverse";
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
        if (COUNTERPOINTAI._capperBuildsAllowed() || cheapNonCappers.length === 0)
        {
            COUNTERPOINTAI._appendCandidateIndexes(ordered, seen, cheapCappers);
        }
        plan.order = ordered;
    },

    _canUseUnreservedFunds : function(plan, transactionCost, funds)
    {
        var safeFunds = Math.max(0, Math.floor(COUNTERPOINTAI._finiteNumber(funds, 0)));
        return COUNTERPOINTAI.AVOID_BUDGET_BASE_SKIPS === true &&
            plan.reservedBudget < safeFunds && transactionCost <= safeFunds;
    },

    _selectPlanCandidate : function(plans, planIndex, funds)
    {
        var plan = plans[planIndex];
        plan.selected = -1;
        var retryable = false;
        var fallbackCandidateIndex = -1;
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
            if (candidate.transactionCost === 0)
            {
                COUNTERPOINTAI._restoreBorrowedBudget(plans, plan);
                plan.selected = candidateIndex;
                return true;
            }
            if (candidate.transactionCost <= plan.reservedBudget)
            {
                plan.selected = candidateIndex;
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
            if (fallbackCandidateIndex < 0 &&
                COUNTERPOINTAI._canUseUnreservedFunds(
                    plan,
                    candidate.transactionCost,
                    funds
                ))
            {
                fallbackCandidateIndex = candidateIndex;
            }
            retryable = true;
        }
        if (fallbackCandidateIndex >= 0)
        {
            plan.selected = fallbackCandidateIndex;
            return true;
        }
        if (retryable)
        {
            return false;
        }
        COUNTERPOINTAI._abandonPlan(plans, planIndex);
        return false;
    },

    _stripTransientCandidateData : function(plans)
    {
        for (var planIndex = 0; planIndex < plans.length; ++planIndex)
        {
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
        var plans = COUNTERPOINTAI._discoverPlans(system, ai, buildings, phase);
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
            enemyBuildings
        );
        COUNTERPOINTAI._allocatePhaseBudgets(
            state,
            plans,
            ai.getPlayer().getFunds()
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
        COUNTERPOINTAI._stripTransientCandidateData(plans);
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
                enemyUnits,
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
        for (var index = 0; index < plans.length; ++index)
        {
            if (plans[index].key === plan.key)
            {
                return index;
            }
        }
        return -1;
    },

    _resolveLivePlanCandidate : function(plans, planIndex, ids, costs, enabled, funds)
    {
        var plan = plans[planIndex];
        while (!plan.complete)
        {
            if (plan.selected < 0 &&
                !COUNTERPOINTAI._selectPlanCandidate(plans, planIndex, funds))
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
                if (COUNTERPOINTAI._canUseUnreservedFunds(
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
            ai.getPlayer().getFunds()
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
        return COUNTERPOINTAI.MENU_SELECTION_SKIP;
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
            ai.getPlayer().getFunds()
        );
        if (!retry)
        {
            COUNTERPOINTAI._savePlannerState(system, state);
            return COUNTERPOINTAI.MENU_SELECTION_RESTART;
        }
        if (!COUNTERPOINTAI._plannerSelectionFits(
                state,
                planIndex,
                plan.selected,
                plan.candidates[plan.selected].transactionCost
            ) ||
            !COUNTERPOINTAI._savePlannerState(system, state))
        {
            return COUNTERPOINTAI.MENU_SELECTION_RESTART;
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
                                ai.getPlayer().getFunds()
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
    var plannerConstants = [
        "STRATEGY_VERSION",
        "ACTION_BUILD_UNITS",
        "MENU_SELECTION_SKIP",
        "MENU_SELECTION_RESTART",
        "PHASE_SPECIAL",
        "PHASE_ORDINARY",
        "PLANNER_STATE_VARIABLE_ID",
        "PLANNER_STATE_SCHEMA_VERSION",
        "RNG_ALGORITHM_VERSION",
        "RNG_COUNTER_MULTIPLIER",
        "RNG_LEFT_SHIFT_A",
        "RNG_RIGHT_SHIFT",
        "RNG_LEFT_SHIFT_B",
        "PLANNER_UINT32_MAX",
        "PLANNER_VALUE_MAX",
        "PLANNER_ID_LENGTH_HARD_LIMIT",
        "PLANNER_KEY_LENGTH_HARD_LIMIT",
        "PLANNER_PLAN_COUNT_HARD_LIMIT",
        "PLANNER_CANDIDATE_COUNT_HARD_LIMIT",
        "PLANNER_TOTAL_CANDIDATE_HARD_LIMIT",
        "PLANNER_DRAW_COUNT_HARD_LIMIT",
        "PLANNER_STATE_LENGTH_HARD_LIMIT"
    ];
    for (var index = 0; index < plannerConstants.length; ++index)
    {
        var name = plannerConstants[index];
        Object.defineProperty(COUNTERPOINTAI, name, {
            value : COUNTERPOINTAI[name],
            writable : false,
            configurable : false,
            enumerable : true
        });
    }
}());
