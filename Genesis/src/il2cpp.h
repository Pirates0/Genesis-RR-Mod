#pragma once
#include <stdint.h>

struct Il2CppClass;
struct Il2CppImage;
struct MethodInfo;
struct Il2CppType;
struct Il2CppString;

struct Il2CppDomain
{
    void* domain;
    void* setup;
    void* default_context;
    const char* friendly_name;
    uint32_t domain_id;

    volatile int threadpool_jobs;
};

struct Il2CppGuid
{
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t data4[8];
};

struct Il2CppObject
{
    union
    {
        Il2CppClass *klass;
        void *vtable;
    };
    void *monitor;
};

struct Il2CppString : public Il2CppObject
{
    int32_t length; ///< Length of string *excluding* the trailing null (which is included in 'chars').
    wchar_t chars[0];
};

struct Il2CppReflectionType : public Il2CppObject
{
    const Il2CppType *type;
};

struct Il2CppArrayBounds
{
    uintptr_t length;
    int32_t lower_bound;
};

struct Il2CppArray : public Il2CppObject
{
    /* bounds is NULL for szarrays */
    Il2CppArrayBounds *bounds;
    /* total number of elements of the array */
    uintptr_t max_length;
    char vector[0];

    typedef Il2CppObject*(*_tGetValue)(Il2CppArray* __this, int index, const MethodInfo* method_info);
    inline static _tGetValue GetValue;

    typedef void(*_tSetValue)(Il2CppArray* __this, Il2CppObject* value, int index, const MethodInfo* method_info);
    inline static _tSetValue SetValue;
};

struct Il2CppException : public Il2CppObject
{
    Il2CppString* className;
    Il2CppString* message;
    Il2CppObject* _data;
    Il2CppException* inner_ex;
    Il2CppString* _helpURL;
    Il2CppArray* trace_ips;
    Il2CppString* stack_trace;
    Il2CppString* remote_stack_trace;
    int remote_stack_index;
    Il2CppObject* _dynamicMethods;
    int32_t hresult;
    Il2CppString* source;
    Il2CppObject* safeSerializationManager;
    Il2CppArray* captured_traces;
    Il2CppArray* native_trace_ips;
};

struct Il2CppGenericInst
{
    uint32_t type_argc;
    const Il2CppType **type_argv;
};

struct Il2CppGenericContext
{
    /* The instantiation corresponding to the class generic parameters */
    const Il2CppGenericInst *class_inst;
    /* The instantiation corresponding to the method generic parameters */
    const Il2CppGenericInst *method_inst;
};

struct Il2CppGenericClass
{
    uint32_t typeDefinitionIndex;    /* the generic type definition */
    Il2CppGenericContext context;   /* a context that contains the type instantiation doesn't contain any method instantiation */
    Il2CppClass *cached_class;  /* if present, the Il2CppClass corresponding to the instantiation.  */
};

struct Il2CppArrayType
{
    const Il2CppType* etype;
    uint8_t rank;
    uint8_t numsizes;
    uint8_t numlobounds;
    int *sizes;
    int *lobounds;
};

