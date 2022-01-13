#ifndef CCB_H
#define CCB_H

#ifdef _ZCC
#define __func__ "TODO: function name"
#endif

#define CCB_X_OBJC
#define CCB_X_OBJX 

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct ccb ccb_t;

struct ccb {
	FILE* input;
	FILE* output;
	bool dump_ast;
	bool silent;
	bool include_code;
	bool include_data;
    int line, col;
    int iline, icol; // Previous increment of line/column (subtracted on an ungetc equivalent!)
};

/*
 * Type: ccb_string_t
 *  A type capable of representing a self-resizing string with
 *  O(1) strlen.
 */
typedef struct ccb_string_s ccb_string_t;

/*
 * Function: ccb_string_create
 *  Create a string object
 */
ccb_string_t* ccb_string_create(void);

/*
 * Function: ccb_string_buffer
 *  Return the raw buffer of a string object
 */
char* ccb_string_buffer(ccb_string_t* string);

/*
 * Function: ccb_string_cat
 *  Append a character to a string object
 */
void ccb_string_cat(ccb_string_t* string, char ch);

void ccb_string_catcstr(ccb_string_t* string, const char* str);
void ccb_string_catint(ccb_string_t* string, long long i);

/*
 * Function: ccb_string_catf
 *  Append a formatted string to a string object
 */
void ccb_string_catf(ccb_string_t* string, const char* fmt, ...);

/*
 * Function: ccb_string_quote
 *  Escape a string's quotes
 */
char* ccb_string_quote(char* p); // C/GNU-style
char* ccb_string_quote_fasm(char* p, char quotechar); // FASM/Intel-style (closes and reopens quotes, adding special chars by number)

/*
 * Macro: CCB_SENTINEL_LIST
 *  Initialize an empty list in place
 */
/*#define CCB_SENTINEL_LIST ((ccb_list_t) { \
        .length    = 0,           \
        .head      = NULL,        \
        .tail      = NULL         \
})*/

 /*
  * Type: ccb_list_iterator_t
  *  A type capable of representing an itrator for a <list>
  */
typedef struct ccb_list_iterator_s ccb_list_iterator_t;

/*
 * Type: ccb_list_t
 *  A standard double-linked list
 */
typedef struct ccb_list_s      ccb_list_t;

/*
 * Function: ccb_list_create
 *  Creates a new list
 */
ccb_list_t* ccb_list_create(void);

/*
 * Function: ccb_list_push
 *  Push an element onto a list
 */
void ccb_list_push(ccb_list_t* list, void* element);

/*
 * Function: ccb_list_pop
 *  Pop an element from a list
 */
void* ccb_list_pop(ccb_list_t* list);

/*
 * Function: ccb_list_length
 *  Used to retrieve length of a list object
 */
int ccb_list_length(ccb_list_t* list);

/*
 * Function: ccb_list_shift
 *  Like a ccb_list_pop but shift from the head (instead of the tail)
 */
void* ccb_list_shift(ccb_list_t* list);

/*
 * Function: ccb_list_reverse
 *  Reverse the contents of a list
 */
ccb_list_t* ccb_list_reverse(ccb_list_t* list);

/*
 * Function: ccb_list_iterator
 *  Create an iterator for a given list object
 */
ccb_list_iterator_t* ccb_list_iterator(ccb_list_t* list);

/*
 * Function: ccb_list_iterator_next
 *  Increment the list iterator while returning the given element
 */
void* ccb_list_iterator_next(ccb_list_iterator_t* iter);

/*
 * Function: ccb_list_iterator_end
 *  Test if the iterator is at the end of the list
 */
bool ccb_list_iterator_end(ccb_list_iterator_t* iter);

/*
 * Function: ccb_list_tail
 *  Get the last element in a list
 */
void* ccb_list_tail(ccb_list_t* list);

typedef struct ccb_list_node_s ccb_list_node_t;

struct ccb_list_s {
    int          length;
    ccb_list_node_t* head;
    ccb_list_node_t* tail;
};

/*
 * Type: ccb_table_t
 *  A key value associative table
 */
typedef struct ccb_table_s ccb_table_t;

struct ccb_table_s {
    ccb_list_t* list;
    ccb_table_t* parent;
};

/*
 * Function: ccb_table_create
 *  Creates a ccb_table_t object
 */
void* ccb_table_create(void* parent);

/*
 * Funciton: ccb_table_find
 *  Searches for a given value in the table based on the
 *  key associated with it.
 */
void* ccb_table_find(ccb_table_t* table, const char* key);

/*
 * Function: ccb_table_insert
 *  Inserts a value for the given key as an entry in the
 *  table.
 */
void ccb_table_insert(ccb_table_t* table, char* key, void* value);

/*
 * Function: ccb_table_parent
 *  Returns the parent opaque object for the given table to
 *  be used as the argument to a new table.
 */
void* ccb_table_parent(ccb_table_t* table);

/*
 * Function: ccb_table_values
 *  Generates a list of all the values in the table, useful for
 *  iterating over the values.
 */
ccb_list_t* ccb_table_values(ccb_table_t* table);

/*
 * Function: ccb_table_keys
 *  Generate a list of all the keys in the table, useful for
 *  iteration over the keys.
 */
ccb_list_t* ccb_table_keys(ccb_table_t* table);

/*
 * Macro: CCB_SENTINEL_TABLE
 *  Initialize an empty table in place
 */
/*#define CCB_SENTINEL_TABLE ((ccb_table_t) { \
    .list   = ccb_list_empty(ccb),   //&CCB_SENTINEL_LIST    \
    .parent = NULL                  \
})*/
ccb_table_t* ccb_table_empty();

#define CCB_MIN(A, B) (((A) < (B)) ? (A) : (B))
#define CCB_MAX(A, B) (((A) > (B)) ? (A) : (B))

 /*
  * Function: ccb_memory_allocate
  * Allocate some memory
  */
void* ccb_memory_allocate(size_t bytes);

int ccb_strcasecmp(const char* s1, const char* s2);
int ccb_strncasecmp(const char* s1, const char* s2, size_t n);

/*
 * Type: ccb_lexer_token_type_t
 *  Type to describe a tokens type.
 *
 *  Remarks:
 *   Implemented as a typedef of an enumeration, ccb_lexer_token_t
 *   is used to describe the current lexer token. The following
 *   tokens exist (as constants).
 *
 *  Tokens:
 *    CCB_LEXER_TOKEN_IDENTIFIER        - Identifier
 *    CCB_LEXER_TOKEN_PUNCT             - Language punctuation
 *    CCB_LEXER_TOKEN_CHAR              - Character literal
 *    CCB_LEXER_TOKEN_STRING            - String literal
 *    CCB_LEXER_TOKEN_NUMBER            - Number (of any type)
 *    CCB_LEXER_TOKEN_EQUAL             - Equal
 *    CCB_LEXER_TOKEN_LEQUAL            - Lesser-or-equal
 *    CCB_LEXER_TOKEN_GEQUAL            - Greater-or-equal
 *    CCB_LEXER_TOKEN_NEQUAL            - Not-equal
 *    CCB_LEXER_TOKEN_INCREMENT         - Pre/post increment
 *    CCB_LEXER_TOKEN_DECREMENT         - Pre/post decrement
 *    CCB_LEXER_TOKEN_ARROW             - Pointer arrow `->`
 *    CCB_LEXER_TOKEN_LSHIFT            - Left shift
 *    CCB_LEXER_TOKEN_RSHIFT            - Right shift
 *    CCB_LEXER_TOKEN_COMPOUND_ADD      - Compound-assignment addition
 *    CCB_LEXER_TOKEN_COMPOUND_SUB      - Compound-assignment subtraction
 *    CCB_LEXER_TOKEN_COMPOUND_MUL      - Compound-assignment multiplication
 *    CCB_LEXER_TOKEN_COMPOUND_DIV      - Compound-assignment division
 *    CCB_LEXER_TOKEN_COMPOUND_MOD      - Compound-assignment moduluas
 *    CCB_LEXER_TOKEN_COMPOUND_OR       - Compound-assignment bit-or
 *    CCB_LEXER_TOKEN_COMPOUND_XOR      - Compound-assignment bit-xor
 *    CCB_LEXER_TOKEN_COMPOUND_LSHIFT   - Compound-assignment left-shift
 *    LEXER_TOKEN_COMPOUND_RSHIFt   - Compound-assignment right-shift
 *    CCB_LEXER_TOKEN_AND               - Logical and
 *    CCB_LEXER_TOKEN_OR                - Logical or
 */
enum ccb_lexer_token_enum {
    CCB_LEXER_TOKEN_IDENTIFIER,
    CCB_LEXER_TOKEN_PUNCT,
    CCB_LEXER_TOKEN_CHAR,
    CCB_LEXER_TOKEN_STRING,
    CCB_LEXER_TOKEN_NUMBER,
    CCB_LEXER_TOKEN_EQUAL = 0x200,
    CCB_LEXER_TOKEN_LEQUAL,
    CCB_LEXER_TOKEN_GEQUAL,
    CCB_LEXER_TOKEN_NEQUAL,
    CCB_LEXER_TOKEN_INCREMENT,
    CCB_LEXER_TOKEN_DECREMENT,
    CCB_LEXER_TOKEN_ARROW,
    CCB_LEXER_TOKEN_LSHIFT,
    CCB_LEXER_TOKEN_RSHIFT,
    CCB_LEXER_TOKEN_COMPOUND_ADD,
    CCB_LEXER_TOKEN_COMPOUND_SUB,
    CCB_LEXER_TOKEN_COMPOUND_MUL,
    CCB_LEXER_TOKEN_COMPOUND_DIV,
    CCB_LEXER_TOKEN_COMPOUND_MOD,
    CCB_LEXER_TOKEN_COMPOUND_AND,
    CCB_LEXER_TOKEN_COMPOUND_OR,
    CCB_LEXER_TOKEN_COMPOUND_XOR,
    CCB_LEXER_TOKEN_COMPOUND_LSHIFT,
    CCB_LEXER_TOKEN_COMPOUND_RSHIFT,
    CCB_LEXER_TOKEN_AND,
    CCB_LEXER_TOKEN_OR
};
typedef enum ccb_lexer_token_enum ccb_lexer_token_type_t;

/*
 * Class: ccb_lexer_token_t
 *  Describes a token in the token stream
 */
typedef struct {
    /*
     * Variable: type
     *  The token type
     */
    ccb_lexer_token_type_t type;

    union {
        long  integer;
        int   punct;
        char* string;
        char  character;
    };
} ccb_lexer_token_t;

/*
 * Function: ccb_lexer_islong
 *  Checks for a given string if it's a long-integer-literal.
 *
 * Parameters:
 *  string  - The string to check
 *
 * Remarks:
 *  Returns `true` if the string is a long-literal,
 *  `false` otherwise.
 */
bool ccb_lexer_islong(ccb_t* ccb, char* string);

/*
 * Function: ccb_lexer_isint
 *  Checks for a given string if it's a int-integer-literal.
 *
 * Parameters:
 *  string  - The string to check
 *
 * Remarks:
 *  Returns `true` if the string is a int-literal,
 * `false` otherwise.
 */
bool ccb_lexer_isint(ccb_t* ccb, char* string);

/*
 * Function: ccb_lexer_isfloat
 *  Checks for a given string if it's a floating-point-literal.
 *
 * Parameters:
 *  string  - The string to check
 *
 * Remarks:
 *  Returns `true` if the string is floating-point-literal,
 * `false` otherwise.
 */
bool ccb_lexer_isfloat(ccb_t* ccb, char* string);

/*
 * Function: ccb_lexer_ispunct
 *  Checks if a given token is language punctuation and matches.
 *
 * Parameters:
 *  token   - The token to test
 *  c       - The punction to test if matches
 *
 * Remarks:
 *  Returns `true` if the given token is language punctuation and
 *  matches *c*.
 */
bool ccb_lexer_ispunct(ccb_t* ccb, ccb_lexer_token_t* token, int c);

/*
 * Function: ccb_lexer_unget
 *  Undo the given token in the token stream.
 *
 * Parameters:
 *  token   - The token to unget
 */
void ccb_lexer_unget(ccb_t* ccb, ccb_lexer_token_t* token);

/*
 * Function: ccb_lexer_next
 *  Get the next token in the token stream.
 *
 * Returns:
 *  The next token in the token stream or NULL
 *  on failure or EOF.
 */
ccb_lexer_token_t* ccb_lexer_next(ccb_t* ccb);

/*
 * Function: ccb_lexer_peek
 *  Look at the next token without advancing the stream.
 *
 * Returns:
 *  The next token without advancing the token stream or NULL on failure
 *  or EOF.
 *
 * Remarks:
 *  The function will peek ahead to see the next token in the stream
 *  without advancing the lexer state.
 */
ccb_lexer_token_t* ccb_lexer_peek(ccb_t* ccb);

/*
 * Function: ccb_lexer_tokenstr
 *  Convert a token to a human-readable representation
 *
 * Parameters:
 *  token   - The token to convert
 *
 * Returns:
 *  A string representation of the token or NULL on failure.
 */
char* ccb_lexer_tokenstr(ccb_t* ccb, ccb_lexer_token_t* token);

typedef struct ccb_ast_s ccb_ast_t;

/*
 * Type: ccb_ast_type_t
 *  The type of ast node
 *
 *  Constants:
 *
 *  CCB_AST_TYPE_LITERAL                 - Literal
 *  CCB_AST_TYPE_STRING                  - String literal
 *  CCB_AST_TYPE_VAR_LOCAL               - Local variable
 *  CCB_AST_TYPE_VAR_GLOBAL              - Global variable
 *  CCB_AST_TYPE_CALL                    - Direct function call
 *  CCB_AST_TYPE_PTRCALL                 - Indirect function call
 *  CCB_AST_TYPE_FUNCTION                - Function
 *  CCB_AST_TYPE_PROTOTYPE               - Prototype
 *  CCB_AST_TYPE_DECLARATION             - Declaration
 *  CCB_AST_TYPE_INITIALIZER             - Initializer
 *  CCB_AST_TYPE_STRUCT                  - Structure
 *  CCB_AST_TYPE_ADDRESS                 - Address of operation
 *  CCB_AST_TYPE_DEREFERENCE             - Pointer dereference
 *  CCB_AST_TYPE_EXPRESSION_TERNARY      - Ternary expression
 *  CCB_AST_TYPE_EXPRESSION_CAST         - Type cast expression
 *  CCB_AST_TYPE_STATEMENT_IF            - If statement
 *  CCB_AST_TYPE_STATEMENT_FOR           - For statement
 *  CCB_AST_TYPE_STATEMENT_WHILE         - While statement
 *  CCB_AST_TYPE_STATEMENT_DO            - Do statement
 *  CCB_AST_TYPE_STATEMENT_SWITCH        - Switch statement
 *  CCB_AST_TYPE_STATEMENT_CASE          - Switch statement case
 *  CCB_AST_TYPE_STATEMENT_DEFAULT       - Switch statement default case
 *  CCB_AST_TYPE_STATEMENT_RETURN        - Return statement
 *  CCB_AST_TYPE_STATEMENT_BREAK         - Break statement
 *  CCB_AST_TYPE_STATEMENT_CONTINUE      - Continue statement
 *  CCB_AST_TYPE_STATEMENT_COMPOUND      - Compound statement
 *  CCB_AST_TYPE_STATEMENT_GOTO          - Goto statement
 *  CCB_AST_TYPE_STATEMENT_LABEL         - Goto statement label
 *  CCB_AST_TYPE_POST_INCREMENT          - Post increment operation
 *  CCB_AST_TYPE_POST_DECREMENT          - Post decrement operation
 *  CCB_AST_TYPE_PRE_INCREMENT           - Pre increment operation
 *  CCB_AST_TYPE_PRE_DECREMENT           - Pre decrement operation
 *  CCB_AST_TYPE_LSHIFT                  - Left shift operation
 *  CCB_AST_TYPE_RSHIFT                  - Right shift operation
 *  CCB_AST_TYPE_EQUAL                   - Equality condition
 *  CCB_AST_TYPE_GEQUAL                  - Greater-or-equal condition
 *  CCB_AST_TYPE_LEQUAL                  - Less-or-equal condition
 *  CCB_AST_TYPE_NEQUAL                  - Not-equal condition
 *  CCB_AST_TYPE_AND                     - Logical-and operation
 *  CCB_AST_TYPE_OR                      - Logical-or operation
 */
typedef enum {
    CCB_AST_TYPE_LITERAL = 0x100,
    CCB_AST_TYPE_STRING,
    CCB_AST_TYPE_VAR_LOCAL,
    CCB_AST_TYPE_VAR_GLOBAL,
    CCB_AST_TYPE_CALL,
    CCB_AST_TYPE_PTRCALL,
    CCB_AST_TYPE_FUNCTION,
    CCB_AST_TYPE_PROTOTYPE,
    CCB_AST_TYPE_DECLARATION,
    CCB_AST_TYPE_INITIALIZER,
    CCB_AST_TYPE_STRUCT,
    CCB_AST_TYPE_ADDRESS,
    CCB_AST_TYPE_DEREFERENCE,
    CCB_AST_TYPE_EXPRESSION_TERNARY,
    CCB_AST_TYPE_EXPRESSION_CAST,
    CCB_AST_TYPE_STATEMENT_IF,
    CCB_AST_TYPE_STATEMENT_FOR,
    CCB_AST_TYPE_STATEMENT_WHILE,
    CCB_AST_TYPE_STATEMENT_DO,
    CCB_AST_TYPE_STATEMENT_SWITCH,
    CCB_AST_TYPE_STATEMENT_CASE,
    CCB_AST_TYPE_STATEMENT_DEFAULT,
    CCB_AST_TYPE_STATEMENT_RETURN,
    CCB_AST_TYPE_STATEMENT_BREAK,
    CCB_AST_TYPE_STATEMENT_CONTINUE,
    CCB_AST_TYPE_STATEMENT_COMPOUND,
    CCB_AST_TYPE_STATEMENT_GOTO,
    CCB_AST_TYPE_STATEMENT_LABEL,
    CCB_AST_TYPE_POST_INCREMENT,
    CCB_AST_TYPE_POST_DECREMENT,
    CCB_AST_TYPE_PRE_INCREMENT,
    CCB_AST_TYPE_PRE_DECREMENT,
    CCB_AST_TYPE_LSHIFT,
    CCB_AST_TYPE_RSHIFT,
    CCB_AST_TYPE_EQUAL,
    CCB_AST_TYPE_GEQUAL,
    CCB_AST_TYPE_LEQUAL,
    CCB_AST_TYPE_NEQUAL,
    CCB_AST_TYPE_AND,
    CCB_AST_TYPE_OR
} ccb_ast_type_t;

/*
 * Type: ccb_type_t
 *  Type describing the ast type.
 *
 *  Constants:
 *
 *  CCB_TYPE_VOID       - void
 *  CCB_TYPE_CHAR       - char
 *  CCB_TYPE_SHORT      - short
 *  CCB_TYPE_INT        - int
 *  CCB_TYPE_LONG       - long
 *  CCB_TYPE_LLONG      - long long
 *  CCB_TYPE_DOUBLE     - double
 *  CCB_TYPE_LDOUBLE    - long double
 *  CCB_TYPE_ARRAY      - array (also contains a ccb_type_t for base type)
 *  CCB_TYPE_POINTER    - pointer (also contains a ccb_type_t for base type)
 *  CCB_TYPE_STRUCTURE  - structure (user defined)
 *  CCB_TYPE_FUNCTION   - function  (user defined)
 *  TYPE_CECL       - used by the parser for dealing with declarations
 */
typedef enum {
    CCB_TYPE_VOID,
    CCB_TYPE_CHAR,
    CCB_TYPE_SHORT,
    CCB_TYPE_INT,
    CCB_TYPE_LONG,
    CCB_TYPE_LLONG,
    CCB_TYPE_FLOAT,
    CCB_TYPE_DOUBLE,
    CCB_TYPE_LDOUBLE,
    CCB_TYPE_ARRAY,
    CCB_TYPE_POINTER,
    CCB_TYPE_STRUCTURE,
    CCB_TYPE_FUNCTION,
    CCB_TYPE_CDECL
#ifdef CCB_X_OBJC
    , CCB_TYPE_ID
#endif
} ccb_type_t;

/*
 * Type: ccb_ast_data_type_t
 *  Type describing the indice into `ast_data_table`
 *
 *  Constants:
 *
 *  CCB_AST_DATA_VOID       - void
 *  CCB_AST_DATA_LONG       - long
 *  CCB_AST_DATA_LLONG      - long long
 *  CCB_AST_DATA_INT        - int
 *  CCB_AST_DATA_SHORT      - short
 *  CCB_AST_DATA_CHAR       - char
 *  CCB_AST_DATA_FLOAT      - float
 *  CCB_AST_DATA_DOUBLE     - double
 *  CCB_AST_DATA_LDOUBLE    - long double
 *  CCB_AST_DATA_UINT       - unsigned int
 *  CCB_AST_DATA_ULONG      - unsigned long
 *  CCB_AST_DATA_ULLONG     - unsigned long long
 *  CCB_AST_DATA_FUNCTION   - function (current)
 */
typedef enum {
    CCB_AST_DATA_VOID,
    CCB_AST_DATA_LONG,
    CCB_AST_DATA_LLONG,
    CCB_AST_DATA_INT,
    CCB_AST_DATA_SHORT,
    CCB_AST_DATA_CHAR,
    CCB_AST_DATA_FLOAT,
    CCB_AST_DATA_DOUBLE,
    CCB_AST_DATA_LDOUBLE,
    CCB_AST_DATA_UINT,
    CCB_AST_DATA_ULONG,
    CCB_AST_DATA_ULLONG,
    CCB_AST_DATA_FUNCTION,
#ifdef CCB_X_OBJC
    CCB_AST_DATA_ID,
#endif
    CCB_AST_DATA_COUNT
} ccb_ast_data_type_t;

/*
 * Type: ccb_cdecl_t
 *  Describes type of declarations
 *
 *  Constants:
 *
 *  CCB_CDECL_BODY          - function body
 *  CCB_CDECL_PARAMETER     - parameters (with name)
 *  CCB_CDECL_OBJCPARAMETER - objc parameter (type in brackets then name)
 *  CCB_CDECL_TYPEONLY      - parameters (without name)
 *  CCB_CDECL_CAST          - cast
 */
typedef enum {
    CCB_CDECL_BODY = 1,
    CCB_CDECL_PARAMETER,
#ifdef CCB_X_OBJC
    CCB_CDECL_OBJCPARAMETER,
#endif
    CCB_CDECL_TYPEONLY,
    CCB_CDECL_CAST
} ccb_cdecl_t;

/*
 * Type: ccb_storage_t
 *  Describes the storage class for a given variable
 *
 *  Constants:
 *
 *  CCB_STORAGE_TYPEDEF     - typedef to another type
 *  CCB_STORAGE_EXTERN      - external linkage
 *  CCB_STORAGE_STATIC      - static storage
 *  CCB_STORAGE_AUTO        - automatic storage (implicit)
 *  CCB_STORAGE_REGISTER    - make use of register for storage
 */
typedef enum {
    CCB_STORAGE_TYPEDEF = 1,
    CCB_STORAGE_EXTERN,
    CCB_STORAGE_STATIC,
    CCB_STORAGE_AUTO,
    CCB_STORAGE_REGISTER
} ccb_storage_t;

/*
 * Struct: ccb_data_type_t
 *  A structure that describes a data type.
 */
typedef struct ccb_data_type_s ccb_data_type_t;
struct ccb_data_type_s {
    /*
     * Variable: type
     *  The type of the data type.
     *
     *  See <ccb_type_t> Constants for a list of
     *  valid constant values.
     */
    ccb_type_t type;

    /*
     * Variable: type
     *  The size of the given data data
     */
    int size;

    /*
     * Variable: sign
     *  Describes if the type is signed or unsigned.
     *
     *  Contains `true` when signed, otherwise `false.
     */
    bool sign;

    /*
     * Variable: isstatic
     *  True if when static (global only)
     */
    bool isstatic;

    /*
     * Variable: length
     *  Instances of the data type.
     *
     *  When used as a base-type, i.e not an array; this will be
     *  1, otherwise it will be the length of the array, or -1
     *  if the size of the array is unknown.
     */
    int length;

    /*
     * Variable: pointer
     *  Pointer to pointer type if pointer
     *
     * When the variable is a pointer type, this will point to another
     * data type that describes the base type of the pointer, NULL other-
     * wise.
     */
    ccb_data_type_t* pointer;

    /* structure */
    struct {
        /* Variable: classname
         *  Name of the declared class, or null (NOTE: This may be dynamically resolved).
         */
        const char* classname;

        /* Variable: supername
         *  Name of the declared superclass (NOTE: This may be dynamically resolved).
         */
        const char* supername;

        /* Variable: supertype
         *  Pointer to the superclass type, if defined.
         */
        //ccb_data_type_t* supertype;

        /*
         * Variable: fields
         *  Pointer to a table of fields (if structure)
         */
        ccb_table_t* fields;

        /*
         * Variable: offset
         *  Offset of the given field in a structure (if a structure base type)
         */
        int offset;

        /*
         * Variable: isstruct
         *  If we're dealing with a structure this will be true, false
         *  otherwise.
         */
        bool isstruct;
    };

    /* function */
    struct {
        /*
         * Variable: returntype
         *  Pointer to a data type which describes the return type
         *  of the function (if a function)
         */
        ccb_data_type_t* returntype;

        /*
         * Variable: parameters
         *  Pointer to a list of parameters for a function.
         */
        ccb_list_t* parameters;

        /*
         * Variable: hasdots
         *  Describes if the given function is variable-argument.
         *
         *  Contains the value `true` when the function has
         *  three dots `...` in it's prototype, otherwise `false`.
         */
        bool hasdots;

        int callconv;
    };
};

/*
 * Struct: ccb_ast_string_t
 *  The *CCB_AST_TYPE_STRING* ast node.
 */
typedef struct {
    /*
     * Variable: data
     *  String contents
     */
    char* data;

    /*
     * Variable: label
     *  Name of the label associated with the string.
     */
    char* label;
} ccb_ast_string_t;

/*
 * Struct: ccb_ast_variable_t
 *  The *CCB_AST_TYPE_VAR_LOCAL* and *CCB_AST_TYPE_VAR_GLOBAL* ast node.
 */
typedef struct {
    /*
     * Variable: name
     *  Name of the variable
     */
    char* name;

    /*
     * Variable: off
     *  Offset of the variable on the stack.
     */
    int off;

    /*
     * Variable: label
     *  Name of the label associated with the variable.
     */
    char* label;

    /*
     * Variable: init
     *  Compound literal list for initialization
     */
    ccb_list_t* init;


    /*
     * Variable: isclassobj
     *  This is set to true for global variables representing class objects, so that initialisation can be done at send-time.
     */
    bool isclassobj;
} ccb_ast_variable_t;

/*
 * Struct ccb_ast_function_call_t
 *  Function call
 *
 *  Remarks:
 *      Not associated with any node. Instead describes the
 *      data associated with a function call for *ccb_ast_function_t*
 */
typedef struct {
    /*
     * Variable: args
     *  Pointer to a list of arguments for a function call
     */
    ccb_list_t* args;

    /*
     * Variable: paramtypes
     *  Pointer to a list of parameter types for the function call.
     */
    ccb_list_t* paramtypes;

    /*
     * Variable: callable
     *  Expression resulting in callable value (function pointer) for a PTRCALL.
     */
    ccb_ast_t* callable;
} ccb_ast_function_call_t;

/*
 * Struct: ccb_ast_function_t
 *  The *CCB_AST_TYPE_FUNCTION* ast node.
 */
typedef struct {
    /*
     * Variable: name
     *  The function name
     */
    char* name;

    /*
     * Variable: call
     *  Data associated with a function call.
     */
    ccb_ast_function_call_t call;

    /*
     * Variable: params
     *  Pointer to a list of parameters.
     */
    ccb_list_t* params;

    /*
     * Variable: locals
     *  Pointer to a list of locals.
     */
    ccb_list_t* locals;

    /*
     * Variable: body
     *  Pointer to an ast node which describes the body.
     *
     * Remarks:
     *  A body is usually composed of a serise of ast nodes,
     *  typically a compound expression, but could also contain
     *  nested compound expressions. Think of this as a pointer
     *  to the head of the beginning of a serise of basic-blocks
     *  which are the forming of the function body.
     */
    ccb_ast_t* body;
} ccb_ast_function_t;

