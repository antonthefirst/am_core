#include "c_emit.h"
#include <stdlib.h> // for malloc
#include <string.h> // for memcpy
#include <assert.h>

#define OUT_BYTESIZE (1024*1024*4)

namespace {

enum {
	SCOPE_BLOCK,
	SCOPE_STRUCT,
	SCOPE_FUNCTION,
	SCOPE_ARGUMENTS
};

enum {
	LINE_START,
	LINE_MID,
	LINE_END,
};

enum {
	ARGS_START,
	ARGS_MID,
};

enum {
	STATEMENT_NONE,
	STATEMENT_STRUCT,
	STATEMENT_BLOCK,
	STATEMENT_FUNCTION_DEFINITION,
	STATEMENT_FUNCTION_CALL,
	STATEMENT_GENERIC,
};

};

void NodePool::alloc() {
	nodes = (Node*)malloc(128*1024*sizeof(Node));
	curr = nodes;
}
void NodePool::free() {
	::free(nodes);
}
Node* NodePool::take(int count) {
	Node* ret = curr;
	curr += count;
	for (int i = 0; i < count; ++i) {
		ret[i] = { }; // zero it out
		if (i < (count-1))
			ret[i].sib = ret + 1;
	}
	return ret;
}

#include <stdio.h>
#include <stdarg.h>
void CodeStream::alloc() {
	code = (char*)malloc(1024*1024*4); //#DODGY 4MB should be enough for anybody
	curr = code;
	indent_level = 0;
}
void CodeStream::free() {
	::free(code);
}
void CodeStream::finalize() {
	*curr = 0;
}
void CodeStream::writes(const char* str, size_t l) {
	memcpy(curr, str, l); //#BUG could overwrite mem
}
void CodeStream::writes(const char* str) {
	while(*str) *curr++ = *str++; //#BUG could overrun mem
}
void CodeStream::writef(const char *format, ...) {
	va_list args;
	va_start(args, format);
	curr += vsprintf(curr, format, args); //#BUG needs to be replaced with a sized version, and check for bounds
	va_end(args);
}
void CodeStream::token(const char* str) {
	writes(str);
	//writes(" ");
}
void CodeStream::space() {
	writes(" ");
}
void CodeStream::list_space() {
	writes(" ");
}
void CodeStream::brace_space() {
	writes(" ");
}
void CodeStream::endstatement() {
	writes(";");
}
void CodeStream::block_newline() {
	writes("\n");
	for (int i = 0; i < indent_level; ++i) writes("\t");
}
void CodeStream::brace_newline() {
	writes("\n");
	for (int i = 0; i < indent_level; ++i) writes("\t");
}
void CodeStream::comma() {
	writes(",");
}
void CodeStream::double_quote() {
	writes("\"");
}
void CodeStream::dot() {
	writes(".");
}
void CodeStream::colon() {
	writes(":");
}
void CodeStream::star() {
	writes("*");
}
void CodeStream::assign(Node::Operator the_op) {
	if (the_op != Node::OPERATOR_NONE)
		op(the_op);
	writes("=");
}
void CodeStream::paren(bool open) {
	writes(open ? "(" : ")");
}
void CodeStream::brace(bool open) {
	writes(open ? "{" : "}");
}
void CodeStream::bracket(bool open) {
	writes(open ? "[" : "]");
}
void CodeStream::angle_bracket(bool open) {
	writes(open ? "<" : ">");
}
void CodeStream::indent(bool in) {
	indent_level += in ? +1 : -1;
	assert(indent_level >= 0);
}
void CodeStream::op(Node::Operator oper) {
	switch (oper) {
	case Node::ADD:                 writes("+");  break;
	case Node::SUBTRACT:            writes("-");  break;
	case Node::MULTIPLY:            writes("*");  break;
	case Node::DIVIDE:              writes("/");  break;
	case Node::MODULO:              writes("%");  break;
	case Node::NEGATE:              writes("-");  break;
	case Node::EQUAL:               writes("=="); break;
	case Node::NOT_EQUAL:           writes("!="); break;
	case Node::GREATER:             writes(">");  break;
	case Node::GREATER_OR_EQUAL:    writes(">="); break;
	case Node::LESS:                writes("<");  break;
	case Node::LESS_OR_EQUAL:       writes("<="); break;
	case Node::LOGICAL_AND:         writes("&&"); break;
	case Node::LOGICAL_OR:          writes("||"); break;
	case Node::LOGICAL_NOT:         writes("!");  break;
	case Node::BITWISE_AND:         writes("&");  break;
	case Node::BITWISE_OR:          writes("|");  break;
	case Node::BITWISE_XOR:         writes("^");  break;
	case Node::BITWISE_NOT:         writes("~");  break;
	case Node::BITWISE_SHIFT_LEFT:  writes("<<");  break;
	case Node::BITWISE_SHIFT_RIGHT: writes(">>");  break;
	default: assert(false);
	};
}
size_t CodeStream::byteSize() {
	return curr - code;
}

