#include <Windows.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <thread>
#include <base64.hpp>
#include <Xorstr.hpp>
#include <cheats.hpp>
#include <MinHook.h>
#include <native.hpp>
#include <native_invoke.hpp>
#include <overridable_fields.hpp>
#include <audio.hpp>

typedef std::map<int, std::vector<unsigned char>> RoleBackup; //keys are RolePermissionType
std::map<int, RoleBackup> role_backups; //keys are GameRoleId
std::map<int, RoleBackup> perm_backups; //keys are RoomRoleId

bool backup_initialized = false;

typedef std::map<cheats::SkinnableTool, std::string> SkinMap;
std::map<native::PhotonPlayer*, SkinMap> player_skin_map;
std::map<native::RecRoom::Core::Combat::RangedWeapon*, bool> rapid_fire_weapons;

int testIndex = 0;

void equipOutfitItem(int index) {
	using namespace native;
    using RecRoom::Avatar::Data::Runtime::AvatarItemWardrobeRuntimeConfig;
    using RecRoom::Avatar::Data::Runtime::AvatarItem;
    using RecRoom::Avatar::Data::Runtime::AvatarItemSelection;
    using System::Collections::Generic::List;
    using UnityEngine::ResourceManagement::AsyncOperations::AsyncOperationHandle_1;
    using UnityEngine::AddressableAssets::AssetReference;

    auto config = AvatarItemWardrobeRuntimeConfig::get_Config(nullptr, nullptr);
    auto list = config->allPossibleCombinations;

	


    if(index < 0 || index >= List::get_Count(list, nullptr)) return;

    auto avatarItems = (AvatarItem*)list->_items->vector;

    auto avatarItem = avatarItems[index];

	


    native_invoke::add([avatarItem]() {
        AvatarItemSelection avatarItemSelection;
        memset(&avatarItemSelection, 0, sizeof(avatarItemSelection));

        avatarItemSelection._avatarItem = avatarItem;
        switch (avatarItem._avatarItemData->OutfitType)
        {
        //None = -1, Head = 0, Torso = 1, LeftHand = 2, RightHand = 3, Mouth = 4
        case 0:
        case 1:
        case 2:
        case 10:
            avatarItemSelection._bodyPart = 0;
            break;
        case 20:
            avatarItemSelection._bodyPart = 4;
            break;
        case 100:
        case 101:
        case 102:
        case 103:
        case 104:
            avatarItemSelection._bodyPart = 1;
            break;
        case 200:
        case 201:
        case 202:
        case 203:
            avatarItemSelection._bodyPart = 3;
            break;
        default:
            return;
        }
        avatarItemSelection._isTrialItem = true;

	
		



        auto playerStatics = Player::statics();
        if(!playerStatics->LocalPlayerExists) return;

        PlayerAvatar::EquipOutfitItemInternal(playerStatics->LocalPlayer->playerAvatar, avatarItemSelection, nullptr);


	


    });




}











void gifttest() {
	using namespace native;
	MH_EnableHook(GiftManager::GenerateGameGift);
}

bool enable_godmode_local(bool value) {
    using namespace native;

    if(!value) {
        return MH_DisableHook(GameCombatManager::MasterDamagePlayer) != MH_OK;
    }
    if(PhotonNetwork::get_isMasterClient(nullptr)) {
        return MH_EnableHook(GameCombatManager::MasterDamagePlayer) == MH_OK;
    }
    return false;
}





bool __state_godmode_local = false;
bool state_godmode_local() {
    using namespace native;

    if(__state_godmode_local && !PhotonNetwork::get_isMasterClient(nullptr)) {
        return MH_DisableHook(GameCombatManager::MasterDamagePlayer) != MH_OK;
    }
    return __state_godmode_local;
}


void score() {
	using namespace native;
	if (PhotonNetwork::get_isMasterClient(nullptr)) {
		MH_EnableHook(GameScoreManager::MasterSetScore);
	}

}


void killall() {

	using namespace native;
	if (PhotonNetwork::get_isMasterClient(nullptr)) {
		MH_EnableHook(RecRoom::Core::Encounters::BaseSpawnEnemyEncounterStep::MasterKillAllLivingEnemies);
	}
}

bool usable_godmode_local() {
    return native::PhotonNetwork::get_isMasterClient(nullptr);
}

void write_bool(bool value, HANDLE handle) {
    DWORD written;
    if(value) {
        WriteFile(handle, XS("dHJ1ZQ==\n"), 9, &written, nullptr);//base64 for true
    }
    else {
        WriteFile(handle, XS("ZmFsc2U=\n"), 9, &written, nullptr);//base64 for false
    }
}

void spawn_img(std::string const &path) {
    using namespace native;

    auto img = std::ifstream(path, std::ios::ate | std::ios::binary);
    if(!img.is_open()) {
        //MessageBoxA(0, XS("1"), path.c_str(), MB_OK);
        return;
    }
    auto len = img.tellg();
    img.seekg(0, std::ios::beg);

    auto array = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Byte")), len);
    if(!img.read(array->vector, len)) {
        //MessageBoxA(0, XS("2"), "", MB_OK);
        return;
    }

    img.close();

    //todo: cache names of uploaded imgs

    native_invoke::add([array, len]() {
        auto type = get_native_type(XS("Assembly-CSharp"), "", XS("Polaroid"));
        auto polaroid_type_obj = get_native_type_object(type);
        if(!polaroid_type_obj) {
            //MessageBoxA(0, XS("3"), "", MB_OK);
            return;
        }
        
        //get the target transform
        if(!Player::statics()->LocalPlayerExists) return;

        auto pos = UnityEngine::Transform::get_position(
            UnityEngine::Component::get_transform(
                Player::statics()->LocalPlayer->head,
                nullptr
            ),
            nullptr
        );

        pos.y += 0.5f;

        //spawn the polaroid object
        auto go = PhotonNetwork::Instantiate(il2cpp_string_new(XS("[Polaroid]")), pos, {0.0f, 0.0f, 0.0f, 1.0f}, 6.0f, 0, nullptr);
        if(!go) {
            //MessageBoxA(0, XS("4"), "", MB_OK);
            return;
        }
        //get polaroid component
        auto polaroid = UnityEngine::GameObject::GetComponentInChildren(go, polaroid_type_obj, nullptr);
        if(!polaroid) {
            //MessageBoxA(0, XS("5"), "", MB_OK);
            return;
        }

        //upload the local img to recnet and then set it as the picture for the polaroid
        auto name_promise = RecNet::Storage::UploadFile(array, 3, il2cpp_string_new(XS("image/jpeg")), nullptr, nullptr);
        Polaroid::AuthoritySetImageName((Polaroid*)polaroid, name_promise, nullptr);
    });
}

void enable_flight(bool enable) {
    if(!native::Player::statics()->LocalPlayerExists) return;

    native_invoke::add([enable]() {
        using namespace native;
        using namespace native::RecRoom::Core::Locomotion;
        using namespace native::RecRoom::Systems::PlayerRoles;

        auto player_movement = Player::statics()->LocalPlayer->playerMovement;
        auto role_manager = PlayerGameRoleManager::get_Instance(nullptr, nullptr);
        PlayerMovement::AddFlyEnabled(player_movement, enable, role_manager, 0x2710, nullptr);
    });
}




bool flight_enabled() {
    using namespace native;
    using namespace native::RecRoom::Core::Locomotion;

    auto player_statics = Player::statics();

    if(!player_statics->LocalPlayerExists) return false;

    auto player_movement = player_statics->LocalPlayer->playerMovement;
    return PlayerMovement::get_IsFlyingEnabled(player_movement, nullptr);
}

bool __state_persistent_flight = false;
bool enable_persistent_flight(bool enable) {
    using namespace native::RecRoom::Core::Locomotion;
    if(enable) {
        __state_persistent_flight = MH_EnableHook(PlayerMovement::AddFlyEnabled) == MH_OK;
    }
    else {
        __state_persistent_flight = MH_DisableHook(PlayerMovement::AddFlyEnabled) != MH_OK;
    }

    enable_flight(enable);

    return __state_persistent_flight;
}