/*
 * Struct: ccb_ast_unary_t
 *  Represents a unary operation in the AST tree
 */
typedef struct {
    /*
     * Variable: operand
     *  Pointer to the operand the unary operation is to
     *  be performed on.
     */
    ccb_ast_t* operand;
} ccb_ast_unary_t;

/*
 * Struct: ccb_ast_decl_t
 *  Represents a declaration in the AST tree
 */
typedef struct {
    /*
     * Variable: var
     *  Pointer to the variable node associated with the
     *  declaration.
     */
    ccb_ast_t* var;

    /*
     * Variable: init
     *  When the declaration includes an initialization this points
     *  to a initlization list.
     */
    ccb_list_t* init;
} ccb_ast_decl_t;

/*
 * Struct: ccb_ast_ifthen_t
 *  Represents a if-then node in the AST tree.
 *
 * Remarks:
 *  Describes a two-branch gaurded by conditional test node in the AST
 *  tree for implementing ternary expressions and if statements.
 */
typedef struct {
    /*
     * Variable: cond
     *  The condition node
     */
    ccb_ast_t* cond;

    /*
     * Variable: then
     *  Basic block for truth path in branch
     */
    ccb_ast_t* then;

    /*
     * Variable: last
     *  Basic block for false path in branch
     */
    ccb_ast_t* last;
} ccb_ast_ifthen_t;

/*
 * Struct: ccb_ast_for_t
 *  Represents a for-loop node in the AST tree.
 *
 * Remarks:
 *  Standard for loop with precondition / initilization expression,
 *  conditionally testsed, and post step / expression, ergo
 *  for(init; cond; step) body;
 */
typedef struct {
    /* Variable: init */
    ccb_ast_t* init;
    /* Variable: cond */
    ccb_ast_t* cond;
    /* Variable: step */
    ccb_ast_t* step;
    /* Variable: body */
    ccb_ast_t* body;
} ccb_ast_for_t;


/*
 * Struct: ccb_ast_init_t
 *  Represents an initializer in the AST tree.
 *
 * Remarks:
 *  Represents array initializer lists, as well as aggregate initializer
 *  lists for structure, enum and union. Also represents a designated
 *  initializer for a structure.
 */
typedef struct {
    /* Variable: value */
    ccb_ast_t* value;

    /* Variable: offset */
    int          offset;

    /* Variable: type */
    ccb_data_type_t* type;
} ccb_ast_init_t;

/*
 * Struct: ccb_ast_switch_t
 *  Represents a switch statement in the AST tree.
 */
typedef struct {
    /* Variable: expr */
    ccb_ast_t* expr;
    /* Variable: body */
    ccb_ast_t* body;
} ccb_ast_switch_t;

/*
 * Struct: ccb_ast_goto_t
 *  Represents a goto statement (or label) in the AST tree.
 */
typedef struct {
    /*
     * Variable: label
     *  When not used as a goto statement, describes the name of a label
     *  that may be 'gone to' with 'goto'
     */
    char* label;

    /*
     * Variable: where
     *  Where to go (label wise) for a goto statement.
     */
    char* where;
} ccb_ast_goto_t;

/*
 * Struct: ccb_ast_t
 *  The monolthic ast tree and node at the same time.
 *
 * Remarks:
 *  The ast tree is just a doubly-linked list of ast nodes which are
 *  capable of being all the possible ast nodes at once. This is
 *  acomplished with a rather large union of all ast nodes. The only
 *  thing that declares what a node actually is, is the nodes type
 *  member. This is beneficial to keeping the complexity of the AST
 *  tree down, while also keeping memory footprint low. One more
 *  interesting aspect of this is the ability to have the AST tree
 *  nodes (next, prev), which make up the doubly-linked list part
 *  of the same union, giving us a free way to terminate the tree
 *  without using additional space to determine it.
 */
struct ccb_ast_s {
    int          type;
    ccb_data_type_t* ctype;

    union {
        int             casevalue;
        long long       integer;
        char            character;
        ccb_ast_string_t    string;
        ccb_ast_variable_t  variable;
        ccb_ast_function_t  function;
        ccb_ast_unary_t     unary;
        ccb_ast_decl_t      decl;
        ccb_ast_ifthen_t    ifstmt;
        ccb_ast_for_t       forstmt;
        ccb_ast_switch_t    switchstmt;
        ccb_ast_t* returnstmt;
        ccb_list_t* compound;
        ccb_ast_init_t      init;
        ccb_ast_goto_t      gotostmt;

        struct {
            ccb_ast_t* left;
            ccb_ast_t* right;
        };

        struct {
            ccb_ast_t* structure;
            char* field;
            ccb_data_type_t* fieldtype;
        };

        struct {
            double value;
            char* label;
        } floating;

    };
};

extern ccb_data_type_t* ccb_ast_data_table[CCB_AST_DATA_COUNT];

extern ccb_list_t* ccb_ast_floats;
extern ccb_list_t* ccb_ast_strings;
extern ccb_list_t* ccb_ast_locals;
extern ccb_list_t* ccb_ast_gotos;
extern ccb_table_t* ccb_ast_globalenv;
extern ccb_table_t* ccb_ast_localenv;
extern ccb_table_t* ccb_ast_structures;
extern ccb_table_t* ccb_ast_unions;
extern ccb_table_t* ccb_ast_labels;

/*
 * Function: ccb_util_init
 *  Initialises any structures required for lists or other utilities.
 */
void ccb_util_init(ccb_t* ccb);

/*
 * Function: ccb_ast_init
 *  Initialises the AST's internal structures according to arch_xxx results.
 */
void ccb_ast_init(ccb_t* ccb);

/*
 * Function: ccb_ast_structure_reference
 *  Creates an structure reference of a given type for a given field
 *
 * Parameters:
 *  type      - The type of the field for reference
 *  structure - The structure that contains said field to be referenced
 *  name      - The name of the field in that structure to reference
 *
 * Returns:
 *  An ast node referencing that field in that paticular structure on
 *  success, otherwise NULL.
 */
ccb_ast_t* ccb_ast_structure_reference(ccb_t* ccb, ccb_data_type_t* type, ccb_ast_t* structure, char* name);

/*
 * Function: ccb_ast_structure_field
 *  Copies a given field data type and changes it offset
 *
 * Parameters:
 *  type   - Pointer to the structure field data type
 *  offset - The offset of the copied data type in the structure
 *
 * Returns:
 *  A copy of the structure fields data type with the supplied
 *  offset on success, NULL otherwise.
 */
ccb_data_type_t* ccb_ast_structure_field(ccb_t* ccb, ccb_data_type_t* type, int offset);

/*
 * Function: ccb_ast_structure_new
 *  Creates a structure data type
 *
 * Parameters;
 *  field    - A table of ccb_data_type_t fields for the structure
 *  size     - The size of the structure
 *  isstruct - true if structure, false if structure-like
 *
 * Returns:
 *  A new structure data type with the specified fields and size on
 *  success, NULL otherwise.
 */
ccb_data_type_t* ccb_ast_structure_new(ccb_t* ccb, ccb_table_t* fields, int size, bool isstruct);


ccb_ast_t* ccb_ast_new_unary(ccb_t* ccb, int type, ccb_data_type_t* data, ccb_ast_t* operand);
ccb_ast_t* ccb_ast_new_binary(ccb_t* ccb, int type, ccb_ast_t* left, ccb_ast_t* right);
ccb_ast_t* ccb_ast_new_integer(ccb_t* ccb, ccb_data_type_t* type, long long int value);
ccb_ast_t* ccb_ast_new_floating(ccb_t* ccb, ccb_data_type_t*, double value);
ccb_ast_t* ccb_ast_new_char(ccb_t* ccb, char value);
ccb_ast_t* ccb_ast_new_string(ccb_t* ccb, char* value);
ccb_ast_t* ccb_ast_new_label(ccb_t* ccb, char*);

char* ccb_ast_label(ccb_t* ccb);

ccb_ast_t* ccb_ast_declaration(ccb_t* ccb, ccb_ast_t* var, ccb_list_t* init);
ccb_ast_t* ccb_ast_variable_local(ccb_t* ccb, ccb_data_type_t* type, char* name);
ccb_ast_t* ccb_ast_variable_global(ccb_t* ccb, ccb_data_type_t* type, char* name);
ccb_ast_t* ccb_ast_call(ccb_t* ccb, ccb_data_type_t* type, char* name, ccb_list_t* args, ccb_list_t* paramtypes);
ccb_ast_t* ccb_ast_ptrcall(ccb_t* ccb, ccb_data_type_t* type, char* name, ccb_ast_t* callable, ccb_list_t* args, ccb_list_t* paramtypes);
ccb_ast_t* ccb_ast_function(ccb_t* ccb, ccb_data_type_t* type, char* name, ccb_list_t* params, ccb_ast_t* body, ccb_list_t* locals);
ccb_ast_t* ccb_ast_initializer(ccb_t* ccb, ccb_ast_t*, ccb_data_type_t*, int);
ccb_ast_t* ccb_ast_if(ccb_t* ccb, ccb_ast_t* cond, ccb_ast_t* then, ccb_ast_t* last);
ccb_ast_t* ccb_ast_for(ccb_t* ccb, ccb_ast_t* init, ccb_ast_t* cond, ccb_ast_t* step, ccb_ast_t* body);
ccb_ast_t* ccb_ast_while(ccb_t* ccb, ccb_ast_t* cond, ccb_ast_t* body);
ccb_ast_t* ccb_ast_do(ccb_t* ccb, ccb_ast_t* cond, ccb_ast_t* body);
ccb_ast_t* ccb_ast_return(ccb_t* ccb, ccb_data_type_t* returntype, ccb_ast_t* val);
ccb_ast_t* ccb_ast_compound(ccb_t* ccb, ccb_list_t* statements);
ccb_ast_t* ccb_ast_ternary(ccb_t* ccb, ccb_data_type_t* type, ccb_ast_t* cond, ccb_ast_t* then, ccb_ast_t* last);
ccb_ast_t* ccb_ast_switch(ccb_t* ccb, ccb_ast_t* expr, ccb_ast_t* body);
ccb_ast_t* ccb_ast_case(ccb_t* ccb, int value);
ccb_ast_t* ccb_ast_goto(ccb_t* ccb, char*);
ccb_ast_t* ccb_ast_make(ccb_t* ccb, int type);

ccb_data_type_t* ccb_ast_prototype(ccb_t* ccb, ccb_data_type_t* returntype, ccb_list_t* paramtypes, bool dots);
ccb_data_type_t* ccb_ast_pointer(ccb_t* ccb, ccb_data_type_t* type);
ccb_data_type_t* ccb_ast_array(ccb_t* ccb, ccb_data_type_t* type, int size);
ccb_data_type_t* ccb_ast_array_convert(ccb_t* ccb, ccb_data_type_t* ast);
ccb_data_type_t* ccb_ast_result_type(ccb_t* ccb, int op, ccb_data_type_t* a, ccb_data_type_t* b);

const char* ccb_ast_type_string(ccb_t* ccb, ccb_data_type_t* type);
bool ccb_ast_type_integer(ccb_t* ccb, ccb_data_type_t* type);
bool ccb_ast_type_floating(ccb_t* ccb, ccb_data_type_t* type);
ccb_data_type_t* ccb_ast_type_copy(ccb_t* ccb, ccb_data_type_t* type);
ccb_data_type_t* ccb_ast_type_copy_incomplete(ccb_t* ccb, ccb_data_type_t* type);
ccb_data_type_t* ccb_ast_type_create(ccb_t* ccb, ccb_type_t type, bool sign);
ccb_data_type_t* ccb_ast_type_stub(ccb_t* ccb);


char* ccb_ast_string(ccb_t* ccb, ccb_ast_t* ast);

/* NOTE: The ccb_target interface will probably be reworked eventually to allow multiple targets
 *       to be managed easier. This will probably involve simplifying or generalising some features
 *       and having the code generators operate via callback functions. Right now all targets are
 *       implemented in the same backend, which has various options.
 */

void ccb_target_init(ccb_t* ccb);

#define CCB_ARCH_FAMILY_X86		1   // Mostly/fully implemented but only partly tested, 64-bit mode only
#define CCB_ARCH_FAMILY_ARM		2   // Not implemented yet
#define CCB_ARCH_FAMILY_RISCV	3   // Partly implemented
#define CCB_ARCH_FAMILY_GENERIC 4   // Mostly implemented but mostly untested, just generates a virtual instruction set
#define CCB_ARCH_FAMILY_GEN1    5   // Partly implemented, aimed at working with an earlier version of my CPU core

int ccb_target_family(ccb_t* ccb);

size_t ccb_target_type_size_char(ccb_t* ccb);
size_t ccb_target_type_size_short(ccb_t* ccb);
size_t ccb_target_type_size_int(ccb_t* ccb);
size_t ccb_target_type_size_long(ccb_t* ccb);
size_t ccb_target_type_size_llong(ccb_t* ccb);
size_t ccb_target_type_size_float(ccb_t* ccb);
size_t ccb_target_type_size_double(ccb_t* ccb);
size_t ccb_target_type_size_ldouble(ccb_t* ccb);
size_t ccb_target_type_size_pointer(ccb_t* ccb);

/*
 * Function: ccb_target_alignment()
 *  The default alignment of structure elements (padding) for the given
 *  architecture / ABI
 */
size_t ccb_target_alignment(ccb_t* ccb);

/*
 * Function: ccb_target_callregisters()
 *  The maximum number of registers to place a direct or indirect
 *  function call for the given architecture / ABI, after which stack
 *  space will be used.
 */
int ccb_target_callregisters(ccb_t* ccb);

/*
 * Function: ccb_target_callregister()
 *  The name of the register used to store the direct argument at the given index (which must be >= 0 and < ccb_target_callregisters()).
 */
const char* ccb_target_callregister(ccb_t* ccb, int idx);

const char* ccb_target_r0(ccb_t* ccb);

const char* ccb_target_r1(ccb_t* ccb);

const char* ccb_target_sp(ccb_t* ccb);

const char* ccb_target_bp(ccb_t* ccb);

#define CCB_TARGET_ASMFMT_GAS		1
#define CCB_TARGET_ASMFMT_FASM		2
#define CCB_TARGET_ASMFMT_RAW       3

int ccb_target_asmfmt(ccb_t* ccb);

#define CCB_TARGET_BINFMT_ELF		1
#define CCB_TARGET_BINFMT_FLAT		2

int ccb_target_binfmt(ccb_t* ccb);

#define CCB_TARGET_CALLCONV_STANDARD    1
#define CCB_TARGET_CALLCONV_WINDOWS     2

int ccb_target_callconv(ccb_t* ccb);

/*
 * Function: ccb_compile_error
 *  Write compiler error diagnostic to stderr, formatted
 *
 * Parameters:
 *  fmt - Standard format specification string
 *  ... - Additional variable arguments
 *
 * Remarks:
 *  This function does not return, it kills execution via
 *  exit(1);
 */
void ccb_compile_error_impl(ccb_t* ccb);//, const char* fmt, ...);
#define ccb_compile_error(ccb,...) \
    do{fprintf(stderr, "ERROR: "); fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");ccb_compile_error_impl(ccb);}while(0);

/*
 * Function: ccb_compile_warn
 *  Same as ccb_compile_error but doesn't exit
 */
void ccb_compile_warn_impl(ccb_t* ccb);//, const char* fmt, ...);
#define ccb_compile_warn(ccb,...) \
    do{fprintf(stderr, "WARNING: "); fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");ccb_compile_warn_impl(ccb);}while(0);


/* TODO: eliminate */
ccb_list_t* ccb_parse_run(ccb_t* ccb);
int ccb_parse_evaluate(ccb_t* ccb, ccb_ast_t* ast);
void ccb_target_gen_data_section(ccb_t* ccb);
void ccb_target_gen_function(ccb_t* ccb, ccb_ast_t* function);

#ifdef CCB_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#include <limits.h>

#define CCB_MEMORY (1024*1024*768)
/*0x800000*/

static unsigned char* ccb_memory_pool = NULL;
static size_t         ccb_memory_next;

static void ccb_memory_cleanup(void) {
    free(ccb_memory_pool);
}

void* ccb_memory_allocate(size_t bytes) {
    void* value;
    //fprintf(stderr, "ccb_memory_allocate: %ld\n", bytes);

    while ((bytes % 16) != 0) {
        bytes++;
    }

    if (!ccb_memory_pool) {
        //fprintf(stderr, "ccb_memory_allocate: B %ld\n", CCB_MEMORY);
        ccb_memory_pool = calloc(CCB_MEMORY,1);
        ccb_memory_next = 0;
        //fprintf(stderr, "ccb_memory_create: C %lx\n", ccb_memory_pool);
        //TODO: atexit(ccb_memory_cleanup);
    }

    if (ccb_memory_next + bytes >= CCB_MEMORY) {
        fprintf(stderr, "FATAL: Out of memory, need to increase compiler reserved memory size!\n");
        ((char*)NULL)[0] = 0; // Trigger debugger
        exit(-1);
    }

    /* TODO: This doesn't self-compile properly yet (it's accepted but results in zero/error)
    value = &(ccb_memory_pool[ccb_memory_next]);
    */
    value = ccb_memory_pool + ccb_memory_next;
    ccb_memory_next += bytes;
    //fprintf(stderr, "ccb_memory_allocate: %lx\n", value);

    return value;
}

void ccb_util_init(ccb_t* ccb) {

}

struct ccb_string_s {
    char* buffer;
    int   allocated;
    int   length;
};

static void ccb_string_reallocate(ccb_string_t* string) {
    int   size = string->allocated * 2;
    char* buffer = ccb_memory_allocate(size);

    strcpy(buffer, string->buffer);
    string->buffer = buffer;
    string->allocated = size;
}

void ccb_string_catcstr(ccb_string_t* string, const char* str) {
    size_t i = 0;
    while (str[i] != 0) {
        ccb_string_cat(string, str[i]);
        i++;
    }
}

/*
**  LTOA.C
**
**  Converts a long integer to a string.
**
**  Copyright 1988-90 by Robert B. Stout dba MicroFirm
**
**  Released to public domain, 1991
**
**  Parameters: 1 - number to be converted
**              2 - buffer in which to build the converted string
**              3 - number base to use for conversion
**
**  Returns:  A character pointer to the converted string if
**            successful, a NULL pointer if the number base specified
**            is out of range.
*/

#include <stdlib.h>
#include <string.h>

#define BUFSIZE (sizeof(long) * 8 + 1)

char *ltoa(long N, char *str, int base)
{
      /* register */ int i = 2;
      long uarg;
      char *tail;
      char *head = str;
      char buf[BUFSIZE];
      if (36 < base || 2 > base)
            base = 10;                    /* can only use 0-9, A-Z        */
      tail = buf + (BUFSIZE - 1);           /* last character position      */
      *tail-- = '\0';

      if (10 == base && N < 0L)
      {
            *head++ = '-';
            uarg    = -N;
      }
      else  uarg = N;
      if (uarg)
      {
            for (i = 1; uarg; ++i)
            {
                  ///*register*/ ldiv_t r;

                  int rrem       = uarg % base;//ldiv(uarg, base);
                  *tail-- = (char)(rrem + ((9L < rrem) ?
                                  ('A' - 10L) : '0'));
                  uarg    = uarg / base;
            }
      }
      else  *tail-- = '0';
      memcpy(head, ++tail, i);
      return str;
}

void ccb_string_catint(ccb_string_t* string, long long i) {
    const char* x = calloc(100, 1);
    ccb_string_catcstr(string, ltoa(i, x, 10));
    free(x);
}

void ccb_string_catf(ccb_string_t* string, const char* fmt, ...) {
    fprintf(stderr, "Catting f '%s'...\n", fmt);
    va_list  va;
    for (;;) {
        int left = string->allocated - string->length;
        int write;

        va_start(va, fmt);
        write = vsnprintf(string->buffer + string->length, left, fmt, va);
        va_end(va);

        if (left <= write) {
            ccb_string_reallocate(string);
            continue;
        }
        string->length += write;
    //fprintf(stderr, "Done catting f...\n");
        return;
    }
}

ccb_string_t* ccb_string_create(void) {
    ccb_string_t* string = ccb_memory_allocate(sizeof(ccb_string_t));
    string->buffer = ccb_memory_allocate(8);
    string->allocated = 8;
    string->length = 0;
    string->buffer[0] = '\0';
    return string;
}

char* ccb_string_buffer(ccb_string_t* string) {
    return string->buffer;
}

void ccb_string_cat(ccb_string_t* string, char ch) {
    if (string->allocated == (string->length + 1))
        ccb_string_reallocate(string);
    string->buffer[string->length++] = ch;
    string->buffer[string->length] = '\0';
}

char* ccb_string_quote(char* p) {
    //printf("Trying to handle quotes of '%s'\n", p);
    ccb_string_t* string = ccb_string_create();
    while (*p) {
        if (*p == '\"' || *p == '\\') {
            //ccb_string_catf(string, "\\%c", *p);
            ccb_string_cat(string, '\\');
            ccb_string_cat(string, *p);
        } else if (*p == '\n') {
            //ccb_string_catf(string, "\\n");
            ccb_string_catcstr(string, "\\n");
        } else {
            ccb_string_cat(string, *p);
        }
        p++;
    }
    return string->buffer;
}

char* ccb_string_quote_fasm(char* p, char quotechar) {
    ccb_string_t* string = ccb_string_create();
    while (*p) {
        if (*p == '\"' || *p == '\\' || *p == '\n')
            ccb_string_catf(string, "%c, %d, %c", quotechar, (int)*p, quotechar);
        else
            ccb_string_cat(string, *p);
        p++;
    }
    return string->buffer;
}

struct ccb_list_node_s {
    void* element;
    ccb_list_node_t* next;
    ccb_list_node_t* prev;
};

struct ccb_list_iterator_s {
    ccb_list_node_t* pointer;
};

ccb_list_t* ccb_list_create(void) {
    ccb_list_t* list = ccb_memory_allocate(sizeof(ccb_list_t));
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void* ccb_list_node_create(void* element) {
    ccb_list_node_t* node = ccb_memory_allocate(sizeof(ccb_list_node_t));
    node->element = element;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void ccb_list_push(ccb_list_t* list, void* element) {
    ccb_list_node_t* node = ccb_list_node_create(element);
    if (!list->head)
        list->head = node;
    else {
        list->tail->next = node;
        node->prev = list->tail;
    }
    list->tail = node;
    list->length++;
}

void* ccb_list_pop(ccb_list_t* list) {
    if (!list->head)
        return NULL;
    void* element = list->tail->element;
    list->tail = list->tail->prev;
    if (list->tail)
        list->tail->next = NULL;
    else
        list->head = NULL;
    list->length--;
    return element;
}

void* ccb_list_shift(ccb_list_t* list) {
    if (!list->head)
        return NULL;
    void* element = list->head->element;
    list->head = list->head->next;
    if (list->head)
        list->head->prev = NULL;
    else
        list->tail = NULL;
    list->length--;
    return element;
}

int ccb_list_length(ccb_list_t* list) {
    return list->length;
}

ccb_list_iterator_t* ccb_list_iterator(ccb_list_t* list) {
    ccb_list_iterator_t* iter = ccb_memory_allocate(sizeof(ccb_list_iterator_t));
    if (iter == NULL) {
        fprintf(stderr, "ICE: %s (out of memory?)", __func__);
        exit(-1);
    }
    if (list == NULL) { // TODO: Added by Zak but not sure if a call with NULL indicates an error?
        iter->pointer = NULL;
    } else {
        iter->pointer = list->head;
    }
    return iter;
}

void* ccb_list_iterator_next(ccb_list_iterator_t* iter) {
    void* ret;

    if (!iter->pointer)
        return NULL;

    ret = iter->pointer->element;
    iter->pointer = iter->pointer->next;

    return ret;
}

bool ccb_list_iterator_end(ccb_list_iterator_t* iter) {
    return !iter->pointer;
}

static void ccb_list_shiftify(ccb_list_t* list, void* element) {
    ccb_list_node_t* node = ccb_list_node_create(element);
    node->next = list->head;
    if (list->head)
        list->head->prev = node;
    list->head = node;
    if (!list->tail)
        list->tail = node;
    list->length++;
}

ccb_list_t* ccb_list_reverse(ccb_list_t* list) {
    ccb_list_t* ret = ccb_list_create();
    for (ccb_list_iterator_t* it = ccb_list_iterator(list); !ccb_list_iterator_end(it); )
        ccb_list_shiftify(ret, ccb_list_iterator_next(it));
    return ret;
}

void* ccb_list_tail(ccb_list_t* list) {
    if (!list->head)
        return NULL;

    ccb_list_node_t* node = list->head;
    while (node->next)
        node = node->next;

    return node->element;
}

typedef struct {
    char* key;
    void* value;
} ccb_table_entry_t;

void* ccb_table_create(void* parent) {
    ccb_table_t* table = ccb_memory_allocate(sizeof(ccb_table_t));
    table->list = ccb_list_create();
    table->parent = parent;

    return table;
}

ccb_table_t* ccb_table_empty() {
    return ccb_table_create(NULL);
}

void* ccb_table_find(ccb_table_t* table, const char* key) {
    for (; table; table = table->parent) {
        for (ccb_list_iterator_t* it = ccb_list_iterator(table->list); !ccb_list_iterator_end(it); ) {
            ccb_table_entry_t* entry = ccb_list_iterator_next(it);
            if (!strcmp(key, entry->key))
                return entry->value;
        }
    }
    return NULL;
}

void ccb_table_insert(ccb_table_t* table, char* key, void* value) {
    ccb_table_entry_t* entry = ccb_memory_allocate(sizeof(ccb_table_entry_t));
    entry->key = key;
    entry->value = value;

    ccb_list_push(table->list, entry);
}

void* ccb_table_parent(ccb_table_t* table) {
    return table->parent;
}

ccb_list_t* ccb_table_values(ccb_table_t* table) {
    ccb_list_t* list = ccb_list_create();
    for (; table; table = table->parent)
        for (ccb_list_iterator_t* it = ccb_list_iterator(table->list); !ccb_list_iterator_end(it); )
            ccb_list_push(list, ((ccb_table_entry_t*)ccb_list_iterator_next(it))->value);
    return list;
}

ccb_list_t* ccb_table_keys(ccb_table_t* table) {
    ccb_list_t* list = ccb_list_create();
    for (; table; table = table->parent)
        for (ccb_list_iterator_t* it = ccb_list_iterator(table->list); !ccb_list_iterator_end(it); )
            ccb_list_push(list, ((ccb_table_entry_t*)ccb_list_iterator_next(it))->key);
    return list;
}

int ccb_strcasecmp(const char* s1, const char* s2) {
    const unsigned char* u1 = (const unsigned char*)s1;
    const unsigned char* u2 = (const unsigned char*)s2;

    while (tolower(*u1) == tolower(*u2++))
        if (*u1++ == '\0')
            return 0;
    return tolower(*u1) - tolower(*--u2);
}

int ccb_strncasecmp(const char* s1, const char* s2, size_t n) {
    const unsigned char* u1 = (const unsigned char*)s1;
    const unsigned char* u2 = (const unsigned char*)s2;

    if (!n)
        return 0;

    do {
        if (tolower(*u1) != tolower(*u2++))
            return tolower(*u1) - tolower(*--u2);
        if (*u1++ == '\0')
            break;
    } while (--n != 0);

    return 0;
}

static int ccb_input_getc(ccb_t* ccb) {
    int result = getc(ccb->input);
    switch (result) {
        case '\n':
            ccb->line++;
            //fprintf(stderr, "@line %d\n", ccb->line);
            ccb->icol = ccb->col;
            ccb->col = 1;
            break;
        default:
            ccb->col++;
    }
    return result;
}

static void ccb_input_ungetc(ccb_t* ccb, int charcode) {
    switch (charcode) {
        case '\n':
            ccb->line--;
            ccb->col = ccb->icol;
            break;
        default:
            ccb->col++;
    }
    ungetc(charcode, ccb->input);
}

static ccb_list_t* ccb_lexer_buffer = NULL; ///*&CCB_SENTINEL_LIST*/ccb_list_create();

static ccb_lexer_token_t* ccb_lexer_token_copy(ccb_t* ccb, ccb_lexer_token_t* token) {
    return memcpy(calloc(1,sizeof(ccb_lexer_token_t)), token, sizeof(ccb_lexer_token_t));
}

static ccb_lexer_token_t* ccb_lexer_identifier(ccb_t* ccb, ccb_string_t* str) {
    ccb_lexer_token_t result = {};
    result.type = CCB_LEXER_TOKEN_IDENTIFIER;
    result.string = ccb_string_buffer(str);
    return ccb_lexer_token_copy(ccb, &result);

    /*return ccb_lexer_token_copy(ccb, &(ccb_lexer_token_t){
        .type = CCB_LEXER_TOKEN_IDENTIFIER,
            .string = ccb_string_buffer(str)
    });*/
}
static ccb_lexer_token_t* ccb_lexer_strtok(ccb_t* ccb, ccb_string_t* str) {
    ccb_lexer_token_t result= {};
    result.type = CCB_LEXER_TOKEN_STRING;
    result.string = ccb_string_buffer(str);
    return ccb_lexer_token_copy(ccb, &result);
    /*return ccb_lexer_token_copy(ccb, &(ccb_lexer_token_t){
        .type = CCB_LEXER_TOKEN_STRING,
            .string = ccb_string_buffer(str)
    });*/
}
static ccb_lexer_token_t* ccb_lexer_punct(ccb_t* ccb, int punct) {
    ccb_lexer_token_t tmp = {};
    tmp.type = CCB_LEXER_TOKEN_PUNCT;
    tmp.punct = punct;
    return ccb_lexer_token_copy(ccb, &tmp);
}
static ccb_lexer_token_t* ccb_lexer_number(ccb_t* ccb, char* string) {
    ccb_lexer_token_t tmp = {};
    tmp.type = CCB_LEXER_TOKEN_NUMBER;
    tmp.string = string;
    return ccb_lexer_token_copy(ccb, &tmp);
}
static ccb_lexer_token_t* ccb_lexer_char(ccb_t* ccb, char value) {
    ccb_lexer_token_t tmp = {};
    tmp.type = CCB_LEXER_TOKEN_CHAR;
    tmp.character = value;
    return ccb_lexer_token_copy(ccb,&tmp);

    /*return ccb_lexer_token_copy(ccb, &(ccb_lexer_token_t){
        .type = CCB_LEXER_TOKEN_CHAR,
            .character = value
    });*/
}

static void ccb_lexer_skip_comment_line(ccb_t* ccb) {
    for (;;) {
        int c = ccb_input_getc(ccb);
        //fputc(c, stderr);
        if (c == '\n' || c == EOF)
            return;
    }
}

static void ccb_lexer_skip_comment_block(ccb_t* ccb) {
    enum {
        comment_outside,
        comment_astrick
    } state = comment_outside;

    for (;;) {
        int c = ccb_input_getc(ccb);
        if (c == '*')
            state = comment_astrick;
        else if (state == comment_astrick && c == '/')
            return;
        else
            state = comment_outside;
    }
}

static int ccb_lexer_skip(ccb_t* ccb) {
    int c;
    while ((c = ccb_input_getc(ccb)) != EOF) {
        if (isspace(c) || c == '\n' || c == '\r') {
            //fprintf(stderr, "Yo got space/newline\n");
            continue;
        }
        //fprintf(stderr, "Not a space/newline: '%c' 0x%x", c, c);
        ccb_input_ungetc(ccb, c);
        return c;
    }
    return EOF;
}

static ccb_lexer_token_t* ccb_lexer_read_number(ccb_t* ccb, int c) {
    ccb_string_t* string = ccb_string_create();
    ccb_string_cat(string, c);
    for (;;) {
        int p = ccb_input_getc(ccb);
        if (!isdigit(p) && !isalpha(p) && p != '.') {
            ccb_input_ungetc(ccb, p);
            return ccb_lexer_number(ccb, ccb_string_buffer(string));
        }
        ccb_string_cat(string, p);
    }
    return NULL;
}

static bool ccb_lexer_read_character_octal_brace(ccb_t* ccb, int c, int* r) {
    if ('0' <= c && c <= '7') {
        *r = (*r << 3) | (c - '0');
        return true;
    }
    return false;
}

static int ccb_lexer_read_character_octal(ccb_t* ccb, int c) {
    int r = c - '0';
    if (ccb_lexer_read_character_octal_brace(ccb, (c = ccb_input_getc(ccb)), &r)) {
        if (!ccb_lexer_read_character_octal_brace(ccb, (c = ccb_input_getc(ccb)), &r))
            ccb_input_ungetc(ccb, c);
    }
    else
        ccb_input_ungetc(ccb, c);
    return r;
}

static int ccb_lexer_read_character_hexadecimal(ccb_t* ccb) {
    int c = ccb_input_getc(ccb);
    int r = 0;

    if (!isxdigit(c))
        ccb_compile_error(ccb, "malformatted hexadecimal character");

    for (;; c = ccb_input_getc(ccb)) {
        switch (c) {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': r = (r << 4) | (c - '0');      continue;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': r = (r << 4) | (c - 'a' + 10); continue;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': r = (r << 4) | (c - 'A' + 10); continue;// Fix by Zak - was minusing 'f'!?

        default:
            ccb_input_ungetc(ccb, c);
            return r;
        }
    }
    return -1;
}

static int ccb_lexer_read_character_escaped(ccb_t* ccb) {
    int c = ccb_input_getc(ccb);

    switch (c) {
    case '\'':        return '\'';
    case '"':         return '"';
    case '?':         return '?';
    case '\\':        return '\\';
    case 'a':         return '\a';
    case 'b':         return '\b';
    case 'f':         return '\f';
    case 'n':         return '\n';
    case 'r':         return '\r';
    case 't':         return '\t';
    case 'v':         return '\v';
    case 'e':         return '\033';
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': return ccb_lexer_read_character_octal(ccb, c);
    case 'x':         return ccb_lexer_read_character_hexadecimal(ccb);
    case EOF:
        ccb_compile_error(ccb, "malformatted escape sequence");

    default:
        return c;
    }
}

static ccb_lexer_token_t* ccb_lexer_read_character(ccb_t* ccb) {
    int c = ccb_input_getc(ccb);
    int r = (c == '\\') ? ccb_lexer_read_character_escaped(ccb) : c;

    if (ccb_input_getc(ccb) != '\'')
        ccb_compile_error(ccb, "unterminated character");

    return ccb_lexer_char(ccb, (char)r);
}

static ccb_lexer_token_t* ccb_lexer_read_string(ccb_t* ccb) {
    ccb_string_t* string = ccb_string_create();
    for (;;) {
        int c = ccb_input_getc(ccb);
        if (c == EOF)
            ccb_compile_error(ccb, "Expected termination for string literal");

        if (c == '"')
            break;
        if (c == '\\')
            c = ccb_lexer_read_character_escaped(ccb);
        ccb_string_cat(string, c);
    }
    return ccb_lexer_strtok(ccb, string);
}

static ccb_lexer_token_t* ccb_lexer_read_identifier(ccb_t* ccb, int c1) {
    ccb_string_t* string = ccb_string_create();
    ccb_string_cat(string, (char)c1);

    for (;;) {
        int c2 = ccb_input_getc(ccb);
        if (isalnum(c2) || c2 == '_' || c2 == '$') {
            ccb_string_cat(string, c2);
        }
        else {
            ccb_input_ungetc(ccb, c2);
            return ccb_lexer_identifier(ccb, string);
        }
    }
    return NULL;
}

static ccb_lexer_token_t* ccb_lexer_read_reclassify_one(ccb_t* ccb, int expect1, int a, int e) {
    int c = ccb_input_getc(ccb);
    if (c == expect1) return ccb_lexer_punct(ccb, a);
    ccb_input_ungetc(ccb, c);
    return ccb_lexer_punct(ccb, e);
}
static ccb_lexer_token_t* ccb_lexer_read_reclassify_two(ccb_t* ccb, int expect1, int a, int expect2, int b, int e) {
    int c = ccb_input_getc(ccb);
    if (c == expect1) return ccb_lexer_punct(ccb, a);
    if (c == expect2) return ccb_lexer_punct(ccb, b);
    ccb_input_ungetc(ccb, c);
    return ccb_lexer_punct(ccb, e);
}

static ccb_lexer_token_t* ccb_lexer_read_token(ccb_t* ccb); // TODO: Predeclaration required for self-compile
static ccb_lexer_token_t* ccb_lexer_read_token_impl(ccb_t* ccb) {
    int c;
    if (ccb_lexer_skip(ccb) == EOF) {
        //printf("ccb_lexer_read_token_impl: Got EOF\n");
        return NULL;
    }

    c = ccb_input_getc(ccb);

    //printf("ccb_lexer_read_token_impl: Got '%c'\n", c);

    switch (c) {
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':  return ccb_lexer_read_number(ccb, c);
    case '"':          return ccb_lexer_read_string(ccb);
    case '\'':         return ccb_lexer_read_character(ccb);
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
    case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
    case 'w': case 'x': case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
    /*not L*/case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
    case 'W': case 'X': case 'Y': case 'Z':
    case '$':
    case '_':
        return ccb_lexer_read_identifier(ccb, c);

    case 'L':
        switch ((c = ccb_input_getc(ccb))) {
        case '"':  return ccb_lexer_read_string(ccb);
        case '\'': return ccb_lexer_read_character(ccb);
        }
        ccb_input_ungetc(ccb, c);
        return ccb_lexer_read_identifier(ccb, 'L');

    case '#':
        fprintf(stderr, "WARNING: Skipping #-line\n"); // TODO: Track line numbers reported by preprocessor
        ccb_lexer_skip_comment_line(ccb);
        return ccb_lexer_read_token(ccb);

    case '/':
        switch ((c = ccb_input_getc(ccb))) {
        case '/':
            ccb_lexer_skip_comment_line(ccb);
            return ccb_lexer_read_token(ccb);
        case '*':
            ccb_lexer_skip_comment_block(ccb);
            return ccb_lexer_read_token(ccb);
        }
        if (c == '=')
            return ccb_lexer_punct(ccb, CCB_LEXER_TOKEN_COMPOUND_DIV);
        ccb_input_ungetc(ccb, c);
        return ccb_lexer_punct(ccb, '/');

    case '(': case ')':
    case ',': case ';':
    case '[': case ']':
    case '{': case '}':
    case '?': case ':':
    case '~':
#ifdef CCB_X_OBJC
    case '@':
#endif
        return ccb_lexer_punct(ccb, c);

    case '+': return ccb_lexer_read_reclassify_two(ccb, '+', CCB_LEXER_TOKEN_INCREMENT, '=', CCB_LEXER_TOKEN_COMPOUND_ADD, '+');
    case '&': return ccb_lexer_read_reclassify_two(ccb, '&', CCB_LEXER_TOKEN_AND, '=', CCB_LEXER_TOKEN_COMPOUND_AND, '&');
    case '|': return ccb_lexer_read_reclassify_two(ccb, '|', CCB_LEXER_TOKEN_OR, '=', CCB_LEXER_TOKEN_COMPOUND_OR, '|');
    case '*': return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_COMPOUND_MUL, '*');
    case '%': return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_COMPOUND_MOD, '%');
    case '=': return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_EQUAL, '=');
    case '!': return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_NEQUAL, '!');
    case '^': return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_COMPOUND_XOR, '^');

    case '-':
        switch ((c = ccb_input_getc(ccb))) {
        case '-': return ccb_lexer_punct(ccb, CCB_LEXER_TOKEN_DECREMENT);
        case '>': return ccb_lexer_punct(ccb, CCB_LEXER_TOKEN_ARROW);
        case '=': return ccb_lexer_punct(ccb, CCB_LEXER_TOKEN_COMPOUND_SUB);
        default:
            break;
        }
        ccb_input_ungetc(ccb, c);
        return ccb_lexer_punct(ccb, '-');

    case '<':
        if ((c = ccb_input_getc(ccb)) == '=')
            return ccb_lexer_punct(ccb, CCB_LEXER_TOKEN_LEQUAL);
        if (c == '<')
            return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_COMPOUND_LSHIFT, CCB_LEXER_TOKEN_LSHIFT);
        ccb_input_ungetc(ccb, c);
        return ccb_lexer_punct(ccb, '<');
    case '>':
        if ((c = ccb_input_getc(ccb)) == '=')
            return ccb_lexer_punct(ccb, CCB_LEXER_TOKEN_GEQUAL);
        if (c == '>')
            return ccb_lexer_read_reclassify_one(ccb, '=', CCB_LEXER_TOKEN_COMPOUND_RSHIFT, CCB_LEXER_TOKEN_RSHIFT);
        ccb_input_ungetc(ccb, c);
        return ccb_lexer_punct(ccb, '>');

    case '.':
        c = ccb_input_getc(ccb);
        if (c == '.') {
            ccb_string_t* str = ccb_string_create();
            //ccb_string_catf(str, "..%c", ccb_input_getc(ccb));
            ccb_string_catcstr(str, "..");
            ccb_string_cat(str, ccb_input_getc(ccb));
            return ccb_lexer_identifier(ccb, str);
        }
        ccb_input_ungetc(ccb, c);
        return ccb_lexer_punct(ccb, '.');

    case EOF:
        return NULL;

    default:
        ccb_compile_error(ccb, "Unexpected character: `%c` (0x%x)", c, c);
    }
    return NULL;
}

static ccb_lexer_token_t* ccb_lexer_read_token(ccb_t* ccb) {
    ccb_lexer_token_t* result = ccb_lexer_read_token_impl(ccb);
    //printf("Got token type %d\n", (result == NULL ? -1 : result->type));
    return result;
}

bool ccb_lexer_ispunct(ccb_t* ccb, ccb_lexer_token_t* token, int c) {
    //printf("Checking '%c' against '%c'\n", c, token->punct);
    bool result = (token != NULL) && (token->type == CCB_LEXER_TOKEN_PUNCT) && (token->punct == c);
    //printf("Got %s\n", result ? "true" : "false");
    return result;
}

void ccb_lexer_unget(ccb_t* ccb, ccb_lexer_token_t* token) {
    if (!token)
        return;
    ccb_list_push(ccb_lexer_buffer, token);
}

ccb_lexer_token_t* ccb_lexer_next(ccb_t* ccb) {
    if (ccb_list_length(ccb_lexer_buffer) > 0)
        return ccb_list_pop(ccb_lexer_buffer);
    return ccb_lexer_read_token(ccb);
}

ccb_lexer_token_t* ccb_lexer_peek(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    ccb_lexer_unget(ccb, token);
    return token;
}

char* ccb_lexer_tokenstr(ccb_t* ccb, ccb_lexer_token_t* token) {
    ccb_string_t* string = ccb_string_create();
    if (!token)
        return "(null)";
    switch (token->type) {
    case CCB_LEXER_TOKEN_PUNCT:
        if (token->punct == CCB_LEXER_TOKEN_EQUAL) {
            ccb_string_catf(string, "==");
            return ccb_string_buffer(string);
        }
    case CCB_LEXER_TOKEN_CHAR:
        ccb_string_cat(string, token->character);
        return ccb_string_buffer(string);
    case CCB_LEXER_TOKEN_NUMBER:
        //ccb_string_catf(string, "%d", token->integer);
        ccb_string_catint(string, token->integer);
        return ccb_string_buffer(string);
    case CCB_LEXER_TOKEN_STRING:
        ccb_string_catf(string, "\"%s\"", token->string);
        return ccb_string_buffer(string);
    case CCB_LEXER_TOKEN_IDENTIFIER:
        return token->string;
    default:
        break;
    }
    ccb_compile_error(ccb, "Internal error: unexpected token");
    return NULL;
}

ccb_data_type_t* ccb_ast_data_table[CCB_AST_DATA_COUNT];// = {
    //&(ccb_data_type_t) { CCB_TYPE_VOID,    0,  true },   /* void                */
    //&(ccb_data_type_t) { CCB_TYPE_LONG,    -1, true },   /* long                */
    //&(ccb_data_type_t) { CCB_TYPE_LLONG,   -1, true },   /* long long           */
    //&(ccb_data_type_t) { CCB_TYPE_INT,     -1, true },   /* int                 */
    //&(ccb_data_type_t) { CCB_TYPE_SHORT,   -1, true },   /* short               */
    //&(ccb_data_type_t) { CCB_TYPE_CHAR,    -1, true },   /* char                */
    //&(ccb_data_type_t) { CCB_TYPE_FLOAT,   -1, true },   /* float               */
    //&(ccb_data_type_t) { CCB_TYPE_DOUBLE,  -1, true },   /* double              */
    //&(ccb_data_type_t) { CCB_TYPE_LDOUBLE, -1, true },   /* long double         */
    //&(ccb_data_type_t) { CCB_TYPE_LONG,    -1, false },  /* unsigned long       */
    //&(ccb_data_type_t) { CCB_TYPE_LLONG,   -1, false },  /* unsigned long long  */
    //#ifdef CCB_X_OBJC // TODO: Fix or remove this?
    //& (ccb_data_type_t) { CCB_TYPE_ID,      -1,      false },  /* objc identifier     */
    //#endif
//};

ccb_data_type_t* ccb_ast_data_function = NULL;

ccb_list_t* ccb_ast_locals = NULL;
ccb_list_t* ccb_ast_gotos = NULL;
ccb_list_t* ccb_ast_floats = NULL; ///*&CCB_SENTINEL_LIST*/ccb_list_create();
ccb_list_t* ccb_ast_strings = NULL; ///*&CCB_SENTINEL_LIST*/ccb_list_create();

ccb_table_t* ccb_ast_labels = NULL;
ccb_table_t* ccb_ast_globalenv = NULL; ///*&CCB_SENTINEL_TABLE*/ccb_table_empty();
ccb_table_t* ccb_ast_localenv = NULL; ///*&CCB_SENTINEL_TABLE*/ccb_table_empty();
ccb_table_t* ccb_ast_structures = NULL; ///*&CCB_SENTINEL_TABLE*/ccb_table_empty();
ccb_table_t* ccb_ast_unions = NULL;// /*&CCB_SENTINEL_TABLE*/ccb_table_empty();

ccb_table_t* ccb_parse_typedefs = NULL; ///*&CCB_SENTINEL_TABLE*/ccb_table_empty();

void ccb_ast_init_t_(ccb_t* ccb, int idx, int typ, int siz, bool sig) {
    ccb_ast_data_table[idx] = ccb_memory_allocate(sizeof(ccb_data_type_t));
    ccb_ast_data_table[idx]->type = typ;
    ccb_ast_data_table[idx]->size = siz;
    ccb_ast_data_table[idx]->sign = sig;
}

void ccb_ast_init(ccb_t* ccb) {
    printf("Doint AST init...\n");
    
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_VOID, CCB_TYPE_VOID,    0,  true );   /* void                */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_LONG, CCB_TYPE_LONG,    -1, true );   /* long                */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_LLONG, CCB_TYPE_LLONG,   -1, true );   /* long long           */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_INT, CCB_TYPE_INT,     -1, true );   /* int                 */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_SHORT, CCB_TYPE_SHORT,   -1, true );   /* short               */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_CHAR, CCB_TYPE_CHAR,    -1, true );   /* char                */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_FLOAT, CCB_TYPE_FLOAT,   -1, true );   /* float               */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_DOUBLE, CCB_TYPE_DOUBLE,  -1, true );   /* double              */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_LDOUBLE, CCB_TYPE_LDOUBLE, -1, true );   /* long double         */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_UINT, CCB_TYPE_INT,    -1, false );  /* unsigned int        */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_ULONG, CCB_TYPE_LONG,    -1, false );  /* unsigned long       */
    ccb_ast_init_t_(ccb,  CCB_AST_DATA_ULLONG, CCB_TYPE_LLONG,   -1, false );  /* unsigned long long  */
    printf("Doint AST init...\n");
    //#ifdef CCB_X_OBJC // TODO: Fix or remove this?
    //ccb_ast_init_t_(ccb, 11, CCB_TYPE_ID,      -1, false );  /* objc identifier     */
    
    //ccb_ast_init_t_(ccb,  0, CCB_TYPE_VOID,    0,  true );   /* void                */
    //ccb_ast_init_t_(ccb,  1, CCB_TYPE_LONG,    -1, true );   /* long                */
    //ccb_ast_init_t_(ccb,  2, CCB_TYPE_LLONG,   -1, true );   /* long long           */
    //ccb_ast_init_t_(ccb,  3, CCB_TYPE_INT,     -1, true );   /* int                 */
    //ccb_ast_init_t_(ccb,  4, CCB_TYPE_SHORT,   -1, true );   /* short               */
    //ccb_ast_init_t_(ccb,  5, CCB_TYPE_CHAR,    -1, true );   /* char                */
    //ccb_ast_init_t_(ccb,  6, CCB_TYPE_FLOAT,   -1, true );   /* float               */
    //ccb_ast_init_t_(ccb,  7, CCB_TYPE_DOUBLE,  -1, true );   /* double              */
    //ccb_ast_init_t_(ccb,  8, CCB_TYPE_LDOUBLE, -1, true );   /* long double         */
    //ccb_ast_init_t_(ccb,  9, CCB_TYPE_LONG,    -1, false );  /* unsigned long       */
    //ccb_ast_init_t_(ccb, 10, CCB_TYPE_LLONG,   -1, false );  /* unsigned long long  */
    printf("Doint AST init...\n");
    //#ifdef CCB_X_OBJC // TODO: Fix or remove this?
    //ccb_ast_init_t_(ccb, 11, CCB_TYPE_ID,      -1, false );  /* objc identifier     */

    //#endif
    printf("Doint AST init...\n");
    
    ccb_lexer_buffer = /*&CCB_SENTINEL_LIST*/ccb_list_create();
    ccb_ast_floats = /*&CCB_SENTINEL_LIST*/ccb_list_create();
    ccb_ast_strings = /*&CCB_SENTINEL_LIST*/ccb_list_create();
    ccb_ast_globalenv = /*&CCB_SENTINEL_TABLE*/ccb_table_empty();
    ccb_ast_structures = /*&CCB_SENTINEL_TABLE*/ccb_table_empty();
    ccb_ast_unions = /*&CCB_SENTINEL_TABLE*/ccb_table_empty();
    ccb_parse_typedefs = /*&CCB_SENTINEL_TABLE*/ccb_table_empty();
    ccb_ast_data_table[CCB_AST_DATA_VOID]->size = 0;
    ccb_ast_data_table[CCB_AST_DATA_CHAR]->size = ccb_target_type_size_char(ccb);
    ccb_ast_data_table[CCB_AST_DATA_SHORT]->size = ccb_target_type_size_short(ccb);
    ccb_ast_data_table[CCB_AST_DATA_INT]->size = ccb_target_type_size_int(ccb);
    ccb_ast_data_table[CCB_AST_DATA_LONG]->size = ccb_target_type_size_long(ccb);
    ccb_ast_data_table[CCB_AST_DATA_LLONG]->size = ccb_target_type_size_llong(ccb);
    ccb_ast_data_table[CCB_AST_DATA_UINT]->size = ccb_target_type_size_int(ccb);
    ccb_ast_data_table[CCB_AST_DATA_ULONG]->size = ccb_target_type_size_long(ccb);
    ccb_ast_data_table[CCB_AST_DATA_ULLONG]->size = ccb_target_type_size_llong(ccb);
    ccb_ast_data_table[CCB_AST_DATA_FLOAT]->size = ccb_target_type_size_float(ccb);
    ccb_ast_data_table[CCB_AST_DATA_DOUBLE]->size = ccb_target_type_size_double(ccb);
    ccb_ast_data_table[CCB_AST_DATA_LDOUBLE]->size = ccb_target_type_size_ldouble(ccb);
    //ccb_ast_data_table[CCB_AST_DATA_ID]->size = ccb_target_type_size_pointer(ccb);
    printf("Doint AST init...\n");

    ccb_ast_data_table[CCB_AST_DATA_ID] = ccb_ast_pointer(ccb, ccb_ast_data_table[CCB_TYPE_VOID]);
    printf("Done?");
}

static ccb_data_type_t* ccb_ast_result_type_impl(ccb_t* ccb, jmp_buf* jmpbuf, char op, ccb_data_type_t* a, ccb_data_type_t* b); // TODO: Predeclaration required for self-compile
static ccb_data_type_t* ccb_ast_result_type_impl(ccb_t* ccb, jmp_buf* jmpbuf, char op, ccb_data_type_t* a, ccb_data_type_t* b) {
    if (a->type > b->type) {
        ccb_data_type_t* t = a;
        a = b;
        b = t;
    }

    //printf("Getting result type of %d and %d...\n", a->type, b->type);

    if (b->type == CCB_TYPE_POINTER) {
        if (op == '=')
            return a;
        if (op != '+' && op != '-')
            goto error;
        if (!ccb_ast_type_integer(ccb, a))
            goto error;

        return b;
    }

    switch (a->type) {
    case CCB_TYPE_VOID:
        goto error;
    case CCB_TYPE_CHAR:
    case CCB_TYPE_SHORT:
    case CCB_TYPE_INT:
        switch (b->type) {
        case CCB_TYPE_CHAR:
        case CCB_TYPE_SHORT:
        case CCB_TYPE_INT:
            return ccb_ast_data_table[CCB_AST_DATA_INT];
        case CCB_TYPE_LONG:
        case CCB_TYPE_LLONG:
            return ccb_ast_data_table[CCB_AST_DATA_LONG];
        case CCB_TYPE_FLOAT:
        case CCB_TYPE_DOUBLE:
        case CCB_TYPE_LDOUBLE:
            return ccb_ast_data_table[CCB_AST_DATA_DOUBLE];
        case CCB_TYPE_ARRAY:
        case CCB_TYPE_POINTER:
            return b;
        default:
            break;
        }
        ccb_compile_error(ccb, "Internal error: ast_result_type %d", b->type);

    case CCB_TYPE_LONG:
    case CCB_TYPE_LLONG:
        switch (b->type) {
        case CCB_TYPE_LONG:
        case CCB_TYPE_LLONG:
            return ccb_ast_data_table[CCB_AST_DATA_LONG];
        case CCB_TYPE_FLOAT:
        case CCB_TYPE_DOUBLE:
        case CCB_TYPE_LDOUBLE:
            return ccb_ast_data_table[CCB_AST_DATA_DOUBLE];
        case CCB_TYPE_ARRAY:
        case CCB_TYPE_POINTER:
            return b;
        default:
            break;
        }
        ccb_compile_error(ccb, "Internal error: ast_result_type (3)");

    case CCB_TYPE_FLOAT:
        if (b->type == CCB_TYPE_FLOAT || b->type == CCB_TYPE_DOUBLE || b->type == CCB_TYPE_LDOUBLE)
            return ccb_ast_data_table[CCB_AST_DATA_DOUBLE];
        goto error;

    case CCB_TYPE_DOUBLE:
    case CCB_TYPE_LDOUBLE:
        if (b->type == CCB_TYPE_DOUBLE || b->type == CCB_TYPE_LDOUBLE)
            return ccb_ast_data_table[CCB_AST_DATA_DOUBLE];
        goto error;

    //case CCB_TYPE_POINTER:
    case CCB_TYPE_ARRAY:
        if (b->type != CCB_TYPE_ARRAY)
            goto error;
        return ccb_ast_result_type_impl(ccb, jmpbuf, op, a->pointer, b->pointer);
    default:
        ccb_compile_error(ccb, "ICE ast_result_type_impl %d", a->type);
    }

error:
    longjmp(*jmpbuf, 1);
    return NULL;
}

ccb_data_type_t* ccb_ast_result_type(ccb_t* ccb, int op, ccb_data_type_t* a, ccb_data_type_t* b) {

    switch (op) {
    case '!':
    case '~':
    case '<':
    case '>':
    case '&':
    case '%':
    case CCB_AST_TYPE_EQUAL:
    case CCB_AST_TYPE_GEQUAL:
    case CCB_AST_TYPE_LEQUAL:
    case CCB_AST_TYPE_NEQUAL:
    case CCB_AST_TYPE_LSHIFT:
    case CCB_AST_TYPE_RSHIFT:
    case CCB_AST_TYPE_AND:
    case CCB_AST_TYPE_OR:
        return ccb_ast_data_table[CCB_AST_DATA_INT];
    }

    jmp_buf jmpbuf;
    if (setjmp(jmpbuf) == 0) {
        return ccb_ast_result_type_impl(ccb,
            &jmpbuf,
            op,
            ccb_ast_array_convert(ccb, a),
            ccb_ast_array_convert(ccb, b)
        );
    }

    ccb_compile_error(ccb,
        "incompatible operands `%s' and `%s' in `%c` operation",
        &(ccb_ast_type_string(ccb, a)[0]),
        &(ccb_ast_type_string(ccb, b)[0]),
        op
    );

    return NULL;
}

ccb_ast_t* ccb_ast_copy(ccb_t* ccb, ccb_ast_t* ast) {
    ccb_ast_t* copy = ccb_memory_allocate(sizeof(ccb_ast_t));
    if (copy == NULL) {
        fprintf(stderr, "FATAL ERROR: Out of memory!\n");
        exit(-1);
    }
    //*copy = *ast;
    memcpy(copy, ast, sizeof(ccb_ast_t));
    return copy;
}

ccb_ast_t* ccb_ast_structure_reference(ccb_t* ccb, ccb_data_type_t* type, ccb_ast_t* structure, char* name) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STRUCT;
    tmp.ctype = type;
    tmp.structure = structure;
    tmp.field = name;
    return ccb_ast_copy(ccb, &tmp);
}

ccb_ast_t* ccb_ast_new_unary(ccb_t* ccb, int type, ccb_data_type_t* data, ccb_ast_t* operand) {
    ccb_ast_t tmp = {};
    tmp.type = type;
    tmp.ctype = data;
    tmp.unary.operand = operand;
    if (operand == NULL) {
        ((char*)NULL)[0] = 0; // Trigger debugger
    }
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to the nested ".unary.operand"
    return ccb_ast_copy(ccb, &(ccb_ast_t) {
        .type = type,
            .ctype = data,
            .unary.operand = operand
    });
    */
}

ccb_ast_t* ccb_ast_new_binary(ccb_t* ccb, int type, ccb_ast_t* left, ccb_ast_t* right) {
    ccb_ast_t tmp = {};
    tmp.type = type;
    tmp.ctype = ccb_ast_result_type(ccb, type, left->ctype, right->ctype);
    ccb_ast_t* ast = ccb_ast_copy(ccb, &tmp);
    if (type != '='
        && ccb_ast_array_convert(ccb, left->ctype)->type != CCB_TYPE_POINTER
        && ccb_ast_array_convert(ccb, right->ctype)->type == CCB_TYPE_POINTER) {

        ast->left = right;
        ast->right = left;
    }
    else {
        ast->left = left;
        ast->right = right;
    }
    return ast;
}

ccb_ast_t* ccb_ast_new_integer(ccb_t* ccb, ccb_data_type_t* type, long long int value) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_LITERAL;
    tmp.ctype = type;
    tmp.integer = value;
    return ccb_ast_copy(ccb, &tmp);
}

ccb_ast_t* ccb_ast_new_floating(ccb_t* ccb, ccb_data_type_t* type, double value) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_LITERAL;
    tmp.ctype = type;
    tmp.floating.value = value;
    ccb_ast_t* ast = ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't compile yet due to nested .floating.value...
    ccb_ast_t* ast = ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_LITERAL,
            .ctype = type,
            .floating.value = value
    });
    */
    ccb_list_push(ccb_ast_floats, ast);
    return ast;
}

ccb_ast_t* ccb_ast_new_string(ccb_t* ccb, char* value) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STRING;
    tmp.ctype = ccb_ast_array(ccb, ccb_ast_data_table[CCB_AST_DATA_CHAR], strlen(value) + 1);
    tmp.string.data = value;
    tmp.string.label = ccb_ast_label(ccb);
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields .string.data/.string.label
    return ccb_ast_copy(ccb, &(ccb_ast_t) {
        .type = CCB_AST_TYPE_STRING,
            .ctype = ccb_ast_array(ccb, ccb_ast_data_table[CCB_AST_DATA_CHAR], strlen(value) + 1),
            .string.data = value,
            .string.label = ccb_ast_label(ccb)
    });
    */
}

ccb_ast_t* ccb_ast_variable_local(ccb_t* ccb, ccb_data_type_t* type, char* name) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_VAR_LOCAL;
    tmp.ctype = type;
    tmp.variable.name = name;
    ccb_ast_t* ast = ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile due to nested .variable.name...
    ccb_ast_t* ast = ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_VAR_LOCAL,
            .ctype = type,
            .variable.name = name
    });
    */
    if (ccb_ast_localenv)
        ccb_table_insert(ccb_ast_localenv, name, ast);
    if (ccb_ast_locals)
        ccb_list_push(ccb_ast_locals, ast);
    return ast;
}

ccb_ast_t* ccb_ast_variable_global(ccb_t* ccb, ccb_data_type_t* type, char* name) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_VAR_GLOBAL;
    tmp.ctype = type;
    tmp.variable.name = name;
    tmp.variable.label = name;
    ccb_ast_t* ast = ccb_ast_copy(ccb, &tmp);
    /* TODO: Doesn't compile due to nested .variable.name
    ccb_ast_t* ast = ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_VAR_GLOBAL,
            .ctype = type,
            .variable.name = name,
            .variable.label = name
    });
    */
    ccb_table_insert(ccb_ast_globalenv, name, ast);
    return ast;
}

ccb_ast_t* ccb_ast_call(ccb_t* ccb, ccb_data_type_t* type, char* name, ccb_list_t* arguments, ccb_list_t* parametertypes) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_CALL;
    tmp.ctype = type;
    tmp.function.call.paramtypes = parametertypes;
    tmp.function.call.args = arguments;
    tmp.function.name = name;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t) {
        .type = CCB_AST_TYPE_CALL,
            .ctype = type,
            .function.call.paramtypes = parametertypes,
            .function.call.args = arguments,
            .function.name = name
    });*/
}

ccb_ast_t* ccb_ast_ptrcall(ccb_t* ccb, ccb_data_type_t* type, char* name, ccb_ast_t* callable, ccb_list_t* arguments, ccb_list_t* parametertypes) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_PTRCALL;
    tmp.ctype = type;
    tmp.function.call.paramtypes = parametertypes;
    tmp.function.call.args = arguments;
    tmp.function.name = name;
    tmp.function.call.callable = callable;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t) {
        .type = CCB_AST_TYPE_PTRCALL,
            .ctype = type,
            .function.call.paramtypes = parametertypes,
            .function.call.args = arguments,
            .function.name = name,
            .function.call.callable = callable
    });*/
}

ccb_ast_t* ccb_ast_function(ccb_t* ccb, ccb_data_type_t* ret, char* name, ccb_list_t* params, ccb_ast_t* body, ccb_list_t* locals) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_FUNCTION;
    tmp.ctype = ret;
    tmp.function.params = params;
    tmp.function.locals = locals;
    tmp.function.name = name;
    tmp.function.body = body;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t) {
        .type = CCB_AST_TYPE_FUNCTION,
            .ctype = ret,
            .function.name = name,
            .function.params = params,
            .function.locals = locals,
            .function.body = body
    });*/
}

ccb_ast_t* ccb_ast_declaration(ccb_t* ccb, ccb_ast_t* var, ccb_list_t* init) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_DECLARATION;
    tmp.ctype = NULL;
    tmp.decl.var = var;
    tmp.decl.init = init;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t) {
        .type = CCB_AST_TYPE_DECLARATION,
            .ctype = NULL,
            .decl.var = var,
            .decl.init = init,
    });*/
}

ccb_ast_t* ccb_ast_initializer(ccb_t* ccb, ccb_ast_t* value, ccb_data_type_t* to, int offset) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_INITIALIZER;
    tmp.init.value = value;
    tmp.init.offset = offset;
    tmp.init.type = to;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_INITIALIZER,
            .init.value = value,
            .init.offset = offset,
            .init.type = to
    });*/
}

ccb_ast_t* ccb_ast_ternary(ccb_t* ccb, ccb_data_type_t* type, ccb_ast_t* cond, ccb_ast_t* then, ccb_ast_t* last) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_EXPRESSION_TERNARY;
    tmp.ctype = type;
    tmp.ifstmt.cond = cond;
    tmp.ifstmt.then = then;
    tmp.ifstmt.last = last;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_EXPRESSION_TERNARY,
            .ctype = type,
            .ifstmt.cond = cond,
            .ifstmt.then = then,
            .ifstmt.last = last
    });*/
}

static ccb_ast_t* ccb_ast_for_intermediate(ccb_t* ccb, int type, ccb_ast_t* init, ccb_ast_t* cond, ccb_ast_t* step, ccb_ast_t* body) {
    ccb_ast_t tmp = {};
    tmp.type = type;
    tmp.ctype = NULL;
    tmp.forstmt.init = init;
    tmp.forstmt.cond = cond;
    tmp.forstmt.step = step;
    tmp.forstmt.body = body;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = type,
            .ctype = NULL,
            .forstmt.init = init,
            .forstmt.cond = cond,
            .forstmt.step = step,
            .forstmt.body = body
    });*/
}

ccb_ast_t* ccb_ast_switch(ccb_t* ccb, ccb_ast_t* expr, ccb_ast_t* body) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_SWITCH;
    tmp.switchstmt.expr = expr;
    tmp.switchstmt.body = body;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_STATEMENT_SWITCH,
            .switchstmt.expr = expr,
            .switchstmt.body = body
    });*/
}

ccb_ast_t* ccb_ast_case(ccb_t* ccb, int value) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_CASE;
    tmp.casevalue = value;
    return ccb_ast_copy(ccb, &tmp);
}

ccb_ast_t* ccb_ast_make(ccb_t* ccb, int type) {
    ccb_ast_t tmp = {};
    tmp.type = type;
    return ccb_ast_copy(ccb, &tmp);
}

ccb_ast_t* ccb_ast_if(ccb_t* ccb, ccb_ast_t* cond, ccb_ast_t* then, ccb_ast_t* last) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_IF;
    tmp.ctype = NULL;
    tmp.ifstmt.cond = cond;
    tmp.ifstmt.then = then;
    tmp.ifstmt.last = last;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_STATEMENT_IF,
            .ctype = NULL,
            .ifstmt.cond = cond,
            .ifstmt.then = then,
            .ifstmt.last = last
    });*/
}

ccb_ast_t* ccb_ast_for(ccb_t* ccb, ccb_ast_t* init, ccb_ast_t* cond, ccb_ast_t* step, ccb_ast_t* body) {
    return ccb_ast_for_intermediate(ccb, CCB_AST_TYPE_STATEMENT_FOR, init, cond, step, body);
}
ccb_ast_t* ccb_ast_while(ccb_t* ccb, ccb_ast_t* cond, ccb_ast_t* body) {
    return ccb_ast_for_intermediate(ccb, CCB_AST_TYPE_STATEMENT_WHILE, NULL, cond, NULL, body);
}
ccb_ast_t* ccb_ast_do(ccb_t* ccb, ccb_ast_t* cond, ccb_ast_t* body) {
    return ccb_ast_for_intermediate(ccb, CCB_AST_TYPE_STATEMENT_DO, NULL, cond, NULL, body);
}

ccb_ast_t* ccb_ast_goto(ccb_t* ccb, char* label) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_GOTO;
    tmp.gotostmt.label = label;
    tmp.gotostmt.where = NULL;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_STATEMENT_GOTO,
            .gotostmt.label = label,
            .gotostmt.where = NULL
    });*/
}

ccb_ast_t* ccb_ast_new_label(ccb_t* ccb, char* label) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_LABEL;
    tmp.gotostmt.label = label;
    tmp.gotostmt.where = NULL;
    return ccb_ast_copy(ccb, &tmp);
    /* TODO: This doesn't self-compile yet due to nested fields
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_STATEMENT_LABEL,
            .gotostmt.label = label,
            .gotostmt.where = NULL
    });*/
}

ccb_ast_t* ccb_ast_return(ccb_t* ccb, ccb_data_type_t* returntype, ccb_ast_t* value) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_RETURN;
    tmp.ctype = returntype;
    tmp.returnstmt = value;
    return ccb_ast_copy(ccb, &tmp);
    /*
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_STATEMENT_RETURN,
            .ctype = returntype,
            .returnstmt = value
    });*/
}

ccb_ast_t* ccb_ast_compound(ccb_t* ccb, ccb_list_t* statements) {
    ccb_ast_t tmp = {};
    tmp.type = CCB_AST_TYPE_STATEMENT_COMPOUND;
    tmp.ctype = NULL;
    tmp.compound = statements;
    return ccb_ast_copy(ccb, &tmp);
    /*
    return ccb_ast_copy(ccb, &(ccb_ast_t){
        .type = CCB_AST_TYPE_STATEMENT_COMPOUND,
            .ctype = NULL,
            .compound = statements
    });*/
}

ccb_data_type_t* ccb_ast_structure_field(ccb_t* ccb, ccb_data_type_t* type, int offset) {
    ccb_data_type_t* field = ccb_ast_type_copy(ccb, type);
    field->offset = offset;
    return field;
}

ccb_data_type_t* ccb_ast_structure_new(ccb_t* ccb, ccb_table_t* fields, int size, bool isstruct) {
    ccb_data_type_t tmp = {};
    tmp.type = CCB_TYPE_STRUCTURE;
    tmp.size = size;
    tmp.fields = fields;
    tmp.isstruct = isstruct;
    return ccb_ast_type_copy(ccb, &tmp/*(ccb_data_type_t) {
        .type = CCB_TYPE_STRUCTURE,
            .size = size,
            .fields = fields,
            .isstruct = isstruct
    }*/);
}

#ifdef _WIN32
#define getpid _getpid
#endif

int ccb_ast_label_index = 0;
char* ccb_ast_label(ccb_t* ccb) {
    //static int index = 0;
    ccb_string_t* string = ccb_string_create();
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1
        || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
        ccb_string_catf(string, "L%d_%d", getpid(), ccb_ast_label_index++); // TODO: Fix this and/or the FASM code above to use a better unique base at least
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86) {
        if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            if (ccb_target_binfmt(ccb) == CCB_TARGET_BINFMT_FLAT) {
                ccb_string_catf(string, "L%d_%d", getpid(), ccb_ast_label_index++); // NOTE: getpid() isn't a good way to get a random number, but is enough to be unlikely to mix up with another run's local data
            }
            else {
                ccb_string_catf(string, "L%d", ccb_ast_label_index++);
            }
        }
        else {
            //ccb_string_catf(string, ".L%d", ccb_ast_label_index++);
            ccb_string_catcstr(string, ".L");
            ccb_string_catint(string, ccb_ast_label_index++);
        }
    }
    else {
        ccb_string_catf(string, ".L%d", ccb_ast_label_index++);
    }
    return ccb_string_buffer(string);
}

bool ccb_ast_type_integer(ccb_t* ccb, ccb_data_type_t* type) {
    return type->type == CCB_TYPE_CHAR
        || type->type == CCB_TYPE_SHORT
        || type->type == CCB_TYPE_INT
        || type->type == CCB_TYPE_LONG
        || type->type == CCB_TYPE_LLONG;
}

bool ccb_ast_type_floating(ccb_t* ccb, ccb_data_type_t* type) {
    return type->type == CCB_TYPE_FLOAT
        || type->type == CCB_TYPE_DOUBLE
        || type->type == CCB_TYPE_LDOUBLE;
}

ccb_data_type_t* ccb_ast_type_copy(ccb_t* ccb, ccb_data_type_t* type) {
    return memcpy(ccb_memory_allocate(sizeof(ccb_data_type_t)), type, sizeof(ccb_data_type_t));
}

ccb_data_type_t* ccb_ast_type_copy_incomplete(ccb_t* ccb, ccb_data_type_t* type) {
    if (!type)
        return NULL;
    return (type->length == -1)
        ? ccb_ast_type_copy(ccb, type)
        : type;
}

ccb_data_type_t* ccb_ast_type_create(ccb_t* ccb, ccb_type_t type, bool sign) {

    ccb_data_type_t* t = ccb_memory_allocate(sizeof(ccb_data_type_t));

    t->type = type;
    t->sign = sign;

    switch (type) {
    case CCB_TYPE_VOID:    t->size = 0;                        break;
    case CCB_TYPE_CHAR:    t->size = ccb_target_type_size_char(ccb);    break;
    case CCB_TYPE_SHORT:   t->size = ccb_target_type_size_short(ccb);   break;
    case CCB_TYPE_INT:     t->size = ccb_target_type_size_int(ccb);     break;
    case CCB_TYPE_LONG:    t->size = ccb_target_type_size_long(ccb);    break;
    case CCB_TYPE_LLONG:   t->size = ccb_target_type_size_llong(ccb);   break;
    case CCB_TYPE_FLOAT:   t->size = ccb_target_type_size_float(ccb);   break;
    case CCB_TYPE_DOUBLE:  t->size = ccb_target_type_size_double(ccb);  break;
    case CCB_TYPE_LDOUBLE: t->size = ccb_target_type_size_ldouble(ccb); break;
    default:
        ccb_compile_error(ccb, "ICE");
    }

    return t;
}

ccb_data_type_t* ccb_ast_type_stub(ccb_t* ccb) {
    ccb_data_type_t tmp = {};
    tmp.type = CCB_TYPE_CDECL;
    tmp.size = 0;
    return ccb_ast_copy(ccb, &tmp);
    /*return ccb_ast_type_copy(ccb, &(ccb_data_type_t) {
        .type = CCB_TYPE_CDECL,
            .size = 0
    });*/
}

ccb_data_type_t* ccb_ast_prototype(ccb_t* ccb, ccb_data_type_t* returntype, ccb_list_t* paramtypes, bool dots) {
    ccb_data_type_t tmp = {};
    tmp.type = CCB_TYPE_FUNCTION;
    tmp.returntype = returntype;
    tmp.parameters = paramtypes;
    tmp.hasdots = dots;
    return ccb_ast_copy(ccb, &tmp);
    /*return ccb_ast_type_copy(ccb, &(ccb_data_type_t){
        .type = CCB_TYPE_FUNCTION,
            .returntype = returntype,
            .parameters = paramtypes,
            .hasdots = dots
    });*/
}

ccb_data_type_t* ccb_ast_array(ccb_t* ccb, ccb_data_type_t* type, int length) {
    ccb_data_type_t tmp = {};
    tmp.type = CCB_TYPE_ARRAY;
    tmp.pointer = type;
    tmp.size = ((length < 0) ? -1 : (type->size * length)); /* TODO: This requires more brackets on self-compilation */
    tmp.length = length;
    return ccb_ast_type_copy(ccb, &tmp/*(ccb_data_type_t){
        .type = CCB_TYPE_ARRAY,
            .pointer = type,
            .size = ((length < 0) ? -1 : type->size * length),
            .length = length
    }*/);
}

ccb_data_type_t* ccb_ast_array_convert(ccb_t* ccb, ccb_data_type_t* type) {
    if (type->type != CCB_TYPE_ARRAY)
        return type;
    return ccb_ast_pointer(ccb, type->pointer);
}

ccb_data_type_t* ccb_ast_pointer(ccb_t* ccb, ccb_data_type_t* type) {
    ccb_data_type_t tmp = {};
    tmp.type = CCB_TYPE_POINTER;
    tmp.pointer = type;
    tmp.size = ccb_target_type_size_pointer(ccb);
    return ccb_ast_type_copy(ccb, &tmp/*(ccb_data_type_t){
        .type = CCB_TYPE_POINTER,
            .pointer = type,
            .size = ccb_target_type_size_pointer(ccb)
    }*/);
}

const char* ccb_ast_type_string(ccb_t* ccb, ccb_data_type_t* type) {
    ccb_string_t* string;

    switch (type->type) {
    case CCB_TYPE_VOID:     return "void";
    case CCB_TYPE_INT:      return "int";
    case CCB_TYPE_CHAR:     return "char";
    case CCB_TYPE_LONG:     return "long";
    case CCB_TYPE_LLONG:    return "long long";
    //TODO... case CCB_TYPE_ULLONG:   return "unsigned long long";
    case CCB_TYPE_SHORT:    return "short";
    case CCB_TYPE_FLOAT:    return "float";
    case CCB_TYPE_DOUBLE:   return "double";
    case CCB_TYPE_LDOUBLE:  return "long double";
#ifdef CCB_X_OBJC
    case CCB_TYPE_ID:       return "id";
#endif

    case CCB_TYPE_FUNCTION:
        string = ccb_string_create();
        ccb_string_cat(string, '(');
        for (ccb_list_iterator_t* it = ccb_list_iterator(type->parameters); !ccb_list_iterator_end(it); ) {
            ccb_data_type_t* next = ccb_list_iterator_next(it);
            ccb_string_catf(string, "%s", ccb_ast_type_string(ccb, next));
            if (!ccb_list_iterator_end(it))
                ccb_string_cat(string, ',');
        }
        ccb_string_catf(string, ") -> %s", ccb_ast_type_string(ccb, type->returntype));
        return ccb_string_buffer(string);

    case CCB_TYPE_POINTER:
        string = ccb_string_create();
        ccb_string_catf(string, "%s*", ccb_ast_type_string(ccb, type->pointer));
        return ccb_string_buffer(string);

    case CCB_TYPE_ARRAY:
        string = ccb_string_create();
        /*ccb_string_catf(
            string,
            "%s[%d]",
            ccb_ast_type_string(ccb, type->pointer),
            type->length
        );*/
        ccb_string_catcstr(string, ccb_ast_type_string(ccb, type->pointer));
        ccb_string_cat(string, '[');
        ccb_string_catint(string, type->length);
        ccb_string_cat(string, ']');
        return ccb_string_buffer(string);

    case CCB_TYPE_STRUCTURE:
        string = ccb_string_create();
        ccb_string_catcstr(string, "(struct...");
        //for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_table_values(type->fields)); !ccb_list_iterator_end(it); )
        //    ccb_string_catf(string, " (%s)", ccb_ast_type_string(ccb, ccb_list_iterator_next(it)));
        ccb_string_cat(string, ')');
        return ccb_string_buffer(string);

    default:
        break;
    }
    return NULL;
}

static void ccb_ast_string_unary(ccb_t* ccb, ccb_string_t* string, const char* op, ccb_ast_t* ast) {
    ccb_string_catf(string, "(%s %s)", op, ccb_ast_string(ccb, ast->unary.operand));
}

static void ccb_ast_string_binary(ccb_t* ccb, ccb_string_t* string, const char* op, ccb_ast_t* ast) {
    ccb_string_catf(string, "(%s %s %s)", op, ccb_ast_string(ccb, ast->left), ccb_ast_string(ccb, ast->right));
}

static void ccb_ast_string_initialization_declaration(ccb_t* ccb, ccb_string_t* string, ccb_list_t* initlist) {
    for (ccb_list_iterator_t* it = ccb_list_iterator(initlist); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* init = ccb_list_iterator_next(it);
        ccb_string_catf(string, "%s", ccb_ast_string(ccb, init));
        if (!ccb_list_iterator_end(it))
            ccb_string_cat(string, ' ');
    }
}

static void ccb_ast_string_impl(ccb_t* ccb, ccb_string_t* string, ccb_ast_t* ast); // TODO: Predeclaration required for self-compile!
static void ccb_ast_string_impl(ccb_t* ccb, ccb_string_t* string, ccb_ast_t* ast) {
    char* left;
    char* right;

    if (!ast) {
        ccb_string_catf(string, "(null)");
        return;
    }

    switch (ast->type) {
    case CCB_AST_TYPE_LITERAL:
        switch (ast->ctype->type) {
        case CCB_TYPE_INT:
        case CCB_TYPE_SHORT:
            //ccb_string_catf(string, "%d", ast->integer);
            ccb_string_catint(string, ast->integer);
            break;

        case CCB_TYPE_FLOAT:
        case CCB_TYPE_DOUBLE:
            ccb_string_catf(string, "%f", ast->floating.value);
            break;

        case CCB_TYPE_LONG:
            ccb_string_catf(string, "%ldL", ast->integer);
            break;

        case CCB_TYPE_CHAR:
            if (ast->integer == '\n')
                ccb_string_catf(string, "'\n'");
            else if (ast->integer == '\\')
                ccb_string_catf(string, "'\\\\'");
            else if (ast->integer == '\0')
                ccb_string_catf(string, "'\\0'");
            else
                ccb_string_catf(string, "'%c'", ast->integer);
            break;

        default:
            ccb_compile_error(ccb, "Internal error: ast_string_impl");
            break;
        }
        break;

    case CCB_AST_TYPE_STRING:
        ccb_string_catf(string, "\"%s\"", ccb_string_quote(ast->string.data));
        break;

    case CCB_AST_TYPE_VAR_LOCAL:
        ccb_string_catf(string, "%s", ast->variable.name);
        if (ast->variable.init) {
            ccb_string_cat(string, '(');
            ccb_ast_string_initialization_declaration(ccb, string, ast->variable.init);
            ccb_string_cat(string, ')');
        }
        break;

    case CCB_AST_TYPE_VAR_GLOBAL:
        ccb_string_catf(string, "%s", ast->variable.name);
        break;

    case CCB_AST_TYPE_CALL:
        ccb_string_catf(string, "(%s)%s(", ccb_ast_type_string(ccb, ast->ctype), ast->function.name);
        for (ccb_list_iterator_t* it = ccb_list_iterator(ast->function.call.args); !ccb_list_iterator_end(it); ) {
            ccb_string_catf(string, "%s", ccb_ast_string(ccb, ccb_list_iterator_next(it)));
            if (!ccb_list_iterator_end(it))
                ccb_string_cat(string, ',');
        }
        ccb_string_cat(string, ')');
        break;

    case CCB_AST_TYPE_FUNCTION:
        ccb_string_catf(string, "(%s)%s(", ccb_ast_type_string(ccb, ast->ctype), ast->function.name);
        for (ccb_list_iterator_t* it = ccb_list_iterator(ast->function.params); !ccb_list_iterator_end(it); ) {
            ccb_ast_t* param = ccb_list_iterator_next(it);
            ccb_string_catf(string, "%s %s", ccb_ast_type_string(ccb, param->ctype), ccb_ast_string(ccb, param));
            if (!ccb_list_iterator_end(it))
                ccb_string_cat(string, ',');
        }
        ccb_string_cat(string, ')');
        ccb_ast_string_impl(ccb, string, ast->function.body);
        break;

    case CCB_AST_TYPE_DECLARATION:
        ccb_string_catf(string, "(decl %s %s",
            ccb_ast_type_string(ccb, ast->decl.var->ctype),
            ast->decl.var->variable.name
        );
        ccb_ast_string_initialization_declaration(ccb, string, ast->decl.init);
        ccb_string_cat(string, ')');
        break;

    case CCB_AST_TYPE_INITIALIZER:
        ccb_string_catf(string, "%s@%d", ccb_ast_string(ccb, ast->init.value), ast->init.offset);
        break;

    case CCB_AST_TYPE_STATEMENT_COMPOUND:
        ccb_string_cat(string, '{');
        for (ccb_list_iterator_t* it = ccb_list_iterator(ast->compound); !ccb_list_iterator_end(it); ) {
            ccb_ast_string_impl(ccb, string, ccb_list_iterator_next(it));
            ccb_string_cat(string, ';');
        }
        ccb_string_cat(string, '}');
        break;

    case CCB_AST_TYPE_STRUCT:
        ccb_ast_string_impl(ccb, string, ast->structure);
        ccb_string_cat(string, '.');
        ccb_string_catf(string, ast->field);
        break;

    case CCB_AST_TYPE_EXPRESSION_TERNARY:
        ccb_string_catf(string, "(? %s %s %s)",
            ccb_ast_string(ccb, ast->ifstmt.cond),
            ccb_ast_string(ccb, ast->ifstmt.then),
            ccb_ast_string(ccb, ast->ifstmt.last)
        );
        break;

    case CCB_AST_TYPE_STATEMENT_IF:
        ccb_string_catf(string, "(if %s %s", ccb_ast_string(ccb, ast->ifstmt.cond), ccb_ast_string(ccb, ast->ifstmt.then));
        if (ast->ifstmt.last)
            ccb_string_catf(string, " %s", ccb_ast_string(ccb, ast->ifstmt.last));
        ccb_string_cat(string, ')');
        break;

    case CCB_AST_TYPE_STATEMENT_FOR:
        ccb_string_catf(string, "(for %s %s %s %s)",
            ccb_ast_string(ccb, ast->forstmt.init),
            ccb_ast_string(ccb, ast->forstmt.cond),
            ccb_ast_string(ccb, ast->forstmt.step),
            ccb_ast_string(ccb, ast->forstmt.body)
        );
        break;

    case CCB_AST_TYPE_STATEMENT_WHILE:
        ccb_string_catf(string, "(while %s %s)",
            ccb_ast_string(ccb, ast->forstmt.cond),
            ccb_ast_string(ccb, ast->forstmt.body)
        );
        break;

    case CCB_AST_TYPE_STATEMENT_DO:
        ccb_string_catf(string, "(do %s %s)",
            ccb_ast_string(ccb, ast->forstmt.cond),
            ccb_ast_string(ccb, ast->forstmt.body)
        );
        break;

    case CCB_AST_TYPE_STATEMENT_RETURN:
        ccb_string_catf(string, "(return %s)", ccb_ast_string(ccb, ast->returnstmt));
        break;

    case CCB_AST_TYPE_ADDRESS:      ccb_ast_string_unary(ccb, string, "&", ast); break;
    case CCB_AST_TYPE_DEREFERENCE:  ccb_ast_string_unary(ccb, string, "*", ast); break;
    case CCB_LEXER_TOKEN_INCREMENT: ccb_ast_string_unary(ccb, string, "++", ast); break;
    case CCB_LEXER_TOKEN_DECREMENT: ccb_ast_string_unary(ccb, string, "--", ast); break;
    case '!':                   ccb_ast_string_unary(ccb, string, "!", ast); break;
    case '&':                   ccb_ast_string_binary(ccb, string, "&", ast); break;
    case '|':                   ccb_ast_string_binary(ccb, string, "|", ast); break;
    case CCB_LEXER_TOKEN_AND:       ccb_ast_string_binary(ccb, string, "&&", ast); break;
    case CCB_LEXER_TOKEN_OR:        ccb_ast_string_binary(ccb, string, "||", ast); break;
    case CCB_LEXER_TOKEN_GEQUAL:    ccb_ast_string_binary(ccb, string, ">=", ast); break;
    case CCB_LEXER_TOKEN_LEQUAL:    ccb_ast_string_binary(ccb, string, "<=", ast); break;
    case CCB_LEXER_TOKEN_NEQUAL:    ccb_ast_string_binary(ccb, string, "!=", ast); break;

    case CCB_AST_TYPE_EXPRESSION_CAST:
        ccb_string_catf(string, "((%s) -> (%s) %s)",
            ccb_ast_type_string(ccb, ast->unary.operand->ctype),
            ccb_ast_type_string(ccb, ast->ctype),
            ccb_ast_string(ccb, ast->unary.operand)
        );
        break;
    
    case CCB_AST_TYPE_STATEMENT_CASE:
        ccb_string_catcstr(string, "(todo: case)");
        break;

    default:
        fprintf(stderr, "Unknown node type %d (0x100+%d)\n", ast->type, ast->type - 0x100);
        left = ccb_ast_string(ccb, ast->left);
        right = ccb_ast_string(ccb, ast->right);
        if (ast->type == CCB_LEXER_TOKEN_EQUAL)
            ccb_string_catf(string, "(== %s %s)", left, right);
        else
            ccb_string_catf(string, "(%c %s %s)", ast->type, left, right);
        break;
    }
}

char* ccb_ast_string(ccb_t* ccb, ccb_ast_t* ast) {
    if (ast == NULL) {
        return "?NULL?";
    }
    ccb_string_t* string = ccb_string_create();
    ccb_ast_string_impl(ccb, string, ast);
    return ccb_string_buffer(string);
}

static ccb_ast_t* ccb_parse_expression(ccb_t* ccb);
static ccb_ast_t* ccb_parse_expression_unary(ccb_t* ccb);
static ccb_ast_t* ccb_parse_expression_intermediate(ccb_t* ccb, int);

static ccb_ast_t* ccb_parse_statement_compound(ccb_t* ccb);
static void         ccb_parse_statement_declaration(ccb_t* ccb, ccb_list_t*);
static ccb_ast_t* ccb_parse_statement(ccb_t* ccb);

static ccb_data_type_t* ccb_parse_declaration_specification(ccb_t* ccb, ccb_storage_t*);
static ccb_list_t* ccb_parse_initializer_declaration(ccb_t* ccb, ccb_data_type_t* type);
static ccb_data_type_t* ccb_parse_declarator(ccb_t* ccb, char**, ccb_data_type_t*, ccb_list_t*, ccb_cdecl_t);
static void         ccb_parse_declaration(ccb_t* ccb, ccb_list_t*, ccb_ast_t* (*)(ccb_t*, ccb_data_type_t*, char*));

static void         ccb_parse_function_parameter(ccb_t* ccb, ccb_data_type_t**, char**, bool);
static ccb_data_type_t* ccb_parse_function_parameters(ccb_t* ccb, ccb_list_t*, ccb_data_type_t*);

static bool ccb_parse_type_check(ccb_t* ccb, ccb_lexer_token_t* token);

static void ccb_parse_semantic_lvalue(ccb_t* ccb, ccb_ast_t* ast, bool allowfunc) {
    switch (ast->type) {
    case CCB_AST_TYPE_FUNCTION:
        if (allowfunc) {
            return;
        }
        break;
    case CCB_AST_TYPE_VAR_LOCAL:
    case CCB_AST_TYPE_VAR_GLOBAL:
    case CCB_AST_TYPE_DEREFERENCE:
    case CCB_AST_TYPE_STRUCT:
        return;
    }
    ccb_compile_error(ccb, "expected lvalue, `%s' isn't a valid lvalue", ccb_ast_string(ccb, ast));
}

static void ccb_parse_semantic_notvoid(ccb_t* ccb, ccb_data_type_t* type) {
    if (type->type == CCB_TYPE_VOID)
        ccb_compile_error(ccb, "void not allowed in expression");
}

static void ccb_parse_semantic_integer(ccb_t* ccb, ccb_ast_t* node) {
    if (!ccb_ast_type_integer(ccb, node->ctype))
        ccb_compile_error(ccb, "expected integer type, `%s' isn't a valid integer type", ccb_ast_string(ccb, node));
}

static bool ccb_parse_semantic_rightassoc(ccb_t* ccb, ccb_lexer_token_t* token) {
    return (token->punct == '=');
}

static void ccb_parse_expect(ccb_t* ccb, char punct) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (!ccb_lexer_ispunct(ccb, token, punct))
        ccb_compile_error(ccb, "expected `%c`, got %s instead", punct, ccb_lexer_tokenstr(ccb, token));
}

static bool ccb_parse_identifier_check(ccb_t* ccb, ccb_lexer_token_t* token, const char* identifier) {
    return token->type == CCB_LEXER_TOKEN_IDENTIFIER && !strcmp(token->string, identifier);
}

int ccb_parse_evaluate(ccb_t* ccb, ccb_ast_t* ast) {
    switch (ast->type) {
    case CCB_AST_TYPE_LITERAL:
        if (ccb_ast_type_integer(ccb, ast->ctype))
            return ast->integer; // TODO: Should this return a 64-bit value??
        ccb_compile_error(ccb, "not a valid integer constant expression `%s'", ccb_ast_string(ccb, ast));
        break;

    case '+':                return ccb_parse_evaluate(ccb, ast->left) + ccb_parse_evaluate(ccb, ast->right);
    case '-':                return ccb_parse_evaluate(ccb, ast->left) - ccb_parse_evaluate(ccb, ast->right);
    case '*':                return ccb_parse_evaluate(ccb, ast->left) * ccb_parse_evaluate(ccb, ast->right);
    case '/':                return ccb_parse_evaluate(ccb, ast->left) / ccb_parse_evaluate(ccb, ast->right);
    case '<':                return ccb_parse_evaluate(ccb, ast->left) < ccb_parse_evaluate(ccb, ast->right);
    case '>':                return ccb_parse_evaluate(ccb, ast->left) > ccb_parse_evaluate(ccb, ast->right);
    case '^':                return ccb_parse_evaluate(ccb, ast->left) ^ ccb_parse_evaluate(ccb, ast->right);
    case '%':                return ccb_parse_evaluate(ccb, ast->left) % ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_AND:    return ccb_parse_evaluate(ccb, ast->left) && ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_OR:     return ccb_parse_evaluate(ccb, ast->left) || ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_EQUAL:  return ccb_parse_evaluate(ccb, ast->left) == ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_LEQUAL: return ccb_parse_evaluate(ccb, ast->left) <= ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_GEQUAL: return ccb_parse_evaluate(ccb, ast->left) >= ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_NEQUAL: return ccb_parse_evaluate(ccb, ast->left) != ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_LSHIFT: return ccb_parse_evaluate(ccb, ast->left) << ccb_parse_evaluate(ccb, ast->right);
    case CCB_LEXER_TOKEN_RSHIFT: return ccb_parse_evaluate(ccb, ast->left) >> ccb_parse_evaluate(ccb, ast->right);

        /* Deal with unary operations differently */
    case '!':                      return !ccb_parse_evaluate(ccb, ast->unary.operand);
    case '~':                      return ~ccb_parse_evaluate(ccb, ast->unary.operand);
    case CCB_AST_TYPE_EXPRESSION_CAST: return  ccb_parse_evaluate(ccb, ast->unary.operand);

    default:
        ccb_compile_error(ccb, "not a valid integer constant expression `%s'", ccb_ast_string(ccb, ast));
    }
    return -1;
}

static int ccb_parse_operator_priority(ccb_t* ccb, ccb_lexer_token_t* token) {
    switch (token->punct) {
    case '[':
    case '.':
    case CCB_LEXER_TOKEN_ARROW:
        return 1;
    case CCB_LEXER_TOKEN_INCREMENT:
    case CCB_LEXER_TOKEN_DECREMENT:
        return 2;
    case '*':
    case '/':
    case '%':
        return 3;
    case '+':
    case '-':
        return 4;
    case CCB_LEXER_TOKEN_LSHIFT:
    case CCB_LEXER_TOKEN_RSHIFT:
        return 5;
    case '<':
    case '>':
        return 6;
    case CCB_LEXER_TOKEN_EQUAL:
    case CCB_LEXER_TOKEN_GEQUAL:
    case CCB_LEXER_TOKEN_LEQUAL:
    case CCB_LEXER_TOKEN_NEQUAL:
        return 7;
    case '&':
        return 8;
    case '^':
        return 9;
    case '|':
        return 10;
    case CCB_LEXER_TOKEN_AND:
        return 11;
    case CCB_LEXER_TOKEN_OR:
        return 12;
    case '?':
        return 13;
    case '=':
    case CCB_LEXER_TOKEN_COMPOUND_ADD:
    case CCB_LEXER_TOKEN_COMPOUND_AND:
    case CCB_LEXER_TOKEN_COMPOUND_DIV:
    case CCB_LEXER_TOKEN_COMPOUND_LSHIFT:
    case CCB_LEXER_TOKEN_COMPOUND_MOD:
    case CCB_LEXER_TOKEN_COMPOUND_MUL:
    case CCB_LEXER_TOKEN_COMPOUND_OR:
    case CCB_LEXER_TOKEN_COMPOUND_RSHIFT:
    case CCB_LEXER_TOKEN_COMPOUND_SUB:
    case CCB_LEXER_TOKEN_COMPOUND_XOR:
        return 14;
    }
    return -1;
}

static ccb_list_t* ccb_parse_parameter_types(ccb_t* ccb, ccb_list_t* parameters) {
    ccb_list_t* list = ccb_list_create();
    for (ccb_list_iterator_t* it = ccb_list_iterator(parameters); !ccb_list_iterator_end(it); )
        ccb_list_push(list, ((ccb_ast_t*)ccb_list_iterator_next(it))->ctype);
    return list;
}

static void ccb_parse_function_typecheck(ccb_t* ccb, const char* name, ccb_list_t* parameters, ccb_list_t* arguments) {
    if (ccb_list_length(arguments) < ccb_list_length(parameters))
        ccb_compile_error(ccb, "too few arguments for function `%s'", name);
    ccb_list_iterator_t *jt = ccb_list_iterator(arguments); // TODO: Secondary variables not handled properly in "for"
    for (ccb_list_iterator_t* it = ccb_list_iterator(parameters); !ccb_list_iterator_end(jt); )
    {
        ccb_data_type_t* parameter = ccb_list_iterator_next(it);
        ccb_data_type_t* argument = ccb_list_iterator_next(jt);

        if (parameter) {
            //printf("Comparing formal params\n");
            ccb_ast_result_type(ccb, '=', parameter, argument);
        } else {
            //printf("Comparing to default int\n");
            ccb_ast_result_type(ccb, '=', argument, ccb_ast_data_table[CCB_AST_DATA_INT]);
        }
    }
}

static ccb_ast_t* ccb_parse_finish_call(ccb_t* ccb, char* name, ccb_list_t* args) {
    ccb_ast_t* func = ccb_table_find(ccb_ast_localenv, name);
    bool isptr = false;
    if (func) {
        ccb_data_type_t* declaration = func->ctype;
        if (declaration->type == CCB_TYPE_POINTER) {
            if (declaration->callconv != 0) {
                fprintf(stderr, "NOTE: Ignoring non-default calling convention %d in indirect call\n", declaration->callconv);
            }
            declaration = declaration->pointer;
            isptr = true;
        }
        if (declaration->type != CCB_TYPE_FUNCTION)
            ccb_compile_error(ccb, "expected a function name, `%s' isn't a function", name);
        ccb_parse_function_typecheck(ccb, name, declaration->parameters, ccb_parse_parameter_types(ccb, args));
        if (declaration->callconv != 0) {
            fprintf(stderr, "NOTE: Ignoring non-default calling convention %d in normal call\n", declaration->callconv);
        }
        if (isptr) {
            return ccb_ast_ptrcall(ccb, declaration->returntype, name, func, args, declaration->parameters);
        }
        else {
            return ccb_ast_call(ccb, declaration->returntype, name, args, declaration->parameters);
        }
    }
    ccb_compile_warn(ccb, "function `%s' is not declared, integer return type implied", name);
    return ccb_ast_call(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], name, args, ccb_list_create());
}

static char* ccb_parse_runtime_fname(ccb_t* ccb, char* name) {
    /* In the future this function might convert internal names to library names
     * according to user preferences (e.g. for calling into an obj-c runtime for
     * @"strings" instead of calling the default function).
     */
    return name;
}

static ccb_ast_t* ccb_parse_runtime_call(ccb_t* ccb, char* name, ccb_list_t* list) {
    return ccb_parse_finish_call(ccb, ccb_parse_runtime_fname(ccb, name), list);
}

static ccb_ast_t* ccb_parse_runtime_call_0(ccb_t* ccb, char* name) {
    ccb_list_t* list = ccb_list_create();
    return ccb_parse_runtime_call(ccb, name, list);
}

static ccb_ast_t* ccb_parse_runtime_call_1(ccb_t* ccb, char* name, ccb_ast_t* arg1) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_push(list, arg1);
    return ccb_parse_runtime_call(ccb, name, list);
}

static ccb_ast_t* ccb_parse_runtime_call_2(ccb_t* ccb, char* name, ccb_ast_t* arg1, ccb_ast_t* arg2) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_push(list, arg1);
    ccb_list_push(list, arg2);
    return ccb_parse_runtime_call(ccb, name, list);
}

static ccb_ast_t* ccb_parse_runtime_call_3(ccb_t* ccb, char* name, ccb_ast_t* arg1, ccb_ast_t* arg2, ccb_ast_t* arg3) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_push(list, arg1);
    ccb_list_push(list, arg2);
    ccb_list_push(list, arg3);
    return ccb_parse_runtime_call(ccb, name, list);
}

static ccb_ast_t* ccb_parse_runtime_call_4(ccb_t* ccb, char* name, ccb_ast_t* arg1, ccb_ast_t* arg2, ccb_ast_t* arg3, ccb_ast_t* arg4) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_push(list, arg1);
    ccb_list_push(list, arg2);
    ccb_list_push(list, arg3);
    ccb_list_push(list, arg4);
    return ccb_parse_runtime_call(ccb, name, list);
}
/* TODO...
static ccb_ast_t* ccb_parse_runtime_call_5(ccb_t* ccb, char* name, ccb_ast_t* arg1, ccb_ast_t* arg2, ccb_ast_t* arg3, ccb_ast_t* arg4, ccb_ast_t* arg5) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_push(list, arg1);
    ccb_list_push(list, arg2);
    ccb_list_push(list, arg3);
    ccb_list_push(list, arg4);
    ccb_list_push(list, arg5);
    return ccb_parse_runtime_call(ccb, name, list);
}

static ccb_ast_t* ccb_parse_runtime_call_6(ccb_t* ccb, char* name, ccb_ast_t* arg1, ccb_ast_t* arg2, ccb_ast_t* arg3, ccb_ast_t* arg4, ccb_ast_t* arg5, ccb_ast_t* arg6) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_push(list, arg1);
    ccb_list_push(list, arg2);
    ccb_list_push(list, arg3);
    ccb_list_push(list, arg4);
    ccb_list_push(list, arg5);
    ccb_list_push(list, arg6);
    return ccb_parse_runtime_call(ccb, name, list);
}
*/

static ccb_ast_t* ccb_parse_function_call(ccb_t* ccb, char* name) {
    ccb_list_t* list = ccb_list_create();
    for (;;) {

        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, ')'))
            break;
        ccb_lexer_unget(ccb, token);
        ccb_list_push(list, ccb_parse_expression(ccb));

        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, ')'))
            break;
        if (!ccb_lexer_ispunct(ccb, token, ','))
            ccb_compile_error(ccb, "unexpected token `%s'", ccb_lexer_tokenstr(ccb, token));
    }

    return ccb_parse_finish_call(ccb, name, list);
}


static ccb_ast_t* ccb_parse_generic(ccb_t* ccb, char* name) {
    ccb_ast_t* var = NULL;
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);

    if (ccb_lexer_ispunct(ccb, token, '('))
        return ccb_parse_function_call(ccb, name);

    ccb_lexer_unget(ccb, token);

    if (!(var = ccb_table_find(ccb_ast_localenv, name))) {
        if (!(var = ccb_table_find(ccb_ast_globalenv, name))) { // TODO: Check if this causes any unwanted side-effects (added by Zak to handle enums in some situations)
            ccb_compile_error(ccb, "undefined variable `%s'", name);
        }
    }

    return var;
}

#ifdef CCB_X_OBJC
ccb_ast_t* ccb_parse_objc_useinitstub(ccb_t* ccb, ccb_ast_t* expr);
static ccb_ast_t* ccb_parse_objc_message(ccb_t* ccb) {
    /* On entry, the '[' token has already been read (but not the target expression). */
    ccb_ast_t* target = ccb_parse_expression(ccb);
    //printf("[] Got AST type 0x100+%d\n", target->type - 0x100);
    if (target->type == CCB_AST_TYPE_VAR_GLOBAL) {
        if (target->variable.isclassobj) {
            printf("Sending to a class!\n");
            target = ccb_parse_objc_useinitstub(ccb, target);
        }
    }

    ccb_lexer_token_t* token = NULL;
    char* selector = NULL;
    ccb_ast_t* selast = NULL;

    token = ccb_lexer_next(ccb);
    if (token->type != CCB_LEXER_TOKEN_IDENTIFIER) {
        ccb_compile_error(ccb, "unexpected token `%s'", ccb_lexer_tokenstr(ccb, token));
    }

    selector = token->string;

    int maxargs = 4;
    int nargs = 0;
    ccb_ast_t* args[4];

    token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, ':')) {
        char* oldsel = selector;
        selector = calloc(strlen(selector) + 150, 1);
        if (selector == NULL) {
            ccb_compile_error(ccb, "Out of memory?");
            return NULL; // Unreachable
        }
        strcat(selector, oldsel);
        strcat(selector, ":");

        bool hasargs = true;

        while (hasargs) {
            /* Append argument */
            if (nargs >= maxargs) {
                ccb_compile_error(ccb, "Too many arguments", NULL);
            }
            args[nargs] = ccb_parse_expression(ccb);
            nargs++;
            /* Check if followed by anotherPart: */
            token = ccb_lexer_next(ccb);
            if (token->type == CCB_LEXER_TOKEN_IDENTIFIER) {
                strcat(selector, token->string);
                ccb_parse_expect(ccb, ':');
                strcat(selector, ":");
            }
            else {
                ccb_lexer_unget(ccb, token);
                token = NULL;
                hasargs = false;
            }
        }
        //ccb_compile_error(ccb, "messages with arguments are not yet supported", NULL);
    }
    else {
        ccb_lexer_unget(ccb, token);
        token = NULL;
    }

    ccb_parse_expect(ccb, ']');

    selast = ccb_ast_new_string(ccb, selector);
    ccb_list_push(ccb_ast_strings, selast);
    selast = ccb_parse_runtime_call_2(ccb, "objx_literal_selector", selast, ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], strlen(selector)));

    switch (nargs) {
    case 0:
        return ccb_parse_runtime_call_2(ccb, "objx_send0", target, selast);
    case 1:
        return ccb_parse_runtime_call_3(ccb, "objx_send1", target, selast, args[0]);
    case 2:
        return ccb_parse_runtime_call_4(ccb, "objx_send2", target, selast, args[0], args[1]);
    /* TODO...
    case 3:
        return ccb_parse_runtime_call_5(ccb, "objx_send3", target, selast, args[0], args[1], args[2]);
    case 4:
        return ccb_parse_runtime_call_6(ccb, "objx_send4", target, selast, args[0], args[1], args[2], args[3]);
    */
    default:
        ccb_compile_error(ccb, "Language Extension Error: Unsupported number of parameters to message send", NULL);
    }
}
#endif

#ifdef CCB_X_OBJC
static ccb_ast_t* ccb_parse_objc_selector(ccb_t* ccb) {
    /* On entry, the '@' and 'selector' have been read already (but not the '('). */
    ccb_lexer_token_t* token = NULL;
    char* selector = NULL;
    char* tmpstr = NULL;
    ccb_ast_t* selast = NULL;

    ccb_parse_expect(ccb, '(');

    token = ccb_lexer_next(ccb);
    if (token->type != CCB_LEXER_TOKEN_IDENTIFIER) {
        ccb_compile_error(ccb, "unexpected token `%s'", ccb_lexer_tokenstr(ccb, token));
    }

    selector = calloc(strlen(token->string) + 100, 1);
    strcat(selector, token->string);

    token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, ':')) {
        tmpstr = selector;
        selector = strcat(tmpstr, ":");
        //free(tmpstr); XXX Wrong place?
        while ((token = ccb_lexer_next(ccb))->type == CCB_LEXER_TOKEN_IDENTIFIER) {
            tmpstr = selector;
            selector = strcat(tmpstr, token->string);
            //free(tmpstr);
            ccb_parse_expect(ccb, ':');
            tmpstr = selector;
            selector = strcat(tmpstr, ":");
            //free(tmpstr);
        }
    }
    ccb_lexer_unget(ccb, token);
    token = NULL;

    ccb_parse_expect(ccb, ')');

    selast = ccb_ast_new_string(ccb, selector);
    ccb_list_push(ccb_ast_strings, selast);

    return ccb_parse_runtime_call_2(ccb, "objx_literal_selector", selast, ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], strlen(selector)));
}
#endif

static ccb_ast_t* ccb_parse_number_integer(ccb_t* ccb, const char* string) {
    const char* p = string;
    int   base = 10;

    if (!ccb_strncasecmp(string, "0x", 2)) {
        base = 16;
        p++;
        p++;
    }
    else if (string[0] == '0' && string[1] != '\0') {
        base = 8;
        p++;
    }

    while (isxdigit(*p)) {
        if (base == 10 && isalpha(*p))
            ccb_compile_error(ccb, "invalid character in decimal literal");
        if (base == 8 && !('0' <= *p && *p <= '7'))
            ccb_compile_error(ccb, "invalid character in octal literal");
        p++;
    }

    if (!ccb_strcasecmp(p, "u"))
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_UINT], strtol(string, NULL, base));
    if (!ccb_strcasecmp(p, "l"))
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], strtol(string, NULL, base));
    if (!ccb_strcasecmp(p, "ul") || !ccb_strcasecmp(p, "lu"))
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_ULONG], strtoul(string, NULL, base));
    if (!ccb_strcasecmp(p, "ll"))
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LLONG], strtoll(string, NULL, base));
    if (!ccb_strcasecmp(p, "ull") || !ccb_strcasecmp(p, "llu"))
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_ULLONG], strtoull(string, NULL, base));
    if (*p != '\0')
        ccb_compile_error(ccb, "invalid suffix for literal");

    // TODO: Was initially using "long long"/strtoll but decided to opt for long just in case that was causing issues
    // I don't think that was the problem (long long always == long?) so I changed it back, but left a todo just in case.
    long long value = strtoll(string, NULL, base);
    return (/*(value & ~(long)UINT_MAX) != 0*/value != (long long) (int) value)
        ? ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], value)
        : ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], value);
}

static ccb_ast_t* ccb_parse_number_floating(ccb_t* ccb, const char* string) {
    const char* p = string;
    char* end;

    while (p[1])
        p++;

    ccb_ast_t* ast;
    if (*p == 'l' || *p == 'L')
        ast = ccb_ast_new_floating(ccb, ccb_ast_data_table[CCB_AST_DATA_LDOUBLE], strtold(string, &end));
    else if (*p == 'f' || *p == 'F')
        ast = ccb_ast_new_floating(ccb, ccb_ast_data_table[CCB_AST_DATA_FLOAT], strtof(string, &end));
    else {
        ast = ccb_ast_new_floating(ccb, ccb_ast_data_table[CCB_AST_DATA_DOUBLE], strtod(string, &end));
        p++;
    }

    if (end != p)
        ccb_compile_error(ccb, "malformatted float literal");

    return ast;
}

static ccb_ast_t* ccb_parse_number(ccb_t* ccb, const char* string) {
    return strpbrk(string, ".pe")
        ? ccb_parse_number_floating(ccb, string)
        : ccb_parse_number_integer(ccb, string);
}

static ccb_data_type_t* ccb_parse_sizeof_type(ccb_t* ccb, bool typename, bool allowvoid);

#ifdef CCB_X_OBJX
static ccb_ast_t* ccb_parse_objx_typeof(ccb_t* ccb, ccb_data_type_t* t); // TODO: Predeclaration required for self-compile
static ccb_ast_t* ccb_parse_objx_typeof(ccb_t* ccb, ccb_data_type_t* t) {
    char* name = NULL;
    bool isflt = false;

    switch (t->type) {
    case CCB_TYPE_VOID:
        return ccb_parse_runtime_call_0(ccb, "objx_typeof_void");
    case CCB_TYPE_CHAR:
        name = "char";
        break;
    case CCB_TYPE_SHORT:
        name = "short";
        break;
    case CCB_TYPE_INT:
        name = "int";
        break;
    case CCB_TYPE_LONG:
        name = "long";
        break;
    case CCB_TYPE_LLONG:
        name = "long long";
        break;
    case CCB_TYPE_FLOAT:
        name = "float";
        isflt = true;
        break;
    case CCB_TYPE_DOUBLE:
        name = "double";
        isflt = true;
        break;
    case CCB_TYPE_LDOUBLE:
        name = "long double";
        isflt = true;
        break;
    case CCB_TYPE_ARRAY:
        return ccb_parse_runtime_call_2(ccb, "objx_typeof_array", ccb_parse_objx_typeof(ccb, t->pointer), ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], t->length));
    case CCB_TYPE_POINTER:
        return ccb_parse_runtime_call_1(ccb, "objx_typeof_pointer", ccb_parse_objx_typeof(ccb, t->pointer));
    case CCB_TYPE_STRUCTURE:
        ccb_compile_error(ccb, "can't get @typeof structures yet");
    case CCB_TYPE_FUNCTION:
    {
        //ccb_compile_error(ccb, "can't get @typeof functions yet");
        ccb_ast_t* params;

        if (t->parameters == NULL || ccb_list_length(t->parameters) == 0) {
            params = ccb_parse_runtime_call_1(ccb, "objx_typeof_params_create", ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], 0));
        }
        else {
            params = ccb_parse_runtime_call_1(ccb, "objx_typeof_params_create", ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], ccb_list_length(t->parameters)));
            ccb_list_iterator_t* iter = ccb_list_iterator(t->parameters);

            while (!ccb_list_iterator_end(iter)) {
                params = ccb_parse_runtime_call_2(ccb, "objx_typeof_params_with", params, ccb_parse_objx_typeof(ccb, ccb_list_iterator_next(iter)));
            }
        }

        return ccb_parse_runtime_call_3(ccb, "objx_typeof_function",
            ccb_parse_objx_typeof(ccb, t->returntype),
            params,
            ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], t->hasdots ? true : t->sign));
    }
    default:
        ccb_compile_error(ccb, "bad type code (%d)", t->type);
    }

    // If the switch fell through to here then it must be a simple integer or float type
    // The name and isflt variables should have been set during or before the switch statement.

    ccb_ast_t* strast = ccb_ast_new_string(ccb, name);
    ccb_list_push(ccb_ast_strings, strast);
    return ccb_parse_runtime_call_4(ccb, isflt ? "objx_typeof_float" : "objx_typeof_integer",
        ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], t->size * 8),
        ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], isflt ? true : t->sign),
        strast,
        ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], strlen(name)));
}
#endif

static ccb_ast_t* ccb_parse_expression_primary(ccb_t* ccb) {
    ccb_lexer_token_t* token;
    ccb_ast_t* ast;

    if (!(token = ccb_lexer_next(ccb)))
        return NULL;

#ifdef CCB_X_OBJC
    bool isobjc = false;
    ccb_lexer_token_t* attoken = NULL;
    if (token->type == CCB_LEXER_TOKEN_PUNCT && token->character == '@') {
        isobjc = true;
        attoken = token;
        if (!(token = ccb_lexer_next(ccb))) {
            ccb_lexer_unget(ccb, attoken);
        }
        //ccb_compile_error(ccb, "Got objc literal");
    }
#endif

    switch (token->type) {
    case CCB_LEXER_TOKEN_IDENTIFIER:
#ifdef CCB_X_OBJC
        if (isobjc) {
            if (strcmp(token->string, "selector") == 0) {
                return ccb_parse_objc_selector(ccb);
#ifdef CCB_X_OBJX
            }
            else if (strcmp(token->string, "typeof") == 0) {
                return ccb_parse_objx_typeof(ccb, ccb_parse_sizeof_type(ccb, false, true));
#endif
            }
            else {
                ccb_lexer_unget(ccb, attoken);
                ccb_lexer_unget(ccb, token);
                return NULL;
            }
        }
#endif
        return ccb_parse_generic(ccb, token->string);
    case CCB_LEXER_TOKEN_NUMBER:
#ifdef CCB_X_OBJC
        if (isobjc) {
            ast = ccb_parse_number(ccb, token->string);
            if (ast->type == CCB_TYPE_FLOAT || ast->type == CCB_TYPE_DOUBLE || ast->type == CCB_TYPE_LDOUBLE) {
                return ccb_parse_runtime_call_1(ccb, "objx_literal_float", ast);
            }
            else {
                return ccb_parse_runtime_call_1(ccb, "objx_literal_integer", ast);
            }
        }
#endif
        return ccb_parse_number(ccb, token->string);
    case CCB_LEXER_TOKEN_CHAR:
#ifdef CCB_X_OBJC
        if (isobjc) {
            return ccb_parse_runtime_call_1(ccb, "objx_literal_character", ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_CHAR], token->character));
        }
#endif
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_CHAR], token->character);
    case CCB_LEXER_TOKEN_STRING:
        ast = ccb_ast_new_string(ccb, token->string);
        ccb_list_push(ccb_ast_strings, ast);
#ifdef CCB_X_OBJC
        if (isobjc) {
            return ccb_parse_runtime_call_2(ccb, "objx_literal_stringn", ast, ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], strlen(token->string)));
        }
#endif
        return ast;
    case CCB_LEXER_TOKEN_PUNCT:
#ifdef CCB_X_OBJC
        if (token->character == '[') {
            return ccb_parse_objc_message(ccb);
        }
        else {
            ccb_lexer_unget(ccb, attoken);
            ccb_lexer_unget(ccb, token);
            return NULL;
        }
#else
        ccb_lexer_unget(ccb, token);
        return NULL;
#endif
    default:
        break;
    }

    ccb_compile_error(ccb, "Internal error: parse_expression_primary");
    return NULL;
}

static ccb_ast_t* ccb_parse_expression_subscript(ccb_t* ccb, ccb_ast_t* ast) {
    ccb_ast_t* subscript = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ']');
    ccb_ast_t* node = ccb_ast_new_binary(ccb, '+', ast, subscript);
    return ccb_ast_new_unary(ccb, CCB_AST_TYPE_DEREFERENCE, node->ctype->pointer, node);
}

static ccb_data_type_t* ccb_parse_sizeof_type(ccb_t* ccb, bool typename, bool allowvoid) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (typename && ccb_parse_type_check(ccb, token)) {
        ccb_lexer_unget(ccb, token);
        ccb_data_type_t* type;
        ccb_parse_function_parameter(ccb, &type, NULL, true);
        return type;
    }

    if (ccb_lexer_ispunct(ccb, token, '(')) {
        ccb_data_type_t* next = ccb_parse_sizeof_type(ccb, true, allowvoid);
        ccb_parse_expect(ccb, ')');
        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '{')) {
            ccb_parse_initializer_declaration(ccb, next);
            ccb_parse_expect(ccb, '}');
        }
        else {
            ccb_lexer_unget(ccb, token);
        }
        return next;
    }

    ccb_lexer_unget(ccb, token);
    ccb_ast_t* expression = ccb_parse_expression_unary(ccb);
    if (!allowvoid && expression->ctype->size == 0)
        ccb_compile_error(ccb, "sizeof(void) illegal");
    return expression->ctype;
}

static ccb_ast_t* ccb_parse_expression_compound_literal(ccb_t* ccb, ccb_data_type_t* type) {
    char* name = ccb_ast_label(ccb);
    ccb_list_t* list = ccb_parse_initializer_declaration(ccb, type);
    ccb_parse_expect(ccb, '}');

    ccb_ast_t* node = ccb_ast_variable_local(ccb, type, name);
    node->variable.init = list;
    return node;
}

static ccb_ast_t* ccb_parse_expression_unary_cast(ccb_t* ccb) {
    ccb_data_type_t* basetype = ccb_parse_declaration_specification(ccb, NULL);
    ccb_data_type_t* casttype = ccb_parse_declarator(ccb, NULL, basetype, NULL, CCB_CDECL_CAST);

    ccb_parse_expect(ccb, ')');

    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, '{'))
        return ccb_parse_expression_compound_literal(ccb, casttype);
    ccb_lexer_unget(ccb, token);

    ccb_ast_t* expression = ccb_parse_expression_unary(ccb);
    return ccb_ast_new_unary(ccb, CCB_AST_TYPE_EXPRESSION_CAST, casttype, expression);
}

static ccb_ast_t* ccb_parse_expression_unary(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);

    if (!token)
        ccb_compile_error(ccb, "unexpected end of input");

    if (ccb_parse_identifier_check(ccb, token, "sizeof")) {
        return ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LONG], ccb_parse_sizeof_type(ccb, false, false)->size);
    }

    if (ccb_lexer_ispunct(ccb, token, '(')) {
        if (ccb_parse_type_check(ccb, ccb_lexer_peek(ccb)))
            return ccb_parse_expression_unary_cast(ccb);
        ccb_ast_t* next = ccb_parse_expression(ccb);
        ccb_parse_expect(ccb, ')');
        return next;
    }
    if (ccb_lexer_ispunct(ccb, token, '&')) {
        ccb_ast_t* operand = ccb_parse_expression_intermediate(ccb, 3);
        ccb_parse_semantic_lvalue(ccb, operand, true);
        return ccb_ast_new_unary(ccb, CCB_AST_TYPE_ADDRESS, ccb_ast_pointer(ccb, operand->ctype), operand);
    }
    if (ccb_lexer_ispunct(ccb, token, '!')) {
        ccb_ast_t* operand = ccb_parse_expression_intermediate(ccb, 3);
        return ccb_ast_new_unary(ccb, '!', ccb_ast_data_table[CCB_AST_DATA_INT], operand);
    }
    if (ccb_lexer_ispunct(ccb, token, '-')) {
        ccb_ast_t* ast = ccb_parse_expression_intermediate(ccb, 3);
        return ccb_ast_new_binary(ccb, '-', ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], 0), ast);
    }
    if (ccb_lexer_ispunct(ccb, token, '~')) {
        ccb_ast_t* ast = ccb_parse_expression_intermediate(ccb, 3);
        if (!ccb_ast_type_integer(ccb, ast->ctype))
            ccb_compile_error(ccb, "invalid expression `%s'", ccb_ast_string(ccb, ast));
        return ccb_ast_new_unary(ccb, '~', ast->ctype, ast);
    }
    if (ccb_lexer_ispunct(ccb, token, '*')) {
        ccb_ast_t* operand = ccb_parse_expression_intermediate(ccb, 3);
        ccb_data_type_t* type = ccb_ast_array_convert(ccb, operand->ctype);

        if (type->type != CCB_TYPE_POINTER)
            ccb_compile_error(ccb, "expected pointer type, `%s' isn't pointer type", ccb_ast_string(ccb, operand));

        return ccb_ast_new_unary(ccb, CCB_AST_TYPE_DEREFERENCE, operand->ctype->pointer, operand);
    }
    if (ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_INCREMENT)
        || ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_DECREMENT))
    {
        ccb_ast_t* next = ccb_parse_expression_intermediate(ccb, 3);
        ccb_parse_semantic_lvalue(ccb, next, false);
        int operand = ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_INCREMENT)
            ? CCB_AST_TYPE_PRE_INCREMENT
            : CCB_AST_TYPE_PRE_DECREMENT;
        return ccb_ast_new_unary(ccb, operand, next->ctype, next);
    }
#ifdef CCB_X_OBJC
    // @expressions may be supported in the future. Only raw literals (@"foo", @123, etc.) are supported right now
    //if (ccb_lexer_ispunct(ccb, token, '@')) {
    //    ccb_ast_t *ast = ccb_parse_expression_intermediate(3);
    //    return ccb_ast_new_unary('@', ast->ctype, ast);
    //}
#endif

    ccb_lexer_unget(ccb, token);
    return ccb_parse_expression_primary(ccb);
}

static ccb_ast_t* ccb_parse_expression_condition(ccb_t* ccb, ccb_ast_t* condition) {
    ccb_ast_t* then = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ':');
    ccb_ast_t* last = ccb_parse_expression(ccb);
    return ccb_ast_ternary(ccb, then->ctype, condition, then, last);
}

static ccb_ast_t* ccb_parse_structure_field(ccb_t* ccb, ccb_ast_t* structure) {
    if (structure->ctype->type != CCB_TYPE_STRUCTURE)
        ccb_compile_error(ccb, "expected structure type, `%s' isn't structure type", ccb_ast_string(ccb, structure));
    ccb_lexer_token_t* name = ccb_lexer_next(ccb);
    if (name->type != CCB_LEXER_TOKEN_IDENTIFIER)
        ccb_compile_error(ccb, "expected field name, got `%s' instead", ccb_lexer_tokenstr(ccb, name));

    ccb_data_type_t* field = ccb_table_find(structure->ctype->fields, name->string);
    if (!field)
        ccb_compile_error(ccb, "structure has no such field `%s'", ccb_lexer_tokenstr(ccb, name));
    return ccb_ast_structure_reference(ccb, field, structure, name->string);
}

static int ccb_parse_operation_compound_operator(ccb_t* ccb, ccb_lexer_token_t* token) {
    if (token->type != CCB_LEXER_TOKEN_PUNCT)
        return 0;

    switch (token->punct) {
    case CCB_LEXER_TOKEN_COMPOUND_RSHIFT: return CCB_LEXER_TOKEN_RSHIFT;
    case CCB_LEXER_TOKEN_COMPOUND_LSHIFT: return CCB_LEXER_TOKEN_LSHIFT;
    case CCB_LEXER_TOKEN_COMPOUND_ADD:    return '+';
    case CCB_LEXER_TOKEN_COMPOUND_AND:    return '&';
    case CCB_LEXER_TOKEN_COMPOUND_DIV:    return '/';
    case CCB_LEXER_TOKEN_COMPOUND_MOD:    return '%';
    case CCB_LEXER_TOKEN_COMPOUND_MUL:    return '*';
    case CCB_LEXER_TOKEN_COMPOUND_OR:     return '|';
    case CCB_LEXER_TOKEN_COMPOUND_SUB:    return '-';
    case CCB_LEXER_TOKEN_COMPOUND_XOR:    return '^';
    default:
        return 0;
    }

    return -1;
}

static int ccb_parse_operation_reclassify(ccb_t* ccb, int punct) {
    switch (punct) {
    case CCB_LEXER_TOKEN_LSHIFT: return CCB_AST_TYPE_LSHIFT;
    case CCB_LEXER_TOKEN_RSHIFT: return CCB_AST_TYPE_RSHIFT;
    case CCB_LEXER_TOKEN_EQUAL:  return CCB_AST_TYPE_EQUAL;
    case CCB_LEXER_TOKEN_GEQUAL: return CCB_AST_TYPE_GEQUAL;
    case CCB_LEXER_TOKEN_LEQUAL: return CCB_AST_TYPE_LEQUAL;
    case CCB_LEXER_TOKEN_NEQUAL: return CCB_AST_TYPE_NEQUAL;
    case CCB_LEXER_TOKEN_AND:    return CCB_AST_TYPE_AND;
    case CCB_LEXER_TOKEN_OR:     return CCB_AST_TYPE_OR;
    default:
        break;
    }
    return punct;
}

static bool ccb_parse_operation_integer_check(ccb_t* ccb, int operation) {
    return operation == '^'
        || operation == '%'
        || operation == CCB_LEXER_TOKEN_LSHIFT
        || operation == CCB_LEXER_TOKEN_RSHIFT;
}

static ccb_ast_t* ccb_parse_expression_intermediate(ccb_t* ccb, int precision) {
    ccb_ast_t* ast;
    ccb_ast_t* next;

    if (!(ast = ccb_parse_expression_unary(ccb)))
        return NULL;

    for (;;) {
        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (token->type != CCB_LEXER_TOKEN_PUNCT) {
            ccb_lexer_unget(ccb, token);
            return ast;
        }

        int pri = ccb_parse_operator_priority(ccb, token);
        if (pri < 0 || precision <= pri) {
            ccb_lexer_unget(ccb, token);
            return ast;
        }

        if (ccb_lexer_ispunct(ccb, token, '?')) {
            ast = ccb_parse_expression_condition(ccb, ast);
            continue;
        }

        if (ccb_lexer_ispunct(ccb, token, '.')) {
            ast = ccb_parse_structure_field(ccb, ast);
            continue;
        }

        if (ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_ARROW)) {
            if (ast->ctype->type != CCB_TYPE_POINTER)
                ccb_compile_error(ccb, "Not a valid pointer type: %s", ccb_ast_string(ccb, ast));
            ast = ccb_ast_new_unary(ccb, CCB_AST_TYPE_DEREFERENCE, ast->ctype->pointer, ast);
            ast = ccb_parse_structure_field(ccb, ast);
            continue;
        }

        if (ccb_lexer_ispunct(ccb, token, '[')) {
            ast = ccb_parse_expression_subscript(ccb, ast);
            continue;
        }

        if (ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_INCREMENT) ||
            ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_DECREMENT)) {

            ccb_parse_semantic_lvalue(ccb, ast, false);
            int operand = ccb_lexer_ispunct(ccb, token, CCB_LEXER_TOKEN_INCREMENT)
                ? CCB_AST_TYPE_POST_INCREMENT
                : CCB_AST_TYPE_POST_DECREMENT;

            ast = ccb_ast_new_unary(ccb, operand, ast->ctype, ast);
            continue;
        }

        int compound = ccb_parse_operation_compound_operator(ccb, token);
        if (ccb_lexer_ispunct(ccb, token, '=') || compound)
            ccb_parse_semantic_lvalue(ccb, ast, false);

        next = ccb_parse_expression_intermediate(ccb, pri + !!ccb_parse_semantic_rightassoc(ccb, token));
        if (!next)
            ccb_compile_error(ccb, "Internal error: parse_expression_intermediate (next)");
        int operation = compound ? compound : token->punct;
        int op = ccb_parse_operation_reclassify(ccb, operation);

        if (ccb_parse_operation_integer_check(ccb, op)) {
            ccb_parse_semantic_integer(ccb, ast);
            ccb_parse_semantic_integer(ccb, next);
        }

        if (compound)
            ast = ccb_ast_new_binary(ccb, '=', ast, ccb_ast_new_binary(ccb, op, ast, next));
        else
            ast = ccb_ast_new_binary(ccb, op, ast, next);
    }
    return NULL;
}

static ccb_ast_t* ccb_parse_expression(ccb_t* ccb) {
    return ccb_parse_expression_intermediate(ccb, 16);
}

static bool ccb_parse_type_check(ccb_t* ccb, ccb_lexer_token_t* token) {
    if (token->type != CCB_LEXER_TOKEN_IDENTIFIER)
        return false;

    static const char* keywords[] = {
        "char",     "short",  "int",     "long",     "float",    "double",
        "struct",   "union",  "signed",  "unsigned", "enum",     "void",
        "typedef",  "extern", "static",  "auto",     "register", "const",
        "volatile", "inline", "restrict"
        #ifdef CCB_X_OBJC
        , "id"
        #endif
    };

    for (int i = 0; i < sizeof(keywords) / sizeof(const char* /* NOTE: keywords[0] doesn't self-compile */); i++)
        if (!strcmp(keywords[i], token->string))
            return true;

    if (ccb_table_find(ccb_parse_typedefs, token->string))
        return true;

    return false;
}

/* struct / union */
static char* ccb_parse_memory_tag(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (token->type == CCB_LEXER_TOKEN_IDENTIFIER)
        return token->string;
    ccb_lexer_unget(ccb, token);
    return NULL;
}

static int ccb_parse_memory_fields_padding(ccb_t* ccb, int offset, int size) {
    size = CCB_MIN(size, (int)ccb_target_alignment(ccb));
    return (offset % size == 0) ? 0 : size - offset % size;
}

static void ccb_parse_memory_fields_squash(ccb_t* ccb, ccb_table_t* table, ccb_data_type_t* unnamed, int offset) {
    for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_table_keys(unnamed->fields)); !ccb_list_iterator_end(it); ) {
        char* name = ccb_list_iterator_next(it);
        ccb_data_type_t* type = ccb_ast_type_copy(ccb, ccb_table_find(unnamed->fields, name));
        type->offset += offset;
        ccb_table_insert(table, name, type);
    }
}

static ccb_table_t* ccb_parse_memory_fields(ccb_t* ccb, int* rsize, bool isstruct) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (!ccb_lexer_ispunct(ccb, token, '{')) {
        ccb_lexer_unget(ccb, token);
        return NULL;
    }

    int      offset = 0;
    int      maxsize = 0;
    ccb_table_t* table = ccb_table_create(NULL);

    for (;;) {
        if (!ccb_parse_type_check(ccb, ccb_lexer_peek(ccb)))
            break;

        ccb_data_type_t* basetype = ccb_parse_declaration_specification(ccb, NULL);

        if (basetype->type == CCB_TYPE_STRUCTURE && ccb_lexer_ispunct(ccb, ccb_lexer_peek(ccb), ';')) {
            ccb_lexer_next(ccb); /* Skip */
            ccb_parse_memory_fields_squash(ccb, table, basetype, offset);
            if (isstruct)
                offset += basetype->size;
            else
                maxsize = CCB_MAX(maxsize, basetype->size);
            continue;
        }

        for (;;) {
            char* name;
            ccb_data_type_t* fieldtype = ccb_parse_declarator(ccb, &name, basetype, NULL, CCB_CDECL_PARAMETER);

            ccb_parse_semantic_notvoid(ccb, fieldtype);

            if (isstruct) {
                offset += ccb_parse_memory_fields_padding(ccb, offset, fieldtype->size);
                fieldtype = ccb_ast_structure_field(ccb, fieldtype, offset);
                offset += fieldtype->size;
            }
            else {
                maxsize = CCB_MAX(maxsize, fieldtype->size);
                fieldtype = ccb_ast_structure_field(ccb, fieldtype, 0);
            }
            ccb_table_insert(table, name, fieldtype);

            token = ccb_lexer_next(ccb);
            if (ccb_lexer_ispunct(ccb, token, ','))
                continue;

            ccb_lexer_unget(ccb, token);
            ccb_parse_expect(ccb, ';');
            break;
        }
    }
    ccb_parse_expect(ccb, '}');
    *rsize = isstruct ? offset : maxsize;
    return table;
}

static ccb_data_type_t* ccb_parse_tag_definition(ccb_t* ccb, ccb_table_t* table, bool isstruct, int initialsize, char* pretag) {
    char* tag = ((pretag == NULL) ? ccb_parse_memory_tag(ccb) : pretag);
    int          size = initialsize;
    ccb_table_t* fields = ((size < 0) ? NULL : ccb_parse_memory_fields(ccb, &size, isstruct));
    ccb_data_type_t* r;

    if (tag) {
        if (!(r = ccb_table_find(table, tag))) {
            r = ccb_ast_structure_new(ccb, NULL, 0, isstruct);
            ccb_table_insert(table, tag, r);
        }
    }
    else {
        r = ccb_ast_structure_new(ccb, NULL, 0, isstruct);
        if (tag)
            ccb_table_insert(table, tag, r);
    }

    if (r && !fields)
        return r;

    if (r && fields) {
        r->fields = fields;
        r->size = size;
        return r;
    }

    return r;
}

/* enum */
static ccb_data_type_t* ccb_parse_enumeration(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (token->type == CCB_LEXER_TOKEN_IDENTIFIER)
        token = ccb_lexer_next(ccb);
    if (!ccb_lexer_ispunct(ccb, token, '{')) {
        ccb_lexer_unget(ccb, token);
        return ccb_ast_data_table[CCB_AST_DATA_INT];
    }
    int accumulate = 0;
    for (;;) {
        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '}'))
            break;

        if (token->type != CCB_LEXER_TOKEN_IDENTIFIER)
            ccb_compile_error(ccb, "NOPE");

        char* name = token->string;
        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '='))
            accumulate = ccb_parse_evaluate(ccb, ccb_parse_expression(ccb));
        else
            ccb_lexer_unget(ccb, token);

        ccb_ast_t* constval = ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], accumulate++);
        ccb_table_insert(ccb_ast_localenv ? ccb_ast_localenv : ccb_ast_globalenv, name, constval);
        //fprintf(stderr, "INSERTED '%s' into %s\n", name, ccb_ast_localenv ? "locals" : "globals");
        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, ','))
            continue;
        if (ccb_lexer_ispunct(ccb, token, '}'))
            break;

        ccb_compile_error(ccb, "NOPE!");
    }
    return ccb_ast_data_table[CCB_AST_DATA_INT];
}

/* initializer */
static void ccb_parse_assign_string(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, char* p, int offset) {
    if (type->length == -1)
        type->length = type->size = strlen(p) + 1;

    int i = 0;
    for (; i < type->length && *p; i++) {
        ccb_list_push(
            init,
            ccb_ast_initializer(ccb,
                ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_CHAR], *p++),
                ccb_ast_data_table[CCB_AST_DATA_CHAR],
                offset + i
            )
        );
    }

    for (; i < type->length; i++) {
        ccb_list_push(
            init,
            ccb_ast_initializer(ccb,
                ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_CHAR], 0),
                ccb_ast_data_table[CCB_AST_DATA_CHAR],
                offset + i
            )
        );
    }
}

static bool ccb_parse_brace_maybe(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, '{'))
        return true;
    ccb_lexer_unget(ccb, token);
    return false;
}

static void ccb_parse_commaskip_maybe(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (!ccb_lexer_ispunct(ccb, token, ','))
        ccb_lexer_unget(ccb, token);
}

static void ccb_parse_brace_skipto(ccb_t* ccb) {
    for (;;) {
        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '}'))
            return;

        if (ccb_lexer_ispunct(ccb, token, '.')) {
            ccb_lexer_next(ccb);
            ccb_parse_expect(ccb, '=');
        }
        else {
            ccb_lexer_unget(ccb, token);
        }

        ccb_ast_t* ignore = ccb_parse_expression_intermediate(ccb, 3);
        if (!ignore)
            return;

        /* TODO aggregate warning */
        token = ccb_lexer_next(ccb);
        if (!ccb_lexer_ispunct(ccb, token, ','))
            ccb_lexer_unget(ccb, token);
    }
}

static ccb_ast_t* ccb_parse_zero(ccb_t* ccb, ccb_data_type_t* type) {
    return ccb_ast_type_floating(ccb, type)
        ? ccb_ast_new_floating(ccb, ccb_ast_data_table[CCB_AST_DATA_DOUBLE], 0.0)
        : ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], 0);
}

static void ccb_parse_initializer_list(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset);
static void ccb_parse_initializer_element(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset) {
    if (type == NULL) {
        ccb_compile_error(ccb, "ICE: %s (NULL type?)", __func__);
    }
    if (type->type == CCB_TYPE_ARRAY || type->type == CCB_TYPE_STRUCTURE)
        ccb_parse_initializer_list(ccb, init, type, offset);
    else {
        ccb_ast_t* expression = ccb_parse_expression_intermediate(ccb, 3);
        if (expression == NULL) {
            ccb_compile_error(ccb, "ICE: %s (NULL expression?)", __func__);
        }
        ccb_ast_result_type(ccb, '=', type, expression->ctype);
        ccb_list_push(init, ccb_ast_initializer(ccb, expression, type, offset));
    }
}

static void ccb_parse_initializer_zero(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset); // TODO: Predeclaration required for self-compile
static void ccb_parse_initializer_zero(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset) {
    if (type->type == CCB_TYPE_STRUCTURE) {
        ccb_list_iterator_t* it = ccb_list_iterator(ccb_table_keys(type->fields));
        while (!ccb_list_iterator_end(it)) {
            char* fieldname = ccb_list_iterator_next(it);
            ccb_data_type_t* fieldtype = ccb_table_find(type->fields, fieldname);

            ccb_parse_initializer_zero(ccb, init, fieldtype, offset + fieldtype->offset);

            if (!type->isstruct)
                return;
        }
        return;
    }

    if (type->type == CCB_TYPE_ARRAY) {
        for (int i = 0; i < type->length; i++)
            ccb_parse_initializer_zero(ccb, init, type->pointer, offset + i * type->pointer->size);
        return;
    }

    ccb_list_push(init, ccb_ast_initializer(ccb, ccb_parse_zero(ccb, type), type, offset));
}

static void ccb_parse_initializer_structure(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset) {
    bool             brace = ccb_parse_brace_maybe(ccb);
    ccb_list_iterator_t* it = ccb_list_iterator(ccb_table_keys(type->fields));
    ccb_table_t* wrote = ccb_table_create(NULL);

    for (;;) {
        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '}')) {
            if (!brace)
                ccb_lexer_unget(ccb, token);
            goto complete;
        }

        char* fieldname;
        ccb_data_type_t* fieldtype;

        if (ccb_lexer_ispunct(ccb, token, '.')) {
            if (!(token = ccb_lexer_next(ccb)) || token->type != CCB_LEXER_TOKEN_IDENTIFIER)
                ccb_compile_error(ccb, "invalid designated initializer");
            fieldname = token->string;
            if (!(fieldtype = ccb_table_find(type->fields, fieldname)))
                ccb_compile_error(ccb, "field doesn't exist in designated initializer");

            ccb_parse_expect(ccb, '=');

            it = ccb_list_iterator(ccb_table_keys(type->fields));
            while (!ccb_list_iterator_end(it))
                if (!strcmp(fieldname, ccb_list_iterator_next(it)))
                    break;
        }
        else {
            ccb_lexer_unget(ccb, token);
            if (ccb_list_iterator_end(it))
                break;

            fieldname = ccb_list_iterator_next(it);
            fieldtype = ccb_table_find(type->fields, fieldname);
        }
        if (ccb_table_find(wrote, fieldname))
            ccb_compile_error(ccb, "field initialized twice in designated initializer");
        ccb_table_insert(wrote, fieldname, (void*)1);
        ccb_parse_initializer_element(ccb, init, fieldtype, offset + fieldtype->offset);
        ccb_parse_commaskip_maybe(ccb);

        if (!type->isstruct)
            break;
    }
    if (brace)
        ccb_parse_brace_skipto(ccb);

complete:
    it = ccb_list_iterator(ccb_table_keys(type->fields));
    while (!ccb_list_iterator_end(it)) {
        char* fieldname = ccb_list_iterator_next(it);
        if (ccb_table_find(wrote, fieldname))
            continue;
        ccb_data_type_t* fieldtype = ccb_table_find(type->fields, fieldname);
        ccb_parse_initializer_zero(ccb, init, fieldtype, offset + fieldtype->offset);
    }
}

static void ccb_parse_initializer_array(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset) {
    bool brace = ccb_parse_brace_maybe(ccb);
    int  size = type->pointer->size;
    int  i;

    for (i = 0; type->length == -1 || i < type->length; i++) {
        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '}')) {
            if (!brace)
                ccb_lexer_unget(ccb, token);
            goto complete;
        }
        ccb_lexer_unget(ccb, token);
        ccb_parse_initializer_element(ccb, init, type->pointer, offset + size * i);
        ccb_parse_commaskip_maybe(ccb);
    }
    if (brace)
        ccb_parse_brace_skipto(ccb);

complete:
    if (type->length == -1) {
        type->length = i;
        type->size = size * i;
    }

    int testn1 = -1;

    printf("Type reports length as %ld, -1 is %ld i is %d\n", type->length, -1, i);

    int itl = type->length;
    /*if (((int) type->length) == -1) {
        return; // TODO: This is required due to condition below failing in self-compilation!
    }*/

    /*for (; i < type->length; i++)*/ while(itl != (int)-1 && i < itl) {
        //printf("Parsing zero %d\n", i);
        ccb_parse_initializer_zero(ccb, init, type->pointer, offset + size * i);
        i++;
    }
}

static void ccb_parse_initializer_list(ccb_t* ccb, ccb_list_t* init, ccb_data_type_t* type, int offset) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (type->type == CCB_TYPE_ARRAY && type->pointer->type == CCB_TYPE_CHAR) {
        if (token->type == CCB_LEXER_TOKEN_STRING) {
            ccb_parse_assign_string(ccb, init, type, token->string, offset);
            return;
        }

        if (ccb_lexer_ispunct(ccb, token, '{') && ccb_lexer_peek(ccb)->type == CCB_LEXER_TOKEN_STRING) {
            ccb_parse_assign_string(ccb, init, type, token->string, offset);
            ccb_parse_expect(ccb, '}');
            return;
        }
    }
    ccb_lexer_unget(ccb, token);

    if (type->type == CCB_TYPE_ARRAY)
        ccb_parse_initializer_array(ccb, init, type, offset);
    else if (type->type == CCB_TYPE_STRUCTURE)
        ccb_parse_initializer_structure(ccb, init, type, offset);
    else
        ccb_compile_error(ccb, "ICE");
}

static ccb_list_t* ccb_parse_initializer_declaration(ccb_t* ccb, ccb_data_type_t* type) {
    ccb_list_t* list = ccb_list_create();
    if (type->type == CCB_TYPE_ARRAY || type->type == CCB_TYPE_STRUCTURE)
        ccb_parse_initializer_list(ccb, list, type, 0);
    else
        ccb_list_push(list, ccb_ast_initializer(ccb, ccb_parse_expression(ccb), type, 0));
    return list;
}

/* declarator */
static ccb_data_type_t* ccb_parse_declaration_specification(ccb_t* ccb, ccb_storage_t* rstorage) {
    ccb_storage_t      storage = 0;
    ccb_lexer_token_t* token = ccb_lexer_peek(ccb);
    if (!token || token->type != CCB_LEXER_TOKEN_IDENTIFIER)
        ccb_compile_error(ccb, "internal error in declaration specification parsing");

    /*
     *  large declaration specification state machine:
     *    There is six pieces of state to the following state machine
     *    for dealing with all the permutations of declaration
     *    specification.
     *
     *    1: The type, most common of course, this is the "base type"
     *       of the declaration.
     *
     *    2: The size, in C, types are also size specifiers on types,
     *       e.g short int, long int, long long int, act as 'sizes'.
     *       Short and long are not technically types, people who use
     *       them as is without a type associated with them (like unsigned)
     *       concludes implicit int. There is no situation where a size
     *       specifier would couple with anything but an int type. It
     *       should be noted that there has to be an "unsized" state for
     *       types on their own.
     *
     *    3: The Signness/signature, for knowing if the declaration is
     *       signed or unsigned. This isn't actually a boolean state because
     *       there needs to be an unsignness state since the char type is
     *       allowed to have it's signeness implementation-defined.
     *
     *    4: constantness
     *         self explanatory
     *    5: vollatileness
     *         self explanatory
     *    6: inlineness
     *         self explanatory
     *
     *    7: user (can include redundant partial specification), e.g
     *        typedef unsigned int foo; signed foo; <--- what to do?
     *        this also includes enums, unions, and structures.
     */
    // TODO: Get rid of these enums (unclear if they apply locally or globally)
    enum {
        kvoid = 1,
        kchar,
        kint,
        kfloat,
        kdouble
#ifdef CCB_X_OBJC
        , kid
#endif
    } type = 0;

    enum {
        kunsize,
        kshort,
        klong,
        kllong
    } size = kunsize;

    enum {
        ksigned = 1,
        kunsigned
    } signature = 0;

#ifdef FUCC_USEUNUSED
    bool __attribute__((unused)) kconst = false;
    bool __attribute__((unused)) kvolatile = false;
    bool __attribute__((unused)) kinline = false;
#else
    bool kconst = false;
    bool kvolatile = false;
    bool kinline = false;
#endif

    ccb_data_type_t* user = NULL;
    ccb_data_type_t* find = NULL;

#define ccb_set_uncheck(STATE, VALUE)                                  \
        do {                                                           \
            STATE = VALUE;                                             \
        } while (0)

#define ccb_set_check(STATE, VALUE)                                    \
        do {                                                           \
            if (STATE != 0) {                                          \
                goto state_machine_error;                              \
            }                                                          \
            ccb_set_uncheck(STATE, VALUE);                                 \
        } while (0)

#define ccb_set_state(STATE, VALUE)                                    \
        do {                                                           \
            ccb_set_check(STATE, VALUE);                                   \
            switch (size) {                                            \
                case kshort:                                           \
                    if (type != 0 && type != kint)                     \
                        goto state_machine_error;                      \
                    break;                                             \
                case klong:                                            \
                    if (type != 0 && type != kint && type != kdouble)  \
                        goto state_machine_error;                      \
                    break;                                             \
                default:                                               \
                    break;                                             \
            }                                                          \
            if (signature != 0) {                                      \
                switch (type) {                                        \
                    case kvoid:                                        \
                    case kfloat:                                       \
                    case kdouble:                                      \
                        goto state_machine_error;                      \
                        break;                                         \
                    default:                                           \
                        break;                                         \
                }                                                      \
            }                                                          \
            if (user && (type != 0 || size != 0 || signature != 0))    \
                goto state_machine_error;                              \
        } while (0)

#define ccb_set_class(VALUE)                                           \
        do {                                                           \
            ccb_set_check(storage, VALUE);                                 \
        } while (0)

#define ccb_state_machine_try(THING) \
        if (!strcmp(token->string, THING))

    for (;;) {
        token = ccb_lexer_next(ccb);
        if (!token)
            ccb_compile_error(ccb, "type specification with unexpected ending");

        if (token->type != CCB_LEXER_TOKEN_IDENTIFIER) {
            ccb_lexer_unget(ccb, token);
            break;
        }

        ccb_state_machine_try("const")    kconst = true;
        else ccb_state_machine_try("volatile") kvolatile = true;
        else ccb_state_machine_try("inline")   kinline = true;

        else ccb_state_machine_try("typedef")  ccb_set_class(CCB_STORAGE_TYPEDEF);
    else ccb_state_machine_try("extern")   ccb_set_class(CCB_STORAGE_EXTERN);
        else ccb_state_machine_try("static")   ccb_set_class(CCB_STORAGE_STATIC);
        else ccb_state_machine_try("auto")     ccb_set_class(CCB_STORAGE_AUTO);
        else ccb_state_machine_try("register") ccb_set_class(CCB_STORAGE_REGISTER);

        else ccb_state_machine_try("void")     ccb_set_state(type, kvoid);
        else ccb_state_machine_try("char")     ccb_set_state(type, kchar);
        else ccb_state_machine_try("int")      ccb_set_state(type, kint);
        else ccb_state_machine_try("float")    ccb_set_state(type, kfloat);
        else ccb_state_machine_try("double")   ccb_set_state(type, kdouble);
#ifdef CCB_X_OBJC
        else ccb_state_machine_try("id")       ccb_set_state(type, kid);
#endif

        else ccb_state_machine_try("signed")   ccb_set_state(signature, ksigned);
        else ccb_state_machine_try("unsigned") ccb_set_state(signature, kunsigned);
        else ccb_state_machine_try("short")    ccb_set_state(size, kshort);

        else ccb_state_machine_try("struct")   ccb_set_state(user, ccb_parse_tag_definition(ccb, ccb_ast_structures, true, 0, NULL));
        else ccb_state_machine_try("union")    ccb_set_state(user, ccb_parse_tag_definition(ccb, ccb_ast_unions, false, 0, NULL));
        else ccb_state_machine_try("enum")     ccb_set_state(user, ccb_parse_enumeration(ccb));
        else ccb_state_machine_try("long") {
        switch (size) {
        case kunsize:
            ccb_set_state(size, klong);
            break;
        case klong:
            ccb_set_uncheck(size, kllong);
            break;
        default:
            goto state_machine_error;
        }
        }
 else if ((find = ccb_table_find(ccb_parse_typedefs, token->string))) {
 ccb_set_state(user, find);
        }
 else {
 ccb_lexer_unget(ccb, token);
 break;
        }

#undef ccb_set_check
#undef ccb_set_class
#undef ccb_set_state
#undef ccb_set_uncheck
    }

    if (rstorage)
        *rstorage = storage;

    if (user)
        return user;

    switch (type) {
    case kvoid:
        return ccb_ast_data_table[CCB_AST_DATA_VOID];
    case kchar:
        return ccb_ast_type_create(ccb, CCB_TYPE_CHAR, signature != kunsigned);
    case kfloat:
        return ccb_ast_type_create(ccb, CCB_TYPE_FLOAT, false);
    case kdouble:
        return ccb_ast_type_create(ccb,
            (size == klong)
            ? CCB_TYPE_LDOUBLE
            : CCB_TYPE_DOUBLE,
            false
        );
#ifdef CCB_X_OBJC
    case kid:
        return ccb_ast_data_table[CCB_AST_DATA_ID];
#endif
    default:
        break;
    }

    switch (size) {
    case kshort:
        return ccb_ast_type_create(ccb, CCB_TYPE_SHORT, signature != kunsigned);
    case klong:
        return ccb_ast_type_create(ccb, CCB_TYPE_LONG, signature != kunsigned);
    case kllong:
        return ccb_ast_type_create(ccb, CCB_TYPE_LLONG, signature != kunsigned);
    default:
        /*
         * You also need to deal with implicit int given the right
         * conditions of the state machine.
         */
        return ccb_ast_type_create(ccb, CCB_TYPE_INT, signature != kunsigned);
    }

    ccb_compile_error(ccb, "State machine error (BAD)");
state_machine_error:
    ccb_compile_error(ccb, "State machine error (GOOD)");

    return NULL;
}

static ccb_data_type_t* ccb_parse_array_dimensions_intermediate(ccb_t* ccb, ccb_data_type_t* basetype); // TODO: Predeclaration required for self-compilation
static ccb_data_type_t* ccb_parse_array_dimensions_intermediate(ccb_t* ccb, ccb_data_type_t* basetype) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (!ccb_lexer_ispunct(ccb, token, '[')) {
        ccb_lexer_unget(ccb, token);
        return NULL;
    }

    int dimension = -1;
    if (!ccb_lexer_ispunct(ccb, ccb_lexer_peek(ccb), ']'))
        dimension = ccb_parse_evaluate(ccb, ccb_parse_expression(ccb));

    ccb_parse_expect(ccb, ']');
    ccb_data_type_t* next = ccb_parse_array_dimensions_intermediate(ccb, basetype);
    if (next) {
        if (next->length == -1 && dimension == -1)
            ccb_compile_error(ccb, "Internal error: parse_array_dimensions_intermediate (2)");
        return ccb_ast_array(ccb, next, dimension);
    }
    return ccb_ast_array(ccb, basetype, dimension);
}

static ccb_data_type_t* ccb_parse_array_dimensions(ccb_t* ccb, ccb_data_type_t* basetype) {
    ccb_data_type_t* data = ccb_parse_array_dimensions_intermediate(ccb, basetype);
    return (data) ? data : basetype;
}

static void ccb_parse_function_parameter(ccb_t* ccb, ccb_data_type_t** rtype, char** name, bool next) {
    ccb_data_type_t* basetype;
    ccb_storage_t    storage;

    basetype = ccb_parse_declaration_specification(ccb, &storage);
    basetype = ccb_parse_declarator(ccb, name, basetype, NULL, next ? CCB_CDECL_TYPEONLY : CCB_CDECL_PARAMETER);
    *rtype = ccb_parse_array_dimensions(ccb, basetype);
}

static ccb_ast_t* ccb_parse_statement_if(ccb_t* ccb) {
    ccb_lexer_token_t* token;
    ccb_ast_t* cond;
    ccb_ast_t* then;
    ccb_ast_t* last;

    ccb_parse_expect(ccb, '(');
    cond = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ')');


    then = ccb_parse_statement(ccb);
    token = ccb_lexer_next(ccb);

    if (!token || token->type != CCB_LEXER_TOKEN_IDENTIFIER || strcmp(token->string, "else")) {
        ccb_lexer_unget(ccb, token);
        return ccb_ast_if(ccb, cond, then, NULL);
    }

    last = ccb_parse_statement(ccb);
    return ccb_ast_if(ccb, cond, then, last);
}

static ccb_ast_t* ccb_parse_statement_declaration_semicolon(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, ';'))
        return NULL;
    ccb_lexer_unget(ccb, token);
    ccb_list_t* list = ccb_list_create();
    ccb_parse_statement_declaration(ccb, list);
    return ccb_list_shift(list);
}

static ccb_ast_t* ccb_parse_expression_semicolon(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, ';'))
        return NULL;
    ccb_lexer_unget(ccb, token);
    ccb_ast_t* read = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ';');
    return read;
}

static ccb_ast_t* ccb_parse_statement_for(ccb_t* ccb) {
    ccb_parse_expect(ccb, '(');
    ccb_ast_localenv = ccb_table_create(ccb_ast_localenv);
    ccb_ast_t* init = ccb_parse_statement_declaration_semicolon(ccb);
    ccb_ast_t* cond = ccb_parse_expression_semicolon(ccb);
    ccb_ast_t* step = ccb_lexer_ispunct(ccb, ccb_lexer_peek(ccb), ')') ? NULL : ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ')');
    ccb_ast_t* body = ccb_parse_statement(ccb);
    ccb_ast_localenv = ccb_table_parent(ccb_ast_localenv);
    return ccb_ast_for(ccb, init, cond, step, body);
}

static ccb_ast_t* ccb_parse_statement_while(ccb_t* ccb) {
    ccb_parse_expect(ccb, '(');
    ccb_ast_t* cond = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ')');
    ccb_ast_t* body = ccb_parse_statement(ccb);
    return ccb_ast_while(ccb, cond, body);
}

static ccb_ast_t* ccb_parse_statement_do(ccb_t* ccb) {
    ccb_ast_t* body = ccb_parse_statement(ccb);
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);

    if (!ccb_parse_identifier_check(ccb, token, "while"))
        ccb_compile_error(ccb, "expected while for do");

    ccb_parse_expect(ccb, '(');
    ccb_ast_t* cond = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ')');
    ccb_parse_expect(ccb, ';');

    return ccb_ast_do(ccb, cond, body);
}

static ccb_ast_t* ccb_parse_statement_break(ccb_t* ccb) {
    ccb_parse_expect(ccb, ';');
    return ccb_ast_make(ccb, CCB_AST_TYPE_STATEMENT_BREAK);
}

static ccb_ast_t* ccb_parse_statement_continue(ccb_t* ccb) {
    ccb_parse_expect(ccb, ';');
    return ccb_ast_make(ccb, CCB_AST_TYPE_STATEMENT_CONTINUE);
}

static ccb_ast_t* ccb_parse_statement_switch(ccb_t* ccb) {
    ccb_parse_expect(ccb, '(');
    ccb_ast_t* expression = ccb_parse_expression(ccb);

    /* TODO lvalueness test propogate?*/

    ccb_parse_expect(ccb, ')');
    ccb_ast_t* body = ccb_parse_statement(ccb);
    return ccb_ast_switch(ccb, expression, body);
}

static ccb_ast_t* ccb_parse_statement_case(ccb_t* ccb) {
    int value = ccb_parse_evaluate(ccb, ccb_parse_expression(ccb));
    ccb_parse_expect(ccb, ':');
    return ccb_ast_case(ccb, value);
}

static ccb_ast_t* ccb_parse_statement_default(ccb_t* ccb) {
    ccb_parse_expect(ccb, ':');
    return ccb_ast_make(ccb, CCB_AST_TYPE_STATEMENT_DEFAULT);
}

static ccb_ast_t* ccb_parse_statement_return(ccb_t* ccb) {
    ccb_ast_t* val = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ';');
    return ccb_ast_return(ccb, ccb_ast_data_table[CCB_AST_DATA_FUNCTION]->returntype, val);
}

static ccb_ast_t* ccb_parse_statement_goto(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (!token || token->type != CCB_LEXER_TOKEN_IDENTIFIER)
        ccb_compile_error(ccb, "expected identifier in goto statement");
    ccb_parse_expect(ccb, ';');

    ccb_ast_t* node = ccb_ast_goto(ccb, token->string);
    ccb_list_push(ccb_ast_gotos, node);

    return node;
}

static void ccb_parse_label_backfill(ccb_t* ccb) {
    for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_ast_gotos); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* source = ccb_list_iterator_next(it);
        char* label = source->gotostmt.label;
        ccb_ast_t* destination = ccb_table_find(ccb_ast_labels, label);

        if (!destination)
            ccb_compile_error(ccb, "undefined label: %s", label);
        if (destination->gotostmt.where)
            source->gotostmt.where = destination->gotostmt.where;
        else
            source->gotostmt.where = destination->gotostmt.where = ccb_ast_label(ccb);
        
        //fprintf(stderr, "Gave goto '%s' the target label '%s'.\n", source->gotostmt.label, source->gotostmt.where);
    }
}

static ccb_ast_t* ccb_parse_label(ccb_t* ccb, ccb_lexer_token_t* token) {
    ccb_parse_expect(ccb, ':');
    char* label = token->string;
    ccb_ast_t* node = ccb_ast_new_label(ccb, label);

    if (ccb_table_find(ccb_ast_labels, label))
        ccb_compile_error(ccb, "duplicate label: %s", label);
    ccb_table_insert(ccb_ast_labels, label, node);

    return node;
}

#ifdef CCB_X_OBJC
static ccb_ast_t* ccb_parse_statement_objc_throw(ccb_t* ccb) {
    ccb_ast_t* val = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ';');
    return ccb_parse_runtime_call_1(ccb, "objx_throw", val);
}
#endif

static ccb_ast_t* ccb_parse_statement(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    ccb_ast_t* ast;

    if (ccb_lexer_ispunct(ccb, token, '{'))            return ccb_parse_statement_compound(ccb);
#ifdef CCB_X_OBJC
    if (ccb_lexer_ispunct(ccb, token, '@')) {
        ccb_lexer_token_t* attoken = token;
        token = ccb_lexer_next(ccb);
        if (ccb_parse_identifier_check(ccb, token, "throw"))   return ccb_parse_statement_objc_throw(ccb);
        ccb_lexer_unget(ccb, token);
    }
#endif
    if (ccb_parse_identifier_check(ccb, token, "if"))          return ccb_parse_statement_if(ccb);
    if (ccb_parse_identifier_check(ccb, token, "for"))         return ccb_parse_statement_for(ccb);
    if (ccb_parse_identifier_check(ccb, token, "while"))       return ccb_parse_statement_while(ccb);
    if (ccb_parse_identifier_check(ccb, token, "do"))          return ccb_parse_statement_do(ccb);
    if (ccb_parse_identifier_check(ccb, token, "return"))      return ccb_parse_statement_return(ccb);
    if (ccb_parse_identifier_check(ccb, token, "switch"))      return ccb_parse_statement_switch(ccb);
    if (ccb_parse_identifier_check(ccb, token, "case"))        return ccb_parse_statement_case(ccb);
    if (ccb_parse_identifier_check(ccb, token, "default"))     return ccb_parse_statement_default(ccb);
    if (ccb_parse_identifier_check(ccb, token, "break"))       return ccb_parse_statement_break(ccb);
    if (ccb_parse_identifier_check(ccb, token, "continue"))    return ccb_parse_statement_continue(ccb);
    if (ccb_parse_identifier_check(ccb, token, "goto"))        return ccb_parse_statement_goto(ccb);

    if (token->type == CCB_LEXER_TOKEN_IDENTIFIER && ccb_lexer_ispunct(ccb, ccb_lexer_peek(ccb), ':'))
        return ccb_parse_label(ccb, token);

    ccb_lexer_unget(ccb, token);

    ast = ccb_parse_expression(ccb);
    ccb_parse_expect(ccb, ';');

    return ast;
}

static void ccb_parse_statement_declaration(ccb_t* ccb, ccb_list_t* list) {
    ccb_lexer_token_t* token = ccb_lexer_peek(ccb);
    if (!token)
        ccb_compile_error(ccb, "statement declaration with unexpected ending");
    if (ccb_parse_type_check(ccb, token))
        ccb_parse_declaration(ccb, list, &ccb_ast_variable_local); // TODO: Should compiler allow function pointers w/o the "&" like gcc does?
    else
        ccb_list_push(list, ccb_parse_statement(ccb));
}

static ccb_ast_t* ccb_parse_statement_compound(ccb_t* ccb) {
    ccb_ast_localenv = ccb_table_create(ccb_ast_localenv);
    ccb_list_t* statements = ccb_list_create();
    for (;;) {
        /* Check for end before checking for inner statements, in order to handle {} without any statements. */
        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '}'))
            break;

        ccb_lexer_unget(ccb, token);

        ccb_parse_statement_declaration(ccb, statements);
    }
    ccb_ast_localenv = ccb_table_parent(ccb_ast_localenv);
    return ccb_ast_compound(ccb, statements);
}

static ccb_data_type_t* ccb_parse_function_parameters(ccb_t* ccb, ccb_list_t* paramvars, ccb_data_type_t* returntype) {
    bool           typeonly = !paramvars;
    ccb_list_t* paramtypes = ccb_list_create();
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    ccb_lexer_token_t* next = ccb_lexer_next(ccb);

    if (ccb_parse_identifier_check(ccb, token, "void") && ccb_lexer_ispunct(ccb, next, ')'))
        return ccb_ast_prototype(ccb, returntype, paramtypes, false);
    ccb_lexer_unget(ccb, next);
    if (ccb_lexer_ispunct(ccb, token, ')'))
        return ccb_ast_prototype(ccb, returntype, paramtypes, true);
    ccb_lexer_unget(ccb, token);

    for (;;) {
        token = ccb_lexer_next(ccb);
        if (ccb_parse_identifier_check(ccb, token, "...")) {
            if (ccb_list_length(paramtypes) == 0)
                ccb_compile_error(ccb, "ICE: %s (0)", __func__);
            ccb_parse_expect(ccb, ')');
            return ccb_ast_prototype(ccb, returntype, paramtypes, true);
        }
        else {
            ccb_lexer_unget(ccb, token);
        }

        ccb_data_type_t* ptype;
        char* name;
        ccb_parse_function_parameter(ccb, &ptype, &name, typeonly);
        ccb_parse_semantic_notvoid(ccb, ptype);
        if (ptype->type == CCB_TYPE_ARRAY)
            ptype = ccb_ast_pointer(ccb, ptype->pointer);
        ccb_list_push(paramtypes, ptype);

        if (!typeonly)
            ccb_list_push(paramvars, ccb_ast_variable_local(ccb, ptype, name));

        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, ')'))
            return ccb_ast_prototype(ccb, returntype, paramtypes, false);

        if (!ccb_lexer_ispunct(ccb, token, ','))
            ccb_compile_error(ccb, "ICE: %s (2)", __func__);
    }
}

static ccb_ast_t* ccb_parse_function_definition(ccb_t* ccb, ccb_data_type_t* functype, char* name, ccb_list_t* parameters) {
    ccb_ast_localenv = ccb_table_create(ccb_ast_localenv);
    ccb_ast_locals = ccb_list_create();
    ccb_ast_data_table[CCB_AST_DATA_FUNCTION] = functype;

    ccb_ast_t* body = ccb_parse_statement_compound(ccb);
    ccb_ast_t* r = ccb_ast_function(ccb, functype, name, parameters, body, ccb_ast_locals);

    ccb_table_insert(ccb_ast_globalenv, name, r);

    ccb_ast_data_table[CCB_AST_DATA_FUNCTION] = NULL;
    ccb_ast_localenv = NULL;
    ccb_ast_locals = NULL;

    return r;
}

static bool ccb_parse_function_definition_check(ccb_t* ccb) {
    ccb_list_t* buffer = ccb_list_create();
    int     nests = 0;
    bool    paren = false;
    bool    ready = true;

    for (;;) {

        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        ccb_list_push(buffer, token);

        //printf("Checking token... %d %s %s\n", nests, paren? "paren":"no-paren", ready?"ready":"no-ready");

        if (!token)
            ccb_compile_error(ccb, "function definition with unexpected ending");

        if (nests == 0 && paren && ccb_lexer_ispunct(ccb, token, '{'))
            break;

        if (nests == 0 && (ccb_lexer_ispunct(ccb, token, ';')
            || ccb_lexer_ispunct(ccb, token, ',')
            || ccb_lexer_ispunct(ccb, token, '=')))
        {
            ready = false;
            break;
        }

        if (ccb_lexer_ispunct(ccb, token, '('))
            nests++;

        if (ccb_lexer_ispunct(ccb, token, ')')) {
            if (nests == 0)
                ccb_compile_error(ccb, "unmatches parenthesis");
            paren = true;
            nests--;
        }
    }

    while (ccb_list_length(buffer) > 0)
        ccb_lexer_unget(ccb, ccb_list_pop(buffer));

    return ready;
}

static ccb_ast_t* ccb_parse_function_definition_intermediate(ccb_t* ccb) {
    ccb_data_type_t* basetype;
    char* name;
    ccb_list_t* parameters = ccb_list_create();

    basetype = ccb_parse_declaration_specification(ccb, NULL);
    ccb_ast_localenv = ccb_table_create(ccb_ast_globalenv);
    ccb_ast_labels = ccb_table_create(NULL);
    ccb_ast_gotos = ccb_list_create();

    ccb_data_type_t* functype = ccb_parse_declarator(ccb, &name, basetype, parameters, CCB_CDECL_BODY);
    ccb_parse_expect(ccb, '{');
    ccb_ast_t* value = ccb_parse_function_definition(ccb, functype, name, parameters);

    ccb_parse_label_backfill(ccb);

    ccb_ast_localenv = NULL;
    return value;
}

