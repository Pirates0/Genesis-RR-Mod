#pragma once

#include "native.h"

struct Il2CppString;

namespace cheats {
    void init();
    void handle_command(char* command_buf, void* ui_stdin_w);
    bool internal_persistent_flight_enabled();
    void internal_handle_room_join();
    void internal_handle_room_leave();
    void internal_handle_fire(native::RecRoom::Core::Combat::RangedWeapon* __this, float charge);

    enum class SkinnableTool {
        Invalid = 0,
        Backpack = 1,
        Basketball = 2,
        Crossbow = 3,
        Crossbow_Hunter = 4,
        DiscGolfDisc = 5,
        DodgeballBall = 6,
        Grenade = 7,
        LaserAutomaticGun = 8,
        LaserRailGun = 9,
        LaserShotgun = 10,
        LaserPistol = 11,
        Longbow = 12,
        MakerPen = 13,
        PaintballAssaultRifle = 14,
        PaintballGrenadeLauncher = 15,
        PaintballGun = 16,
        PaintballShield = 17,
        PaintballShotgun = 18,
        PaintballRifleScoped = 19,
        Sword = 20,
        QuestShield = 21,
        SoccerShield = 22,
        Wand = 23,
        ShareCamera = 24,
    };
    SkinnableTool internal_get_skin_tool(Il2CppString* prefab_name);
    bool internal_try_get_skin_guid(SkinnableTool tool, native::PhotonPlayer* for_player, char* buf);
}