bool __state_local_player_invisible = false;
bool local_player_invisible(bool enable) {
    using namespace native;
    auto player_statics = Player::statics();
    if(!player_statics->LocalPlayerExists) return false;

    native_invoke::add([enable, player_statics]() {
        using namespace UnityEngine;

        // auto trans = Component::get_transform(player_statics->LocalPlayer, nullptr);

        // auto pos = Transform::get_position(trans, nullptr);
        // auto rot = Transform::get_rotation(trans, nullptr);

        // auto new_player = PhotonNetwork::Instantiate(il2cpp_string_new(XS("[Player]")), pos, rot, 1.0f, 0, nullptr);
        

        // std::thread([new_player]() {
        //     Sleep(100000);
        //     native_invoke::add([new_player](){
        //         PhotonNetwork::Destroy(new_player, nullptr);
        //     });
        // }).detach();




        auto pv = Photon::MonoBehaviour::get_photonView(player_statics->LocalPlayer, nullptr);
        auto method_name = enable ? il2cpp_string_new(XS("RpcBroadcastDespawnPlayer")) : il2cpp_string_new(XS("RpcBroadcastSpawnPlayer"));

        PhotonNetwork::RPC(
            pv,
            method_name,
            1,
            false,
            nullptr,
            nullptr
        );

        __state_local_player_invisible = enable;
    });
    return true;
}

bool get_local_player_invisible() {
    using namespace native;
    if(__state_local_player_invisible && !Player::statics()->LocalPlayerExists) {
        __state_local_player_invisible = false;
        return false;
    }

    return __state_local_player_invisible;
}

void write_current_players(HANDLE ui_stdin_w) {
    using namespace native;

    auto players = PhotonNetwork::get_otherPlayers(nullptr);
    long long len = players ? players->max_length : -1;
    if(len < 0) {
        DWORD written;
        WriteFile(ui_stdin_w, "\n", 1, &written, nullptr);
        return;
    }

    std::stringstream write_buf;
    PhotonPlayer* cur_photon_player = PhotonNetwork::get_player(nullptr); // always put the local player on top
    Il2CppString* cur_name = PhotonPlayer::get_name(cur_photon_player, nullptr);

    write_buf << base64_encode((const unsigned char*)cur_name->chars, (cur_name->length * 2)) << ' ';
    write_buf << base64_encode((const unsigned char*)&cur_photon_player, 8) << (len == 0 ? '\n' : ' ');

    while(--len >= 0) {
        cur_photon_player = (PhotonPlayer*)Il2CppArray::GetValue(players, len, nullptr);
        cur_name = PhotonPlayer::get_name(cur_photon_player, nullptr);

        write_buf << base64_encode((const unsigned char*)cur_name->chars, (cur_name->length * 2)) << ' ';
        write_buf << base64_encode((const unsigned char*)&cur_photon_player, 8) << (len == 0 ? '\n' : ' ');
    }
    
    auto write_str = write_buf.str();

    DWORD written;
    WriteFile(ui_stdin_w, write_str.data(), write_str.length(), &written, nullptr);
}

bool player_exists(native::PhotonPlayer* player) {
    using namespace native;




    auto players = PhotonNetwork::get_playerList(nullptr);
    long long len = players ? players->max_length : 0;

    while(--len >= 0) {
        if(player == Il2CppArray::GetValue(players, len, nullptr)) 
            return true;
    }

    auto iter = player_skin_map.find(player);
    if(iter != player_skin_map.end()) {
        player_skin_map.erase(iter);
    }
    return false;
}

native::Player* find_rr_player(native::PhotonPlayer* p_player) {
    using namespace native;
    using namespace System::Collections::Generic;

    auto all_players = Player::statics()->All;
    auto len = List::get_Count(all_players, nullptr);

    Player* current;
    while(--len >= 0) {
        current = (Player*)List::get_Item(all_players, len, nullptr);
        if(Player::get_PhotonPlayer(current, nullptr) == p_player)
            return current;
    }

    return nullptr;
}

bool locked = false;

void player_lockon(bool enable){
    using namespace native;
    using namespace UnityEngine;
    auto player = PhotonNetwork::get_playerList(nullptr);
    auto players = (native::PhotonPlayer *)Il2CppArray::GetValue(player, 0, nullptr);

    if (enable) {
	    locked = true;
	    native_invoke::add_recurring([enable, players]() {
		    auto player_statics = Player::statics();
		    if (!player_statics->LocalPlayerExists || !player_exists(players)) return false;

		    auto remote_player = find_rr_player(players);
		    if (!remote_player) return false;

		    auto target = Component::get_transform(remote_player->head, nullptr);
		    auto from = Component::get_transform(player_statics->LocalPlayer->head, nullptr);
		    auto own_transform = Component::get_transform(player_statics->LocalPlayer, nullptr);

		    auto target_pos = Transform::get_position(target, nullptr);
		    target_pos.y += 0.5f;

		    auto from_pos = Transform::get_position(from, nullptr);

		    Vector3 displacement;
		    displacement.x = target_pos.x - from_pos.x;
		    displacement.y = target_pos.y - from_pos.y;
		    displacement.z = target_pos.z - from_pos.z;

		    auto base_pos = Transform::get_position(own_transform, nullptr);
		    base_pos.x += displacement.x;
		    base_pos.y += displacement.y;
		    base_pos.z += displacement.z;

		    if (locked) {
			    Transform::set_position(own_transform, base_pos, nullptr);
			    return true;
		    }
		    else {
                return false;
            }
		});
	} 
    else {
	    locked = false;
    }
}



bool get_locked_on() {
	
	using namespace native;
	using namespace UnityEngine;
	auto player = PhotonNetwork::get_playerList(nullptr);
	auto players = (native::PhotonPlayer*)Il2CppArray::GetValue(player, 0, nullptr);
	auto player_statics = Player::statics();

	if (locked && !player_statics->LocalPlayerExists || !player_exists(players)) {
		locked = false;
		return false;

	}

	
	return locked;

}




void swap_players(native::PhotonPlayer* player) {


	using namespace native;
	using namespace UnityEngine;
	auto player_statics = Player::statics();
	if (!player_statics->LocalPlayerExists || !player_exists(player)) return;

	auto rr_clowns = find_rr_player(player);

	auto from = Component::get_transform(player_statics->LocalPlayer->head, nullptr);

	auto target = Component::get_transform(rr_clowns->head, nullptr);
	
	auto own_transform = Component::get_transform(player_statics->LocalPlayer, nullptr);



	if (!rr_clowns) return;
	auto feedback_type = get_native_type(XS("Assembly-CSharp"), "", XS("PlayerProgression/Feedback"));
	if (!feedback_type) {
		MessageBoxA(0, XS("no type found"), "", MB_OK);
		return;
	}

	native_invoke::add([rr_clowns, from, player, target,own_transform]() {


		auto from_pos = Transform::get_position(from, nullptr);
		auto pv = Photon::MonoBehaviour::get_photonView(RecRoomSceneManager::get_Instance(nullptr, nullptr), nullptr);
		auto method_name = il2cpp_string_new(XS("RpcRespawnPlayer"));

		auto params = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Object")), 4);
		auto vector3_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Vector3"));
		auto quaternion_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
		auto bool_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Boolean"));
		auto target_pos = Transform::get_position(target, nullptr);
		target_pos.y += 0.5f;

		//auto from_pos = Transform::get_position(from, nullptr);

		Vector3 displacement;
		displacement.x = target_pos.x - from_pos.x;
		displacement.y = target_pos.y - from_pos.y;
		displacement.z = target_pos.z - from_pos.z;

		auto base_pos = Transform::get_position(own_transform, nullptr);
		base_pos.x += displacement.x;
		base_pos.y += displacement.y;
		base_pos.z += displacement.z;

		UnityEngine::Vector3 raw_pos = from_pos;
		UnityEngine::Quaternion raw_rot = { 0, 0, 0, 1.0f };
		auto spawn_parent = Photon::MonoBehaviour::get_photonView(rr_clowns, nullptr);
		bool raw_dropTools = true;

		Il2CppArray::SetValue(params, il2cpp_value_box(vector3_type, &raw_pos), 0, nullptr);
		Il2CppArray::SetValue(params, il2cpp_value_box(quaternion_type, &raw_rot), 1, nullptr);
		Il2CppArray::SetValue(params, spawn_parent, 2, nullptr);
		Il2CppArray::SetValue(params, il2cpp_value_box(bool_type, &raw_dropTools), 3, nullptr);


	

		Transform::set_position(own_transform, base_pos, nullptr);

		PhotonNetwork::RPC_player(
			pv,
			method_name,
			player,
			false,
			params,
			nullptr
		);



		});

		



}

