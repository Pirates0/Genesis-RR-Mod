#pragma once

#define _OFFSET(Off) (Off - 0x180000000)										

constexpr size_t OFFSET_UNITYENGINE_GAMEOBJECT_GETCOMPONENTINCHILDREN = _OFFSET(0x181DE4860); //public Component GetComponentInChildren(Type type); // RVA:
constexpr size_t OFFSET_UNITYENGINE_GAMEOBJECT_GETCOMPONENTSINCHILDREN = _OFFSET(0x181DE49B0); // public Component[] GetComponentsInChildren(Type type); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_GAMEOBJECT_GET_TRANSFORM = _OFFSET(0x181DE5860); // public Transform get_transform(); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_GAMEOBJECT_ADDCOMPONENT = _OFFSET(0x181DE43B0); //public Component AddComponent(Type componentType); // RVA: 

constexpr size_t OFFSET_UNITYENGINE_COMPONENT_GET_TRANSFORM = _OFFSET(0x181AAB110); //public Transform get_transform(); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_COMPONENT_GET_GAMEOBJECT = _OFFSET(0x181AAB040);//public GameObject get_gameObject(); // RVA: 

constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_GET_POSITION = _OFFSET(0x1821F45E0); //public Vector3 get_position(); // RVA:
constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_SET_POSITION = _OFFSET(0x1821F5280); //public void set_position(Vector3 value); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_GET_FORWARD = _OFFSET(0x1821F3D20); //public Vector3 get_forward(); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_GET_ROTATION = _OFFSET(0x1821F4850); // public Quaternion get_rotation(); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_SET_LOCALSCALE = _OFFSET(0x1821F50D0); //public void set_localScale(Vector3 value); // RVA: 
constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_GETCHILD = _OFFSET(0x1821F0EA0); //public Transform GetChild(int index); // RVA:
constexpr size_t OFFSET_UNITYENGINE_TRANSFORM_SETPARENT_O = _OFFSET(0x1821F2D60); //public void SetParent(Transform parent, bool worldPositionStays); // RVA: 

constexpr size_t OFFSET_UNITYENGINE_QUATERNION_GET_EULERANGLES = _OFFSET(0x1802ADDC0); //public Vector3 get_eulerAngles(); // RVA:
constexpr size_t OFFSET_UNITYENGINE_QUATERNION_SET_EULERANGLES = _OFFSET(0x1821F4BC0); //public void set_eulerAngles(Vector3 value); // RVA: 


constexpr size_t OFFSET_PLAYERAVATR_EQUIPOUTFITITEM = _OFFSET(0x1807DB210);


constexpr size_t OFFSET_UNITYENGINE_RENDERER_SET_ENABLED = _OFFSET(0x181F1A670); //public void set_enabled(bool value); // RVA: 

constexpr size_t OFFSET_UNITYENGINE_AUDIOCLIP_CREATE = _OFFSET(0x1824E1820); //public static AudioClip Create(string name, int lengthSamples, int channels, int frequency, bool stream); // RVA:
constexpr size_t OFFSET_UNITYENGINE_AUDIOCLIP_SETDATA = _OFFSET(0x1824E1D10); //public bool SetData(float[] data, int offsetSamples); // RVA: 

constexpr size_t OFFSET_RECNET_STORAGE_UPLOADFILE = _OFFSET(0x181251F20); //public static IPromise`1<string> UploadFile(byte[] data, Storage.FileType fileType, optional string contentType, optional IReadOnlyCollection`1<string> referencedFilenames); // RVA:

constexpr size_t OFFSET_RECNET_AVATARS_LOCALCONSUMEGIFTPACKAGE = _OFFSET(0x181349590); //	public static IEnumerator LocalConsumeGiftPackage(Avatars.GiftPackage gift, Core.ApiCallback callback); // RVA: 