void emitStructDef(CodeStream* cod, Node* who) {
	cod->token("struct");
	cod->space();
	emit(cod, who->kid);           // name
	if (who->kid->sib) { // structs without a body are just declarations
		cod->brace_space();  
		cod->brace(true);
		cod->indent(true);
		cod->brace_newline();
		emit(cod, who->kid->sib); // body
		cod->indent(false);
		cod->block_newline();
		cod->brace(false);
	}
}
void emitFunctionDef(CodeStream* cod, Node* who) {
	if (who->is_static) {
		cod->token("static");
		cod->space();
	}
	if (who->is_explicit) {
		cod->token("explicit");
		cod->space();
	}
	if (who->is_inline) {
		cod->token("inline");
		cod->space();
	}
	emit(cod, who->kid);                // type
	cod->space();
	emit(cod, who->kid->sib);           // name
	cod->paren(true);
	emit(cod, who->kid->sib->sib);      // args
	cod->paren(false);
	if (who->is_const) {
		cod->space();
		cod->token("const");
	}
	if (who->kid->sib->sib->sib) { // if there's a body
		cod->brace_space();
		cod->brace(true);
		if (who->kid->sib->sib->sib->kid) { // if the body has contents
			cod->indent(true);
			cod->brace_newline();
			emit(cod, who->kid->sib->sib->sib); // body
			cod->indent(false);
			cod->block_newline();
		}
		cod->brace(false);
	}
}
void emitConstructorDef(CodeStream* cod, Node* who) {
	if (who->is_explicit) {
		cod->token("explicit");
		cod->space();
	}
	if (who->is_inline) {
		cod->token("inline");
		cod->space();
	}
	emit(cod, who->kid);           // name
	cod->paren(true);
	emit(cod, who->kid->sib);      // args
	cod->paren(false);
	if (who->kid->sib->sib->kid) { // if initializer list is not empty
		cod->colon();
		emit(cod, who->kid->sib->sib);
	}
	if (who->kid->sib->sib->sib) { // if there's a body
		cod->brace_space();
		cod->brace(true);
		if (who->kid->sib->sib->sib->kid) { // if body has contents
			cod->indent(true);
			cod->brace_newline();
			emit(cod, who->kid->sib->sib->sib); // body
			cod->indent(false);
			cod->block_newline();
		}
		cod->brace(false);
	}
}
void emitFunctionCall(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->paren(true);
	emit(cod, who->kid->sib);
	cod->paren(false);
}
void emitBlock(CodeStream* cod, Node* who) {
	Node* kid = who->kid;
	while(kid) {
		emit(cod, kid);
		if ((kid->type != Node::FUNCTION_DEF || !kid->kid->sib->sib->sib) && //#REFACTOR check for 'body exists'
			(kid->type != Node::CONSTRUCTOR_DEF || !kid->kid->sib->sib->sib) && 
			kid->type != Node::BLOCK &&
			kid->type != Node::PRAGMA && kid->type != Node::INCLUDE_HEADER && kid->type != Node::INCLUDE_SOURCE)
			cod->endstatement();
		if (kid->sib)
			cod->block_newline();
		kid = kid->sib;
	}
}
void emitList(CodeStream* cod, Node* who) {
	Node* kid = who->kid;
	while(kid) {
		emit(cod, kid);
		if (kid->sib) {
			cod->comma();
			cod->list_space();
		}
		kid = kid->sib;
	}
}
void emitConstructor(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->paren(true);
	emit(cod, who->kid->sib);
	cod->paren(false);
}
void emitDeclaration(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->space();
	emit(cod, who->kid->sib);
}
void emitAssignment(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->assign(Node::Operator(who->op));
	emit(cod, who->kid->sib);
}
void emitAccess(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->dot();
	emit(cod, who->kid->sib);
}
void emitIndex(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->bracket(true);
	emit(cod, who->kid->sib);
	cod->bracket(false);
}
void emitScope(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->colon();
	cod->colon();
	emit(cod, who->kid->sib);
}
void emitCast(CodeStream* cod, Node* who) {
	cod->paren(true);
	emit(cod, who->kid);
	cod->paren(false);
	emit(cod, who->kid->sib);
}
void emitParenSurround(CodeStream* cod, Node* who) {
	cod->paren(true);
	emit(cod, who->kid);
	cod->paren(false);
}
void emitPointer(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->star();
}
void emitUnaryOperator(CodeStream* cod, Node* who) {
	cod->op(Node::Operator(who->op));
	emit(cod, who->kid);
}
void emitBinaryOperator(CodeStream* cod, Node* who) {
	emit(cod, who->kid);
	cod->op(Node::Operator(who->op));
	emit(cod, who->kid->sib);
}
void emitTernaryOperator(CodeStream* cod, Node* who) {

}
void emitReturn(CodeStream* cod, Node* who) {
	cod->token("return");
	cod->space();
	emit(cod, who->kid);
}
void emitLiteral(CodeStream* cod, Node* who) {

}
void emitIdentifier(CodeStream* cod, Node* who) {
	if (who->is_const) {
		cod->token("const");
		cod->space();
	}
	cod->token(who->identifier);
	if (who->is_reference) {
		cod->token("&");
	}
}
void emitPragma(CodeStream* cod, Node* who) {
	cod->token("#pragma");
	cod->space();
	cod->token(who->identifier);
}
void emitIncludeHeader(CodeStream* cod, Node* who) {
	cod->token("#include");
	cod->space();
	cod->angle_bracket(true);
	cod->token(who->identifier);
	cod->angle_bracket(false);
}
void emitIncludeSource(CodeStream* cod, Node* who) {
	cod->token("#include");
	cod->space();
	cod->double_quote();
	cod->token(who->identifier);
	cod->double_quote();
}
void emit(CodeStream* cod, Node* who) {
	switch(who->type) {
	case Node::STRUCT_DEF:       emitStructDef(cod, who);          break;
	case Node::FUNCTION_DEF:     emitFunctionDef(cod, who);        break;
	case Node::CONSTRUCTOR_DEF:  emitConstructorDef(cod, who);     break;
	case Node::FUNCTION_CALL:    emitFunctionCall(cod, who);       break;
	case Node::BLOCK:            emitBlock(cod, who);              break;
	case Node::LIST:             emitList(cod, who);               break;
	case Node::CONSTRUCTOR:      emitConstructor(cod, who);        break; 
	case Node::DECLARATION:      emitDeclaration(cod, who);        break; 
	case Node::ASSIGNMENT:       emitAssignment(cod, who);         break; 
	case Node::ACCESS:           emitAccess(cod, who);             break; 
	case Node::INDEX:            emitIndex(cod, who);              break; 
	case Node::SCOPE:            emitScope(cod, who);              break; 
	case Node::CAST:             emitCast(cod, who);               break; 
	case Node::PAREN_SURROUND:   emitParenSurround(cod, who);      break; 
	case Node::POINTER:          emitPointer(cod, who);            break; 
	case Node::UNARY_OPERATOR:   emitUnaryOperator(cod, who);      break;
	case Node::BINARY_OPERATOR:  emitBinaryOperator(cod, who);     break;
	case Node::TERNARY_OPERATOR: emitTernaryOperator(cod, who);    break;
	case Node::RETURN:           emitReturn(cod, who);             break;
	case Node::LITERAL:          emitLiteral(cod,who);             break;
	case Node::PRAGMA:           emitPragma(cod, who);             break;
	case Node::INCLUDE_HEADER:   emitIncludeHeader(cod,who);             break;
	case Node::INCLUDE_SOURCE:   emitIncludeSource(cod,who);             break;
	case Node::IDENTIFIER:       emitIdentifier(cod,who);		   break;
	default: assert(false);
	};
}

