#include <Windows.h>
#include <Xorstr.hpp>
#include <native.hpp>
#include <hooking.hpp>
#include <offsets.h>
#include <MinHook.h>
#include <native_invoke.hpp>
#include <cheats.hpp>
#include <string>
#include <fstream>

using namespace native;
GameCombatManager::_tMasterDamagePlayer oMasterDamagePlayer;
void hMasterDamagePlayer(GameCombatManager* __this, PhotonPlayer* player, int damage, bool ignoreShield, int* damageResult,int* shieldEffect,const MethodInfo* method_info) {
    if(player != PhotonNetwork::get_player(nullptr)) {
        oMasterDamagePlayer(__this, player, damage, ignoreShield, damageResult, shieldEffect, method_info);
        return;
    }

    *damageResult = 1;
    *shieldEffect = 0;
}

using namespace native;
GameScoreManager::_tMasterSetScore oMasterSetScore;
void hMasterSetScore(GameScoreManager* __this, const int team, int score, const MethodInfo* method_info) {
	oMasterSetScore(__this, team, 9999999, method_info);
}
using namespace native;


bool hIsWearingAnyTrialItems(void*, void*) {

	return false;
}



RecRoom::Core::Encounters::BaseSpawnEnemyEncounterStep::_tMasterKillAllLivingEnemies oMasterKillAllLivingEnenmies;
void hMasterKillAllLivingEnenmies(RecRoom::Core::Encounters::BaseSpawnEnemyEncounterStep* __this, const MethodInfo* method_info) {



	oMasterKillAllLivingEnenmies(__this, method_info);

	__this->livingEnemies - 100;

	
}

RecRoom::Core::AI::EnemyManager::_tget_AllEnemies oget_AllEnemies;

void get_AllEnemies(RecRoom::Core::AI::EnemyManager*__this,MethodInfo*mi){}
RecRoom::Core::AI::Enemy::_tMasterKill oMasterKill;


/*
void hMasterKill(RecRoom::Core::AI::Enemy* __this, System::Collections::Generic::List* Enemy, const MethodInfo* mi) {

	using namespace native;
	using namespace System::Collections::Generic;
	using namespace RecRoom::Core::AI;


	


	auto list = EnemyManager::get_AllEnemies(nullptr);
	auto len = List::get_Count(list,nullptr);

	
	RecRoom::Core::AI::Enemy* current;
	
	for(int i = 0; i < len; i++){

		current = (RecRoom::Core::AI::Enemy*)List::get_Item(list, i, nullptr);

		oMasterKill(__this, Enemy, mi);
	}
	


}//Work in progress-itterate through list of enemies and master kill

*/

CodeStage::AntiCheat::Detectors::RRCheatDetector::_tUpdate oUpdate;
void hUpdate(CodeStage::AntiCheat::Detectors::RRCheatDetector* __this, const MethodInfo* method_info) {
    native_invoke::run();
    oUpdate(__this, method_info);
}

void dead() {}

RecRoom::Core::Locomotion::PlayerMovement::_tAddFlyEnabled oAddFlyEnabled;
void hAddFlyEnabled(RecRoom::Core::Locomotion::PlayerMovement* __this, bool enable, Il2CppObject* token, int priority, const MethodInfo* method_info) {
    oAddFlyEnabled(__this, enable ? enable : cheats::internal_persistent_flight_enabled(), token, priority, method_info);
}

using namespace native;
using namespace RecRoom::Activities::Quest;
RecRoom::Activities::Quest::QuestManager::_tGetNextGameplayRoomIndex oGetNextGameplayRoomIndex;
int hGetNextGameplayRoomIndex(native::RecRoom::Activities::Quest::QuestManager* __this, const MethodInfo* method_info) {

	using namespace native;
	using namespace RecRoom::Activities::Quest;

	auto org = oGetNextGameplayRoomIndex(__this, nullptr);
	if (org < __this->totalRoomCount) {
		return __this->totalRoomCount - 1;
	}
	else {
		return org;
	}
}