constexpr size_t OFFSET_PHOTON_MONOBEHAVIOUR_GET_PHOTONVIEW = _OFFSET(0x181152EE0); //	public PhotonView get_photonView(); // RVA:
constexpr size_t OFFSET_PHOTON_MONOBEHAVIOUR_GET_AUTHORITY = _OFFSET(0x181152D60); //public PhotonPlayer get_authority(); // RVA:

constexpr size_t OFFSET_GAMECOMBATMANAGER_MASTERDAMAGEPLAYER = _OFFSET(0x18087A080); //	public void MasterDamagePlayer(PhotonPlayer player, int damage, bool ignoreShield, out GameCombatManager.PlayerDamageResult damageResult, out GameCombatManager.PlayerShieldEffect shieldEffect); // RVA:

constexpr size_t OFFSET_PHOTONNETWORK_GET_ISMASTERCLIENT = _OFFSET(0x181164BD0); //public static bool get_isMasterClient(); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_GET_OTHERPLAYERS = _OFFSET(0x181165030); //public static PhotonPlayer[] get_otherPlayers(); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_GET_PLAYER = _OFFSET(0x181165200); //public static PhotonPlayer get_player(); // RVA: 
constexpr size_t OFFSET_PHOTONNETWORK_GET_PLAYERLIST = _OFFSET(0x1811650E0); //	public static PhotonPlayer[] get_playerList(); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_INSTANTIATE = _OFFSET(0x18115DCF0); //	public static GameObject Instantiate(string prefabName, Vector3 position, Quaternion rotation, float scale, byte group); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_RPC = _OFFSET(0x18115FFC0); //	internal static void RPC(PhotonView view, string methodName, PhotonTargets target, bool encrypt, object[] parameters); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_RPC_O = _OFFSET(0x18115FD60); //	internal static void RPC(PhotonView view, string methodName, PhotonPlayer targetPlayer, bool encrpyt, object[] parameters); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_DESTROY = _OFFSET(0x18115BC40); //public static void Destroy(GameObject targetGo); // RVA:
constexpr size_t OFFSET_PHOTONNETWORK_DESTROYPLAYEROBJECTS = _OFFSET(0x18115B8A0); //public static void DestroyPlayerObjects(PhotonPlayer targetPlayer); // RVA:

constexpr size_t OFFSET_PHOTONPLAYER_GET_NAME = _OFFSET(0x1803185C0);
constexpr size_t OFFSET_PHOTONPLAYER_GET_ID = _OFFSET(0x18031CE60);
//////////



constexpr size_t OFFSET_POLAROID_AUTHORITYSETIMAGENAME = _OFFSET(0x18083E140); //public void AuthoritySetImageName(IPromise`1<string> imageNamePromise); // RVA: 
constexpr size_t OFFSET_POLAROID_AUTHORITYSETIMAGENAMEB__35_0 = _OFFSET(0x18083F860); //	private void <AuthoritySetImageName>b__35_0(string imageName); // RVA:

constexpr size_t OFFSET_CODESTAGE_ANTICHEAT_DETECTORS_RRCHEATDETECTOR_UPDATE = _OFFSET(0x1816E95F0); //private void Update(); // RVA: 





constexpr size_t OFFSET_RECROOM_CORE_LOCOMOTION_PLAYERMOVEMENT_ADDFLYENABLED = _OFFSET(0x18131F820); //public void AddFlyEnabled(bool enabled, object token, int priority); // RVA: 
constexpr size_t OFFSET_RECROOM_CORE_LOCOMOTION_PLAYERMOVEMENT_GET_ISFLYINGENABLED = _OFFSET(0x18132A800); //public bool get_IsFlyingEnabled(); // RVA:



constexpr size_t OFFSET_RECROOM_SYSTEMS_PLAYERROLES_PLAYERGAMEROLEMANAGER_GET_INSTANCE = _OFFSET(0x180A30E70); //public static PlayerGameRoleManager get_Instance(); // RVA:  ---> private GameRole get_InGameRoleDefinition