void player_lockon2(native::PhotonPlayer* player) {

	native_invoke::add_recurring([player]() {
		using namespace native;
		using namespace UnityEngine;
		auto player_statics = Player::statics();
		if (!player_statics->LocalPlayerExists || !player_exists(player)) return false;
		auto remote_player = find_rr_player(player);
		if (!remote_player) return false;

		auto target = Component::get_transform(remote_player->head, nullptr);
		auto from = Component::get_transform(player_statics->LocalPlayer->head, nullptr);
		


		auto own_transform = Component::get_transform(remote_player->head, nullptr);


		auto target_pos = Transform::get_position(target, nullptr);

		target_pos.y += 0.5f;

		auto from_pos = Transform::get_position(from, nullptr);

		Vector3 displacement;
		displacement.x = target_pos.x - from_pos.x;
		displacement.y = target_pos.y - from_pos.y;
		displacement.z = target_pos.z - from_pos.z;

		auto base_pos = Transform::get_position(own_transform, nullptr);
		base_pos.x += displacement.x;
		base_pos.y += displacement.y;
		base_pos.z += displacement.z;

		Transform::set_position(own_transform, base_pos, nullptr);
		return true;
		});

}

/*
void player_lockon(native::PhotonPlayer* player) {

	native_invoke::add_recurring([player]() {
		using namespace native;
		using namespace UnityEngine;
		auto player_statics = Player::statics();
		if (!player_statics->LocalPlayerExists || !player_exists(player)) return false;
		auto remote_player = find_rr_player(player);
		if (!remote_player) return false;
		auto target = Component::get_transform(remote_player->head, nullptr);
		auto from = Component::get_transform(player_statics->LocalPlayer->head, nullptr);
		auto own_transform = Component::get_transform(player_statics->LocalPlayer, nullptr);
		auto target_pos = Transform::get_position(target, nullptr);
		target_pos.y += 0.5f;

		auto from_pos = Transform::get_position(from, nullptr);

		Vector3 displacement;
		displacement.x = target_pos.x - from_pos.x;
		displacement.y = target_pos.y - from_pos.y;
		displacement.z = target_pos.z - from_pos.z;

		auto base_pos = Transform::get_position(own_transform, nullptr);
		base_pos.x += displacement.x;
		base_pos.y += displacement.y;
		base_pos.z += displacement.z;

		Transform::set_position(own_transform, base_pos, nullptr);
		return true;
		});
	
}


*/
void teleport_to_player(native::PhotonPlayer* player) {
    using namespace native;
    using namespace UnityEngine;
    auto player_statics = Player::statics();
    if(!player_statics->LocalPlayerExists || !player_exists(player)) return;

    auto remote_player = find_rr_player(player);
    if(!remote_player) return;

    auto target = Component::get_transform(remote_player->head, nullptr);
    auto from = Component::get_transform(player_statics->LocalPlayer->head, nullptr);
    auto own_transform = Component::get_transform(player_statics->LocalPlayer, nullptr);

    native_invoke::add([target, from, own_transform]() {
        auto target_pos = Transform::get_position(target, nullptr);
        target_pos.y += 0.5f;

        auto from_pos = Transform::get_position(from, nullptr);

        Vector3 displacement;
        displacement.x = target_pos.x - from_pos.x;
        displacement.y = target_pos.y - from_pos.y;
        displacement.z = target_pos.z - from_pos.z;

        auto base_pos = Transform::get_position(own_transform, nullptr);
        base_pos.x += displacement.x;
        base_pos.y += displacement.y;
        base_pos.z += displacement.z;
		
        Transform::set_position(own_transform, base_pos, nullptr);
    });
}

void set_perm(int manager_kind, int role_id, int perm_id, bool is_local, std::vector<unsigned char> const &bytes) {
    using namespace native;
    using namespace RecRoom::Systems::PlayerRoles;
    Il2CppClass* role_id_type = nullptr;
    Il2CppClass* role_perm_type = nullptr;
    if(manager_kind == 0) {
        role_id_type = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("RoomRoleId"));
        role_perm_type = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("RoomRole/RolePermission"));
    }
    else if(manager_kind == 1) {
        role_id_type = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("GameRoleId"));
        role_perm_type = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("GameRole/RolePermission"));
    }
    if(!role_id_type || !role_perm_type) return;

    //the type is an enum with int value in both cases so this works
    auto _role_id = il2cpp_object_new(role_id_type);
    ((int*)_role_id)[4] = role_id;
    auto role_perm = il2cpp_object_new(role_perm_type);
    ((int*)role_perm)[4] = perm_id;

    auto byte_count = bytes.size();

    auto bytes_arg = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Byte")), byte_count);
    if(byte_count) {
        if(memcpy_s(bytes_arg->vector, bytes_arg->max_length, bytes.data(), byte_count)) 
            return; //zero on success
    }
    
    auto params = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Object")), 3);
    Il2CppArray::SetValue(params, _role_id, 0, nullptr);
    Il2CppArray::SetValue(params, role_perm, 1, nullptr);
    Il2CppArray::SetValue(params, bytes_arg, 2, nullptr);
        
    PhotonView* target;
    if(manager_kind == 0) {
        auto player_rm = PlayerRoomRoleManager::get_Instance(nullptr, nullptr);
        target = Photon::MonoBehaviour::get_photonView(player_rm, nullptr);
    }
    else if(manager_kind == 1) {
        auto player_gm = PlayerGameRoleManager::get_Instance(nullptr, nullptr);
        target = Photon::MonoBehaviour::get_photonView(player_gm, nullptr);
    }
            
    auto method_name = il2cpp_string_new(XS("RpcModifyRoleDefinition"));

    if(is_local)
        PhotonNetwork::RPC_player(target, method_name, Player::get_PhotonPlayer(Player::statics()->LocalPlayer, nullptr), false, params, nullptr);
    else
        PhotonNetwork::RPC(target, method_name, 0, false, params, nullptr);
}

void write_get_perm(int manager_kind, int role_id, int perm_id, bool is_local, HANDLE ui_stdin_w) {
    using namespace native;
    using namespace RecRoom::Systems::PlayerRoles;
    using namespace RecRoom::Utils::OverridableFields;
    using namespace RecRoom::Protobuf;

    auto write_combined_bool_perms = [ui_stdin_w](bool previous, OverridableBoolData* role) {
        if(!role) {
            write_bool(false, ui_stdin_w);
            return;
        }

        if(role->overrides) {
            write_bool(role->innervalue, ui_stdin_w);
        }
        else {
            write_bool(previous, ui_stdin_w);
        }
    };

    if(manager_kind == 0) {
        auto role_mgr = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("PlayerRoomRoleManager"));
        auto get_def_info = il2cpp_class_get_method_from_name(role_mgr, XS("GetRoleDefinition"), 1);
        auto get_default_info = il2cpp_class_get_method_from_name(role_mgr, XS("get_DefaultRoleDefinition"), 0);

        auto get_def = reinterpret_cast<RoomRole*(*)(PlayerRoomRoleManager*, int, const MethodInfo*)>(get_def_info->methodPointer);
        auto mgr = PlayerRoomRoleManager::get_Instance(nullptr, nullptr);
        auto room_role = mgr ? get_def(mgr, role_id, get_def_info) : nullptr;
        auto default_role = mgr ? reinterpret_cast<RoomRole*(*)(PlayerRoomRoleManager*, const MethodInfo*)>(get_default_info->methodPointer)(mgr, get_default_info) : nullptr;


        if(room_role && default_role) {
            if(perm_id == 4) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canInvite, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canInvite, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 5) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canEditCircuits, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canEditCircuits, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 6) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canTalk, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canTalk, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 7) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canPrintPhotos, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canPrintPhotos, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 8) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canStartGames, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canStartGames, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 9) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canSelfRevive, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canSelfRevive, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 12) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canChangeGameMode, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canChangeGameMode, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 13) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canUseMakerPen, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canUseMakerPen, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 14) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canUseDeleteAllButton, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canUseDeleteAllButton, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 15) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canSaveInventions, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canSaveInventions, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 16) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->disableMicAutoMute, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->disableMicAutoMute, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 17) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canEndGamesEarly, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canEndGamesEarly, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 18) {
                auto value = MutableOverridableBool::SerializeToProtobuf(room_role->canUseShareCam, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canUseShareCam, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
        }
        else {
            write_bool(false, ui_stdin_w);
            return;
        }
        //todo: handle other perm types here
    }
    else if(manager_kind == 1) {
        auto role_mgr = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("PlayerGameRoleManager"));
        auto get_def_info = il2cpp_class_get_method_from_name(role_mgr, XS("GetRoleDefinition"), 1);
        auto get_default_info = il2cpp_class_get_method_from_name(role_mgr, XS("get_DefaultRoleDefinition"), 0);

        auto get_def = reinterpret_cast<GameRole*(*)(PlayerGameRoleManager*, int, const MethodInfo*)>(get_def_info->methodPointer);
        auto mgr = PlayerGameRoleManager::get_Instance(nullptr, nullptr);
        auto game_role = mgr ? get_def(mgr, role_id, get_def_info) : nullptr;
        auto default_role = mgr ? reinterpret_cast<GameRole*(*)(PlayerGameRoleManager*, const MethodInfo*)>(get_default_info->methodPointer)(mgr, get_default_info) : nullptr;

        //handle all bool perms
        if(game_role && default_role) {
            if(perm_id == 4) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->canMove, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canMove, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 8) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->canFly, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canFly, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0xd) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->canSwitchTeams, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->canSwitchTeams, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 20) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->hidesName, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->hidesName, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x16) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->vrCanSprint, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->vrCanSprint, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x19) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->screensCanSprint, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->screensCanSprint, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x30) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->screensCanWallRun, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->screensCanWallRun, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x31) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->vrCanWallRun, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->vrCanWallRun, nullptr)->innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x32) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->vrCanGetPushed, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->vrCanGetPushed, nullptr)-> innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x33) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->screensCanGetPushed, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->screensCanGetPushed, nullptr)-> innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x34) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->vrCanRam, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->vrCanRam, nullptr)-> innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
            if(perm_id == 0x35) {
                auto value = MutableOverridableBool::SerializeToProtobuf(game_role->screensCanRam, nullptr);
                auto default_value = MutableOverridableBool::SerializeToProtobuf(default_role->screensCanRam, nullptr)-> innervalue;
                write_combined_bool_perms(default_value, value);
                return;
            }
        }
        else {
            write_bool(false, ui_stdin_w);
            return;
        }
    }
    //code should not reach here, if it does the menu will hang
}

