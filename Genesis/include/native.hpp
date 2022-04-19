#pragma once

#include <il2cpp.hpp>
#include <Xorstr.hpp>

namespace native {
    inline size_t game_assembly_base;
    inline Il2CppClass* get_native_type(const char* assembly_name, const char* namespaze, const char* type_name) {
        auto assembly = il2cpp_domain_assembly_open(il2cpp_domain_get(), assembly_name);
        if(!assembly) return nullptr;
        auto image = il2cpp_assembly_get_image(assembly);
        if(!image) return nullptr;
        return il2cpp_class_from_name(image, namespaze, type_name);
    }

    inline Il2CppObject* get_native_type_object(Il2CppClass* klass) {
        auto type = il2cpp_class_get_type(klass);
        return il2cpp_type_get_object(type);
    }
	

	struct LocomotionTransientState {

		
	};

	struct PlayerLocomotionController : LocomotionTransientState {


		typedef void(*_tAddTeleportCooldown)(PlayerLocomotionController* __this, float cooldown, Il2CppObject* token, int priority, MethodInfo*mi);
		inline static _tAddTeleportCooldown AddTeleportCooldown;

	};

	//public void AddTeleportCooldown(float cooldown, object token, int priority); // RVA: 0x181320300 Offset: 0x131F700

    namespace System {
        namespace Collections {
            namespace Generic {
                struct List : Il2CppObject {
                    typedef int(*_tget_Count)(List* __this, const MethodInfo* methodInfo);
                    inline static _tget_Count get_Count;

                    typedef Il2CppObject*(*_tget_Item)(List* __this, int index, const MethodInfo* methodInfo);
                    inline static _tget_Item get_Item;

                    Il2CppArray* _items;
                };
            }
        }
    }
	

    namespace UnityEngine {
        struct Vector3 {
            float x;
            float y;
            float z;
        };

        struct Quaternion {
            float x;
            float y;
            float z;
            float w;

            typedef Vector3(*_tget_eulerAngles)(Il2CppObject* __this, const MethodInfo* method_info);
            inline static _tget_eulerAngles get_eulerAngles;
        };

        struct Transform;
        struct Object : Il2CppObject {
            typedef Object*(*_tInstantiate)(Object* original, Vector3 pos, Quaternion rot, Transform* parent, const MethodInfo* mi);
            inline static _tInstantiate Instantiate;

            void* m_CachedPtr;
        };

        struct GameObject;
        struct Component : Object {
            typedef Transform*(*_tget_transform)(Component* __this, const MethodInfo* method_info);
            inline static _tget_transform get_transform;

            typedef GameObject*(*_tget_gameObject)(Component* __this, const MethodInfo* method_info);
            inline static _tget_gameObject get_gameObject;
        };

        struct Behaviour : Component {

        };

        struct MonoBehaviour : Behaviour {

        };

		struct SingletonMonoBehaviour {


		};

		
        struct GameObject : Object {
            typedef Component*(*_tGetComponentInChildren)(
                GameObject* __this, 
                Il2CppObject* type, 
                const MethodInfo* method_info
            );
            inline static _tGetComponentInChildren GetComponentInChildren;

            typedef Il2CppArray*(*_tGetComponentsInChildren)(
                GameObject* __this, 
                Il2CppObject* type, 
                const MethodInfo* method_info
            );
            inline static _tGetComponentsInChildren GetComponentsInChildren;

            typedef Transform*(*_tget_transform)(GameObject* __this, const MethodInfo* method_info);
            inline static _tget_transform get_transform;

            typedef Component*(*_tAddComponent)(GameObject* __this, Il2CppObject* type, const MethodInfo* method_info);
            inline static _tAddComponent AddComponent;
        };

        struct Transform : Component {
            typedef Vector3(*_tget_position)(Transform* __this, const MethodInfo* method_info);
            inline static _tget_position get_position;

            typedef void(*_tset_localScale)(Transform* __this, Vector3 value, const MethodInfo* method_info);
            inline static _tset_localScale set_localScale;

            typedef void(*_tset_position)(Transform* __this, Vector3 value, const MethodInfo* method_info);
            inline static _tset_position set_position;

            typedef Vector3(*_tget_forward)(Transform* __this, const MethodInfo* method_info);
            inline static _tget_forward get_forward;

            typedef Quaternion(*_tget_rotation)(Transform* __this, const MethodInfo* method_info);
            inline static _tget_rotation get_rotation;

            typedef Transform*(*_tGetChild)(Transform* __this, int index, const MethodInfo* method_info);
            inline static _tGetChild GetChild;

            typedef Transform*(*_tSetParent)(Transform* __this, Transform* parent, bool worldPositionStays, const MethodInfo* method_info);
            inline static _tSetParent SetParent;
        };