typedef enum Il2CppTypeEnum
{
    IL2CPP_TYPE_END        = 0x00,       /* End of List */
    IL2CPP_TYPE_VOID       = 0x01,
    IL2CPP_TYPE_BOOLEAN    = 0x02,
    IL2CPP_TYPE_CHAR       = 0x03,
    IL2CPP_TYPE_I1         = 0x04,
    IL2CPP_TYPE_U1         = 0x05,
    IL2CPP_TYPE_I2         = 0x06,
    IL2CPP_TYPE_U2         = 0x07,
    IL2CPP_TYPE_I4         = 0x08,
    IL2CPP_TYPE_U4         = 0x09,
    IL2CPP_TYPE_I8         = 0x0a,
    IL2CPP_TYPE_U8         = 0x0b,
    IL2CPP_TYPE_R4         = 0x0c,
    IL2CPP_TYPE_R8         = 0x0d,
    IL2CPP_TYPE_STRING     = 0x0e,
    IL2CPP_TYPE_PTR        = 0x0f,       /* arg: <type> token */
    IL2CPP_TYPE_BYREF      = 0x10,       /* arg: <type> token */
    IL2CPP_TYPE_VALUETYPE  = 0x11,       /* arg: <type> token */
    IL2CPP_TYPE_CLASS      = 0x12,       /* arg: <type> token */
    IL2CPP_TYPE_VAR        = 0x13,       /* Generic parameter in a generic type definition, represented as number (compressed unsigned integer) number */
    IL2CPP_TYPE_ARRAY      = 0x14,       /* type, rank, boundsCount, bound1, loCount, lo1 */
    IL2CPP_TYPE_GENERICINST = 0x15,     /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I          = 0x18,
    IL2CPP_TYPE_U          = 0x19,
    IL2CPP_TYPE_FNPTR      = 0x1b,        /* arg: full method signature */
    IL2CPP_TYPE_OBJECT     = 0x1c,
    IL2CPP_TYPE_SZARRAY    = 0x1d,       /* 0-based one-dim-array */
    IL2CPP_TYPE_MVAR       = 0x1e,       /* Generic parameter in a generic method definition, represented as number (compressed unsigned integer)  */
    IL2CPP_TYPE_CMOD_REQD  = 0x1f,       /* arg: typedef or typeref token */
    IL2CPP_TYPE_CMOD_OPT   = 0x20,       /* optional arg: typedef or typref token */
    IL2CPP_TYPE_INTERNAL   = 0x21,       /* CLR internal type */

    IL2CPP_TYPE_MODIFIER   = 0x40,       /* Or with the following types */
    IL2CPP_TYPE_SENTINEL   = 0x41,       /* Sentinel for varargs method signature */
    IL2CPP_TYPE_PINNED     = 0x45,       /* Local var that points to pinned object */

    IL2CPP_TYPE_ENUM       = 0x55        /* an enumeration */
} Il2CppTypeEnum;

struct Il2CppType
{
    union
    {
        // We have this dummy field first because pre C99 compilers (MSVC) can only initializer the first value in a union.
        void* dummy;
        uint32_t klassIndex; /* for VALUETYPE and CLASS */
        const Il2CppType *type;   /* for PTR and SZARRAY */
        Il2CppArrayType *array; /* for ARRAY */
        //MonoMethodSignature *method;
        uint32_t genericParameterIndex; /* for VAR and MVAR */
        Il2CppGenericClass *generic_class; /* for GENERICINST */
    } data;
    unsigned int attrs    : 16; /* param attributes or field flags */
    Il2CppTypeEnum type     : 8;
    unsigned int num_mods : 6;  /* max 64 modifiers follow at the end */
    unsigned int byref    : 1;
    unsigned int pinned   : 1;  /* valid when included in a local var signature */
    //MonoCustomMod modifiers [MONO_ZERO_LEN_ARRAY]; /* this may grow */
};

struct Il2CppTypeDefinition
{
    uint32_t nameIndex;
    uint32_t namespaceIndex;
    uint32_t byvalTypeIndex;
    uint32_t byrefTypeIndex;

    uint32_t declaringTypeIndex;
    uint32_t parentIndex;
    uint32_t elementTypeIndex; // we can probably remove this one. Only used for enums

    uint32_t rgctxStartIndex;
    int32_t rgctxCount;

    uint32_t genericContainerIndex;

    uint32_t flags;

    uint32_t fieldStart;
    uint32_t methodStart;
    uint32_t eventStart;
    uint32_t propertyStart;
    uint32_t nestedTypesStart;
    uint32_t interfacesStart;
    uint32_t vtableStart;
    uint32_t interfaceOffsetsStart;

    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;

    // bitfield to portably encode boolean values as single bits
    // 01 - valuetype;
    // 02 - enumtype;
    // 03 - has_finalize;
    // 04 - has_cctor;
    // 05 - is_blittable;
    // 06 - is_import_or_windows_runtime;
    // 07-10 - One of nine possible PackingSize values (0, 1, 2, 4, 8, 16, 32, 64, or 128)
    uint32_t bitfield;
    uint32_t token;
};