std::string WstrToUtf8Str(const std::wstring& wstr)
{
  std::string retStr;
  if (!wstr.empty())
  {
    int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

    if (sizeRequired > 0)
    {
      std::vector<char> utf8String(sizeRequired);
      int bytesConverted = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),    
                           -1, &utf8String[0], utf8String.size(), NULL, 
                           NULL);
      if (bytesConverted != 0)
      {
        retStr = &utf8String[0];
      }
      else
      {
        return std::string();
      }
    }
  }
  return retStr;
}

void dumpOutfits() {
    //native_invoke::add([]() {
        using namespace native;
        using namespace RecRoom::Avatar::Data::Runtime;
        using namespace RecRoom::Avatar::Data::Shared;
        using namespace System::Collections::Generic;
    
        auto config = AvatarItemWardrobeRuntimeConfig::get_Config(nullptr, nullptr);

        // auto countInfo = il2cpp_class_get_method_from_name(config->allPossibleCombinations->klass, XS("get_Count"), 0);
        // auto itemInfo = il2cpp_class_get_method_from_name(config->allPossibleCombinations->klass, XS("get_Item"), 1);

        std::ofstream guidOutput("outfits.txt", std::ios::app | std::ios::out);
        auto list = config->allPossibleCombinations;
        auto len = List::get_Count(list, nullptr);
        guidOutput << XS("found ") << len << XS(" different outfit items:") << std::endl;

        AvatarItem* currentItem = (AvatarItem*)list->_items->vector;
        char buf[512];
        for(int i = 0; i < len; i++) {
            guidOutput << i << ": ";

            //name
            if(currentItem && currentItem->_avatarItemData && currentItem->_avatarItemData->Name) {
                buf[wcstombs(buf, currentItem->_avatarItemData->Name->chars, currentItem->_avatarItemData->Name->length)] = 0;
                guidOutput << buf << ' ';
            }

            //prefab guid
            if(currentItem && currentItem->_avatarItemVisualData && currentItem->_avatarItemVisualData->prefabGuid) {
                buf[wcstombs(buf, currentItem->_avatarItemVisualData->prefabGuid->chars, currentItem->_avatarItemVisualData->prefabGuid->length)] = 0;
                guidOutput << "P: " << buf << ' ';
            }

            //mask guid
            if(currentItem && currentItem->_avatarItemVisualData && currentItem->_avatarItemVisualData->maskGuid && currentItem->_avatarItemVisualData->maskGuid->length > 0) {
                buf[wcstombs(buf, currentItem->_avatarItemVisualData->prefabGuid->chars, currentItem->_avatarItemVisualData->prefabGuid->length)] = 0;
                guidOutput << "M: " << buf << ' ';
            }

            //swatch guid
            if(currentItem && currentItem->_avatarItemVisualData && currentItem->_avatarItemVisualData->swatchGuid && currentItem->_avatarItemVisualData->swatchGuid->length > 0) {
                buf[wcstombs(buf, currentItem->_avatarItemVisualData->swatchGuid->chars, currentItem->_avatarItemVisualData->swatchGuid->length)] = 0;
                guidOutput << "S: " << buf << ' ';
            }

            //decal guid
            if(currentItem && currentItem->_avatarItemVisualData && currentItem->_avatarItemVisualData->decalGuid && currentItem->_avatarItemVisualData->decalGuid->length > 0) {
                buf[wcstombs(buf, currentItem->_avatarItemVisualData->decalGuid->chars, currentItem->_avatarItemVisualData->decalGuid->length)] = 0;
                guidOutput << "D: " << buf;
            }
            guidOutput << std::endl;
            currentItem += 1;
        }
        guidOutput.close();
    //});
}

bool __loading_sound = false;
void room_play_sound(const std::string &file_path) {
    if(__loading_sound) return;
    native_invoke::add([file_path]() {
        using namespace native;

        __loading_sound = true;

        float duration;
        auto clip = audio::clip_from_wav(file_path, duration);
        if(!clip) {
            __loading_sound = false;
            return;
        }

        auto test = PhotonNetwork::Instantiate(il2cpp_string_new(XS("[Player]")), {0,1.0f,0}, {0,0,0,1.0f}, 0, 0, nullptr);

        std::thread([test, clip, duration](){
            Sleep(5000);
            native_invoke::add([test, clip, duration](){
                auto recorder = (PhotonVoiceRecorder*)UnityEngine::GameObject::GetComponentInChildren(test, get_native_type_object(get_native_type(XS("Assembly-CSharp"), "", XS("PhotonVoiceRecorder"))), nullptr);

                if(!recorder) {
                    MessageBoxA(0, "", "", MB_OK);
                    return;
                }
                //auto recorder = Player::statics()->LocalPlayer->voiceRecorder;

                recorder->AudioClip = clip;
                recorder->loopAudioClip = false;
                recorder->Source = 1;

                PhotonVoiceRecorder::UpdateAudioSource(recorder, nullptr);
                PhotonVoiceRecorder::set_Transmit(recorder, true, nullptr);
                PhotonVoiceRecorder::set_DebugEchoMode(recorder, true, nullptr);
                __loading_sound = false;
            });
            Sleep(((uint32_t)duration * 1000) + 500);
            native_invoke::add([test](){
                PhotonNetwork::Destroy(test, nullptr);
            });
        }).detach();
    });
}



void lock_on(native::PhotonPlayer* player) {


	using namespace native;
	using namespace UnityEngine;
	auto player_statics = Player::statics();
	if (!player_statics->LocalPlayerExists || !player_exists(player)) return;
	
	auto rr_clowns = find_rr_player(player);

	auto from = Component::get_transform(player_statics->LocalPlayer->head, nullptr);


	

	if (!rr_clowns) return;
	auto feedback_type = get_native_type(XS("Assembly-CSharp"), "", XS("PlayerProgression/Feedback"));
	if (!feedback_type) {
		MessageBoxA(0, XS("no type found"), "", MB_OK);
		return;
	}
	native_invoke::add([rr_clowns,from, player]() {

		
		auto from_pos = Transform::get_position(from, nullptr);
		auto pv = Photon::MonoBehaviour::get_photonView(RecRoomSceneManager::get_Instance(nullptr, nullptr), nullptr);
		auto method_name = il2cpp_string_new(XS("RpcRespawnPlayer"));

		auto params = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Object")), 4);
		auto vector3_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Vector3"));
		auto quaternion_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
		auto bool_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Boolean"));

		UnityEngine::Vector3 raw_pos = from_pos;
		UnityEngine::Quaternion raw_rot = { 0, 0, 0, 1.0f };
		auto spawn_parent = Photon::MonoBehaviour::get_photonView(rr_clowns, nullptr);
		bool raw_dropTools = true;

		Il2CppArray::SetValue(params, il2cpp_value_box(vector3_type, &raw_pos), 0, nullptr);
		Il2CppArray::SetValue(params, il2cpp_value_box(quaternion_type, &raw_rot), 1, nullptr);
		Il2CppArray::SetValue(params, spawn_parent, 2, nullptr);
		Il2CppArray::SetValue(params, il2cpp_value_box(bool_type, &raw_dropTools), 3, nullptr);



		PhotonNetwork::RPC_player(
			pv,
			method_name,
			player,
			false,
			params,
			nullptr
		);



		});
}