        struct Renderer : Component {
            typedef void(*_tset_enabled)(Renderer* __this, bool value, const MethodInfo* method_info);
            inline static _tset_enabled set_enabled;
        };

        struct Material : Object {

        };

        struct AudioClip : Object {
            typedef AudioClip*(*_tCreate)(Il2CppString* name, int lengthSamples, int channels, int frequency, bool stream, const MethodInfo* method_info);
            inline static _tCreate Create;

            typedef bool(*_tSetData)(AudioClip* __this, Il2CppArray* data, int offsetSamples, const MethodInfo* method_info);
            inline static _tSetData SetData;
        };

        struct ScriptableObject : Object {

        };

        namespace ResourceManagement {
            namespace AsyncOperations {
                struct AsyncOperationHandle_1 {
                    typedef bool(*_tget_IsDone)(Il2CppObject* __this, const MethodInfo* mi);
                    inline static _tget_IsDone get_IsDone;

                    typedef Il2CppObject*(*_tget_Result)(Il2CppObject* __this, const MethodInfo* mi);
                    inline static _tget_Result get_Result;
                };
            }
        }

        namespace AddressableAssets {
            struct AssetReference : Il2CppObject {
                typedef ResourceManagement::AsyncOperations::AsyncOperationHandle_1(*_tInstantiate)(AssetReference* __this, Vector3 pos, Quaternion rot, Transform* parent, const MethodInfo* mi);

                typedef Object*(*_tget_Asset)(AssetReference* __this, const MethodInfo* mi);
                inline static _tget_Asset get_Asset;

                typedef void(*_tctor)(AssetReference* __this, Il2CppString* guid, const MethodInfo* mi);
                inline static _tctor ctor;
            };
        }
    }

    struct PhotonView : UnityEngine::MonoBehaviour {

    };

    struct PhotonPlayer : Il2CppObject {
        typedef Il2CppString*(*_tget_name)(PhotonPlayer* __this, const MethodInfo* method_info);
        inline static _tget_name get_name;

        typedef int(*_tget_ID)(PhotonPlayer* __this, const MethodInfo* method_info);
        inline static _tget_ID get_ID;
    };

    struct PhotonMessageInfo {
        int timestamp;
        int pad;
        PhotonPlayer* sender;
        PhotonView* target;
    };

    namespace Photon {
        struct MonoBehaviour : UnityEngine::MonoBehaviour {
            PhotonView* pvCache;
            bool pvCacheInitialized;
            Il2CppObject* OnDestroyedEvent;

            typedef PhotonView*(*_tget_photonView)(MonoBehaviour* __this, const MethodInfo* method_info);
            inline static _tget_photonView get_photonView;

            typedef PhotonPlayer*(*_tget_authority)(MonoBehaviour* __this, const MethodInfo* method_info);
            inline static _tget_authority get_authority;
        };
    }
	struct RankBucket {

	};


	struct GameScoreManager : UnityEngine::MonoBehaviour{
		typedef void(*_tMasterSetScore)(GameScoreManager* __this, const int GameTeam, int score, const MethodInfo* method_info);
		inline static _tMasterSetScore MasterSetScore;
	};

    namespace CodeStage {
        namespace AntiCheat {
            namespace Detectors {
                struct ActDetectorBase : UnityEngine::MonoBehaviour{

                };

                struct RRCheatDetector : ActDetectorBase {
                    typedef void(*_tUpdate)(RRCheatDetector* __this, const MethodInfo* method_info);
                    inline static _tUpdate Update;
                };
            }
        }
    }

    struct GiftContextMapping : Il2CppObject {
        Il2CppString* name;
		int context;
		UnityEngine::Material* material;
		Il2CppString* label;
    };

    struct GiftPackageVariant : Il2CppObject {
	    Il2CppString* name;
	    int dequeueBehavior;
	    Photon::MonoBehaviour* prefab;
	    Il2CppArray* giftContextMappings; //GiftContextMapping[]
    };

    struct SingletonMonoBehaviourBase : UnityEngine::MonoBehaviour {
        int gameRestartBehavior;
    };

	struct NetworkedSingletonMonoBehaviour : UnityEngine::MonoBehaviour {


	};

	struct CreationManager : NetworkedSingletonMonoBehaviour {

		char __pad1[0x68];
		int allCreationObjects;

		typedef bool(*_tget_IsOverInkLimit)(CreationManager* __this, MethodInfo* mi); // RVA: 0x180F3DDF0 Offset: 0xF3D1F0
		inline static _tget_IsOverInkLimit get_IsOverInkLimit;


	};
	
    struct SessionManager : SingletonMonoBehaviourBase {


		typedef bool(*_tIsWearingTrialItems)(SessionManager* __this, MethodInfo* mi);
		inline static _tIsWearingTrialItems IsWearingTrialItems;

