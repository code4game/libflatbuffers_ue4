#include "sampler.h"

#include "Monster_generated.h"

#include <functional>

#if defined(LIBFLATBUFFERS_UE4_PLATFORM_WINDOWS) && !defined(NDEBUG)
#include <crtdbg.h>
#endif

#if defined(LIBFLATBUFFERS_UE4_PLATFORM_WINDOWS)
#include <windows.h>
#endif

#if !defined(NDEBUG)
#define PERFORMANCE_COUNT       0xFFFFF
#else
#define PERFORMANCE_COUNT       0xFFFFFF
#endif

double_t Run(std::function<void()> runFunc)
{
    static LARGE_INTEGER s_frequency;
    QueryPerformanceFrequency(&s_frequency);

    static LARGE_INTEGER s_start_time;
    QueryPerformanceCounter(&s_start_time);

    runFunc();

    static LARGE_INTEGER s_end_time;
    QueryPerformanceCounter(&s_end_time);

    return (1000.0 * (s_end_time.QuadPart - s_start_time.QuadPart)) / (1.0 * s_frequency.QuadPart);
}

int main(int argc, char* argv)
{
#if defined(LIBFLATBUFFERS_UE4_PLATFORM_WINDOWS) && !defined(NDEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace MyGame::Sample;

    printf("count: %d\n", PERFORMANCE_COUNT);

    flatbuffers::FlatBufferBuilder builder;
    {
        auto monster = CreateMonster(builder);
        builder.Finish(monster);

        uint32_t data_size = builder.GetSize();
        printf("data size: %d\n", data_size);
    }

    {
        builder.ForceDefaults(true);

        auto monster = CreateMonster(builder);
        builder.Finish(monster);

        uint32_t data_size = builder.GetSize();
        printf("data size: %d\n", data_size);
    }

    double_t time0 = Run([]() { });

    double_t time1 = Run([&builder]() {
        Monster* monster_ptr = GetMutableMonster(builder.GetBufferPointer());
        for (std::size_t i = 0; i < PERFORMANCE_COUNT; ++i)
        {
            monster_ptr->mutate_mana(static_cast<int16_t>(i % 0xFF));
        }
    });

    int16_t mana = 0;
    double_t time2 = Run([&builder, &mana]() {
        const Monster* monster_ptr = GetMonster(builder.GetBufferPointer());
        const Weapon* weapon_ptr = static_cast<const Weapon*>(monster_ptr->equipped());
        for (std::size_t i = 0; i < PERFORMANCE_COUNT; ++i)
        {
            mana = monster_ptr->mana();
        }
    });
    printf("%d\n", mana);

    printf("time1 %fms\n", time1);
    printf("time2 %fms\n", time2);

    return 0;
}