void teleport_players(native::PhotonPlayer* player) {

	using namespace native;

	auto rr_daters = find_rr_player(player);

	if (!rr_daters) return;
	auto feedback_type = get_native_type(XS("Assembly-CSharp"), "", XS("PlayerProgression/Feedback"));
	if (!feedback_type) {
		MessageBoxA(0, XS("no type found"), "", MB_OK);
		return;
	}
	native_invoke::add([rr_daters, player]() {

		auto pv = Photon::MonoBehaviour::get_photonView(RecRoomSceneManager::get_Instance(nullptr, nullptr), nullptr);
		auto method_name = il2cpp_string_new(XS("RpcRespawnPlayer"));

		auto params = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Object")), 4);
		auto vector3_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Vector3"));
		auto quaternion_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
		auto bool_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Boolean"));

		UnityEngine::Vector3 raw_pos = { 0, 0, 0 };
		UnityEngine::Quaternion raw_rot = { 0, 0, 0, 1.0f };
		auto spawn_parent = Photon::MonoBehaviour::get_photonView(rr_daters, nullptr);
		bool raw_dropTools = true;

		Il2CppArray::SetValue(params, il2cpp_value_box(vector3_type, &raw_pos), 0, nullptr);
		Il2CppArray::SetValue(params, il2cpp_value_box(quaternion_type, &raw_rot), 1, nullptr);
		Il2CppArray::SetValue(params, spawn_parent, 2, nullptr);
		Il2CppArray::SetValue(params, il2cpp_value_box(bool_type, &raw_dropTools), 3, nullptr);



		PhotonNetwork::RPC_player(
			pv,
			method_name,
			player,
			false,
			params,
			nullptr
		);



		});
}