        typedef void(*_tLocalPlayerSpawned)(SessionManager* __this, const MethodInfo* method_info);
        inline static _tLocalPlayerSpawned LocalPlayerSpawned;

        typedef Il2CppObject*(*_tJoinRoom)(
            SessionManager* __this, 
            Il2CppString* roomName, 
            Il2CppString* roomSceneName, 
            bool isPrivate, 
            int inviteMode, 
            bool bypassMovementModeRestriction, 
            const MethodInfo* method_info
        );
        inline static _tJoinRoom JoinRoom;


		
    };

    struct NetworkedSingletonMonoBehaviourBase : Photon::MonoBehaviour {
        int gameRestartBehavior; // 0x30
    };
    
    namespace RecNet { struct GiftPackage; }

    struct GiftManager : SingletonMonoBehaviourBase {

		typedef void(*_tGenerateGameGift)(GiftManager* __this, const int item_context, const MethodInfo* method_info);
		inline static _tGenerateGameGift GenerateGameGift;

		
        typedef bool(*_tget_IsAGiftBoxCurrentlySpawned)(GiftManager* __this, const MethodInfo* method_info);
        inline static _tget_IsAGiftBoxCurrentlySpawned get_IsAGiftBoxCurrentlySpawned;

        typedef void(*_tOnGiftPackageReceivedEvent)(GiftManager* __this, RecNet::GiftPackage* gift, bool showImmediately, const MethodInfo* method_info);
        inline static _tOnGiftPackageReceivedEvent OnGiftPackageReceivedEvent;

        typedef GiftPackageVariant*(*_tGetPackageVariantByContext)(GiftManager* __this, int context, const MethodInfo* method_info);
        inline static _tGetPackageVariantByContext GetPackageVariantByContext;

        typedef UnityEngine::Material*(*_tget_DefaultGiftBoxMaterial)(GiftManager* __this, const MethodInfo* method_info);
        inline static _tget_DefaultGiftBoxMaterial get_DefaultGiftBoxMaterial;

        //not part of the native code
        inline static GiftManager* helper_get_instance() {
            return *(GiftManager**)(get_native_type(XS("Assembly-CSharp"), "", XS("GiftManager"))->parent->static_fields);
        }
    };

    namespace RecNet {
        struct Storage : Il2CppObject {
            typedef Il2CppObject*(*_tUploadFile)(Il2CppArray* data, int fileType, Il2CppString* contentType, Il2CppObject* referencedFileNames, const MethodInfo* method_info);
            inline static _tUploadFile UploadFile;
        };

        struct GiftPackage : Il2CppObject {
                long long Id__BackingField; // 0x10
	            Il2CppObject* FromPlayerId__BackingField; // 0x18
	            Il2CppString* ConsumableItemDesc__BackingField; // 0x20
	            Il2CppObject* AvatarItemType__BackingField; // 0x28
	            Il2CppString* AvatarItemDescOrHairDyeDesc__BackingField; // 0x30
	            Il2CppString* EquipmentPrefabName__BackingField; // 0x38
	            Il2CppString* EquipmentModificationGuid__BackingField; // 0x40
	            int CurrencyType__BackingField; // 0x48
	            int Currency__BackingField; // 0x4C
	            int Xp__BackingField; // 0x50
	            int Level__BackingField; // 0x54
	            int GiftContext__BackingField; // 0x58
	            int GiftRarity__BackingField; // 0x5C
	            Il2CppString* Message__BackingField; // 0x60
	            int Platform__BackingField; // 0x68
	            UnityEngine::Material* PackageMaterial__BackingField; // 0x70
	            GiftPackageVariant* PackageVariant__BackingField; // 0x78
	            bool Consumed__BackingField; // 0x80
	            bool IsValid__BackingField; // 0x81
	            Il2CppString* ErrorMessage__BackingField; // 0x88
	            bool SupportsCurrentPlatform__BackingField; // 0x90
	            bool IsGifted; // 0x91

                inline static GiftPackage* helper_create_tokens(GiftManager* gm, const char* msg, int amount) {
                    int context = 1;
                    auto type = get_native_type(XS("Assembly-CSharp"), XS("RecNet"), XS("Avatars/GiftPackage"));
                    auto result = (GiftPackage*)il2cpp_object_new(type);

                    auto package_variant = GiftManager::GetPackageVariantByContext(gm, context, nullptr);
                    auto package_material = (UnityEngine::Material*)Il2CppArray::GetValue(package_variant->giftContextMappings, 0, nullptr);

                    result->Currency__BackingField = amount;
                    result->CurrencyType__BackingField = 2;
                    result->GiftContext__BackingField = context;
                    result->GiftRarity__BackingField = 50;
                    result->IsValid__BackingField = true;
                    result->Message__BackingField = il2cpp_string_new(msg);
                    result->PackageMaterial__BackingField = package_material;
                    result->PackageVariant__BackingField = package_variant;
                    result->SupportsCurrentPlatform__BackingField = true;

                    return result;
                }
            };

