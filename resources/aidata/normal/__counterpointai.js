;var COUNTERPOINTAI =
{
    STRATEGY_VERSION : 1,
    DOMAIN_GROUND : "ground",
    DOMAIN_AIR : "air",
    DOMAIN_NAVAL : "naval",
    DOMAIN_HOVER : "hover",

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
    }
};