void exterminate_player(native::PhotonPlayer* player) {

    // room_play_sound(XS("C:\\Users\\Jenkins\\Downloads\\runner_complete.wav"));
    // return;

    using namespace native;

    auto rr_player = find_rr_player(player);

    if(!rr_player) return;

    // auto feedback_type = get_native_type(XS("Assembly-CSharp"), "", XS("PlayerProgression/Feedback"));
    // if(!feedback_type)  {
    //     MessageBoxA(0, XS("no type found"), "", MB_OK);
    //     return;
    // }
    native_invoke::add([rr_player, player]() {
        // auto pv = Photon::MonoBehaviour::get_photonView(RecRoomSceneManager::get_instance(nullptr, nullptr), nullptr);
        // auto method_name = il2cpp_string_new(XS("RpcRespawnPlayer"));

        // auto params = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Object")), 4);
        // auto vector3_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Vector3"));
        // auto quaternion_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
        // auto bool_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Boolean"));

        // UnityEngine::Vector3 raw_pos = {99999.0f, 99999.0f, 99999.0f};
        // UnityEngine::Quaternion raw_rot = {0, 0, 0, 1.0f};
        // auto spawn_parent = Photon::MonoBehaviour::get_photonView(rr_player, nullptr);
        // bool raw_dropTools = true;

        // Il2CppArray::SetValue(params, il2cpp_value_box(vector3_type, &raw_pos), 0, nullptr);  
        // Il2CppArray::SetValue(params, il2cpp_value_box(quaternion_type, &raw_rot), 1, nullptr);  
        // Il2CppArray::SetValue(params, spawn_parent, 2, nullptr);  
        // Il2CppArray::SetValue(params, il2cpp_value_box(bool_type, &raw_dropTools), 3, nullptr);

        // PhotonNetwork::RPC_player(
        //     pv,
        //     method_name,
        //     player,
        //     false,
        //     params,
        //     nullptr
        // );
        using namespace UnityEngine;

        auto head_trans = Component::get_transform(rr_player->head, nullptr);
        auto head_pos = Transform::get_position(head_trans, nullptr);
        head_pos.y += 1.0f;
        Quaternion gun_rot = {0, 0, 0.70711f, 0.70711f}; //straight down
        auto power_weapon = PhotonNetwork::Instantiate(il2cpp_string_new(XS("[Arena_RocketLauncher]")), head_pos, gun_rot, 0, 0, nullptr);
        auto ranged_weapon_type = get_native_type_object(get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Core.Combat"), XS("RangedWeapon")));

        auto pv = Photon::MonoBehaviour::get_photonView((Photon::MonoBehaviour*)GameObject::GetComponentInChildren(power_weapon, ranged_weapon_type, nullptr), nullptr);
        auto method_name = il2cpp_string_new(XS("RpcFireShot"));
        auto params = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Object")), 6);

        auto vector3_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Vector3"));
        auto quaternion_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
        auto float_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Single"));
        auto bool_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Boolean"));
        auto byte_type = il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Byte"));

        Vector3 velocity = {0, -1000.0f, 0}; 
        float charge = 1.0f;
        unsigned char shot_id = 0;

        Il2CppArray::SetValue(params, il2cpp_value_box(vector3_type, &head_pos), 0, nullptr);
        Il2CppArray::SetValue(params, il2cpp_value_box(quaternion_type, &gun_rot), 1, nullptr);
        Il2CppArray::SetValue(params, il2cpp_value_box(vector3_type, &velocity), 2, nullptr);
        Il2CppArray::SetValue(params, il2cpp_value_box(float_type, &charge), 3, nullptr);
        Il2CppArray::SetValue(params, il2cpp_object_new(bool_type), 4, nullptr);
        Il2CppArray::SetValue(params, il2cpp_object_new(byte_type), 5, nullptr);
        
        for(int i = 0; i < 7000; i++) {
            PhotonNetwork::RPC_player(
                pv,
                method_name,
                player,
                false,
                params,
                nullptr
            );
            shot_id++;
            Il2CppArray::SetValue(params, il2cpp_value_box(byte_type, &shot_id), 5, nullptr);
        }

        auto delayed_destroy = std::thread([power_weapon]() {
            Sleep(2000);
            native_invoke::add([power_weapon]() {
                PhotonNetwork::Destroy(power_weapon, nullptr);
            });
        });
        delayed_destroy.detach();
    }); 
}

void dump_tool_skins() {
    using namespace native;
    using namespace RecRoom::Core::Equipment;
    using namespace System::Collections::Generic;

    auto output = std::ofstream(XS("skins.txt"), std::ios::trunc);
    if(!output.is_open()) return;

    auto em = EquipmentManager::helper_get_instance();
    if(!em) return;
    auto wardrobe = em->equipmentWardrobe;
    if(!em) return;
    auto maps = wardrobe->toolSkinMaps;
    if(!maps) return;
    EquipmentSkinMappingData* current;
    int count = List::get_Count(maps, nullptr);

    SkinData* current_data;
    int count_data;

    while(--count >= 0) {
        current = (EquipmentSkinMappingData*)List::get_Item(maps, count, nullptr);
        if(!current) continue;
        
        count_data = List::get_Count(current->skins, nullptr);
        while(--count_data >= 0) {
            current_data = (SkinData*)List::get_Item(current->skins, count_data, nullptr);
            if(!current_data) continue;

            auto skin_name = WstrToUtf8Str(current_data->skinAssetName->chars + current->equipment->prefabName->length - 1);

            output << WstrToUtf8Str(current->equipment->prefabName->chars) << ' ' << skin_name << ' ' << WstrToUtf8Str(current_data->skinGuid->chars) << std::endl;
        }
    }
}

// void dump_hwid() {
//     using namespace native;

//     auto output = std::ofstream(XS("hwid.txt"), std::ios::trunc);
//     if(!output.is_open()) return;

//     auto result = reinterpret_cast<Il2CppString*(*)(const MethodInfo*)>(game_assembly_base + OFFSET_TEMP)(nullptr);

//     output << result->chars;
// }

void write_current_skin_override(native::PhotonPlayer* player, cheats::SkinnableTool tool, HANDLE ui_stdin_w) {
    DWORD written;
    auto player_iter = player_skin_map.find(player);
    if(player_iter != player_skin_map.end()) {
        auto skin_iter = player_iter->second.find(tool);
        if(skin_iter != player_iter->second.end()) {
            auto encoded = base64_encode((unsigned char*)skin_iter->second.c_str(), skin_iter->second.length());
            encoded += '\n';
            WriteFile(ui_stdin_w, encoded.c_str(), encoded.length(), &written, nullptr);
            return;
        }
    }
        
    WriteFile(ui_stdin_w, XS("bm9uZQ==\n"), 9, &written, nullptr); //base64 for "none". it's what the ui process puts as guid on the default item
}

void set_skin_to_apply(std::string prefab_name, std::string skin_guid, native::PhotonPlayer* for_player, bool enable) {
    auto key = cheats::internal_get_skin_tool(il2cpp_string_new(prefab_name.c_str())); //todo: lazy, could be faster

    if(key == cheats::SkinnableTool::Invalid) return;

    auto player_iter = player_skin_map.find(for_player);
    if(enable) {
        if(player_iter == player_skin_map.end()) {
            player_skin_map[for_player][key] = skin_guid; //create a new player entry
        }
        else {
            player_iter->second[key] = skin_guid; //set the requested skin in the already present player entry
        }
    }
    else if(player_iter != player_skin_map.end()) {
        auto skin_iter = player_iter->second.find(key);
        if(skin_iter != player_iter->second.end()) {
            player_iter->second.erase(skin_iter);
        }
    }
}
bool fast = false;
bool __state_rapidfire = false;
bool __state_infammo = false;
bool is_trial_item = false;
bool inf = false;
cheats::SkinnableTool cheats::internal_get_skin_tool(Il2CppString* prefab_name) {
    if(wcscmp(XWS(L"[RecRoyale_Backpack]"), prefab_name->chars) == 0) {
        return SkinnableTool::Backpack;
    }
    else if(wcscmp(XWS(L"[Basketball]"), prefab_name->chars) == 0) {
        return SkinnableTool::Basketball;
    }
    else if(wcscmp(XWS(L"[Crossbow]"), prefab_name->chars) == 0) {
        return SkinnableTool::Crossbow;
    }
    else if(wcscmp(XWS(L"[Crossbow_Hunter]"), prefab_name->chars) == 0) {
        return SkinnableTool::Crossbow_Hunter;
    }
    else if(wcscmp(XWS(L"[DiscGolfDisc]"), prefab_name->chars) == 0) {
        return SkinnableTool::DiscGolfDisc;
    }
    else if(wcscmp(XWS(L"[DodgeballBall]"), prefab_name->chars) == 0) {
        return SkinnableTool::DodgeballBall;
    }
    else if(wcscmp(XWS(L"[Grenade]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[Arena_Grenade]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[RecRoyale_Grenade]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[SciFi_Grenade]"), prefab_name->chars) == 0) {
        return SkinnableTool::Grenade;
    }
    else if(wcscmp(XWS(L"[Quest_SciFi_AutomaticGun]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[Arena_AutomaticGun]"), prefab_name->chars) == 0) {
        return SkinnableTool::LaserAutomaticGun;
    }
    else if(wcscmp(XWS(L"[Quest_SciFi_RailGun]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[Arena_RailGun]"), prefab_name->chars) == 0) {
        return SkinnableTool::LaserRailGun;
    }
    else if(wcscmp(XWS(L"[Quest_SciFi_Shotgun]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[Arena_Shotgun]"), prefab_name->chars) == 0) {
        return SkinnableTool::LaserShotgun;
    }
    else if(wcscmp(XWS(L"[Quest_SciFi_Pistol]"), prefab_name->chars) == 0 || wcscmp(XWS(L"[Arena_Pistol]"), prefab_name->chars) == 0) {
        return SkinnableTool::LaserPistol;
    }
    else if(wcscmp(XWS(L"[Longbow]"), prefab_name->chars) == 0) {
        return SkinnableTool::Longbow;
    }
    else if(wcscmp(XWS(L"[MakerPen]"), prefab_name->chars) == 0) {
        return SkinnableTool::MakerPen;
    }
    else if(wcscmp(XWS(L"[PaintballAssaultRifle]"), prefab_name->chars) == 0) {
        return SkinnableTool::PaintballAssaultRifle;
    }
    else if(wcscmp(XWS(L"[PaintballGrenadeLauncher]"), prefab_name->chars) == 0) {
        return SkinnableTool::PaintballGrenadeLauncher;
    }
    else if(wcscmp(XWS(L"[PaintballGun]"), prefab_name->chars) == 0) {
        return SkinnableTool::PaintballGun;
    }
    else if(wcscmp(XWS(L"[PaintballShield]"), prefab_name->chars) == 0) {
        return SkinnableTool::PaintballShield;
    }
    else if(wcscmp(XWS(L"[PaintballShotgun]"), prefab_name->chars) == 0) {
        return SkinnableTool::PaintballShotgun;
    }
    else if(wcscmp(XWS(L"[PaintballRifleScoped]"), prefab_name->chars) == 0) {
        return SkinnableTool::PaintballRifleScoped;
    }
    else if(wcscmp(XWS(L"[QuestSword]"), prefab_name->chars) == 0) {
        return SkinnableTool::Sword;
    }
    else if(wcscmp(XWS(L"[QuestShield]"), prefab_name->chars) == 0) {
        return SkinnableTool::QuestShield;
    }
    else if(wcscmp(XWS(L"[SoccerShield]"), prefab_name->chars) == 0) {
        return SkinnableTool::SoccerShield;
    }
    else if(wcscmp(XWS(L"[Quest_Goblin_Wand]"), prefab_name->chars) == 0) {
        return SkinnableTool::Wand;
    }
    else if(wcscmp(XWS(L"[ShareCamera]"), prefab_name->chars) == 0) {
        return SkinnableTool::ShareCamera;
    }
    else {
        return SkinnableTool::Invalid;
    }
}

bool cheats::internal_try_get_skin_guid(SkinnableTool tool, native::PhotonPlayer* for_player, char* buf) {
    if(tool == SkinnableTool::Invalid) return false;

    auto player_iter = player_skin_map.find(for_player);
    if(player_iter != player_skin_map.end()) {
        if(!player_exists(for_player)) {
            player_skin_map.erase(player_iter);
            return false;
        }
        auto skin_iter = player_iter->second.find(tool);
        if(skin_iter != player_iter->second.end()) {
            strcpy(buf, skin_iter->second.c_str());
            return true;
        }
    }
    return false;
}

bool cheats::internal_persistent_flight_enabled() {
    return __state_persistent_flight;
}

void cheats::internal_handle_room_join() {
    using namespace native;
    using namespace RecRoom::Systems::PlayerRoles;

    role_backups.clear();
    perm_backups.clear();

    auto backup_role_routine = [](const MethodInfo* mi, int manager_kind, int role_id) {
        using namespace overridable_fields;
        using namespace RecRoom::Utils::OverridableFields;


        if(manager_kind == 0) {
            auto perms = RoleBackup();
            auto get_def = reinterpret_cast<RoomRole*(*)(PlayerRoomRoleManager*, int, const MethodInfo*)>(mi->methodPointer);
            auto room_role = get_def(PlayerRoomRoleManager::get_Instance(nullptr, nullptr), role_id, mi);

            if(!room_role) return;

            //perms.emplace(3, get_bytes(MutableOverridableVoteKickType::SerializeToProtobuf(room_role->voteKickPermission, nullptr)));
            perms.emplace(4, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canInvite, nullptr)));
            perms.emplace(5, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canEditCircuits, nullptr)));
            perms.emplace(6, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canTalk, nullptr)));
            perms.emplace(7, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canPrintPhotos, nullptr)));
            perms.emplace(8, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canStartGames, nullptr)));
            perms.emplace(9, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canSelfRevive, nullptr)));
            perms.emplace(12, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canChangeGameMode, nullptr)));
            perms.emplace(13, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canUseMakerPen, nullptr)));
            perms.emplace(14, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canUseDeleteAllButton, nullptr)));
            perms.emplace(15, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canSaveInventions, nullptr)));
            perms.emplace(16, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->disableMicAutoMute, nullptr)));
            perms.emplace(17, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canEndGamesEarly, nullptr)));
            perms.emplace(18, get_bytes(MutableOverridableBool::SerializeToProtobuf(room_role->canUseShareCam, nullptr)));

            perm_backups.emplace(role_id, perms);
        }
        else if(manager_kind == 1) {
            auto roles = RoleBackup();
            auto get_def = reinterpret_cast<GameRole*(*)(PlayerGameRoleManager*, int, const MethodInfo*)>(mi->methodPointer);
            auto game_role = get_def(PlayerGameRoleManager::get_Instance(nullptr, nullptr), role_id, mi);

            if(!game_role) return;

            roles.emplace(2, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->teleportDelay, nullptr)));
            roles.emplace(4, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canMove, nullptr)));
            roles.emplace(8, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canFly, nullptr)));
            roles.emplace(9, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->isInvincible, nullptr)));
            roles.emplace(13, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canSwitchTeams, nullptr)));
            roles.emplace(14, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->maxTeleportDistance, nullptr)));
            roles.emplace(15, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->maxTeleportDropHeight, nullptr)));
            roles.emplace(17, get_bytes(MutableOverridableInt::SerializeToProtobuf(game_role->voiceRolloffMaxDistance, nullptr)));
            roles.emplace(20, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->hidesName, nullptr)));
            roles.emplace(21, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->vrWalkSpeed, nullptr)));
            roles.emplace(22, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanSprint, nullptr)));
            roles.emplace(24, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->screensWalkSpeed, nullptr)));
            roles.emplace(25, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->screensCanSprint, nullptr)));
            roles.emplace(27, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanDualWield, nullptr)));
            roles.emplace(28, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanJump, nullptr)));
            roles.emplace(29, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanDodge, nullptr)));
            roles.emplace(30, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->screensCanJump, nullptr)));
            roles.emplace(31, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->screensCanDodge, nullptr)));
            roles.emplace(32, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->maxTeleportAngle, nullptr)));
            roles.emplace(42, get_bytes(MutableOverridableInt::SerializeToProtobuf(game_role->screensDefaultThirdPersonControlType, nullptr)));
            roles.emplace(43, get_bytes(MutableOverridableInt::SerializeToProtobuf(game_role->screensDefaultFirstPersonControlType, nullptr)));
            roles.emplace(44, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->vrMaxJumpHeight, nullptr)));
            roles.emplace(45, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->screensMaxJumpHeight, nullptr)));
            roles.emplace(48, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->screensCanWallRun, nullptr)));
            roles.emplace(49, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanWallRun, nullptr)));
            roles.emplace(50, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanGetPushed, nullptr)));
            roles.emplace(51, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->screensCanGetPushed, nullptr)));
            roles.emplace(52, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->vrCanRam, nullptr)));
            roles.emplace(53, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->screensCanRam, nullptr)));
            roles.emplace(55, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->vrSprintMaxSpeedWalkSpeedMultiplier, nullptr)));
            roles.emplace(56, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->screensSprintMaxSpeedWalkSpeedMultiplier, nullptr)));
            roles.emplace(57, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canAutoSprint, nullptr)));
            roles.emplace(58, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->airControlParameter, nullptr)));
            roles.emplace(59, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canJumpCutoff, nullptr)));
            roles.emplace(60, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canClamber, nullptr)));
            roles.emplace(61, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->wallRunDuration, nullptr)));
            roles.emplace(62, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->wallRunDropRateParameter, nullptr)));
            roles.emplace(63, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->wallRunStartSpeedBoost, nullptr)));
            roles.emplace(64, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->wallRunJumpHeight, nullptr)));
            roles.emplace(65, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->wallRunJumpDirectionParameter, nullptr)));
            roles.emplace(66, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->wallRunJumpAirControlMultiplier, nullptr)));
            roles.emplace(67, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canSlide, nullptr)));
            roles.emplace(68, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canSlideDownSlopes, nullptr)));
            roles.emplace(69, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->slideMinDuration, nullptr)));
            roles.emplace(70, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->slideSteeringParameter, nullptr)));
            roles.emplace(71, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->slideStartSpeedBoost, nullptr)));
            roles.emplace(72, get_bytes(MutableOverridableFloat::SerializeToProtobuf(game_role->slideAirControlMultiplier, nullptr)));
            roles.emplace(73, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canWallClimb, nullptr)));
            roles.emplace(74, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canWallClimbWithoutMarkup, nullptr)));
            roles.emplace(75, get_bytes(MutableOverridableBool::SerializeToProtobuf(game_role->canWallRunWithoutMarkup, nullptr)));

            role_backups.emplace(role_id, roles);
        }
    };

    auto t_cur_role_mgr = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("PlayerRoomRoleManager"));
    auto get_role_def_info = il2cpp_class_get_method_from_name(t_cur_role_mgr, XS("GetRoleDefinition"), 1);

    backup_role_routine(get_role_def_info, 0, 0);
    for(int i = 1; i < 32; i++) backup_role_routine(get_role_def_info, 0, 1 << i);

    t_cur_role_mgr = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("PlayerGameRoleManager"));
    get_role_def_info = il2cpp_class_get_method_from_name(t_cur_role_mgr, XS("GetRoleDefinition"), 1); 

    backup_role_routine(get_role_def_info, 1, 0);
    for(int i = 1; i < 32; i++) backup_role_routine(get_role_def_info, 1, 1 << i);

    backup_initialized = true;
}