        struct Avatars : Il2CppObject {
            typedef Il2CppObject*(*_tLocalConsumeGiftPackage)(GiftPackage* gift, Il2CppObject* callback, const MethodInfo* method_info);
            inline static _tLocalConsumeGiftPackage LocalConsumeGiftPackage;
        };
    }




	struct OutfitManager {

		Il2CppString* outfit;
	};
	

    struct Tool : Photon::MonoBehaviour {
        
    };

    namespace RecRoom {
        namespace Avatar {
            namespace Data {
                namespace Shared {
                    struct AvatarItemData : Il2CppObject {
                        Il2CppString* Name;
                        Il2CppString* AvatarItemGuid;
                        int OutfitType;
                        int OutfitSet;
                        //etc
                    };

                    struct AvatarItemVisualData : Il2CppObject {
                        typedef void(*_tctor)(AvatarItemVisualData* __this, Il2CppString* prefabGuid, Il2CppString* maskGuid, Il2CppString* swatchGuid, Il2CppString* decalGuid, const MethodInfo* mi);
                        inline static _tctor ctor;

                        UnityEngine::AddressableAssets::AssetReference* outfitPrefabReference;
                        UnityEngine::AddressableAssets::AssetReference* maskReference;
                        UnityEngine::AddressableAssets::AssetReference* swatchReference;
                        UnityEngine::AddressableAssets::AssetReference* decalReference;
                        Il2CppString* prefabGuid;
                        Il2CppString* maskGuid;
                        Il2CppString* swatchGuid;
                        Il2CppString* decalGuid;
                        Il2CppString* _cachedRecNetString;
                    };
                }

                namespace Runtime {
                    struct AvatarItem {
                        Shared::AvatarItemData* _avatarItemData;
                        Shared::AvatarItemVisualData* _avatarItemVisualData;
                    };

                    struct AvatarItemSelection {
                        AvatarItem _avatarItem;
                        int _bodyPart;
                        bool _isTrialItem;
                    };

                    struct AvatarItemWardrobeRuntimeConfig : UnityEngine::Object {
                        typedef AvatarItemWardrobeRuntimeConfig*(*_tget_Config)(void* __static, const MethodInfo*);
                        inline static _tget_Config get_Config;

                        char __pad234905[0x60];
                        System::Collections::Generic::List* allPossibleCombinations;
                    };
                }
            }
        }

        namespace Core {



			namespace AI {

				struct EnemyManager {



					typedef System::Collections::Generic::List* (*_tget_AllEnemies)(EnemyManager* __this, const MethodInfo* mi);
					inline static _tget_AllEnemies get_AllEnemies;

					
					//public static int get_EnemyCount(); // RVA: 0x18113E8D0 Offset: 0x113DCD0
					//public static List`1<Enemy > get_AllEnemies(); // RVA: 0x18113E870 Offset: 0x113DC70
				};

				struct Enemy : Photon::MonoBehaviour {

					typedef void(*_tMasterKill)(Enemy* __this, System::Collections::Generic::List* Enemy, const MethodInfo* mi);
					inline static _tMasterKill MasterKill;

				};
			}

	    	namespace Encounters {
				struct GameEncounterStep {

				};

				struct SpawnEnemyEncounterStep : BaseSpawnEnemyEncounterStep {

					typedef void(*_tset_EnemySpawnCount)(SpawnEnemyEncounterStep*__this,int value, const MethodInfo*mi); // RVA: 0x180D02660 Offset: 0xD01A60)
					inline static _tset_EnemySpawnCount set_EnemySpawnCount;


					// RVA: 0x180D02660 Offset: 0xD01A60) Memory
				};

				struct BaseSpawnEnemyEncounterStep : GameEncounterStep {
					char __pad1[0x68];
					int livingEnemies; // 0x68
					

					typedef void (*_tMasterKillAllLivingEnemies)(BaseSpawnEnemyEncounterStep* __this, const MethodInfo* method_info);
					inline static _tMasterKillAllLivingEnemies MasterKillAllLivingEnemies;

				};
			}


            namespace Equipment {
                struct EquipmentData : Il2CppObject {
                    Il2CppString* prefabName; // 0x10
                    Il2CppString* toolAssetName; // 0x18
                };

                struct SkinData : Il2CppObject {
                    Il2CppString* skinAssetName; // 0x10
                    Il2CppString* skinGuid; // 0x18
                };

                struct EquipmentSkinMappingData : Il2CppObject {
                    EquipmentData* equipment; // 0x10
                    System::Collections::Generic::List* skins; // 0x18 List<SkinData>
                };