struct ParameterInfo
{
    const char* name;
    int32_t position;
    uint32_t token;
    const Il2CppType* parameter_type;
};

struct EventInfo
{
    const char* name;
    const Il2CppType* eventType;
    Il2CppClass* parent;
    const MethodInfo* add;
    const MethodInfo* remove;
    const MethodInfo* raise;
    uint32_t token;
};

struct PropertyInfo
{
    Il2CppClass *parent;
    const char *name;
    const MethodInfo *get;
    const MethodInfo *set;
    uint32_t attrs;
    uint32_t token;
};

struct Il2CppRuntimeInterfaceOffsetPair
{
    Il2CppClass* interfaceType;
    int32_t offset;
};

struct FieldInfo
{
    const char* name;
    const Il2CppType* type;
    Il2CppClass *parent;
    int32_t offset; // If offset is -1, then it's thread static
    uint32_t token;
};

struct Il2CppInteropData
{
    void* delegatePInvokeWrapperFunction;
    void* pinvokeMarshalToNativeFunction;
    void* pinvokeMarshalFromNativeFunction;
    void* pinvokeMarshalCleanupFunction;
    void* createCCWFunction;
    const Il2CppGuid* guid;
    const Il2CppType* type;
};

struct Il2CppAssemblyName
{
    const char* name;
    const char* culture;
    const char* hash_value;
    const char* public_key;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[8];
};

struct Il2CppAssembly
{
    Il2CppImage* image;
    uint32_t token;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyName aname;
};

struct Il2CppImage
{
    const char* name;
    const char *nameNoExt;
    Il2CppAssembly* assembly;

    uint32_t typeStart;
    uint32_t typeCount;

    uint32_t exportedTypeStart;
    uint32_t exportedTypeCount;

    uint32_t customAttributeStart;
    uint32_t customAttributeCount;

    uint32_t entryPointIndex;

#ifdef __cplusplus
    mutable
#endif
    void * nameToClassHashTable;

    uint32_t token;
    uint8_t dynamic;
};

struct MethodInfo
{
    void* methodPointer;
    void* invoker_method;
    const char* name;
    Il2CppClass *klass;
    const Il2CppType *return_type;
    const ParameterInfo* parameters;

    union
    {
        const void* rgctx_data; /* is_inflated is true and is_generic is false, i.e. a generic instance method */
        const void* methodDefinition;
    };

    /* note, when is_generic == true and is_inflated == true the method represents an uninflated generic method on an inflated type. */
    union
    {
        const void* genericMethod; /* is_inflated is true */
        const void* genericContainer; /* is_inflated is false and is_generic is true */
    };

    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1; /* true if method is a generic method definition */
    uint8_t is_inflated : 1; /* true if declaring_type is a generic instance or if method is a generic instance*/
    uint8_t wrapper_type : 1; /* always zero (MONO_WRAPPER_NONE) needed for the debugger */
    uint8_t is_marshaled_from_native : 1; /* a fake MethodInfo wrapping a native function pointer */
};

struct VirtualInvokeData
{
    void* methodPtr;
    const MethodInfo* method;
};

struct Il2CppClass
{
    // The following fields are always valid for a Il2CppClass structure
    const Il2CppImage* image; // 0x0
    void* gc_desc; // 0x8
    const char* name; // 0x10
    const char* namespaze; // 0x18
    Il2CppType byval_arg; // 0x20
    Il2CppType this_arg; // 0x30
    Il2CppClass* element_class; // 0x40
    Il2CppClass* castClass; // 0x48
    Il2CppClass* declaringType; // 0x50
    Il2CppClass* parent; // 0x58
    Il2CppGenericClass *generic_class; // 0x60
    const Il2CppTypeDefinition* typeDefinition; // 0x68 non-NULL for Il2CppClass's constructed from type defintions
    const Il2CppInteropData* interopData; // 0x70
    Il2CppClass* klass; // 0x78 hack to pretend we are a MonoVTable. Points to ourself
    // End always valid fields