void cheats::internal_handle_room_leave() {
    rapid_fire_weapons.clear();
    if(!backup_initialized) return;

    using namespace native;
    using namespace RecRoom::Systems::PlayerRoles;

    auto room_role_mgr = PlayerRoomRoleManager::get_Instance(nullptr, nullptr);
    auto game_role_mgr = PlayerGameRoleManager::get_Instance(nullptr, nullptr);

    for(auto room_role = perm_backups.begin(); room_role != perm_backups.end(); room_role++) {
        for(auto perm = room_role->second.begin(); perm != room_role->second.end(); perm++) {
            set_perm(0, room_role->first, perm->first, false, perm->second);
        }
    }
}

void cheats::internal_handle_fire(native::RecRoom::Core::Combat::RangedWeapon* __this, float charge) {
    auto iter = rapid_fire_weapons.find(__this);
    if(__state_rapidfire && iter == rapid_fire_weapons.end()) {
        iter = rapid_fire_weapons.emplace(__this, false).first;
    }
    else if(!__state_rapidfire) {
        rapid_fire_weapons.clear();
        return;
    }

    if(!iter->second) {
        native_invoke::add_recurring([__this, charge]() {
            auto iter = rapid_fire_weapons.find(__this);
            if(iter == rapid_fire_weapons.end()) {
                return false;
            }
            if(!__this->_tool || !native::RecRoom::Core::Combat::Weapon::get_HasEnoughMagazineAmmunition(__this, nullptr)) {
                rapid_fire_weapons.erase(iter); //will only be called after this function so it's fine
                return false;
            }

            auto velocity = native::UnityEngine::Transform::get_forward(__this->barrelTransform, nullptr);
            velocity.x *= __this->bulletFireSpeed;
            velocity.y *= __this->bulletFireSpeed;
            velocity.z *= __this->bulletFireSpeed;

            native::RecRoom::Core::Combat::RangedWeapon::Fire(__this, velocity, charge - 0.001f, nullptr);
            return true;
        });

        iter->second = true;
    }
    else if(charge == 1.0f) {
        rapid_fire_weapons.erase(iter);
    }
}

void cheats::init() {
    perm_backups = std::map<int, RoleBackup>();
    role_backups = std::map<int, RoleBackup>();

    player_skin_map = std::map<native::PhotonPlayer*, SkinMap>();
    MH_EnableHook(native::ToolSkinMapper::ThisTool_PostPickupEvent);

    rapid_fire_weapons = std::map<native::RecRoom::Core::Combat::RangedWeapon*, bool>();
    MH_EnableHook(native::RecRoom::Core::Combat::RangedWeapon::Fire);

    //dumpOutfits();
	//auto statics = native::Player::statics();
    //if(statics && statics->LocalPlayerExists) {
        //internal_handle_room_join();
    //}
    //MH_EnableHook(native::SessionManager::JoinRoom);
}





