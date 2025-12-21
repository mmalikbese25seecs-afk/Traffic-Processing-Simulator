#pragma once
#include <random>

static unsigned int g_RandomSeed = 123456789;

inline void SetRandomSeed(unsigned int seed)
{
    g_RandomSeed = seed;
}

inline int GetRandomInt(int min, int max)
{
    std::mt19937 generator(g_RandomSeed);
    std::uniform_int_distribution<int> distribution(min, max - 1);
    int randomValue = distribution(generator);
    // update seed for next call
    g_RandomSeed = generator();
    return randomValue;
}

inline bool GetRandomChance(float probability)
{
    std::mt19937 generator(g_RandomSeed);
    std::uniform_real_distribution<float> distribution(0.f, 1.f);
    float randomValue = distribution(generator);
    // update seed for next call
    g_RandomSeed = generator();
    return randomValue < probability;
}

inline float GetRandomFloat(float min, float max)
{
    std::mt19937 generator(g_RandomSeed);
    std::uniform_real_distribution<float> distribution(min, max);
    float randomValue = distribution(generator);
    // update seed for next call
    g_RandomSeed = generator();
    return randomValue;
}