    // The following fields need initialized before access. This can be done per field or as an aggregate via a call to Class::Init
    FieldInfo* fields; // 0x80 Initialized in SetupFields
    const EventInfo* events; // 0x88 Initialized in SetupEvents
    const PropertyInfo* properties; // 0x90 Initialized in SetupProperties
    const MethodInfo** methods; // 0x98 Initialized in SetupMethods
    Il2CppClass** nestedTypes; // 0xA0 Initialized in SetupNestedTypes
    Il2CppClass** implementedInterfaces; // 0xA8 Initialized in SetupInterfaces
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets; // 0xB0 Initialized in Init
    void* static_fields; // 0xB8 Initialized in Init
    const void* rgctx_data; // 0xC0 Initialized in Init
    // used for fast parent checks
    Il2CppClass** typeHierarchy; // 0xC8 Initialized in SetupTypeHierachy
    // End initialization required fields

    uint32_t initializationExceptionGCHandle;

    uint32_t cctor_started;
    uint32_t cctor_finished;
    uint64_t cctor_thread;

    // Remaining fields are always valid except where noted
    uint32_t genericContainerIndex;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;

    uint16_t method_count; // lazily calculated for arrays, i.e. when rank > 0
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count; // lazily calculated for arrays, i.e. when rank > 0
    uint16_t interfaces_count;
    uint16_t interface_offsets_count; // lazily calculated for arrays, i.e. when rank > 0

    uint8_t typeHierarchyDepth; // Initialized in SetupTypeHierachy
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t packingSize;

    // this is critical for performance of Class::InitFromCodegen. Equals to initialized && !has_initialization_error at all times.
    // Use Class::UpdateInitializedAndNoError to update
    uint8_t initialized_and_no_error : 1;

    uint8_t valuetype : 1;
    uint8_t initialized : 1;
    uint8_t enumtype : 1;
    uint8_t is_generic : 1;
    uint8_t has_references : 1;
    uint8_t init_pending : 1;
    uint8_t size_inited : 1;
    uint8_t has_finalize : 1;
    uint8_t has_cctor : 1;
    uint8_t is_blittable : 1;
    uint8_t is_import_or_windows_runtime : 1;
    uint8_t is_vtable_initialized : 1;
    uint8_t has_initialization_error : 1;
    VirtualInvokeData vtable[0];
};

#define DO_API(r, n, a) extern "C" r n a

DO_API(const Il2CppImage*, il2cpp_get_corlib, ());

DO_API(void*, il2cpp_alloc, (size_t size));
DO_API(void, il2cpp_free, (void* ptr));

// array
DO_API(Il2CppClass*, il2cpp_array_class_get, (Il2CppClass * element_class, uint32_t rank));
DO_API(uint32_t, il2cpp_array_length, (Il2CppArray * array));
DO_API(uint32_t, il2cpp_array_get_byte_length, (Il2CppArray * array));
DO_API(Il2CppArray*, il2cpp_array_new, (Il2CppClass * elementTypeInfo, uintptr_t length));
DO_API(Il2CppArray*, il2cpp_array_new_specific, (Il2CppClass * arrayTypeInfo, uintptr_t length));
DO_API(Il2CppArray*, il2cpp_array_new_full, (Il2CppClass * array_class, uintptr_t * lengths, uintptr_t * lower_bounds));
DO_API(Il2CppClass*, il2cpp_bounded_array_class_get, (Il2CppClass * element_class, uint32_t rank, bool bounded));
DO_API(int, il2cpp_array_element_size, (const Il2CppClass * array_class));

// assembly
DO_API(const Il2CppImage*, il2cpp_assembly_get_image, (const Il2CppAssembly * assembly));

// class
DO_API(const Il2CppType*, il2cpp_class_enum_basetype, (Il2CppClass * klass));
DO_API(bool, il2cpp_class_is_generic, (const Il2CppClass * klass));
DO_API(bool, il2cpp_class_is_inflated, (const Il2CppClass * klass));
DO_API(bool, il2cpp_class_is_assignable_from, (Il2CppClass * klass, Il2CppClass * oklass));
DO_API(bool, il2cpp_class_is_subclass_of, (Il2CppClass * klass, Il2CppClass * klassc, bool check_interfaces));
DO_API(bool, il2cpp_class_has_parent, (Il2CppClass * klass, Il2CppClass * klassc));
DO_API(Il2CppClass*, il2cpp_class_from_il2cpp_type, (const Il2CppType * type));
DO_API(Il2CppClass*, il2cpp_class_from_name, (const Il2CppImage * image, const char* namespaze, const char *name));
DO_API(Il2CppClass*, il2cpp_class_from_system_type, (Il2CppReflectionType * type));
DO_API(Il2CppClass*, il2cpp_class_get_element_class, (Il2CppClass * klass));
DO_API(const EventInfo*, il2cpp_class_get_events, (Il2CppClass * klass, void* *iter));
DO_API(FieldInfo*, il2cpp_class_get_fields, (Il2CppClass * klass, void* *iter));
DO_API(Il2CppClass*, il2cpp_class_get_nested_types, (Il2CppClass * klass, void* *iter));
DO_API(Il2CppClass*, il2cpp_class_get_interfaces, (Il2CppClass * klass, void* *iter));
DO_API(FieldInfo*, il2cpp_class_get_field_from_name, (Il2CppClass * klass, const char *name));
DO_API(const MethodInfo*, il2cpp_class_get_methods, (Il2CppClass * klass, void* *iter));
DO_API(const MethodInfo*, il2cpp_class_get_method_from_name, (Il2CppClass * klass, const char* name, int argsCount));
DO_API(const char*, il2cpp_class_get_name, (Il2CppClass * klass));
DO_API(const char*, il2cpp_class_get_namespace, (Il2CppClass * klass));
DO_API(Il2CppClass*, il2cpp_class_get_parent, (Il2CppClass * klass));
DO_API(Il2CppClass*, il2cpp_class_get_declaring_type, (Il2CppClass * klass));
DO_API(int32_t, il2cpp_class_instance_size, (Il2CppClass * klass));
DO_API(size_t, il2cpp_class_num_fields, (const Il2CppClass * enumKlass));
DO_API(bool, il2cpp_class_is_valuetype, (const Il2CppClass * klass));
DO_API(int32_t, il2cpp_class_value_size, (Il2CppClass * klass, uint32_t * align));
DO_API(bool, il2cpp_class_is_blittable, (const Il2CppClass * klass));
DO_API(int, il2cpp_class_get_flags, (const Il2CppClass * klass));
DO_API(bool, il2cpp_class_is_abstract, (const Il2CppClass * klass));
DO_API(bool, il2cpp_class_is_interface, (const Il2CppClass * klass));
DO_API(int, il2cpp_class_array_element_size, (const Il2CppClass * klass));
DO_API(Il2CppClass*, il2cpp_class_from_type, (const Il2CppType * type));
DO_API(const Il2CppType*, il2cpp_class_get_type, (Il2CppClass * klass));
DO_API(uint32_t, il2cpp_class_get_type_token, (Il2CppClass * klass));
DO_API(bool, il2cpp_class_has_attribute, (Il2CppClass * klass, Il2CppClass * attr_class));
DO_API(bool, il2cpp_class_has_references, (Il2CppClass * klass));
DO_API(bool, il2cpp_class_is_enum, (const Il2CppClass * klass));
DO_API(const Il2CppImage*, il2cpp_class_get_image, (Il2CppClass * klass));
DO_API(const char*, il2cpp_class_get_assemblyname, (const Il2CppClass * klass));
DO_API(int, il2cpp_class_get_rank, (const Il2CppClass * klass));

// domain
DO_API(Il2CppDomain*, il2cpp_domain_get, ());
DO_API(const Il2CppAssembly*, il2cpp_domain_assembly_open, (Il2CppDomain * domain, const char* name));
DO_API(const Il2CppAssembly**, il2cpp_domain_get_assemblies, (const Il2CppDomain * domain, size_t * size));

// field
DO_API(int, il2cpp_field_get_flags, (FieldInfo * field));
DO_API(size_t, il2cpp_field_get_offset, (FieldInfo * field));
DO_API(const Il2CppType*, il2cpp_field_get_type, (FieldInfo * field));
DO_API(void, il2cpp_field_static_get_value, (FieldInfo * field, void *value));
DO_API(void, il2cpp_field_static_set_value, (FieldInfo * field, void *value));

// method
DO_API(const Il2CppType*, il2cpp_method_get_return_type, (const MethodInfo * method));
DO_API(Il2CppClass*, il2cpp_method_get_declaring_type, (const MethodInfo * method));
DO_API(bool, il2cpp_method_is_generic, (const MethodInfo * method));
DO_API(bool, il2cpp_method_is_inflated, (const MethodInfo * method));
DO_API(bool, il2cpp_method_is_instance, (const MethodInfo * method));
DO_API(uint32_t, il2cpp_method_get_param_count, (const MethodInfo * method));
DO_API(const Il2CppType*, il2cpp_method_get_param, (const MethodInfo * method, uint32_t index));
DO_API(Il2CppClass*, il2cpp_method_get_class, (const MethodInfo * method));
DO_API(const char*, il2cpp_method_get_param_name, (const MethodInfo * method, uint32_t index));

// object
DO_API(uint32_t, il2cpp_object_get_size, (Il2CppObject * obj));
DO_API(const MethodInfo*, il2cpp_object_get_virtual_method, (Il2CppObject * obj, const MethodInfo * method));
DO_API(Il2CppObject*, il2cpp_object_new, (const Il2CppClass * klass));
DO_API(void*, il2cpp_object_unbox, (Il2CppObject * obj));
DO_API(Il2CppObject*, il2cpp_value_box, (Il2CppClass * klass, void* data));

// runtime
DO_API(Il2CppObject*, il2cpp_runtime_invoke, (const MethodInfo * method, void *obj, void **params, Il2CppException **exc));

// string
DO_API(Il2CppString*, il2cpp_string_new, (const char* str));
DO_API(Il2CppString*, il2cpp_string_new_len, (const char* str, uint32_t length));
DO_API(Il2CppString*, il2cpp_string_new_utf16, (const wchar_t * text, int32_t len));

// type
DO_API(Il2CppObject*, il2cpp_type_get_object, (const Il2CppType * type));
DO_API(int, il2cpp_type_get_type, (const Il2CppType * type));
DO_API(Il2CppClass*, il2cpp_type_get_class_or_element_class, (const Il2CppType * type));
DO_API(char*, il2cpp_type_get_name, (const Il2CppType * type));
DO_API(bool, il2cpp_type_is_byref, (const Il2CppType * type));
DO_API(uint32_t, il2cpp_type_get_attrs, (const Il2CppType * type));
DO_API(bool, il2cpp_type_equals, (const Il2CppType * type, const Il2CppType * otherType));
DO_API(char*, il2cpp_type_get_assembly_qualified_name, (const Il2CppType * type));

// image
DO_API(const Il2CppAssembly*, il2cpp_image_get_assembly, (const Il2CppImage * image));
DO_API(const char*, il2cpp_image_get_name, (const Il2CppImage * image));
DO_API(const char*, il2cpp_image_get_filename, (const Il2CppImage * image));
DO_API(const MethodInfo*, il2cpp_image_get_entry_point, (const Il2CppImage * image));

DO_API(size_t, il2cpp_image_get_class_count, (const Il2CppImage * image));
DO_API(const Il2CppClass*, il2cpp_image_get_class, (const Il2CppImage * image, size_t index));

// Logging
DO_API(void, il2cpp_register_log_callback, (void(*method)(const char*)));