Node* make(NodePool* pool, Node::Type type) {
	Node* n = pool->take(1);
	n->type = type;
	//if (kid_count[type] > 0) {
	//	Node* k = pool->take(kid_count[type]);
	//	n->kid = k;
	//}
	return n;
}
Node* makeKid(NodePool* pool, Node* par, Node::Type type) {
	Node* kid = make(pool, type);
	addKid(par, kid);
	return kid;
}
void setType(Node* who, Node* type) {
	assert(who->type == Node::DECLARATION);
}
void setName(Node* who, Node* name) {

}
Node* getType(Node* who) {
	assert(who->type == Node::DECLARATION);
	return who->kid + 0;
}
Node* getName(Node* who) {
	assert(who->type == Node::DECLARATION);
	return who->kid + 1;
}
void setIdentifier(Node* who, const char* val) {
	assert(who->type == Node::EMPTY);
	who->type = Node::IDENTIFIER;
	who->identifier = val;
}
void addToArguments(Node* who) {
	assert(false);
}
void addToBody(Node* who) {
	assert(false);
	Node* bod = who->kid;

}
void addKid(Node* par, Node* kid) {
	if (par->kid) { // kid's sibling
		Node* who = par->kid;
		while (who->sib) who = who->sib;
		who->sib = kid;
	} else { // first kid
		par->kid = kid;
	}
}

