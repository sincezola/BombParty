#ifndef _JSON_H
#define _JSON_H

// chType
#define JSONTYPE_BEGIN       'B'

#define JSONTYPE_END          0
#define JSONTYPE_STRING      'S'
#define JSONTYPE_INT         'I'
#define JSONTYPE_LONG        'L'
#define JSONTYPE_FLOAT       'F'
#define JSONTYPE_DOUBLE      'D'
#define JSONTYPE_CHAR        'C'
#define JSONTYPE_UCHAR       'U'
#define JSONTYPE_GROUP       'G'
#define JSONTYPE_PROC        'P'
#define JSONTYPE_SKIP        'K'
#define JSONTYPE_ENDTAG      'E'

#ifndef json_char
   #define json_char char
#endif

#ifndef json_int_t
   #ifndef _MSC_VER
      #include <inttypes.h>
      #define json_int_t int64_t
   #else
      #define json_int_t __int64
   #endif
#endif

#include <stdlib.h>

#ifdef __cplusplus

   #include <string.h>

   extern "C"
   {

#endif
     
typedef struct
{
   unsigned long max_memory;
   int settings;

   /*Custom allocator support (leave null to use malloc/free)*/

   void * (* mem_alloc) (size_t, int zero, void * user_data);
   void   (* mem_free)  (void *, void * user_data);

   /* will be passed to mem_alloc and mem_free */
   void * user_data;

   /* how much extra space to allocate for values? */
   size_t value_extra;  

} json_settings;

#define json_enable_comments  0x01

typedef enum
{
   json_none,
   json_object,
   json_array,
   json_integer,
   json_double,
   json_string,
   json_boolean,
   json_null

} json_type;

extern const struct _json_value json_value_none;
       
typedef struct _json_object_entry
{
    json_char * name;
    unsigned int name_length;
    
    struct _json_value * value;
    
} json_object_entry;

typedef struct _json_value
{
   struct _json_value * parent;

   json_type type;

   union
   {
      int boolean;
      json_int_t integer;
      double dbl;

      struct
      {
         unsigned int length;
         json_char * ptr; /* null terminated */

      } string;

      struct
      {
         unsigned int length;

         json_object_entry * values;

         #if defined(__cplusplus) && __cplusplus >= 201103L
         decltype(values) begin () const
         {  return values;
         }
         decltype(values) end () const
         {  return values + length;
         }
         #endif

      } object;

      struct
      {
         unsigned int length;
         struct _json_value ** values;

         #if defined(__cplusplus) && __cplusplus >= 201103L
         decltype(values) begin () const
         {  return values;
         }
         decltype(values) end () const
         {  return values + length;
         }
         #endif

      } array;

   } u;

   union
   {
      struct _json_value * next_alloc;
      void * object_mem;

   } _reserved;

   #ifdef JSON_TRACK_SOURCE

      /* Location of the value in the source JSON
       */
      unsigned int line, col;

   #endif


   /* Some C++ operator sugar */

   #ifdef __cplusplus

      public:

         inline _json_value ()
         {  memset (this, 0, sizeof (_json_value));
         }

         inline const struct _json_value &operator [] (int index) const
         {
            if (type != json_array || index < 0
                     || ((unsigned int) index) >= u.array.length)
            {
               return json_value_none;
            }

            return *u.array.values [index];
         }

         inline const struct _json_value &operator [] (const char * index) const
         { 
            if (type != json_object)
               return json_value_none;

            for (unsigned int i = 0; i < u.object.length; ++ i)
               if (!strcmp (u.object.values [i].name, index))
                  return *u.object.values [i].value;

            return json_value_none;
         }

         inline operator const char * () const
         {  
            switch (type)
            {
               case json_string:
                  return u.string.ptr;

               default:
                  return "";
            };
         }

         inline operator json_int_t () const
         {  
            switch (type)
            {
               case json_integer:
                  return u.integer;

               case json_double:
                  return (json_int_t) u.dbl;

               default:
                  return 0;
            };
         }

         inline operator bool () const
         {  
            if (type != json_boolean)
               return false;

            return u.boolean != 0;
         }

         inline operator double () const
         {  
            switch (type)
            {
               case json_integer:
                  return (double) u.integer;

               case json_double:
                  return u.dbl;

               default:
                  return 0;
            };
         }

   #endif

} json_value;

typedef int (*JSONCALLBACK)(json_value *pfJSON, char* szKey, char* szData, void* pItemDesc);
typedef struct 
{
  const char   *pszKey;
  char          chType;
  unsigned int  uBytes;
  void         *pData;
  JSONCALLBACK  pCallBack;
}
  STRUCT_JSON;      
       
json_value * json_parse (const json_char * json,
                         size_t length);

#define json_error_max 128
json_value * json_parse_ex (json_settings * settings,
                            const json_char * json,
                            size_t length,
                            char * error);

void json_value_free (json_value *);


/* Not usually necessary, unless you used a custom mem_alloc and now want to
 * use a custom mem_free.
 */
void json_value_free_ex (json_settings * settings,
                         json_value *);

void process_value(json_value* value, STRUCT_JSON astDesc[]);
void process_value_first(json_value* value, STRUCT_JSON astDesc[]);
 int iJSON_LoadKey(char *szJson, STRUCT_JSON astDesc[]);
 int iJSON_LoadKey_First(char *szJson, STRUCT_JSON astDesc[]);

#ifdef __cplusplus
   } /* extern "C" */
#endif

#endif