void cheats::handle_command(char* command_buf, HANDLE ui_stdin_w) {
    std::string command;
    std::vector<std::string> args;
    int last_whitespace = 0;
    bool has_args = false;

    int command_size = strlen(command_buf);

    //split the string by single spaces
    for(int i = 0; i < command_size; i++) {
        if(command_buf[i] == ' ') {
            has_args = true;
            command_buf[i] = 0;
            if(last_whitespace == 0) {
                command = command_buf;
            }
            else {
                args.push_back(base64_decode(command_buf + last_whitespace + 1));
            }
            last_whitespace = i;
        }
    }
    if(has_args) {
        args.push_back(base64_decode(command_buf + last_whitespace + 1)); //there is no whitespace at the end, so push the last arg 'manually'
    }
    else {
        command = command_buf; //no arguments so the command consists of the enire buffer
    }

    if(command == XS("0")) { //enable local godmode
        write_bool(enable_godmode_local(args[0] == XS("true")), ui_stdin_w);
    }
    else if(command == XS("1")) { //get state of local godmode
        write_bool(state_godmode_local(), ui_stdin_w);
    }
    else if(command == XS("2")) { //check if local godmode can be enabled
        write_bool(usable_godmode_local(), ui_stdin_w);
    }
    else if(command == XS("3")) { //spawn a polaroid with a local img on it
        spawn_img(args[0]);
    }
    else if(command == XS("4")) { //change flying enabled
        enable_flight(args[0] == XS("true"));
    }
    else if(command == XS("5")) { //check if flying is enabled
        write_bool(flight_enabled(), ui_stdin_w);
    }
    else if(command == XS("6")) { //enable always-on flight
        write_bool(enable_persistent_flight(args[0] == "true"), ui_stdin_w);
    }
    else if(command == XS("7")) { //check if always-on flight is enabled
        write_bool(__state_persistent_flight, ui_stdin_w);
    }
    else if(command == XS("8")) { //make local player invisible
        write_bool(local_player_invisible(args[0] == "true"), ui_stdin_w);
    }
    else if(command == XS("9")) { //check if local player is invisible
        write_bool(get_local_player_invisible(), ui_stdin_w);
    }
    else if(command == XS("A")) { //get a list of players
        write_current_players(ui_stdin_w);
    }
    else if(command == XS("B")) { //check if a player exists
        write_bool(player_exists(*(native::PhotonPlayer**)args[0].data()), ui_stdin_w);
    }
    else if(command == XS("C")) { //teleport to a player
        teleport_to_player(*(native::PhotonPlayer**)args[0].data());
    }






    else if(command == XS("D")) { //spawn token box
        auto text = args[0];
        auto amount = *(int*)args[1].data();
        native_invoke::add([text, amount](){
            using namespace native;

            auto gm = GiftManager::helper_get_instance();
            if(GiftManager::get_IsAGiftBoxCurrentlySpawned(gm, nullptr)) return;

            auto gift = RecNet::GiftPackage::helper_create_tokens(gm, text.c_str(), amount);
            if(!gift) return;

            MH_EnableHook(RecNet::Avatars::LocalConsumeGiftPackage); //will be disabled again by the hook code, necessary to not trigger anything in the servers for this gp
            GiftManager::OnGiftPackageReceivedEvent(gm, gift, true, nullptr);
        });
    }
    else if(command == XS("E")) { //set a permission or role, either for the user or everyone in the room
        using namespace native;
        using namespace RecRoom::Systems::PlayerRoles;

        auto is_local = *(bool*)args[0].data();
        int manager_kind = *(int*)args[1].data();
        int perm_id = *(int*)args[2].data();
        auto perm_bytes = std::vector<unsigned char>(args[3].begin(), args[3].end());
        native_invoke::add([is_local, manager_kind, perm_id, perm_bytes]() {
            if(!Player::statics()->LocalPlayerExists) return;
            set_perm(manager_kind, 0, perm_id, is_local, perm_bytes);
            for(int i = 1; i < 32; i++) set_perm(manager_kind, 1 << i, perm_id, is_local, perm_bytes);
        });
        write_bool(Player::statics()->LocalPlayerExists, ui_stdin_w);
    }
    else if(command == XS("F")) { //spawn object for player
        using namespace native;
        auto player = find_rr_player(*(PhotonPlayer**)args[0].data());
        if(player) {
            auto prefab_name = il2cpp_string_new(args[1].c_str());
            native_invoke::add([player, prefab_name]() {
                auto target_transform = UnityEngine::Component::get_transform(player->head, nullptr);
                auto pos = UnityEngine::Transform::get_position(target_transform, nullptr);
                auto fwd = UnityEngine::Transform::get_forward(target_transform, nullptr);

                pos.x += fwd.x;
                pos.y += fwd.y;
                pos.z += fwd.z;

                auto rot = UnityEngine::Transform::get_rotation(target_transform, nullptr);
				/*
                auto quaterion_type = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
                auto rot_obj = il2cpp_value_box(quaterion_type, &rot);
                auto rot_euler = UnityEngine::Quaternion::get_eulerAngles(rot_obj, nullptr);
                rot_euler.y -= 90.0f;
                UnityEngine::Quaternion::set_eulerAngles(rot_obj, rot_euler, nullptr);

                rot = *(UnityEngine::Quaternion*)il2cpp_object_unbox(rot_obj);
				*/

                PhotonNetwork::Instantiate(prefab_name, pos, rot, 1.0f, 0, nullptr);
            });
        }
    }
    else if(command == XS("G")) { //get value of perm
        int manager_kind = *(int*)args[0].data();
        int role_id = 0;
        int perm_id = *(int*)args[1].data();
        bool is_local = *(bool*)args[2].data();
        write_get_perm(manager_kind, role_id, perm_id, is_local, ui_stdin_w);
	}
	else if (command == XS("H")) { //set should apply skin
	    set_skin_to_apply(args[0], args[1], *(native::PhotonPlayer**)args[3].data(), *(bool*)args[2].data());
	}
	else if (command == XS("I")) { //get currently overriden skin
	    write_current_skin_override(*(native::PhotonPlayer**)args[1].data(), internal_get_skin_tool(il2cpp_string_new(args[0].c_str())), ui_stdin_w);
	}
	else if (command == XS("J")) { //exterminate player
	    exterminate_player(*(native::PhotonPlayer**)args[0].data());
	}
	else if (command == XS("K")) { //toggle rapid fire
	    if (__state_rapidfire) {
		    __state_rapidfire = MH_DisableHook(native::RecRoom::Core::Combat::RangedWeapon::get_IsOnCooldown) != MH_OK;
	    }
	    else {
		    __state_rapidfire = MH_EnableHook(native::RecRoom::Core::Combat::RangedWeapon::get_IsOnCooldown) == MH_OK;
	    }
	    write_bool(__state_rapidfire, ui_stdin_w);
	}
	// else if (command == XS("(")) {
	//     if (inf) {
	// 	    inf = MH_DisableHook(native::CreationManager::get_IsOverInkLimit) != MH_OK;
	//     }
    //     else {
	// 	    inf = MH_EnableHook(native::CreationManager::get_IsOverInkLimit) != MH_OK;
	//     }
	//     write_bool(inf, ui_stdin_w);
	// }
	// else if (command == XS(")")) {
	//     write_bool(inf, ui_stdin_w);
	// }
	else if (command == XS("L")) { //toggle inf ammo
	    if (__state_infammo) {
		    __state_infammo = MH_DisableHook(native::RecRoom::Core::Combat::Weapon::SetAmmunition) != MH_OK;
	    }
	    else {
		    __state_infammo = MH_EnableHook(native::RecRoom::Core::Combat::Weapon::SetAmmunition) == MH_OK;
	    }
	    write_bool(__state_infammo, ui_stdin_w);
	}
	else if (command == XS("M")) { //get rapid fire state
	    write_bool(__state_rapidfire, ui_stdin_w);
	}
	else if (command == XS("N")) { //get inf ammo state
	    write_bool(__state_infammo, ui_stdin_w);
	}
	// else if (command == XS("O")) { //play sound in this room
	//     room_play_sound(args[0]);
	// }
	else if (command == XS("P")) { //Teleports players to orgin
	    teleport_players(*(native::PhotonPlayer**)args[0].data());
	}
	else if (command == XS("Q")) { //lock
	    lock_on(*(native::PhotonPlayer**)args[0].data());
	}
	else if (command == XS("R")) { //change flying enabled
	    player_lockon(args[0] == XS("true"));
	}
	else if (command == XS("S")) { //WIP
	    write_bool(get_locked_on(), ui_stdin_w);
	}
    else if(command == XS("T")) { //test command, could be anything
        equipOutfitItem(testIndex++);
    }
	else if (command == XS("U")) { 
	    swap_players(*(native::PhotonPlayer**)args[0].data());
	}
    else if (command == XS("V")) { //instant finish quest
		if (fast) {
			fast = MH_DisableHook(native::RecRoom::Activities::Quest::QuestManager::GetNextGameplayRoomIndex) != MH_OK;
		}	
		else {
			fast = MH_EnableHook(native::RecRoom::Activities::Quest::QuestManager::GetNextGameplayRoomIndex) == MH_OK;
		}
		write_bool(fast, ui_stdin_w);
	}
    else if (command == XS("W")) {

    }
	else if (command == XS("@")) {
	    gifttest();
	}
	else if (command == XS("!")) {
	    score();
	}
	else if (command == XS("#")) {
	    killall();
	}
	else if (command == ("*")){
		write_bool(is_trial_item, ui_stdin_w);// Failed attemt at outfit hack. The session is smarter than I thought. Must go deeper!!!
	}
	else if (command == XS("/")) {
		write_bool(fast, ui_stdin_w);
	}
	else if (command == XS("~")) { //lockon2
		player_lockon2(*(native::PhotonPlayer**)args[0].data());
	}
}