                struct EquipmentWardrobeRuntimeConfig : UnityEngine::ScriptableObject {
                    System::Collections::Generic::List* toolSkinMaps; // 0x18 List<EquipmentSkinMappingData>
                };
            }

            namespace Locomotion {
                struct PlayerMovement : UnityEngine::MonoBehaviour {
                    typedef void(*_tAddFlyEnabled)(
                        PlayerMovement* __this, 
                        bool enabled, 
                        Il2CppObject* token, 
                        int priority, 
                        const MethodInfo* method_info
                    );
                    inline static _tAddFlyEnabled AddFlyEnabled;

                    typedef bool(*_tget_IsFlyingEnabled)(PlayerMovement* __this, const MethodInfo* method_info);
                    inline static _tget_IsFlyingEnabled get_IsFlyingEnabled;
                };
            }

            namespace Combat {
                struct Weapon : Photon::MonoBehaviour {
                    Tool* _tool; // 0x30
                    char __pad1[0x1C8];

                    typedef void (*_tSetAmmunition)(Weapon* __this, int mag, int reserve, const MethodInfo* method_info);
                    inline static _tSetAmmunition SetAmmunition;

                    typedef bool(*_tget_HasEnoughMagazineAmmunition)(Weapon* __this, const MethodInfo* method_info);
                    inline static _tget_HasEnoughMagazineAmmunition get_HasEnoughMagazineAmmunition;
                };

                struct RangedWeapon : Weapon {
                    char __pad2[0x10];
                    float bulletFireSpeed; // 0x210
                    char __pad4[0x8];
                    float maximumFiringRate; // 0x21C
                    char __pad3[0x3C];
                    UnityEngine::Transform* barrelTransform;
                    char __pad435[0x5C];
                    float lastFireTime; // 0x2C4

                    typedef void(*_tFire)(RangedWeapon* __this, UnityEngine::Vector3 velocity, float charge, const MethodInfo* method_info);
                    inline static _tFire Fire;

                    typedef bool(*_tget_IsOnCooldown)(RangedWeapon* __this, const MethodInfo* method_info);
                    inline static _tget_IsOnCooldown get_IsOnCooldown;
                };

                //constexpr int testoff = offsetof(RangedWeapon, lastFireTime);
            }

        }

        namespace Protobuf {
            struct OverridableBoolData;
            struct OverridableFloatData;
            struct OverridableIntData;
        }

        namespace Utils {
            namespace OverridableFields {
                struct MutableOverridableBool { //only a placeholder, does actually have a size
                    typedef Protobuf::OverridableBoolData*(*_tSerializeToProtobuf)(MutableOverridableBool* __this, const MethodInfo* method_info);
                    inline static _tSerializeToProtobuf SerializeToProtobuf;
                };
                struct MutableOverridableFloat { //only a placeholder, does actually have a size
                    typedef Protobuf::OverridableFloatData*(*_tSerializeToProtobuf)(MutableOverridableFloat* __this, const MethodInfo* method_info);
                    inline static _tSerializeToProtobuf SerializeToProtobuf;
                };
                struct MutableOverridableInt { //only a placeholder, does actually have a size
                    typedef Protobuf::OverridableIntData*(*_tSerializeToProtobuf)(MutableOverridableInt* __this, const MethodInfo* method_info);
                    inline static _tSerializeToProtobuf SerializeToProtobuf;
                };
				/*
                struct MutableOverridableVoteKickType { //only a placeholder, does actually have a size
                    typedef Protobuf::OverridableIntData*(*_tSerializeToProtobuf)(MutableOverridableVoteKickType* __this, const MethodInfo* method_info);
                    inline static _tSerializeToProtobuf SerializeToProtobuf;
                };
				*/
            }
        }

        namespace Protobuf {
                struct OverridableBoolData : Il2CppObject {
                    bool overrides;
                    bool innervalue;
                };
                struct OverridableFloatData : Il2CppObject {
                    bool overrides;
                    float innervalue;
                };
                struct OverridableIntData : Il2CppObject {
                    bool overrides;
                    int innervalue;
                };
            }
		struct GameRuleManager {

		};
		
		namespace Activities {
			namespace Quest {
				struct QuestManager : GameRuleManager {
					char __pad1[0x148];
					int totalRoomCount; // 0x150

					typedef int(*_tGetNextGameplayRoomIndex)(QuestManager* __this, const MethodInfo* method_info);
					inline static _tGetNextGameplayRoomIndex GetNextGameplayRoomIndex;
					
				};
			}
		}

        namespace Systems {
            namespace PlayerRoles {
                struct GameRole : Il2CppObject {
                    char __pad1[0x18];
                    Utils::OverridableFields::MutableOverridableFloat* teleportDelay;
                    Utils::OverridableFields::MutableOverridableBool* canMove;
                    Utils::OverridableFields::MutableOverridableFloat* vrWalkSpeed;
                    Utils::OverridableFields::MutableOverridableBool* vrCanSprint;
                    Utils::OverridableFields::MutableOverridableFloat* vrSprintMaxSpeedWalkSpeedMultiplier;
                    Utils::OverridableFields::MutableOverridableBool* vrCanDualWield;
                    Utils::OverridableFields::MutableOverridableBool* vrCanJump;
                    Utils::OverridableFields::MutableOverridableFloat* vrMaxJumpHeight;
                    Utils::OverridableFields::MutableOverridableBool* vrCanDodge;
                    Utils::OverridableFields::MutableOverridableInt* screensDefaultThirdPersonControlType;
                    Utils::OverridableFields::MutableOverridableFloat* screensWalkSpeed;
                    Utils::OverridableFields::MutableOverridableBool* screensCanSprint;
                    Utils::OverridableFields::MutableOverridableFloat* screensSprintMaxSpeedWalkSpeedMultiplier;
                    Utils::OverridableFields::MutableOverridableBool* screensCanJump;
                    Utils::OverridableFields::MutableOverridableFloat* screensMaxJumpHeight;
                    Utils::OverridableFields::MutableOverridableBool* screensCanDodge;
                    Utils::OverridableFields::MutableOverridableFloat* maxTeleportDistance;
                    Utils::OverridableFields::MutableOverridableFloat* maxTeleportDropHeight;
                    Utils::OverridableFields::MutableOverridableFloat* maxTeleportAngle;
                    Utils::OverridableFields::MutableOverridableBool* canFly;
                    Utils::OverridableFields::MutableOverridableBool* isInvincible;
                    char __pad2[0x18];
                    Utils::OverridableFields::MutableOverridableBool* canSwitchTeams;
                    Utils::OverridableFields::MutableOverridableInt* voiceRolloffMaxDistance;
                    Utils::OverridableFields::MutableOverridableBool* hidesName;
                    char __pad3[0x38];
                    Utils::OverridableFields::MutableOverridableInt* screensDefaultFirstPersonControlType;
                    Utils::OverridableFields::MutableOverridableBool* screensCanWallRun;
                    Utils::OverridableFields::MutableOverridableBool* vrCanWallRun;
                    Utils::OverridableFields::MutableOverridableBool* vrCanGetPushed;
                    Utils::OverridableFields::MutableOverridableBool* screensCanGetPushed;
                    Utils::OverridableFields::MutableOverridableBool* vrCanRam;
                    Utils::OverridableFields::MutableOverridableBool* screensCanRam;
                    Utils::OverridableFields::MutableOverridableBool* canAutoSprint;
                    Utils::OverridableFields::MutableOverridableFloat* airControlParameter;
                    Utils::OverridableFields::MutableOverridableBool* canJumpCutoff;
                    Utils::OverridableFields::MutableOverridableBool* canClamber;
                    Utils::OverridableFields::MutableOverridableFloat* wallRunDuration;
                    Utils::OverridableFields::MutableOverridableFloat* wallRunDropRateParameter;
                    Utils::OverridableFields::MutableOverridableFloat* wallRunStartSpeedBoost;
                    Utils::OverridableFields::MutableOverridableFloat* wallRunJumpHeight;
                    Utils::OverridableFields::MutableOverridableFloat* wallRunJumpDirectionParameter;
                    Utils::OverridableFields::MutableOverridableFloat* wallRunJumpAirControlMultiplier;
                    Utils::OverridableFields::MutableOverridableBool* canSlide;
                    Utils::OverridableFields::MutableOverridableBool* canSlideDownSlopes;
                    Utils::OverridableFields::MutableOverridableFloat* slideMinDuration;
                    Utils::OverridableFields::MutableOverridableFloat* slideSteeringParameter;
                    Utils::OverridableFields::MutableOverridableFloat* slideStartSpeedBoost;
                    Utils::OverridableFields::MutableOverridableFloat* slideAirControlMultiplier;
                    Utils::OverridableFields::MutableOverridableBool* canWallClimb;
                    Utils::OverridableFields::MutableOverridableBool* canWallClimbWithoutMarkup;
                    Utils::OverridableFields::MutableOverridableBool* canWallRunWithoutMarkup;
                    bool IsRoleActive__BackingField;
                    bool IsModifiedByPlayer__BackingField;
                };

                struct RoomRole : Il2CppObject {
                    char __pad1[0x28];
                    //Utils::OverridableFields::MutableOverridableVoteKickType* voteKickPermission;
                    Utils::OverridableFields::MutableOverridableBool* canInvite;
                    Utils::OverridableFields::MutableOverridableBool* canEditCircuits;
                    Utils::OverridableFields::MutableOverridableBool* canTalk;
                    Utils::OverridableFields::MutableOverridableBool* canPrintPhotos;
                    Utils::OverridableFields::MutableOverridableBool* canStartGames;
                    Utils::OverridableFields::MutableOverridableBool* canChangeGameMode;
                    Utils::OverridableFields::MutableOverridableBool* canSelfRevive;
                    Utils::OverridableFields::MutableOverridableBool* canEndGamesEarly;
                    char __pad2[0x8];
                    Utils::OverridableFields::MutableOverridableBool* canUseMakerPen;
                    Utils::OverridableFields::MutableOverridableBool* canUseDeleteAllButton;
                    Utils::OverridableFields::MutableOverridableBool* canSaveInventions;
                    Utils::OverridableFields::MutableOverridableBool* disableMicAutoMute;
                    Utils::OverridableFields::MutableOverridableBool* canUseShareCam;
                    bool IsRoleActive__BackingField;
                    bool IsModifiedByPlayer__BackingField;
                };

                struct PlayerGameRoleManager : NetworkedSingletonMonoBehaviourBase {
                    typedef PlayerGameRoleManager*(*_tget_Instance)(void* __static, const MethodInfo* method_info);
                    inline static _tget_Instance get_Instance;

                    typedef GameRole*(*_tGetRoleDefinition)(PlayerGameRoleManager* __this, int roleId, const MethodInfo* method_info);
                };

                struct PlayerRoomRoleManager : NetworkedSingletonMonoBehaviourBase {
                    typedef PlayerRoomRoleManager*(*_tget_Instance)(void* __static, const MethodInfo* method_info);
                    inline static _tget_Instance get_Instance;

                    typedef RoomRole*(*_tGetRoleDefinition)(PlayerRoomRoleManager* __this, int roleId, const MethodInfo* method_info);
                };
            }
        }

        namespace Players {
            struct PlayerEmotes : Photon::MonoBehaviour {

            };
        }
    }

    struct PlayerAvatar : UnityEngine::MonoBehaviour {
		typedef bool(*_tEquipOutfitItem)(PlayerAvatar* __this, RecRoom::Avatar::Data::Runtime::AvatarItem item, int body, int handtype, bool trialitems, const MethodInfo* mi);
		//inline static _tEquipOutfitItem EquipOutfitItem;

        typedef bool(*_tEquipOutfitItemInternal)(PlayerAvatar* __this, RecRoom::Avatar::Data::Runtime::AvatarItemSelection itemSelection, const MethodInfo* mi);
		inline static _tEquipOutfitItemInternal EquipOutfitItemInternal;
	};

    struct RecRoomSceneManager : Photon::MonoBehaviour {
        typedef RecRoomSceneManager*(*_tget_Instance)(void* __static, const MethodInfo* method_info);
        inline static _tget_Instance get_Instance;
    };

    struct EquipmentManager : NetworkedSingletonMonoBehaviourBase {
        char __pad1[0x38];
        RecRoom::Core::Equipment::EquipmentWardrobeRuntimeConfig* equipmentWardrobe;

        //not part of the native code
        inline static EquipmentManager* helper_get_instance() {
            return *(EquipmentManager**)(get_native_type(XS("Assembly-CSharp"), "", XS("EquipmentManager"))->parent->static_fields);
        }
    };

    struct AvatarSelectionElement : UnityEngine::ScriptableObject {
        Il2CppString* Guid;
    };

    struct EquipmentSkin : AvatarSelectionElement {

    };

    struct ToolSkinMapper : Photon::MonoBehaviour {
        char __pad1[0x58];
        Il2CppString* PrefabName;

        typedef void(*_tApplySkin)(ToolSkinMapper* __this, Il2CppString* skinGuid, bool playNotifications, const MethodInfo* mi);
        inline static _tApplySkin ApplySkin;

        typedef void(*_tThisTool_PostPickupEvent)(ToolSkinMapper* __this, Tool* thisTool, const MethodInfo* mi);
        inline static _tThisTool_PostPickupEvent ThisTool_PostPickupEvent;
    };

    struct GameCombatManager : Il2CppObject {
        
        typedef void(*_tMasterDamagePlayer)(
            GameCombatManager* __this, 
            PhotonPlayer* player, 
            int damage, 
            bool ignoreShield, 
            int* damageResult,
            int* shieldEffect,
            const MethodInfo* method_info
        );
        inline static _tMasterDamagePlayer MasterDamagePlayer;
    };

    struct PhotonNetwork {
        typedef bool(*_tget_isMasterClient)(const MethodInfo* method_info);
        inline static _tget_isMasterClient get_isMasterClient;

        typedef PhotonPlayer*(*_tget_player)(const MethodInfo* method_info);
        inline static _tget_player get_player;

        typedef UnityEngine::GameObject*(*_tInstantiate)(
            Il2CppString* prefabName,
            UnityEngine::Vector3 position,
            UnityEngine::Quaternion rotation,
            float scale,
            unsigned char group,
            const MethodInfo* method_info
        );
        inline static _tInstantiate Instantiate;

        typedef void(*_tRPC)(
            PhotonView* pv,
            Il2CppString* methodName,
            int target,
            bool encrypt,
            Il2CppArray* params,
            const MethodInfo* method_info
        );
        inline static _tRPC RPC;

        typedef void(*_tRPC_player)(
            PhotonView* pv,
            Il2CppString* methodName,
            PhotonPlayer* target,
            bool encrypt,
            Il2CppArray* params,
            const MethodInfo* method_info
        );
        inline static _tRPC_player RPC_player;

        typedef void(*_tDestroy)(UnityEngine::GameObject* go, const MethodInfo* method_info);
        inline static _tDestroy Destroy;

        typedef void(*_tDestroyPlayerObjects)(PhotonPlayer* target, const MethodInfo* method_info);
        inline static _tDestroyPlayerObjects DestroyPlayerObjects;

        typedef Il2CppArray*(*_tget_otherPlayers)(const MethodInfo* method_info);
        inline static _tget_otherPlayers get_otherPlayers;

        typedef Il2CppArray*(*_tget_playerList)(const MethodInfo* method_info);
        inline static _tget_playerList get_playerList;
    };

    struct Polaroid : Photon::MonoBehaviour {
        typedef void(*_tAuthoritySetImageName)(Polaroid* __this, Il2CppObject* imageNamePromise, const MethodInfo* method_info);
        inline static _tAuthoritySetImageName AuthoritySetImageName;

        typedef void(*_tAuthoritySetImageNameStr)(Polaroid* __this, Il2CppString* imageName, const MethodInfo* method_info);
        inline static _tAuthoritySetImageNameStr AuthoritySetImageNameStr;
    };

    struct PlayerHead : Photon::MonoBehaviour {
        char __pad1[0x20];

    };

    struct PhotonVoiceRecorder : Photon::MonoBehaviour {
        char __pad1[0x20];
        int Source; // 0x50
        char __pad2[0x0C];
        UnityEngine::AudioClip* AudioClip; //0x60
        bool loopAudioClip; // 0x68

        typedef void(*_tset_DebugEchoMode)(PhotonVoiceRecorder* __this, bool value, const MethodInfo* method_info);
        inline static _tset_DebugEchoMode set_DebugEchoMode;
        
        typedef _tset_DebugEchoMode _tset_Transmit;
        inline static _tset_Transmit set_Transmit;

        typedef void(*_tUpdateAudioSource)(PhotonVoiceRecorder* __this, const MethodInfo* method_info);
        inline static _tUpdateAudioSource UpdateAudioSource;
    };

    constexpr int off = offsetof(PhotonVoiceRecorder, AudioClip);

    struct PlayerHand : Photon::MonoBehaviour {

    };

    struct PlayerProgression : Photon::MonoBehaviour {

    };

    struct Player : Photon::MonoBehaviour {
        char __pad1[0x18];
        PlayerHead* head; // 0x48
        char __pad2[0x18];
        PlayerHand* leftHand; // 0x68
        PlayerHand* rightHand; // 0x70
        char __pad49[0x78];
        PlayerAvatar* playerAvatar; // 0xF0
        char __pad5[0x28];
        PlayerProgression* playerProgression;
        RecRoom::Players::PlayerEmotes* playerEmotes; // 0x128
        char __pad3[0x8];
        RecRoom::Core::Locomotion::PlayerMovement* playerMovement; // 0x138
        char __pad4[0x30];
        PhotonVoiceRecorder* voiceRecorder; //0x170

        struct StaticFields {
            System::Collections::Generic::List* All; // 0x0
	        char __pad1[0x48];
	        Player* LocalPlayer; // 0x50
	        bool LocalPlayerExists; // 0x58
        };
        
        inline static StaticFields* statics() {
            return (StaticFields*)(get_native_type(XS("Assembly-CSharp"), "", XS("Player"))->static_fields);
        }

        typedef PhotonPlayer*(*_tget_PhotonPlayer)(Player* __this, const MethodInfo* method_info);
        inline static _tget_PhotonPlayer get_PhotonPlayer;

        typedef void(*_tset_DeveloperDisplayMode)(Player* __this, unsigned char value, const MethodInfo* method_info);
        inline static _tset_DeveloperDisplayMode set_DeveloperDisplayMode;
    };

    //constexpr int off789 = offsetof(Player, playerProgression);
}