////////////
static NodePool* P;
void setNodePool(NodePool* pool) {
	P = pool;
}
Node* list() {
	return make(P, Node::LIST);
}
Node* list(Node* kid0) {
	Node* who = list();
	addKid(who, kid0);
	return who;
}
Node* list(Node* kid0, Node* kid1) {
	Node* who = list();
	addKid(who, kid0);
	addKid(who, kid1);
	return who;
}
Node* list(Node* kid0, Node* kid1, Node* kid2) {
	Node* who = list();
	addKid(who, kid0);
	addKid(who, kid1);
	addKid(who, kid2);
	return who;
}
Node* block() {
	return make(P, Node::BLOCK);
}
Node* block(Node* kid0) {
	Node* who = block();
	addKid(who, kid0);
	return who;
}
Node* block(Node* kid0, Node* kid1) {
	Node* who = block();
	addKid(who, kid0);
	addKid(who, kid1);
	return who;
}
void add(Node* list_or_block, Node* who) {
	addKid(list_or_block, who);
}
Node* iden(const char* str, int qualifier_flags) {
	Node* who = make(P, Node::IDENTIFIER);
	who->identifier = str;
	set_qualifiers(who, qualifier_flags);
	return who;
}
Node* decl(Node* type, Node* name) {
	Node* who = make(P, Node::DECLARATION);
	addKid(who, type);
	addKid(who, name);
	return who;
}
Node* decl(const char* type_iden, const char* name_iden) {
	return decl(iden(type_iden), iden(name_iden));
}
Node* assign(Node* lhs, Node* rhs) {
	Node* who = make(P, Node::ASSIGNMENT);
	addKid(who, lhs);
	addKid(who, rhs);
	return who;
}
Node* assign(const char* lhs_iden, const char* rhs_iden) {
	return assign(iden(lhs_iden), iden(rhs_iden));
}
Node* assign(Node* lhs, Node::Operator op, Node* rhs) {
	Node* who = make(P, Node::ASSIGNMENT);
	addKid(who, lhs);
	who->op = op;
	addKid(who, rhs);
	return who;
}
Node* access(Node* name, Node* member) {
	Node* who = make(P, Node::ACCESS);
	addKid(who, name);
	addKid(who, member);
	return who;
}
Node* access(const char* name_iden, const char* member_iden) {
	return access(iden(name_iden), iden(member_iden));
}
Node* function_def(Node* return_type, Node* name, Node* argument_list, Node* body_block, int qualifier_flags) {
	Node* who = make(P, Node::FUNCTION_DEF);
	set_qualifiers(who, qualifier_flags);
	addKid(who, return_type);
	addKid(who, name);
	addKid(who, argument_list);
	addKid(who, body_block);
	return who;	
}
Node* ret(Node* what) {
	Node* who = make(P, Node::RETURN);
	addKid(who, what);
	return who;
}
Node* op(Node::Operator op, Node* rhs) {
	Node* who = make(P, Node::UNARY_OPERATOR);
	who->op = op;
	addKid(who, rhs);
	return who;
}
Node* op(Node* lhs, Node::Operator op, Node* rhs) {
	Node* who = make(P, Node::BINARY_OPERATOR);
	addKid(who, lhs);
	who->op = op;
	addKid(who, rhs);
	return who;
}
Node* construct(Node* type, Node* argument_list) {
	Node* who = make(P, Node::CONSTRUCTOR);
	addKid(who, type);
	addKid(who, argument_list);
	return who;
}
Node* construct(const char* type_iden, Node* argument_list) {
	return construct(iden(type_iden), argument_list);
}
Node* function_call(Node* type, Node* argument_list) {
	Node* who = make(P, Node::FUNCTION_CALL);
	addKid(who, type);
	addKid(who, argument_list);
	return who;
}
Node* function_call(const char* type_iden, Node* argument_list) {
	return function_call(iden(type_iden), argument_list);
}
Node* struct_def(Node* name, Node* body_block) {
	Node* who = make(P, Node::STRUCT_DEF);
	addKid(who, name);
	addKid(who, body_block);
	return who;
}
Node* struct_def(const char* name_iden, Node* body_block) {
	return struct_def(iden(name_iden), body_block);
}
Node* constructor_def(Node* name, Node* argument_list, Node* init_list, Node* body_block, int qualifier_flags) {
	Node* who = make(P, Node::CONSTRUCTOR_DEF);
	set_qualifiers(who, qualifier_flags);
	addKid(who, name);
	addKid(who, argument_list);
	addKid(who, init_list);
	addKid(who, body_block);
	return who;
}
Node* constructor_def(const char* name_iden, Node* argument_list,  Node* init_list,  Node* body_block, int qualifier_flags) {
	return constructor_def(iden(name_iden), argument_list, init_list, body_block);
}
Node* scope(Node* par, Node* kid) {
	Node* who = make(P, Node::SCOPE);
	addKid(who, par);
	addKid(who, kid);
	return who;
}
Node* scope(const char* par_iden, const char* kid_iden) {
	return scope(iden(par_iden), iden(kid_iden));
}
Node* cast(Node* to, Node* from) {
	Node* who = make(P, Node::CAST);
	addKid(who, to);
	addKid(who, from);
	return who;
}
Node* pragma_directive(const char* str) {
	Node* who = make(P, Node::PRAGMA);
	who->string_literal = str;
	return who;
}
Node* include_header(const char* str) {
	Node* who = make(P, Node::INCLUDE_HEADER);
	who->string_literal = str;
	return who;
}
Node* include_source(const char* str) {
	Node* who = make(P, Node::INCLUDE_SOURCE);
	who->string_literal = str;
	return who;
}
Node* pointer(Node* type) {
	Node* who = make(P, Node::POINTER);
	addKid(who, type);
	return who;
}
Node* pointer(const char* type_iden) {
	return pointer(iden(type_iden));
}
Node* paren_surround(Node* what) {
	Node* who = make(P, Node::PAREN_SURROUND);
	addKid(who, what);
	return who;
}
Node* index(Node* name, Node* idx) {
	Node* who = make(P, Node::INDEX);
	addKid(who, name);
	addKid(who, idx);
	return who;
}
void set_qualifiers(Node* who, int qualifier_flags) {
	if (qualifier_flags & Node::CONST_FLAG)      who->is_const = 1;
	if (qualifier_flags & Node::STATIC_FLAG)     who->is_static = 1;
	if (qualifier_flags & Node::EXPLICIT_FLAG)   who->is_explicit = 1;
	if (qualifier_flags & Node::INLINE_FLAG)     who->is_inline = 1;
	if (qualifier_flags & Node::MUTABLE_FLAG)    who->is_mutable = 1;
	if (qualifier_flags & Node::REFERENCE_FLAG)  who->is_reference = 1;
}