constexpr size_t  OFFSET_RECROOM_CORE_ENCOUNTERS_BASESPAWNENEMYENCOUNTERSTEP_MASTERKILLALLLIVINGENEMIES = _OFFSET(0x180CEEDD0);

constexpr size_t OFFSET_PLAYERAVATAR_ISWEARINGANYTRIALITEMS = _OFFSET(0x1807DBC80);

constexpr size_t OFFSET_CREATIONMANAGER_GET_ISOVERINKLIMIT = _OFFSET(0x180F3DDF0);

constexpr size_t OFFSET_RECROOM_SYSTEMS_PLAYERROLES_PLAYERROOMROLEMANAGER_GET_INSTANCE = _OFFSET(0x180A34480); //public static PlayerRoomRoleManager get_Instance(); // RVA:  ---> protected override int[] get_AllRoleDefinitionIds();



constexpr size_t OFFSET_RECROOM_UTILS_OVERRIDABLEFIELDS_MUTABLEOVERRIDABLEBOOL_SERIALIZETOPROTOBUF = _OFFSET(0x180B4E610); //public override OverridableBoolData SerializeToProtobuf(); // RVA: ---> [ser]

constexpr size_t OFFSET_RECROOM_UTILS_OVERRIDABLEFIELDS_MUTABLEOVERRIDABLEFLOAT_SERIALIZETOPROTOBUF = _OFFSET(0x180B4EE10); //public override OverridableFloatData SerializeToProtobuf(); // RVA:   ---> [ser]

constexpr size_t OFFSET_RECROOM_UTILS_OVERRIDABLEFIELDS_MUTABLEOVERRIDABLEINT_SERIALIZETOPROTOBUF = _OFFSET(0x180B4F1D0); //public override OverridableIntData SerializeToProtobuf(); // RVA: ---> [ser]

//constexpr size_t OFFSET_RECROOM_UTILS_OVERRIDABLEFIELDS_MUTABLEOVERRIDABLEVOTEKICKTYPE_SERIALIZETOPROTOBUF = _OFFSET(0x180A94BD0); //public override OverridableIntData SerializeToProtobuf(); // RVA:





constexpr size_t OFFSET_SYSTEM_ARRAY_GETVALUE = _OFFSET(0x180517E50); //public object GetValue(int index); // RVA:
constexpr size_t OFFSET_SYSTEM_ARRAY_SETVALUE = _OFFSET(0x180519430); //public void SetValue(object value, int index); // RVA:




constexpr size_t OFFSET_PLAYER_GET_PHOTONPLAYER = _OFFSET(0x180CEC980); //	public PhotonPlayer get_PhotonPlayer(); // RVA:
constexpr size_t OFFSET_PLAYER_SET_DEVELOPERDISPLAYMODE = _OFFSET(0x180CED5B0); //public void set_DeveloperDisplayMode(SettingsManager.DeveloperDisplayModes value); // RVA:






constexpr size_t OFFSET_GIFTMANAGER_GET_ISAGIFTBOXCURRENTLYSPAWNED = _OFFSET(0x180B97830); //	public bool get_IsAGiftBoxCurrentlySpawned(); // RVA:
constexpr size_t OFFSET_GIFTMANAGER_ONGIFTPACKAGERECEIVEDEVENT = _OFFSET(0x180B96900); //	private void OnGiftPackageReceivedEvent(Avatars.GiftPackage giftPackage, bool showImmediately); // RVA: 
constexpr size_t OFFSET_GIFTMANAGER_GETPACKAGEVARIANTBYCONTEXT = _OFFSET(0x180B96300); //	public GiftPackageVariant GetPackageVariantByContext(GiftManager.GiftContext context); // RVA:
constexpr size_t OFFSET_GIFTMANAGER_GET_DEFAULTGIFTBOXMATERIAL = _OFFSET(0x180478A60); //public Material get_DefaultGiftBoxMaterial(); // RVA:





constexpr size_t OFFSET_SESSIONMANAGER_LOCALPLAYERSPAWNED = _OFFSET(0x180C9A850); //public void LocalPlayerSpawned(); // RVA:
constexpr size_t OFFSET_SESSIONMANAGER_JOINROOM = _OFFSET(0x180C99FC0); //	private IPromise`1<Matchmaking.MatchmakingErrorCode> JoinRoom(string roomName, string roomSceneName, bool isPrivate, SessionManager.JoinRoomInviteMode inviteMode, bool bypassMovementModeRestriction); // RVA:

constexpr size_t OFFSET_TOOLSKINMAPPER_APPLYSKIN = _OFFSET(0x180D67AD0); //public void ApplySkin(string skinGuid, optional bool playNotifications); // RVA:
constexpr size_t OFFSET_TOOLSKINMAPPER_THISTOOL_POSTPICKUPEVENT = _OFFSET(0x180D69490); 	//private void ThisTool_PostPickupEvent(Tool thisTool); // RVA:



constexpr size_t OFFSET_RECROOM_ACTIVITIES_QUEST_QUESTMANAGER_GETNEXTGAMEPLAYROOMINDEX = _OFFSET(0x180FB2810);


//constexpr size_t OFFSET_RECROOM_ACTIVITIES_QUEST_QUESTMANAGER_KILLALLENEMIES = _OFFSET(0x31D300);

constexpr size_t OFFSET_RECROOM_CORE_COMBAT_RANGEDWEAPON_FIRE_O = _OFFSET(0x180DA8BA0); //	public virtual void Fire(Vector3 velocity, optional float chargeAmount); // RVA:
constexpr size_t OFFSET_RECROOM_CORE_COMBAT_RANGEDWEAPON_GET_ISONCOOLDOWN = _OFFSET(0x180DAA4F0); //	public bool get_IsOnCooldown(); // RVA:




constexpr size_t OFFSET_RECROOM_CORE_COMBAT_WEAPON_GET_HASENOUGHMAGAZINEAMMUNITION = _OFFSET(0x180DB35D0); //public bool get_HasEnoughMagazineAmmunition(); // RVA:
constexpr size_t OFFSET_RECROOM_CORE_COMBAT_WEAPON_SETAMMUNITION = _OFFSET(0x180DB17D0); //protected void SetAmmunition(int magazineAmmunition, int reserveAmmunition); // RVA:

constexpr size_t OFFSET_RECROOMSCENEMANAGER_GET_INSTANCE = _OFFSET(0x180DFE3A0); //	public static RecRoomSceneManager get_Instance(); // RVA:


constexpr size_t OFFSET_GIFTMANAGER_GENERATEGAMEGIFT = _OFFSET(0x180B96010);

constexpr size_t OFFSET_PHOTONVOICERECORDER_SET_DEBUGECHOMODE = _OFFSET(0x180CDB5C0); //	public void set_DebugEchoMode(bool value); // RVA:
constexpr size_t OFFSET_PHOTONVOICERECORDER_SET_TRANSMIT = _OFFSET(0x180CDB8A0); //public void set_Transmit(bool value); // RVA:
constexpr size_t OFFSET_PHOTONVOICERECORDER_UPDATEAUDIOSOURCE = _OFFSET(0x180CD9B80); //public void UpdateAudioSource(); // RVA:


constexpr size_t OFFSET_GAMESCOREMANAGER_MASTERSETSCORE = _OFFSET(0x180BF90F0);

constexpr size_t OFFSET_SYSTEM_COLLECTIONS_GENERIC_LIST_GET_COUNT = _OFFSET(0x180320E60);
constexpr size_t OFFSET_SYSTEM_COLLECTIONS_GENERIC_LIST_GET_ITEM = _OFFSET(0x1821E5D60); //public T get_Item(int index); // RVA:
