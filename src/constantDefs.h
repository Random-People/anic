#ifndef _CONSTANT_DEFS_H_
#define _CONSTANT_DEFS_H_

#define OUTPUT_FILE_DEFAULT "a.out"
#define VERBOSE_OUTPUT_DEFAULT false
#define SILENT_MODE_DEFAULT false
#define CODED_OUTPUT_DEFAULT false
#define EVENTUALLY_GIVE_UP_DEFAULT true

#define MIN_OPTIMIZATION_LEVEL 0
#define MAX_OPTIMIZATION_LEVEL 3
#define DEFAULT_OPTIMIZATION_LEVEL 1

#define MIN_TAB_MODULUS 1
#define MAX_TAB_MODULUS 80
#define TAB_MODULUS_DEFAULT 4

#define TOLERABLE_ERROR_LIMIT 256

#define MAX_STRING_LENGTH (sizeof(char)*4096)
#define MAX_INT_STRING_LENGTH 21

#define STD_IN_FILE_NAME "<stdin>"

#define MAX_TOKEN_LENGTH 1024*128
#define ESCAPE_CHARACTER '\\'
#define TOKEN_RAW_VECTOR INT_MAX

#define STANDARD_LIBRARY_STRING "std"
#define STANDARD_LIBRARY_FILE_INDEX UINT_MAX
#define STANDARD_LIBRARY_FILE_NAME "<stdlib>"

#define BLOCK_NODE_STRING "{}"
#define FILTER_NODE_STRING "[Tx]"
#define OBJECT_NODE_STRING "[]"
#define INSTRUCTOR_NODE_STRING "=[]"
#define OUTSTRUCTOR_NODE_STRING "=[-->]"
#define INSTANTIATION_NODE_STRING "[T]"
#define FAKE_RECALL_NODE_PREFIX ".."
#define IMPORT_DECL_STRING "<UNRESOLVED_IMPORT>"
#define STANDARD_IMPORT_DECL_STRING "<STD_IMPORT>"

#define UNIQUE_ID_PREFIX "idLabel"

#define VERBOSE(s) if (verboseOutput) {s}

#endif