Il2CppObject* hLocalConsumeGiftPackage(RecNet::GiftPackage* gift, Il2CppObject* callback, const MethodInfo* method_info) {
    MH_DisableHook(RecNet::Avatars::LocalConsumeGiftPackage);
    return nullptr;
}

SessionManager::_tLocalPlayerSpawned oLocalPlayerSpawned;
void hLocalPlayerSpawned(SessionManager* __this, const MethodInfo* method_info) {
    oLocalPlayerSpawned(__this, method_info);

    cheats::internal_handle_room_join();
    MH_DisableHook(SessionManager::LocalPlayerSpawned);
}

SessionManager::_tJoinRoom oJoinRoom;
Il2CppObject* hJoinRoom(SessionManager* __this, Il2CppString* roomName, Il2CppString* roomSceneName, bool isPrivate, int inviteMode, bool bypassMovementModeRestriction, const MethodInfo* method_info) {
    cheats::internal_handle_room_leave();

    MH_EnableHook(SessionManager::LocalPlayerSpawned);

    return oJoinRoom(__this, roomName, roomSceneName, isPrivate, inviteMode, bypassMovementModeRestriction, method_info);
}

ToolSkinMapper::_tThisTool_PostPickupEvent oThisTool_PostPickupEvent;
void hThisTool_PostPickupEvent(ToolSkinMapper* __this, Tool* thisTool, const MethodInfo* mi) {
    using namespace cheats;

    oThisTool_PostPickupEvent(__this, thisTool, mi);

    auto tool = internal_get_skin_tool(__this->PrefabName);

    if(tool == SkinnableTool::Invalid) return;

    char guid_buf[64];
    if(internal_try_get_skin_guid(tool, Tool::get_authority(thisTool, nullptr), guid_buf)) {
        ToolSkinMapper::ApplySkin(__this, il2cpp_string_new(guid_buf), false, nullptr);
    }
}

PlayerAvatar::_tEquipOutfitItemInternal oEquipOutfitItemInternal;
bool hEquipOutfitItemInternal(PlayerAvatar* __this, RecRoom::Avatar::Data::Runtime::AvatarItemSelection itemSelection, const MethodInfo* mi) {
    using namespace native;
    using namespace RecRoom::Avatar::Data::Runtime;
    using namespace RecRoom::Avatar::Data::Shared;
    using namespace System::Collections::Generic;

    

    return oEquipOutfitItemInternal(__this, itemSelection, mi);
}


PlayerLocomotionController::_tAddTeleportCooldown oAddTeleportCooldown;
void hPlayerLocomotionController(PlayerLocomotionController* __this, float cooldown, Il2CppObject* token, int priority, MethodInfo*mi) {

	oAddTeleportCooldown(__this, 0.1, token, priority, mi);
}


RecRoom::Core::Encounters::SpawnEnemyEncounterStep::_tset_EnemySpawnCount oset_EnemySpawnCount;
void hset_EnemySpawnCount(RecRoom::Core::Encounters::SpawnEnemyEncounterStep* __this, int value, const MethodInfo* mi) {

	oset_EnemySpawnCount(__this, 0, mi);

}




bool is_recurring = false;
bool disable = false;
RecRoom::Core::Combat::RangedWeapon::_tFire oFire;
void hFire(RecRoom::Core::Combat::RangedWeapon* __this, UnityEngine::Vector3 velocity, float charge, const MethodInfo* mi) {
    cheats::internal_handle_fire(__this, charge);

    oFire(__this, velocity, charge, mi);
}

RecRoom::Core::Combat::Weapon::_tSetAmmunition oSetAmmunition;
void hSetAmmunition(RecRoom::Core::Combat::Weapon* __this, int mag, int reserve, const MethodInfo* mi) {
    oSetAmmunition(__this, 1, 1, mi);
}