/* TODO: Predeclaration is required to self-compile recursive functions. */
static ccb_data_type_t* ccb_parse_declarator_direct_restage(ccb_t* ccb, ccb_data_type_t* basetype, ccb_list_t* parameters);
static ccb_data_type_t* ccb_parse_declarator_direct_restage(ccb_t* ccb, ccb_data_type_t* basetype, ccb_list_t* parameters) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    if (ccb_lexer_ispunct(ccb, token, '[')) {
        int length;
        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, ']'))
            length = -1;
        else {
            ccb_lexer_unget(ccb, token);
            length = ccb_parse_evaluate(ccb, ccb_parse_expression(ccb));
            ccb_parse_expect(ccb, ']');
        }

        ccb_data_type_t* type = ccb_parse_declarator_direct_restage(ccb, basetype, parameters);
        if (type->type == CCB_TYPE_FUNCTION)
            ccb_compile_error(ccb, "array of functions");
        return ccb_ast_array(ccb, type, length);
    }
    if (ccb_lexer_ispunct(ccb, token, '(')) {
        if (basetype->type == CCB_TYPE_FUNCTION)
            ccb_compile_error(ccb, "function returning function");
        if (basetype->type == CCB_TYPE_ARRAY)
            ccb_compile_error(ccb, "function returning array");
        return ccb_parse_function_parameters(ccb, parameters, basetype);
    }
    ccb_lexer_unget(ccb, token);
    return basetype;
}

static void ccb_parse_qualifiers_skip(ccb_t* ccb) {
    for (;;) {
        ccb_lexer_token_t* token = ccb_lexer_next(ccb);
        if (ccb_parse_identifier_check(ccb, token, "const")
            || ccb_parse_identifier_check(ccb, token, "volatile")
            || ccb_parse_identifier_check(ccb, token, "restrict")) {
            continue;
        }
        ccb_lexer_unget(ccb, token);
        return;
    }
}

// TODO: Predeclaration is required for recursive functions
static ccb_data_type_t* ccb_parse_declarator_direct(ccb_t* ccb, char** rname, ccb_data_type_t* basetype, ccb_list_t* parameters, ccb_cdecl_t context);
static ccb_data_type_t* ccb_parse_declarator_direct(ccb_t* ccb, char** rname, ccb_data_type_t* basetype, ccb_list_t* parameters, ccb_cdecl_t context) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    ccb_lexer_token_t* next = ccb_lexer_peek(ccb);
    bool hascallconv = false;
    int callconv = 0;

    if (ccb_lexer_ispunct(ccb, token, '(') && !ccb_parse_type_check(ccb, next) && !ccb_lexer_ispunct(ccb, next, ')')) {
        if (ccb_parse_identifier_check(ccb, next, "__cdecl")) {
            fprintf(stderr, "NOTE: Parsed a __cdecl but this may be ignored\n");
            // TODO..
            hascallconv = true;
            callconv = 100;
            next = ccb_lexer_next(ccb);
        }
        ccb_data_type_t* stub = ccb_ast_type_stub(ccb);
        ccb_data_type_t* type = ccb_parse_declarator_direct(ccb, rname, stub, parameters, context);
        //fprintf(stderr, "AAA\n");
        ccb_parse_expect(ccb, ')');
        //fprintf(stderr, "AAB\n");

        // TODO: This doesn't compile yet: *stub = *ccb_parse_declarator_direct_restage(ccb, basetype, parameters);
        ccb_data_type_t* tmp = ccb_parse_declarator_direct_restage(ccb, basetype, parameters);
        memcpy(stub, tmp, sizeof(ccb_data_type_t));
        if (hascallconv) {
            type->callconv = callconv;
        }
        return type;
    }

    if (ccb_lexer_ispunct(ccb, token, '*')) {
        ccb_parse_qualifiers_skip(ccb);
        ccb_data_type_t* stub = ccb_ast_type_stub(ccb);
        ccb_data_type_t* type = ccb_parse_declarator_direct(ccb, rname, stub, parameters, context);

        // TODO: This doesn't compile yet: *stub = *ccb_ast_pointer(ccb, basetype);
        ccb_data_type_t* tmp = ccb_ast_pointer(ccb, basetype);;
        memcpy(stub, tmp, sizeof(ccb_data_type_t));
        return type;
    }

#ifdef CCB_X_OBJC
    if (context == CCB_CDECL_OBJCPARAMETER) {
        if (ccb_lexer_ispunct(ccb, token, ')')) {
            token = ccb_lexer_next(ccb);
        }
        else {
            ccb_compile_error(ccb, "expected `)' to finish objc parameter type");
        }
    }
#endif

    if (ccb_parse_identifier_check(ccb, token, "__cdecl")) {
        fprintf(stderr, "NOTE: Parsed a __cdecl but this may be ignored\n");
        // TODO..
        hascallconv = true;
        callconv = 100;
        token = ccb_lexer_next(ccb);
    }

    if (token->type == CCB_LEXER_TOKEN_IDENTIFIER) {
        if (context == CCB_CDECL_CAST || rname == NULL)
            ccb_compile_error(ccb, "wasn't expecting identifier `%s'", ccb_lexer_tokenstr(ccb, token));
        *rname = token->string;
        ccb_data_type_t* type =  ccb_parse_declarator_direct_restage(ccb, basetype, parameters);
        if (hascallconv) {
            type->callconv = callconv;
        }
        return type;
    }

#ifdef CCB_X_OBJC
    if (context == CCB_CDECL_BODY || context == CCB_CDECL_PARAMETER || context == CCB_CDECL_OBJCPARAMETER)
#else
    if (context == CCB_CDECL_BODY || context == CCB_CDECL_PARAMETER)
#endif
        ccb_compile_error(ccb, "expected identifier, `(` or `*` for declarator");

    ccb_lexer_unget(ccb, token);

    ccb_data_type_t* result =  ccb_parse_declarator_direct_restage(ccb, basetype, parameters);
    if (hascallconv) {
        result->callconv = callconv;
    }
    return result;
}

static void ccb_parse_array_fix(ccb_t* ccb, ccb_data_type_t* type); // TODO: Predeclaration required for self-compile
static void ccb_parse_array_fix(ccb_t* ccb, ccb_data_type_t* type) {
    if (type->type == CCB_TYPE_ARRAY) {
        ccb_parse_array_fix(ccb, type->pointer);
        type->size = type->length * type->pointer->size;
    }
    else if (type->type == CCB_TYPE_POINTER) {
        ccb_parse_array_fix(ccb, type->pointer);
    }
    else if (type->type == CCB_TYPE_FUNCTION) {
        ccb_parse_array_fix(ccb, type->returntype);
    }
}

static ccb_data_type_t* ccb_parse_declarator(ccb_t* ccb, char** rname, ccb_data_type_t* basetype, ccb_list_t* parameters, ccb_cdecl_t context) {
    ccb_data_type_t* type = ccb_parse_declarator_direct(ccb, rname, basetype, parameters, context);
#ifdef CCB_X_OBJC
    if (context != CCB_CDECL_OBJCPARAMETER)
#endif
        ccb_parse_array_fix(ccb, type);
    return type;
}

static void ccb_parse_declaration(ccb_t* ccb, ccb_list_t* list, ccb_ast_t* (*make)(ccb_t* ccb, ccb_data_type_t*, char*)) {
    ccb_storage_t      storage;
    ccb_data_type_t* basetype = ccb_parse_declaration_specification(ccb, &storage);
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);

    if (ccb_lexer_ispunct(ccb, token, ';'))
        return;

    ccb_lexer_unget(ccb, token);

    for (;;) {
        char* name = NULL;
        ccb_data_type_t* type = ccb_parse_declarator(ccb, &name, ccb_ast_type_copy_incomplete(ccb, basetype), NULL, CCB_CDECL_BODY);

        if (storage == CCB_STORAGE_STATIC)
            type->isstatic = true;

        token = ccb_lexer_next(ccb);
        if (ccb_lexer_ispunct(ccb, token, '=')) {
            if (storage == CCB_STORAGE_TYPEDEF)
                ccb_compile_error(ccb, "invalid use of typedef");
            ccb_parse_semantic_notvoid(ccb, type);
            ccb_ast_t* var = make(ccb, type, name);
            ccb_list_push(list, ccb_ast_declaration(ccb, var, ccb_parse_initializer_declaration(ccb, var->ctype)));
            token = ccb_lexer_next(ccb);
        }
        else if (storage == CCB_STORAGE_TYPEDEF) {
            ccb_table_insert(ccb_parse_typedefs, name, type);
        }
        else if (type->type == CCB_TYPE_FUNCTION) {
            make(ccb, type, name);
        }
        else {
            ccb_ast_t* var = make(ccb, type, name);
            if (storage != CCB_STORAGE_EXTERN)
                ccb_list_push(list, ccb_ast_declaration(ccb, var, NULL));
        }
        if (ccb_lexer_ispunct(ccb, token, ';'))
            return;
        if (!ccb_lexer_ispunct(ccb, token, ','))
            ccb_compile_error(ccb, "Confused!");
    }
}

#ifdef CCB_X_OBJC
bool ccb_parse_objc_definition_check(ccb_t* ccb) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);

    if (ccb_lexer_ispunct(ccb, token, '@')) {
        token = ccb_lexer_next(ccb);
        if (token->type != CCB_LEXER_TOKEN_IDENTIFIER) {
            ccb_compile_error(ccb, "unexpected token following @: `%s'", ccb_lexer_tokenstr(ccb, token));
        }

        if (strcmp(token->string, "interface") == 0) {
            ccb_lexer_unget(ccb, token);
            return true;
        }
        else if (strcmp(token->string, "implementation") == 0) {
            ccb_lexer_unget(ccb, token);
            return true;
        }
        else if (strcmp(token->string, "class") == 0) {
            ccb_lexer_unget(ccb, token);
            return true;
        }
        else if (strcmp(token->string, "protocol") == 0) {
            ccb_compile_error(ccb, "protocols aren't supported yet");
            return false; // Unreachable
        }
        else {
            ccb_compile_error(ccb, "unexpected identifier following @: `%s'", token->string);
            return false; // Unreachable
        }
    }
    else {
        ccb_lexer_unget(ccb, token);
        return false;
    }
}
#endif

#ifdef CCB_X_OBJC
/* Parses method (definition and/or implementation) and returns it or NULL when finished. */
ccb_ast_t* ccb_parse_objc_method(ccb_t* ccb, char* classname, bool isimpl, ccb_list_t* initstmts) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb); // Should be either + or - if this is a method
    bool           ismeta = false;
    char* selector = NULL;
    char* abiname = NULL;
    char* tmp = NULL;
    bool           moreargs = false;

    if (ccb_lexer_ispunct(ccb, token, '+')) {
        ismeta = true;
    }
    else if (ccb_lexer_ispunct(ccb, token, '-')) {
        ismeta = false;
    }
    else {
        ccb_lexer_unget(ccb, token);
        return NULL;
    }

    if (isimpl) {

        ccb_ast_localenv = ccb_table_create(ccb_ast_globalenv);
        ccb_ast_labels = ccb_table_create(NULL);
        ccb_ast_gotos = ccb_list_create();
    }

    token = ccb_lexer_next(ccb);
    ccb_data_type_t* rettype = NULL;
    if (ccb_lexer_ispunct(ccb, token, '(')) {
        ccb_data_type_t* basetype = ccb_parse_declaration_specification(ccb, NULL);
        rettype = ccb_parse_declarator(ccb, NULL, basetype, NULL, CCB_CDECL_TYPEONLY);
        ccb_parse_expect(ccb, ')');
        token = ccb_lexer_next(ccb);
    }

    if (token->type != CCB_LEXER_TOKEN_IDENTIFIER) {
        ccb_compile_error(ccb, "unexpected token following `%s': `%s'", ismeta ? "+" : "-", ccb_lexer_tokenstr(ccb, token));
    }

    selector = calloc(strlen(token->string) + 1000, 1); // TODO: A more appropriate limit
    abiname = calloc(strlen(token->string) + 1000, 1); // TODO: A more appropriate limit
    strcat(selector, token->string);// _strdup(token->string);
    strcat(abiname, classname);
    strcat(abiname, "__");// _strdup(token->string);
    if (ismeta) {
        strcat(abiname, "meta__");
    } else {
        strcat(abiname, "instance__");
    }
    strcat(abiname, token->string);// _strdup(token->string);

    ccb_list_t* paramtypes = ccb_list_create();
    ccb_list_t* paramvars = ccb_list_create();

    ccb_list_push(paramtypes, ccb_ast_data_table[CCB_AST_DATA_INT]);
    ccb_list_push(paramvars, ccb_ast_variable_local(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], "self"));

    if (ccb_lexer_ispunct(ccb, token = ccb_lexer_next(ccb), ':')) {
        moreargs = true;
        do {
            tmp = selector;
            selector = strcat(tmp, ":");
            strcat(abiname, "_");
            //free(tmp); XXX wrong place?
            token = ccb_lexer_next(ccb);
            char* argname;
            if (ccb_lexer_ispunct(ccb, token, '(')) {
                ccb_data_type_t* basetype = ccb_parse_declaration_specification(ccb, NULL);
                ccb_data_type_t* casttype = ccb_parse_declarator(ccb, &argname, basetype, NULL, CCB_CDECL_OBJCPARAMETER);
                ccb_list_push(paramtypes, casttype);
                ccb_list_push(paramvars, ccb_ast_variable_local(ccb, casttype, argname));
            }
            else {
                ccb_lexer_unget(ccb, token);
                token = NULL;
            }
            token = ccb_lexer_next(ccb);
            if (token->type == CCB_LEXER_TOKEN_IDENTIFIER) {
                tmp = selector;
                selector = strcat(tmp, token->string);
                strcat(abiname, token->string);
                //free(tmp);
                ccb_parse_expect(ccb, ':');
                moreargs = true;
            }
            else {
                ccb_lexer_unget(ccb, token);
                token = NULL;
                moreargs = false;
            }
        } while (moreargs);
    }
    else {
        ccb_lexer_unget(ccb, token);
    }

    // ccb_data_type_t* ccb_ast_prototype(ccb_t* ccb, ccb_data_type_t* returntype, ccb_list_t* paramtypes, bool dots);
    ccb_data_type_t* functype = ccb_ast_prototype(ccb, rettype, paramtypes, false);

    if (isimpl) {
        
        ccb_ast_localenv = ccb_table_create(ccb_ast_localenv);
        ccb_ast_locals = ccb_list_create();
        ccb_ast_data_table[CCB_AST_DATA_FUNCTION] = functype;

        /*if (!ccb_lexer_ispunct(ccb, token, '{')) {
            ccb_compile_error(ccb, "Expected {...} for method implementation");
        }*/
        //ccb_lexer_next(ccb);
        //fprintf(stderr, "Parsing body...\n");
        ccb_ast_t* body = ccb_parse_statement(ccb);//_compound(ccb);
        //fprintf(stderr, "Done parsing body...\n");

        ccb_ast_t* r = ccb_ast_function(ccb, functype, abiname, paramvars, body, ccb_ast_locals);

        ccb_table_insert(ccb_ast_globalenv, abiname, r);

        ccb_ast_t* selstr = ccb_ast_new_string(ccb, selector);
        ccb_list_push(ccb_ast_strings, selstr);

        ccb_list_push(initstmts,
            ccb_parse_runtime_call_3(ccb, ismeta ? "objx_addmetamethod" : "objx_addinstancemethod",
                ccb_ast_variable_global(ccb, ccb_ast_data_table[CCB_AST_DATA_ID], classname),
                selstr,
                ccb_ast_new_unary(ccb, CCB_AST_TYPE_ADDRESS, ccb_ast_pointer(ccb, r->ctype), r)));//ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], 1234)));


        /* NOTE: This is a bit of a hack, but the objx_addinstancemethod/objx_addmetamethod call is
         * added to the initialisation stub function BEFORE we leave the context of the function we're adding.
         * This is so that objx_addinstancemethod/objx_addmetamethod gets looked up properly (otherwise, we'd
         * have to re-enter the init stub context instead, but the effect would be the same).
         */
        ccb_ast_data_table[CCB_AST_DATA_FUNCTION] = NULL;
        ccb_ast_localenv = NULL;
        ccb_ast_locals = NULL;

        return r;
    }
    else {
        ccb_parse_expect(ccb, ';');
    }

    return (void*)(size_t)1; // XXX hack...
}

ccb_ast_t* ccb_parse_objc_useinitstub(ccb_t* ccb, ccb_ast_t* expr) {
    char* abiname = calloc(1000, 1); // TODO: A more appropriate limit
    strcat(abiname, expr->variable.name);
    strcat(abiname, "__init");
    ccb_ast_t* initfunc = ccb_table_find(ccb_ast_globalenv, abiname);
    if (initfunc == NULL) {
        ccb_compile_error(ccb, "Can't find initialisation function '%s'", abiname);
    }
    ccb_ast_t* funcptrexpr = ccb_ast_new_unary(ccb, CCB_AST_TYPE_ADDRESS, ccb_ast_pointer(ccb, initfunc->ctype), initfunc);
    return ccb_parse_runtime_call_2(ccb, "objx_getclass",
        ccb_ast_new_unary(ccb, CCB_AST_TYPE_ADDRESS,
            ccb_ast_pointer(ccb, ccb_ast_data_table[CCB_AST_DATA_ID]),
            expr),
        funcptrexpr);
}

ccb_ast_t* ccb_parse_objc_mkinitstub(ccb_t* ccb, char* classname, ccb_list_t* stmts) {
    //bool isimpl = true;
    //ccb_lexer_token_t* token = ccb_lexer_next(ccb); // Should be either + or - if this is a method
    //bool           ismeta = false;
    //char* selector = NULL;
    char* abiname = NULL;
    //char* tmp = NULL;
    //bool           moreargs = false;


    ccb_data_type_t* typ = ccb_table_find(ccb_parse_typedefs, classname);
    const char* supname = "";
    int siz = -1;
    if (typ == NULL) {
        ccb_compile_warn(ccb, "No corresponding @interface for '%s'", classname);
    } else {
        supname = typ->supername;
        siz = typ->size;
        //if (supname == NULL) {
            //supname = "";
        //}
    }

    ccb_ast_localenv = ccb_table_create(ccb_ast_globalenv);
    ccb_ast_labels = ccb_table_create(NULL);
    ccb_ast_gotos = ccb_list_create();

    ccb_data_type_t* rettype = ccb_ast_data_table[CCB_AST_DATA_VOID];

    abiname = calloc(1000, 1); // TODO: A more appropriate limit
    strcat(abiname, classname);
    strcat(abiname, "__init");// _strdup(token->string);

    ccb_list_t* paramtypes = ccb_list_create();
    ccb_list_t* paramvars = ccb_list_create();

    //ccb_list_push(paramtypes, ccb_ast_data_table[CCB_AST_DATA_INT]);
    //ccb_list_push(paramvars, ccb_ast_variable_local(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], "self"));

    // ccb_data_type_t* ccb_ast_prototype(ccb_t* ccb, ccb_data_type_t* returntype, ccb_list_t* paramtypes, bool dots);
    ccb_data_type_t* functype = ccb_ast_prototype(ccb, rettype, paramtypes, false);

    ccb_ast_localenv = ccb_table_create(ccb_ast_localenv);
    ccb_ast_locals = ccb_list_create();
    ccb_ast_data_table[CCB_AST_DATA_FUNCTION] = functype;

    ccb_ast_t* body = ccb_ast_compound(ccb, (stmts == NULL) ? ccb_list_create() : stmts);

    ccb_ast_t* namestr = ccb_ast_new_string(ccb, classname);
    ccb_list_push(ccb_ast_strings, namestr);
    //ccb_ast_t* superstr = ccb_ast_new_string(ccb, supname);
    //ccb_list_push(ccb_ast_strings, superstr);

    if (stmts != NULL) {
        ccb_list_push(stmts,
            ccb_parse_runtime_call_4(ccb, "objx_addclass",
                ccb_ast_new_unary(ccb, CCB_AST_TYPE_ADDRESS, ccb_ast_pointer(ccb, ccb_ast_data_table[CCB_AST_DATA_ID]), ccb_ast_variable_global(ccb, ccb_ast_data_table[CCB_AST_DATA_ID], classname)),
                namestr,
                ((supname == NULL) ? ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_LLONG], 0) : ccb_parse_objc_useinitstub(ccb, ccb_ast_variable_global(ccb, ccb_ast_data_table[CCB_AST_DATA_ID], supname))),//superstr,
                ccb_ast_new_integer(ccb, ccb_ast_data_table[CCB_AST_DATA_INT], siz))); // Instance size
    }

    ccb_ast_t* r = ccb_ast_function(ccb, functype, abiname, paramvars, body, ccb_ast_locals);

    ccb_table_insert(ccb_ast_globalenv, abiname, r);

    ccb_ast_data_table[CCB_AST_DATA_FUNCTION] = NULL;
    ccb_ast_localenv = NULL;
    ccb_ast_locals = NULL;
    return r;
}
#endif

#ifdef CCB_X_OBJC
void ccb_parse_objc_interface(ccb_t* ccb, ccb_list_t* list, char* name) {
    ccb_lexer_token_t* token = ccb_lexer_next(ccb);
    char* supname = NULL;
    ccb_data_type_t* suptype = NULL;
    int supsize = 0;
    ccb_ast_t* method = NULL;

    if (ccb_lexer_ispunct(ccb, token, ':')) {
        token = ccb_lexer_next(ccb);
        if (token->type == CCB_LEXER_TOKEN_IDENTIFIER) {
            supname = token->string;
            suptype = ccb_table_find(ccb_parse_typedefs, supname);
            if (suptype == NULL) {
                ccb_compile_error("Undefined base class '%s'", supname);
            } else {
                supsize = suptype->size;
                while ((supsize % ccb_target_type_size_pointer(ccb)) != 0) {
                    supsize++;
                }
            }
        }
        else {
            ccb_compile_error(ccb, "unexpected token following `:': `%s'", ccb_lexer_tokenstr(ccb, token));
        }
    }
    else {
        ccb_lexer_unget(ccb, token);
        token = NULL;
    }

    /* Before we actually begin parsing the class, which initially happens by parsing the "struct"-like part,
     * we first create a dummy typedef.
     * That is, an "@interface Foo {...}" translates into a "typedef struct Foo Foo", followed by the "struct Foo {...}"
     * (the main difference for the struct part being that a flag is set to say that it's a class).
     */

    /* The first call to ccb_parse_tag_definition(...) is given a -1 initial size, indicating that it isn't to
     * attempt to read any fields yet.
     */
    ccb_data_type_t* foo = ccb_parse_tag_definition(ccb, ccb_ast_structures, true, -1, name);

    foo->classname = name;
    foo->supername = supname;

    ccb_table_insert(ccb_parse_typedefs, name, foo);

    foo = ccb_parse_tag_definition(ccb, ccb_ast_structures, true, supsize, name);

    foo->classname = name;
    foo->supername = supname;

    ccb_table_insert(ccb_parse_typedefs, name, foo); //ccb_ast_data_table[CCB_AST_DATA_INT]);

    while ((method = ccb_parse_objc_method(ccb, name, false, NULL)) != NULL) {
        //ccb_list_push(list, method);
        ccb_compile_warn(ccb, "ignoring @interface method (TODO)");
    }

    //ccb_compile_error(ccb, "TODO: @interface");

    ccb_parse_expect(ccb, '@');
    token = ccb_lexer_next(ccb);
    if (token->type != CCB_LEXER_TOKEN_IDENTIFIER || strcmp(token->string, "end") != 0) {
        ccb_compile_error(ccb, "unexpected token following `@': `%s'", ccb_lexer_tokenstr(ccb, token));
    }
    ccb_ast_t* globvar = ccb_ast_variable_global(ccb, ccb_ast_data_table[CCB_AST_DATA_ID], name);
    globvar->variable.isclassobj = true;

    //ccb_list_t* initstmts = ccb_list_create();
    ccb_parse_objc_mkinitstub(ccb, name, NULL); //initstmts);
}
#endif

#ifdef CCB_X_OBJC
void ccb_parse_objc_implementation(ccb_t* ccb, ccb_list_t* list, char* name) {
    ccb_lexer_token_t* token = NULL;
    ccb_ast_t* method = NULL;

    ccb_list_t* initstmts = ccb_list_create();
    ccb_ast_t* initstub = ccb_parse_objc_mkinitstub(ccb, name, initstmts);

    ccb_list_push(list, initstub);

    while ((method = ccb_parse_objc_method(ccb, name, true, initstmts)) != NULL) {
        ccb_list_push(list, method);
        //ccb_compile_warn(ccb, "ignoring @implementation method (TODO)");
    }

    ccb_parse_expect(ccb, '@');
    token = ccb_lexer_next(ccb);
    if (token->type != CCB_LEXER_TOKEN_IDENTIFIER || strcmp(token->string, "end") != 0) {
        ccb_compile_error(ccb, "unexpected token following `@': `%s'", ccb_lexer_tokenstr(ccb, token));
    }

    // Has already been declared like an extern in the @interface section, but the class object is declared as a global here
    ccb_ast_t* globvar = ccb_ast_variable_global(ccb, ccb_ast_data_table[CCB_AST_DATA_ID], name);
    globvar->variable.isclassobj = true;
    ccb_list_push(list, ccb_ast_declaration(ccb, globvar, ccb_list_create()));

    //ccb_compile_error(ccb, "TODO: @implementation");
}
#endif

#ifdef CCB_X_OBJC
void ccb_parse_objc_forwardclass(ccb_t* ccb, ccb_list_t* list, char* name) {
    ccb_parse_expect(ccb, ';');

    ccb_table_insert(ccb_parse_typedefs, name, ccb_ast_data_table[CCB_AST_DATA_ID]);

    ccb_compile_warn(ccb, "TODO: Finish @class");

    ccb_list_push(list, ccb_ast_declaration(ccb, ccb_ast_variable_global(ccb, ccb_ast_data_table[CCB_AST_DATA_ID], name), ccb_list_create()));
}
#endif

#ifdef CCB_X_OBJC
void ccb_parse_objc_definition(ccb_t* ccb, ccb_list_t* list) {
    // The '@' token has already been read (but the 'interface'/'implementation'/... and name tokens haven't)
    ccb_lexer_token_t* ttoken = ccb_lexer_next(ccb);
    ccb_lexer_token_t* ntoken = NULL;

    if (ttoken->type == CCB_LEXER_TOKEN_IDENTIFIER) {
        ntoken = ccb_lexer_next(ccb);
        if (ntoken->type != CCB_LEXER_TOKEN_IDENTIFIER) {
            ccb_compile_error(ccb, "unexpected token following @%s: `%s'", ttoken->string, ccb_lexer_tokenstr(ccb, ntoken));
        }

        if (strcmp(ttoken->string, "interface") == 0) {
            ccb_parse_objc_interface(ccb, list, ntoken->string);
            return;
        }
        else if (strcmp(ttoken->string, "implementation") == 0) {
            ccb_parse_objc_implementation(ccb, list, ntoken->string);
            return;
        }
        else if (strcmp(ttoken->string, "class") == 0) {
            ccb_parse_objc_forwardclass(ccb, list, ntoken->string);
            return;
        }
        ccb_lexer_unget(ccb, ntoken);
    }

    ccb_compile_error(ccb, "TODO: @%s", ccb_lexer_tokenstr(ccb, ttoken));
}
#endif

ccb_list_t* ccb_parse_run(ccb_t* ccb) {
    ccb_list_t* list = ccb_list_create();
    for (;;) {
        if (!ccb_lexer_peek(ccb))
            return list;
#ifdef CCB_X_OBJC
        if (ccb_parse_objc_definition_check(ccb)) {
            ccb_parse_objc_definition(ccb, list);
        }
        else
#endif
            if (ccb_parse_function_definition_check(ccb))
                ccb_list_push(list, ccb_parse_function_definition_intermediate(ccb));
            else
                ccb_parse_declaration(ccb, list, &ccb_ast_variable_global);
    }
    return NULL;
}


/* From ifdef of CCB_IMPLEMENTATION: */
#endif

/* From ifndef at top of file: */
#endif
