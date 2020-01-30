#pragma once

/* 

Simple C-Like code emitter

*/

struct Node {
	enum Type {
		EMPTY,            
		STRUCT_DEF,       
		FUNCTION_DEF,	  
		CONSTRUCTOR_DEF,  
		FUNCTION_CALL,	  
		BLOCK,			  
		LIST,             
		CONSTRUCTOR,      
		DECLARATION,      
		ASSIGNMENT,		
		ACCESS,
		INDEX,
		SCOPE,
		CAST,
		PAREN_SURROUND,
		POINTER,
		UNARY_OPERATOR,	  
		BINARY_OPERATOR,  
		TERNARY_OPERATOR, 
		RETURN,           
		LITERAL,		  
		IDENTIFIER,		
		PRAGMA,
		INCLUDE_HEADER,
		INCLUDE_SOURCE,
		TYPE_COUNT,
	};

	enum Operator {
		OPERATOR_NONE,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		MODULO,
		NEGATE,
		EQUAL,
		NOT_EQUAL,
		GREATER,
		GREATER_OR_EQUAL,
		LESS,
		LESS_OR_EQUAL,
		LOGICAL_AND,
		LOGICAL_OR,
		LOGICAL_NOT,
		BITWISE_AND,
		BITWISE_OR,
		BITWISE_XOR,
		BITWISE_NOT,
		BITWISE_SHIFT_LEFT,
		BITWISE_SHIFT_RIGHT,
		OPERATOR_COUNT,
	};

	enum Literal {
		UINT8,
		SINT8,
		UINT16,
		SINT16,
		UINT32,
		SINT32,
		UINT64,
		SINT64,
		FLOAT32,
		FLOAT64,
		STRING,
		LITERAL_COUNT,
	};

	enum List {
		COMMA,
		SEMICOLON,
	};

	enum QualifierFlag {
		CONST_FLAG     = (1<<0), 
		STATIC_FLAG    = (1<<1),
		EXPLICIT_FLAG  = (1<<2),
		INLINE_FLAG    = (1<<3),
		VOLATILE_FLAG  = (1<<4),
		MUTABLE_FLAG   = (1<<5),
		REFERENCE_FLAG = (1<<6),
	};

	unsigned char type;
	unsigned char op;      // #OPT these are mutually exclusive, can union if need more space
	unsigned char literal;
	struct { // qualifiers
		unsigned char is_const : 1;
		unsigned char is_static : 1;
		unsigned char is_explicit : 1;
		unsigned char is_inline : 1;
		unsigned char is_volatile : 1;
		unsigned char is_mutable : 1;
		unsigned char is_reference : 1;
		unsigned char unused_qualifier2 : 1;
	};

	// Inner nodes have both kid and sib
	// Leaf nodes have constant data instead of kid, and additional data instead of sib
	union {
		Node* kid;

		float       float_literal;
		int         int_literal;
		const char* string_literal; // null terminated
		const char* identifier;     // null terminated
	};
	union {
		Node* sib;

		size_t string_len;
	};
};

struct NodePool {
	Node* nodes; // replace with slab allocator later (pointers must not be invalidated on new node allocs)
	Node* curr;
	void alloc();
	void free();
	Node* take(int count); // take COUNT contiguous nodes and establish links between them
};

Node* make(NodePool* pool, Node::Type type);
Node* makeKid(NodePool* pool, Node* par, Node::Type type);
void setIdentifier(Node* who, const char* val);
void  setType(Node* who, Node* type);
void  setName(Node* who, Node* name);
Node* getType(Node* who);
Node* getName(Node* who);
void  addToArguments(Node*);
void  addToBody(Node*);
void  addKid(Node* par, Node* kid);

void setNodePool(NodePool* pool);
Node* list();
Node* list(Node* kid0);
Node* list(Node* kid0, Node* kid1);
Node* list(Node* kid0, Node* kid1, Node* kid2); //#TODO varargs
Node* block();
Node* block(Node* kid0);
Node* block(Node* kid0, Node* kid1);
void  add(Node* list_or_block, Node* who);
Node* iden(const char* str, int qualifier_flags = 0);
Node* decl(Node* type, Node* name);
Node* decl(const char* type_iden, const char* name_iden);
Node* assign(Node* lhs, Node* rhs);
Node* assign(const char* lhs_iden, const char* rhs_iden);
Node* assign(Node* lhs, Node::Operator op, Node* rhs); // compound assignment
Node* access(Node* name, Node* member);
Node* access(const char* name_iden, const char* member_iden);
Node* function_def(Node* return_type, Node* name, Node* argument_list, Node* body_block, int qualifier_flags = 0);
Node* ret(Node* who);
Node* op(Node::Operator op, Node* rhs);            // unary
Node* op(Node* lhs, Node::Operator op, Node* rhs); // binary
Node* construct(Node* type, Node* argument_list);
Node* construct(const char* type_iden, Node* argument_list);
Node* function_call(Node* type, Node* argument_list);
Node* function_call(const char* type_iden, Node* argument_list);
Node* struct_def(Node* name, Node* body_block);
Node* struct_def(const char* name_iden, Node* body_block);
Node* constructor_def(Node* name, Node* argument_list, Node* init_list, Node* body_block, int qualifier_flags = 0);
Node* constructor_def(const char* name_iden, Node* argument_list, Node* init_list, Node* body_block, int qualifier_flags = 0);
Node* scope(Node* par, Node* kid);
Node* scope(const char* par_iden, const char* kid_iden);
Node* cast(Node* to, Node* from);
Node* pragma_directive(const char* str);
Node* include_header(const char* str);
Node* include_source(const char* str);
Node* pointer(Node* type);
Node* pointer(const char* type_iden);
Node* paren_surround(Node* who);
Node* index(Node* name, Node* idx);
void set_qualifiers(Node* who, int qualifier_flags);

// formatting goes here! (hints will come from emitNode, but the actual punctuation and the like can happen here, separate from ast logic. basically this is the 'struct of state we pass around', both the output and the format state (since nodes don't have state, and node pool is just for construction)
struct CodeStream {
	char* code;
	char* curr;
	int indent_level;
	void alloc();
	void free();
	void finalize();
	void writes(const char* str, size_t len);
	void writes(const char* str);
	void writef(const char *format, ...);
	void token(const char* str);
	void space();      // syntactic 
	void list_space(); // optional
	void brace_space();
	void endstatement();
	void block_newline();
	void brace_newline();
	void comma();
	void double_quote();
	void dot();
	void colon();
	void star();
	void assign(Node::Operator op);
	void paren(bool open);
	void brace(bool open);
	void bracket(bool open);
	void angle_bracket(bool open);
	void indent(bool in);
	void op(Node::Operator op);
	size_t byteSize();
};

void emit(CodeStream* str, Node* who);