GiftManager::_tGenerateGameGift oGenerateGameGift;
void hGenerateGameGift(GiftManager* __this, const int item_context, const MethodInfo* mi) {

	oGenerateGameGift(__this, 4503, mi);
}

	
bool hget_IsOverInkLimit(void*, void*) {

	return false;
}


bool hget_IsOnCooldown(void*, void*) {
    return false;
}

bool init2() {
    int count = 0;
    //Polaroid
    auto currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("Polaroid"));
    void* iter = nullptr;
    auto currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("AuthoritySetImageName")) == 0) {
            Polaroid::AuthoritySetImageName = reinterpret_cast<Polaroid::_tAuthoritySetImageName>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("<AuthoritySetImageName>b__35_0")) == 0) {
            Polaroid::AuthoritySetImageNameStr = reinterpret_cast<Polaroid::_tAuthoritySetImageNameStr>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //PlayerMovement
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Core.Locomotion"), XS("PlayerMovement"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("AddFlyEnabled")) == 0) {
            RecRoom::Core::Locomotion::PlayerMovement::AddFlyEnabled = reinterpret_cast<RecRoom::Core::Locomotion::PlayerMovement::_tAddFlyEnabled>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_IsFlyingEnabled")) == 0) {
            RecRoom::Core::Locomotion::PlayerMovement::get_IsFlyingEnabled = reinterpret_cast<RecRoom::Core::Locomotion::PlayerMovement::_tget_IsFlyingEnabled>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;
    
    //RangedWeapon
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Core.Combat"), XS("RangedWeapon"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("Fire")) == 0 && currentMethod->parameters_count == 2) {
            RecRoom::Core::Combat::RangedWeapon::Fire = reinterpret_cast<RecRoom::Core::Combat::RangedWeapon::_tFire>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_IsOnCooldown")) == 0) {
            RecRoom::Core::Combat::RangedWeapon::get_IsOnCooldown = reinterpret_cast<RecRoom::Core::Combat::RangedWeapon::_tget_IsOnCooldown>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //Weapon
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Core.Combat"), XS("Weapon"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("SetAmmunition")) == 0) {
            RecRoom::Core::Combat::Weapon::SetAmmunition = reinterpret_cast<RecRoom::Core::Combat::Weapon::_tSetAmmunition>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_HasEnoughMagazineAmmunition")) == 0) {
            RecRoom::Core::Combat::Weapon::get_HasEnoughMagazineAmmunition = reinterpret_cast<RecRoom::Core::Combat::Weapon::_tget_HasEnoughMagazineAmmunition>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //PlayerGameRoleManager
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("PlayerGameRoleManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_Instance")) == 0) {
            RecRoom::Systems::PlayerRoles::PlayerGameRoleManager::get_Instance = reinterpret_cast<RecRoom::Systems::PlayerRoles::PlayerGameRoleManager::_tget_Instance>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;
    
    //PlayerRoomRoleManager
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Systems.PlayerRoles"), XS("PlayerRoomRoleManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_Instance")) == 0) {
            RecRoom::Systems::PlayerRoles::PlayerRoomRoleManager::get_Instance = reinterpret_cast<RecRoom::Systems::PlayerRoles::PlayerRoomRoleManager::_tget_Instance>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //GameScoreManager
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("GameScoreManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("MasterSetScore")) == 0) {
            GameScoreManager::MasterSetScore = reinterpret_cast<GameScoreManager::_tMasterSetScore>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //Array
    currentClass = get_native_type(XS("mscorlib"), XS("System"), XS("Array"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("GetValue")) == 0 && currentMethod->parameters_count == 1 && strcmp(il2cpp_class_from_type(currentMethod->parameters[0].parameter_type)->name, XS("Int32")) == 0) {
            Il2CppArray::GetValue = reinterpret_cast<Il2CppArray::_tGetValue>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("SetValue")) == 0 && currentMethod->parameters_count == 2 && strcmp(il2cpp_class_from_type(currentMethod->parameters[1].parameter_type)->name, XS("Int32")) == 0) {
            Il2CppArray::SetValue = reinterpret_cast<Il2CppArray::_tSetValue>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //Player
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("Player"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_PhotonPlayer")) == 0) {
            Player::get_PhotonPlayer = reinterpret_cast<Player::_tget_PhotonPlayer>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_BothHands")) == 0) {
            //List
            int count2 = 0;
            auto listClass = il2cpp_class_from_type(currentMethod->return_type);
            void* iter2 = nullptr;
            auto currentMethod2 = il2cpp_class_get_methods(listClass, &iter2);

            for(; currentMethod2; currentMethod2 = il2cpp_class_get_methods(listClass, &iter2)) {
                if(strcmp(currentMethod2->name, XS("get_Count")) == 0) {
                    System::Collections::Generic::List::get_Count = reinterpret_cast<System::Collections::Generic::List::_tget_Count>(currentMethod2->methodPointer);
                    count2++;
                }
                else if(strcmp(currentMethod2->name, XS("get_Item")) == 0) {
                    System::Collections::Generic::List::get_Item = reinterpret_cast<System::Collections::Generic::List::_tget_Item>(currentMethod2->methodPointer);
                    count2++;
                }
            }
            if(count2 != 2 || !System::Collections::Generic::List::get_Item) return false;
        }
    }
    if(count != 1) return false;
    count = 0;

    //SessionManager
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("SessionManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("LocalPlayerSpawned")) == 0) {
            SessionManager::LocalPlayerSpawned = reinterpret_cast<SessionManager::_tLocalPlayerSpawned>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("JoinRoom")) == 0) {
            SessionManager::JoinRoom = reinterpret_cast<SessionManager::_tJoinRoom>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("IsWearingTrialItems")) == 0) {
            SessionManager::IsWearingTrialItems = reinterpret_cast<SessionManager::_tIsWearingTrialItems>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 3) return false;
    count = 0;

    //GiftManager
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("GiftManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_IsAGiftBoxCurrentlySpawned")) == 0) {
            GiftManager::get_IsAGiftBoxCurrentlySpawned = reinterpret_cast<GiftManager::_tget_IsAGiftBoxCurrentlySpawned>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_DefaultGiftBoxMaterial")) == 0) {
            GiftManager::get_DefaultGiftBoxMaterial = reinterpret_cast<GiftManager::_tget_DefaultGiftBoxMaterial>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("OnGiftPackageReceivedEvent")) == 0) {
            GiftManager::OnGiftPackageReceivedEvent = reinterpret_cast<GiftManager::_tOnGiftPackageReceivedEvent>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("GetPackageVariantByContext")) == 0 && currentMethod->parameters_count == 1) {
            GiftManager::GetPackageVariantByContext = reinterpret_cast<GiftManager::_tGetPackageVariantByContext>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("GenerateGameGift")) == 0) {
            GiftManager::GenerateGameGift = reinterpret_cast<GiftManager::_tGenerateGameGift>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 5) return false;
    count = 0;

    //MutableOverridableBool
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Utils.OverridableFields"), XS("MutableOverridableBool"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("SerializeToProtobuf")) == 0) {
            RecRoom::Utils::OverridableFields::MutableOverridableBool::SerializeToProtobuf = reinterpret_cast<RecRoom::Utils::OverridableFields::MutableOverridableBool::_tSerializeToProtobuf>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;
    
    //MutableOverridableFloat
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Utils.OverridableFields"), XS("MutableOverridableFloat"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("SerializeToProtobuf")) == 0) {
            RecRoom::Utils::OverridableFields::MutableOverridableFloat::SerializeToProtobuf = reinterpret_cast<RecRoom::Utils::OverridableFields::MutableOverridableFloat::_tSerializeToProtobuf>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //MutableOverridableInt
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Utils.OverridableFields"), XS("MutableOverridableInt"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("SerializeToProtobuf")) == 0) {
            RecRoom::Utils::OverridableFields::MutableOverridableInt::SerializeToProtobuf = reinterpret_cast<RecRoom::Utils::OverridableFields::MutableOverridableInt::_tSerializeToProtobuf>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //ToolSkinMapper
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("ToolSkinMapper"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("ApplySkin")) == 0) {
            ToolSkinMapper::ApplySkin = reinterpret_cast<ToolSkinMapper::_tApplySkin>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("ThisTool_PostPickupEvent")) == 0) {
            ToolSkinMapper::ThisTool_PostPickupEvent = reinterpret_cast<ToolSkinMapper::_tThisTool_PostPickupEvent>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;
    
    //RecRoomSceneManager
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("RecRoomSceneManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_Instance")) == 0) {
            RecRoomSceneManager::get_Instance = reinterpret_cast<RecRoomSceneManager::_tget_Instance>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //AudioClip
    currentClass = get_native_type(XS("UnityEngine.AudioModule"), XS("UnityEngine"), XS("AudioClip"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("Create")) == 0 && currentMethod->parameters_count == 5) {
            UnityEngine::AudioClip::Create = reinterpret_cast<UnityEngine::AudioClip::_tCreate>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("SetData")) == 0) {
            UnityEngine::AudioClip::SetData = reinterpret_cast<UnityEngine::AudioClip::_tSetData>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //PhotonVoiceRecorder
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("PhotonVoiceRecorder"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("set_DebugEchoMode")) == 0) {
            PhotonVoiceRecorder::set_DebugEchoMode = reinterpret_cast<PhotonVoiceRecorder::_tset_DebugEchoMode>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("set_Transmit")) == 0) {
            PhotonVoiceRecorder::set_Transmit = reinterpret_cast<PhotonVoiceRecorder::_tset_DebugEchoMode>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("UpdateAudioSource")) == 0) {
            PhotonVoiceRecorder::UpdateAudioSource = reinterpret_cast<PhotonVoiceRecorder::_tUpdateAudioSource>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 3) return false;
    count = 0;

    //AvatarItemVisualData
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Avatar.Data.Shared"), XS("AvatarItemVisualData"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS(".ctor")) == 0 && currentMethod->parameters_count == 4) {
            RecRoom::Avatar::Data::Shared::AvatarItemVisualData::ctor = reinterpret_cast<RecRoom::Avatar::Data::Shared::AvatarItemVisualData::_tctor>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //AvatarItemWardrobeRuntimeConfig
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Avatar.Data.Runtime"), XS("AvatarItemWardrobeRuntimeConfig"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_Config")) == 0) {
            RecRoom::Avatar::Data::Runtime::AvatarItemWardrobeRuntimeConfig::get_Config = reinterpret_cast<RecRoom::Avatar::Data::Runtime::AvatarItemWardrobeRuntimeConfig::_tget_Config>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //AsyncOperationHandle`1
    currentClass = get_native_type(XS("Unity.ResourceManager"), XS("UnityEngine.ResourceManagement.AsyncOperations"), XS("AsyncOperationHandle`1"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_IsDone")) == 0) {
            UnityEngine::ResourceManagement::AsyncOperations::AsyncOperationHandle_1::get_IsDone = reinterpret_cast<UnityEngine::ResourceManagement::AsyncOperations::AsyncOperationHandle_1::_tget_IsDone>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_Result")) == 0) {
            UnityEngine::ResourceManagement::AsyncOperations::AsyncOperationHandle_1::get_Result = reinterpret_cast<UnityEngine::ResourceManagement::AsyncOperations::AsyncOperationHandle_1::_tget_Result>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //AssetReference
    currentClass = get_native_type(XS("Unity.Addressables"), XS("UnityEngine.AddressableAssets"), XS("AssetReference"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_Asset")) == 0) {
            UnityEngine::AddressableAssets::AssetReference::get_Asset = reinterpret_cast<UnityEngine::AddressableAssets::AssetReference::_tget_Asset>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS(".ctor")) == 0 && currentMethod->parameters_count == 1) {
            UnityEngine::AddressableAssets::AssetReference::ctor = reinterpret_cast<UnityEngine::AddressableAssets::AssetReference::_tctor>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //Object
    currentClass = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Object"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("Instantiate")) == 0 && currentMethod->parameters_count == 4 && currentMethod->methodPointer) {
            UnityEngine::Object::Instantiate = reinterpret_cast<UnityEngine::Object::_tInstantiate>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;
	
	return MH_Initialize() == MH_OK &&
		MH_CreateHook(GameCombatManager::MasterDamagePlayer, &hMasterDamagePlayer, (void**)&oMasterDamagePlayer) == MH_OK &&
		MH_CreateHook(CodeStage::AntiCheat::Detectors::RRCheatDetector::Update, &hUpdate, (void**)&oUpdate) == MH_OK &&
		MH_CreateHook(RecRoom::Core::Locomotion::PlayerMovement::AddFlyEnabled, &hAddFlyEnabled, (void**)&oAddFlyEnabled) == MH_OK &&
		MH_CreateHook(RecNet::Avatars::LocalConsumeGiftPackage, &hLocalConsumeGiftPackage, nullptr) == MH_OK &&
		MH_CreateHook(SessionManager::LocalPlayerSpawned, &hLocalPlayerSpawned, (void**)&oLocalPlayerSpawned) == MH_OK &&
		MH_CreateHook(SessionManager::JoinRoom, &hJoinRoom, (void**)&oJoinRoom) == MH_OK &&
		MH_CreateHook(ToolSkinMapper::ThisTool_PostPickupEvent, &hThisTool_PostPickupEvent, (void**)&oThisTool_PostPickupEvent) == MH_OK &&
		MH_CreateHook(RecRoom::Core::Combat::RangedWeapon::Fire, &hFire, (void**)&oFire) == MH_OK &&
		MH_CreateHook(RecRoom::Core::Combat::RangedWeapon::get_IsOnCooldown, &hget_IsOnCooldown, nullptr) == MH_OK &&
		MH_CreateHook(RecRoom::Core::Combat::Weapon::SetAmmunition, &hSetAmmunition, (void**)&oSetAmmunition) == MH_OK &&
		MH_CreateHook(RecRoom::Activities::Quest::QuestManager::GetNextGameplayRoomIndex, &hGetNextGameplayRoomIndex, (void**)&oGetNextGameplayRoomIndex) == MH_OK &&
		MH_CreateHook(GameScoreManager::MasterSetScore, &hMasterSetScore, (void**)&oMasterSetScore) == MH_OK &&
		MH_CreateHook(GiftManager::GenerateGameGift, &hGenerateGameGift, (void**)&oGenerateGameGift) == MH_OK &&
        true;
}

bool hooking::init() {
    using namespace native;
    //load the base address
    game_assembly_base = (size_t)GetModuleHandleA(XS("GameAssembly.dll"));

    //fragile version check, assuming the offset won't be used for another method
    auto test = reinterpret_cast<unsigned char*>(game_assembly_base + OFFSET_RECNET_STORAGE_UPLOADFILE);
    if(test[-1] != 0xCC) return false;

    int count = 0;

    //load method ptrs

    //GameObject
    auto currentClass = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("GameObject"));
    void* iter = nullptr;
    auto currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("GetComponentInChildren")) == 0 && currentMethod->parameters_count == 1 && strcmp(currentMethod->parameters[0].name, XS("type")) == 0) {
            UnityEngine::GameObject::GetComponentInChildren = reinterpret_cast<UnityEngine::GameObject::_tGetComponentInChildren>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("GetComponentsInChildren")) == 0 && currentMethod->parameters_count == 1 && strcmp(currentMethod->parameters[0].name, XS("type")) == 0) {
            UnityEngine::GameObject::GetComponentsInChildren = reinterpret_cast<UnityEngine::GameObject::_tGetComponentsInChildren>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_transform")) == 0) {
            UnityEngine::GameObject::get_transform = reinterpret_cast<UnityEngine::GameObject::_tget_transform>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("AddComponent")) == 0 && currentMethod->parameters_count == 1 && strcmp(currentMethod->parameters[0].name, XS("componentType")) == 0) {
            UnityEngine::GameObject::AddComponent = reinterpret_cast<UnityEngine::GameObject::_tAddComponent>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 4) return false;
    count = 0;

    //Component
    currentClass = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Component"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_transform")) == 0) {
            UnityEngine::Component::get_transform = reinterpret_cast<UnityEngine::Component::_tget_transform>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_gameObject")) == 0) {
            UnityEngine::Component::get_gameObject = reinterpret_cast<UnityEngine::Component::_tget_gameObject>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //Transform
    currentClass = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Transform"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_position")) == 0) {
            UnityEngine::Transform::get_position = reinterpret_cast<UnityEngine::Transform::_tget_position>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("set_position")) == 0) {
            UnityEngine::Transform::set_position = reinterpret_cast<UnityEngine::Transform::_tset_position>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_rotation")) == 0) {
            UnityEngine::Transform::get_rotation = reinterpret_cast<UnityEngine::Transform::_tget_rotation>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("set_localScale")) == 0) {
            UnityEngine::Transform::set_localScale = reinterpret_cast<UnityEngine::Transform::_tset_localScale>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("GetChild")) == 0) {
            UnityEngine::Transform::GetChild = reinterpret_cast<UnityEngine::Transform::_tGetChild>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("SetParent")) == 0 && currentMethod->parameters_count == 2) {
            UnityEngine::Transform::SetParent = reinterpret_cast<UnityEngine::Transform::_tSetParent>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_forward")) == 0) {
            UnityEngine::Transform::get_forward = reinterpret_cast<UnityEngine::Transform::_tget_forward>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 7) return false;
    count = 0;

    //Quaternion
    currentClass = get_native_type(XS("UnityEngine.CoreModule"), XS("UnityEngine"), XS("Quaternion"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_eulerAngles")) == 0) {
            UnityEngine::Quaternion::get_eulerAngles = reinterpret_cast<UnityEngine::Quaternion::_tget_eulerAngles>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

	//RecRoom::Core::Encounters::BaseSpawnEnemyEncounterStep::MasterKillAllLivingEnemies = reinterpret_cast<RecRoom::Core::Encounters::BaseSpawnEnemyEncounterStep::_tMasterKillAllLivingEnemies>(game_assembly_base + OFFSET_RECROOM_CORE_ENCOUNTERS_BASESPAWNENEMYENCOUNTERSTEP_MASTERKILLALLLIVINGENEMIES);
    //Storage
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecNet"), XS("Storage"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("UploadFile")) == 0) {
            RecNet::Storage::UploadFile = reinterpret_cast<RecNet::Storage::_tUploadFile>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //Avatars
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecNet"), XS("Avatars"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("LocalConsumeGiftPackage")) == 0) {
            RecNet::Avatars::LocalConsumeGiftPackage = reinterpret_cast<RecNet::Avatars::_tLocalConsumeGiftPackage>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;
    
    //RRCheatDetector
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("CodeStage.AntiCheat.Detectors"), XS("RRCheatDetector"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("Update")) == 0) {
            CodeStage::AntiCheat::Detectors::RRCheatDetector::Update = reinterpret_cast<CodeStage::AntiCheat::Detectors::RRCheatDetector::_tUpdate>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //MonoBehaviour
    currentClass = get_native_type(XS("Assembly-CSharp"), XS("Photon"), XS("MonoBehaviour"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_photonView")) == 0) {
            Photon::MonoBehaviour::get_photonView = reinterpret_cast<Photon::MonoBehaviour::_tget_photonView>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_authority")) == 0) {
            Photon::MonoBehaviour::get_authority = reinterpret_cast<Photon::MonoBehaviour::_tget_authority>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    //PlayerAvatar
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("PlayerAvatar"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("EquipOutfitItemInternal")) == 0) {
            PlayerAvatar::EquipOutfitItemInternal = reinterpret_cast<PlayerAvatar::_tEquipOutfitItemInternal>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //QuestManager
	currentClass = get_native_type(XS("Assembly-CSharp"), XS("RecRoom.Activities.Quest"), XS("QuestManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("GetNextGameplayRoomIndex")) == 0) {
            RecRoom::Activities::Quest::QuestManager::GetNextGameplayRoomIndex = reinterpret_cast<RecRoom::Activities::Quest::QuestManager::_tGetNextGameplayRoomIndex>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //GameCombatManager
	currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("GameCombatManager"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("MasterDamagePlayer")) == 0 && currentMethod->parameters_count == 5) {
            GameCombatManager::MasterDamagePlayer = reinterpret_cast<GameCombatManager::_tMasterDamagePlayer>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 1) return false;
    count = 0;

    //PhotonNetwork
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("PhotonNetwork"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_isMasterClient")) == 0) {
            PhotonNetwork::get_isMasterClient = reinterpret_cast<PhotonNetwork::_tget_isMasterClient>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_player")) == 0) {
            PhotonNetwork::get_player = reinterpret_cast<PhotonNetwork::_tget_player>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("Instantiate")) == 0 && currentMethod->parameters_count == 5) {
            PhotonNetwork::Instantiate = reinterpret_cast<PhotonNetwork::_tInstantiate>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("RPC")) == 0 && currentMethod->parameters_count == 5 && strcmp(currentMethod->parameters[2].name, XS("target")) == 0) {
            PhotonNetwork::RPC = reinterpret_cast<PhotonNetwork::_tRPC>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("RPC")) == 0 && currentMethod->parameters_count == 5 && strcmp(currentMethod->parameters[2].name, XS("targetPlayer")) == 0) {
            PhotonNetwork::RPC_player = reinterpret_cast<PhotonNetwork::_tRPC_player>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("Destroy")) == 0 && currentMethod->parameters_count == 1 && strcmp(currentMethod->parameters[0].name, XS("targetGo")) == 0) {
            PhotonNetwork::Destroy = reinterpret_cast<PhotonNetwork::_tDestroy>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_otherPlayers")) == 0) {
            PhotonNetwork::get_otherPlayers = reinterpret_cast<PhotonNetwork::_tget_otherPlayers>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_playerList")) == 0) {
            PhotonNetwork::get_playerList = reinterpret_cast<PhotonNetwork::_tget_playerList>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 8) return false;
    count = 0;

    //PhotonPlayer
    currentClass = get_native_type(XS("Assembly-CSharp"), XS(""), XS("PhotonPlayer"));
    iter = nullptr;
    currentMethod = il2cpp_class_get_methods(currentClass, &iter);

    for(; currentMethod; currentMethod = il2cpp_class_get_methods(currentClass, &iter)) {
        if(strcmp(currentMethod->name, XS("get_name")) == 0) {
            PhotonPlayer::get_name = reinterpret_cast<PhotonPlayer::_tget_name>(currentMethod->methodPointer);
            count++;
        }
        else if(strcmp(currentMethod->name, XS("get_ID")) == 0) {
            PhotonPlayer::get_ID = reinterpret_cast<PhotonPlayer::_tget_ID>(currentMethod->methodPointer);
            count++;
        }
    }
    if(count != 2) return false;
    count = 0;

    return init2();
}

bool hooking::destroy() {
    return MH_DisableHook(MH_ALL_HOOKS) == MH_OK &&
        MH_Uninitialize() == MH_OK;
}