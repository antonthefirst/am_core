#include "demo.h"
#include "render.h"
#include "core/log.h"


#define STD "std::"

//#define LIB_ENABLE

#ifdef LIB_ENABLE
#include "mathslib.h"
#endif

// generate string conversion functions; requires TempStr
#define GENERATE_PRINTING_FUNCTIONS 0

// notations
#define FUNC    0  // always prefix
#define SYMBOL  1  // infix for binaries, prefix for unaries

// result
#define ELEMENTWISE 0
#define REDUCTION   1
#define LOGICAL     2 // also always elementwise

// input
#define UNARY    0
#define BINARY   1
#define TERNARY  2
#define COMPOUND 3 // forms like +=, where lhs is modified by rhs. always binary

struct SimpleMathOp {
	const char* name;
	const char* intrinsic;
	int intrinsic_notation;
	int result;
	int input;
};

struct MathType {
	const char* name;
	const char* suffix;
	int element_count;
	const char* GetElement(int i) {
		if (element_count == 0) {
			return "";
		} else {
			const char* names[] = {".x", ".y", ".z", ".w"};
			assert(i < ARRSIZE(names));
			return names[i];
		}
	}
	const char* GetElementName(int i) {
		if (element_count == 0) {
			return "";
		}
		else {
			const char* names[] = { "x", "y", "z", "w" };
			assert(i < ARRSIZE(names));
			return names[i];
		}
	}
};

struct Code {
	char* text;
	char* func_decoration = "CUDA_INLINE ";
	int func_count = 0;
	void startline() {
		text += sprintf(text, "\t");
	}
	void endstatement() {
		text += sprintf(text, "; ");
	}
	void endline(const char * format, ...) {
		va_list args;
		va_start(args, format);
		text += vsprintf(text, format, args);
		endline();
		va_end(args);
	}
	void newline() {
		text += sprintf(text, "\n");
	}
	void endline() {
		text += sprintf(text, ";");
		newline();
	}
	void constructor_open(const char* type_name) {
		text += sprintf(text, "%s{", type_name);
	}
	void constructor_close() {
		text += sprintf(text, "}");
	}
	void make_open(const char* type_name) {
		text += sprintf(text, "make_%s(", type_name);
	}
	void make_close() {
		text += sprintf(text, ")");
	}
	void line(const char * format, ...) {
		va_list args;
		va_start(args, format);
		text += sprintf(text, "\t");
		text += vsprintf(text, format, args);
		endline();
		va_end(args);
	}
	void midline(const char * format, ...) {
		va_list args;
		va_start(args, format);
		text += vsprintf(text, format, args);
		va_end(args);
	}
	void returnstart() {
		text += sprintf(text, "\treturn ");
	}
	void returnline(const char* format, ...) {
		va_list args;
		va_start(args, format);
		text += sprintf(text, "\treturn ");
		text += vsprintf(text, format, args);
		endline();
		va_end(args);
	}
	void decl(const char* ret_type, const char* name, const char* a_type, const char* b_type = NULL, const char* c_type = NULL) {
		text += sprintf(text, func_decoration);
		text += sprintf(text, "%s %s(%s a", ret_type, name, a_type);
		if (b_type) text += sprintf(text, ", %s b", b_type);
		if (c_type) text += sprintf(text, ", %s c", c_type);
		text += sprintf(text, ") {\n");
		func_count += 1;
	}
	void declstruct(const char* type_name) {
		text += sprintf(text, "struct %s {\n", type_name);
	}
	void declforward(const char* type_name) {
		text += sprintf(text, "struct %s;\n", type_name);
	}
	void declfreeform(const char* ret_type, const char* name, const char* a_type, const char* a_name, const char* a_default, const char* b_type = NULL,  const char* b_name = NULL, const char* b_default = NULL) {
		text += sprintf(text, func_decoration);
		text += sprintf(text, "%s %s(%s %s%s", ret_type, name, a_type, a_name, a_default);
		if (b_type) text += sprintf(text, ", %s %s%s", b_type, b_name, b_default);
		text += sprintf(text, ") {\n");
		func_count += 1;
	}

	void declcompound(const char* ret_type, const char* name, const char* lhs_type, const char* rhs_type) {
		text += sprintf(text, func_decoration);
		text += sprintf(text, "%s %s(%s& lhs, %s rhs) {\n", ret_type, name, lhs_type, rhs_type);
		func_count += 1;
	}
	void end() {
		text += sprintf(text, "}\n");
	}
	void endstruct() {
		text += sprintf(text, "};\n");
	}
	void oneline(const char* format, ...) {
		va_list args;
		va_start(args, format);
		text += sprintf(text, "\treturn ");
		text += vsprintf(text, format, args);
		endline();
		va_end(args);
	}
};

void genSimpleUnaryReduction(Code& code, SimpleMathOp op, MathType ret, MathType a) {
	code.decl(ret.name, op.name, a.name);
	code.returnstart();
	if (op.intrinsic_notation == SYMBOL) {	
		for (int i = 0; i < a.element_count; ++i) {
			if (i == 0)
				code.midline(" a%s", a.GetElement(i));
			else
				code.midline(" %s a%s", op.intrinsic, a.GetElement(i));
		}
	}
	if (op.intrinsic_notation == FUNC) {
		for (int i = 0; i < a.element_count; ++i) {
			if (i != a.element_count-1)
				code.midline(" %s(a%s,", op.intrinsic, a.GetElement(i));
			else
				code.midline(" a%s", a.GetElement(i));
		}
		for (int i = 0; i < a.element_count-1; ++i)
			code.midline(")");
	}
	code.endline();
	code.end();
}

void genSimpleUnaryElementwise(Code& code, SimpleMathOp op, MathType ret, MathType a) {
	code.decl(ret.name, op.name, a.name);
	code.returnstart();
	code.constructor_open(ret.name);
	if (op.intrinsic_notation == SYMBOL) {
		for (int i = 0; i < ret.element_count; ++i) {
			code.midline(" %sa%s", op.intrinsic, a.GetElement(i));
			if (i != ret.element_count - 1)
				code.midline(",");
		}
	}
	if (op.intrinsic_notation == FUNC) {
		for (int i = 0; i < ret.element_count; ++i) {
			code.midline(" %s(a%s)", op.intrinsic, a.GetElement(i));
			if (i != ret.element_count - 1)
				code.midline(",");
		}
	}
	code.constructor_close();
	code.endline();
	code.end();
}
void genSimpleBinaryElementwise(Code& code, SimpleMathOp op, MathType ret, MathType a, MathType b) {
	code.decl(ret.name, op.name, a.name, b.name);
	code.returnstart();
	code.constructor_open(ret.name);
	if (op.intrinsic_notation == SYMBOL) {
		for (int i = 0; i < ret.element_count; ++i) {
			code.midline(" a%s %s b%s", a.GetElement(i), op.intrinsic, b.GetElement(i));
			if (i != ret.element_count - 1)
				code.midline(",");
		}
	}
	if (op.intrinsic_notation == FUNC) {
		for (int i = 0; i < ret.element_count; ++i) {
			code.midline(" %s(a%s, b%s)", op.intrinsic, a.GetElement(i), b.GetElement(i));
			if (i != ret.element_count - 1)
				code.midline(",");
		}
	}
	code.constructor_close();
	code.endline();
	code.end();
}
void genSimpleTernaryElementwise(Code& code, SimpleMathOp op, MathType ret, MathType a, MathType b, MathType c) {
	code.decl(ret.name, op.name, a.name, b.name, c.name);
	code.returnstart();
	code.constructor_open(ret.name);
	assert(op.intrinsic_notation == FUNC); // there are no "ternary" symbol operators supported on math types
	for (int i = 0; i < ret.element_count; ++i) {
		code.midline(" %s(a%s, b%s, c%s)", op.intrinsic, a.GetElement(i), b.GetElement(i), c.GetElement(i));
		if (i != ret.element_count - 1)
			code.midline(",");
	}
	code.constructor_close();
	code.endline();
	code.end();
}
void genSimpleCompoundElementwise(Code& code, SimpleMathOp op, MathType lhs, MathType rhs) {
	code.declcompound("void", op.name, lhs.name, rhs.name);
	code.startline();
	assert(op.intrinsic_notation == SYMBOL); // there are no non-symbol compound operators
	for (int i = 0; i < lhs.element_count; ++i) {
		code.midline("lhs%s %s rhs%s", lhs.GetElement(i), op.intrinsic, rhs.GetElement(i));
		code.endstatement();
	}
	code.newline();
	code.end();
}

void genBasicOpMin(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "min", arg_type.name, arg_type.name);
	code.oneline("a < b ? a : b");
	code.end();
}
void genBasicOpMax(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "max", arg_type.name, arg_type.name);
	code.oneline("a > b ? a : b");
	code.end();
}
void genBasicOpClamp(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "clamp", arg_type.name, arg_type.name, arg_type.name);
#ifdef STD
	code.oneline("std::min(std::max(a, b), c)");
#else
	code.oneline("min(max(a, b), c)");
#endif
	code.end();
}
void genBasicOpSq(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "sq", arg_type.name);
	code.oneline("a * a");
	code.end();
}

void genBasicRealOpSign(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "sign", arg_type.name);
	code.oneline("(a > 0.0%s) - (a < 0.0%s)", arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpFract(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "fract", arg_type.name);
	code.oneline("a - floor(a)");
	code.end();
}
void genBasicRealOpMod(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "mod", arg_type.name, arg_type.name);
	code.oneline("a - b * floor(a/b)");
	code.end();
}
void genBasicRealOpStep(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "step", arg_type.name, arg_type.name);
	code.oneline("a < b ? 0.0%s : 1.0%s", arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpStepZero(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "step", arg_type.name);
	code.oneline("a < 0.0%s ? 0.0%s : 1.0%s", arg_type.suffix, arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpRamp(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "ramp", arg_type.name, arg_type.name, arg_type.name);
	code.oneline("clamp((c - a) / (b - a), 0.0%s, 1.0%s)", arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpSmoothstep(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "smoothstep", arg_type.name, arg_type.name, arg_type.name);
	code.line("c = ramp(a, b, c)");
	code.oneline("c * c * (3.0%s - 2.0%s * c)", arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpSmoothstepNoRamp(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "smoothstep", arg_type.name);
	code.oneline("a * a * (3.0%s - 2.0%s * a)", arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpSmootherstep(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "smootherstep", arg_type.name, arg_type.name, arg_type.name);
	code.line("c = ramp(a, b, c)");
	code.oneline("c * c * c * (c * (c * 6.0%s - 15.0%s) + 10.0%s)", arg_type.suffix, arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpSmootherstepNoRamp(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "smootherstep", arg_type.name);
	code.oneline("a * a * a * (a * (a * 6.0%s - 15.0%s) + 10.0%s)", arg_type.suffix, arg_type.suffix, arg_type.suffix);
	code.end();
}
void genBasicRealOpLerp(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "lerp", arg_type.name, arg_type.name, arg_type.name);
	code.oneline("a + (b - a) * c");
	code.end();
}
void genBasicRealOpSaturate(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "saturate", arg_type.name);
	code.oneline("clamp(a, 0.0%s, 1.0%s)", arg_type.suffix, arg_type.suffix);
	code.end();
}
static void gen_basic_funcs_for_integers(Code& code, MathType integer_type) {
	genBasicOpMin(code, integer_type);
	genBasicOpMax(code, integer_type);
	genBasicOpClamp(code, integer_type);
	genBasicOpSq(code, integer_type);
}
static void gen_basic_funcs_for_reals(Code& code, MathType real_type) {
	//genBasicOpMin(code, real_type);
	//genBasicOpMax(code, real_type);
	genBasicOpClamp(code, real_type);
	genBasicOpSq(code, real_type);
	genBasicRealOpSign(code, real_type);
	genBasicRealOpFract(code, real_type);
	genBasicRealOpMod(code, real_type);
	genBasicRealOpStep(code, real_type);
	genBasicRealOpStepZero(code, real_type);
	genBasicRealOpRamp(code, real_type);
	genBasicRealOpSmoothstep(code, real_type);
	genBasicRealOpSmoothstepNoRamp(code, real_type);
	genBasicRealOpSmootherstep(code, real_type);
	genBasicRealOpSmootherstepNoRamp(code, real_type);
	genBasicRealOpLerp(code, real_type);
	genBasicRealOpSaturate(code, real_type);
}
void genRealVectorOpLengthSq(Code& code, MathType ret_type, MathType arg_type) {
	code.decl(ret_type.name, "lengthsq", arg_type.name);
	code.oneline("sum(sq(a))");
	code.end();
}
void genRealVectorOpLength(Code& code, MathType ret_type, MathType arg_type) {
	code.decl(ret_type.name, "length", arg_type.name);
	code.oneline("sqrt(lengthsq(a))");
	code.end();
}
void genRealVectorNormalize(Code& code, MathType arg_type) {
	code.decl(arg_type.name, "normalize", arg_type.name);
	code.oneline("a / length(a)");
	code.end();
}
void genRealVectorNormalizeOr0(Code& code, MathType base_type, MathType arg_type) {
	code.decl(arg_type.name, "normalize_or_0", arg_type.name);
	code.line("%s len = length(a)", base_type.name);
	code.returnline("len > 0.0%s ? a / len : %s(0.0%s)", arg_type.suffix, arg_type.name, arg_type.suffix);
	code.end();
}
void genRealVectorOpDot(Code& code, MathType ret, MathType arg) {
	code.decl(ret.name, "dot", arg.name, arg.name);
	code.returnstart();
	for (int i = 0; i < arg.element_count; ++i) {
		code.midline("a%s * b%s ", arg.GetElement(i), arg.GetElement(i));
		if (i != arg.element_count - 1)
			code.midline("+");
	}
	code.endline();
	code.end();
}
void genRealVectorOpStr(Code& code, MathType arg, bool add_newline) {
	code.declfreeform("TempStr", add_newline ? "strnl" : "str", arg.name, "a", "", "const char*", "format", " = \"%3.2f\"");
	code.startline();
	code.midline("TempStr format_str = TempStr(\"(");
	for (int i = 0; i < arg.element_count; ++i) {
		code.midline("%%s");
		if (i != arg.element_count - 1)
			code.midline(", ");
	}
	code.midline(")");
	if (add_newline)
		code.midline("\\n");
	code.midline("\", ");
	for (int i = 0; i < arg.element_count; ++i) {
		code.midline("format");
		if (i != arg.element_count - 1)
			code.midline(", ");
	}
	code.midline(")");
	code.endline();

	code.returnstart();
	code.midline("TempStr(format_str, ");
	for (int i = 0; i < arg.element_count; ++i) {
		code.midline("a%s", arg.GetElement(i));
		if (i != arg.element_count - 1)
			code.midline(", ");
	}
	code.midline(")");
	code.endline();
	code.end();
}
void genRealVectorOpDet2(Code& code, MathType ret, MathType arg) {
	code.decl(ret.name, "det", arg.name, arg.name);
	code.oneline("a%s * b%s - a%s * b%s", arg.GetElement(0), arg.GetElement(1), arg.GetElement(1), arg.GetElement(0));
	code.end();
}
void genRealVectorOpCross3(Code& code, MathType arg) {
	code.decl(arg.name, "cross", arg.name, arg.name);
	code.returnstart();
	code.constructor_open(arg.name);
	for (int i = 0; i < arg.element_count; ++i) {
		code.midline(" a%s * b%s - a%s * b%s", arg.GetElement((i+1)%3), arg.GetElement((i+2)%3), arg.GetElement((i + 2) % 3), arg.GetElement((i+1)%3));
		if (i != arg.element_count - 1)
			code.midline(",");
	}
	code.constructor_close();
	code.endline();
	code.end();
}
void genRealVectorOpDet3(Code& code, MathType ret, MathType arg) {
	code.decl(ret.name, "det", arg.name, arg.name);
	code.oneline("length(cross(a,b))", arg.GetElement(0), arg.GetElement(1), arg.GetElement(1), arg.GetElement(0));
	code.end();
}
static void gen_vector_ops_for_reals(Code& code, MathType* arg_types) {
	for (int t = 2; t <= 4; ++t) {
		genRealVectorOpLengthSq(code, arg_types[0], arg_types[t]);
		genRealVectorOpLength(code, arg_types[0], arg_types[t]);
		genRealVectorOpDot(code, arg_types[0], arg_types[t]);
		genRealVectorNormalize(code, arg_types[t]);
		genRealVectorNormalizeOr0(code, arg_types[0], arg_types[t]);
#if GENERATE_PRINTING_FUNCTIONS
		genRealVectorOpStr(code, arg_types[t], false);
		genRealVectorOpStr(code, arg_types[t], true);
#endif
		if (t == 2) { // special cases for two-vectors
			genRealVectorOpDet2(code, arg_types[0], arg_types[t]);
		}
		if (t == 3) {
			genRealVectorOpCross3(code, arg_types[t]);
			genRealVectorOpDet3(code, arg_types[0], arg_types[t]);
		}
	}
}
static void gen_simple_funcs_for_type(Code& code, MathType* bool_types, MathType* arg_types, SimpleMathOp* ops, int op_count) {
	for (int t = 2; t <= 4; ++t) {
		MathType arg_type = arg_types[t];
		for (int i = 0; i < op_count; ++i) {
			SimpleMathOp op = ops[i];
			MathType ret = op.result == REDUCTION ? arg_types[0] : arg_type;
			if (op.input == UNARY && op.result == REDUCTION)
				genSimpleUnaryReduction(code, op, ret, arg_type);
			if (op.input == UNARY && op.result == ELEMENTWISE)
				genSimpleUnaryElementwise(code, op, ret, arg_type);
			if (op.input == BINARY && op.result == ELEMENTWISE)
				genSimpleBinaryElementwise(code, op, ret, arg_type, arg_type);
			if (op.input == TERNARY && op.result == ELEMENTWISE)
				genSimpleTernaryElementwise(code, op, ret, arg_type, arg_type, arg_type);
			if (op.input == COMPOUND && op.result == ELEMENTWISE)
				genSimpleCompoundElementwise(code, op, arg_type, arg_type);
			if (op.input == UNARY && op.result == LOGICAL)
				genSimpleUnaryElementwise(code, op, bool_types[t], arg_type);
			if (op.input == BINARY && op.result == LOGICAL)
				genSimpleBinaryElementwise(code, op, bool_types[t], arg_type, arg_type);
		}
	}
}
static void gen_forward_decl_for_type(Code& code, MathType* types) {
	for (int t = 2; t <= 4; ++t) {
		code.declforward(types[t].name);
	}
}
static void gen_structs_for_type(Code& code, MathType* types, MathType** relatives = 0, int relative_count = 0) {
	for (int t = 2; t <= 4; ++t) {
		code.declstruct(types[t].name);

		// vars
		code.startline();
		code.midline("%s ", types[0].name);
		for (int i = 0; i < types[t].element_count; ++i) {
			code.midline("%s", types[t].GetElementName(i));
			if (i != types[t].element_count - 1)
				code.midline(", ");
		}
		code.endline();

#if 0
		// empty constructor
		code.startline();
		code.midline("%s() { }", types[t].name);
		code.newline();

		// elements constructor
		code.startline();
		code.midline("%s(", types[t].name);
		for (int i = 0; i < types[t].element_count; ++i) {
			code.midline("%s _%s", types[0].name, types[t].GetElementName(i));
			if (i != types[t].element_count - 1)
				code.midline(", ");
		}
		code.midline(") : ");
		for (int i = 0; i < types[t].element_count; ++i) {
			code.midline("%s(_%s)", types[t].GetElementName(i), types[t].GetElementName(i));
			if (i != types[t].element_count - 1)
				code.midline(", ");
		}
		code.midline(" { }");
		code.newline();

		// scalar constructor
		code.startline();
		code.midline("%s(%s _s) : ", types[t].name, types[0].name);
		for (int i = 0; i < types[t].element_count; ++i) {
			code.midline("%s(_s)", types[t].GetElementName(i));
			if (i != types[t].element_count - 1)
				code.midline(", ");
		}
		code.midline(" { }");
		code.newline();

		// relatives constructor definitions
		for (int r = 0; r < relative_count; ++r) {
			MathType* rel_types = relatives[r];
			if (rel_types != types) {
				code.line("explicit %s(const %s& v)", types[t].name, rel_types[t].name);
			}
		}
#endif

		code.endstruct();
	}
}
static void gen_relative_constructor_definitions_for_type(Code& code, MathType* types, MathType** relatives = 0, int relative_count = 0) {
	for (int t = 2; t <= 4; ++t) {
		for (int r = 0; r < relative_count; ++r) {
			MathType* rel_types = relatives[r];
			if (rel_types != types) {
				code.midline("%s::%s(const %s& v) : ", types[t].name, types[t].name, rel_types[t].name);
				for (int i = 0; i < types[t].element_count; ++i) {
					code.midline("%s(%s(v%s))", types[t].GetElementName(i), types[0].name, rel_types[t].GetElement(i));
					if (i != types[t].element_count - 1)
						code.midline(", ");
				}
				code.midline(" { }");
				code.newline();
			}
		}
	}
}
static void gen_make_for_type(Code& code, MathType* types, MathType** relatives = 0, int relative_count = 0) {
	for (int t = 2; t <= 4; ++t) {
		TempStr func_name("make_%s", types[t].name);

		code.decl(types[t].name, func_name, types[0].name);
		code.returnstart();
		code.midline("%s {", types[t].name);
		for (int i = 0; i < types[t].element_count; ++i) {
			code.midline("a");
			if (i != types[t].element_count - 1)
				code.midline(", ");
		}
		code.midline("}");
		code.endline();
		code.end();


		for (int r = 0; r < relative_count; ++r) {
			MathType* rel_types = relatives[r];
			if (rel_types != types) {
				code.decl(types[t].name, func_name, rel_types[t].name);
				code.returnstart();
				code.midline("%s {", types[t].name);
				for (int i = 0; i < types[t].element_count; ++i) {
					code.midline("%s(a%s)", types[0].name, rel_types[t].GetElement(i));
					if (i != types[t].element_count - 1)
						code.midline(", ");
				}
				code.midline("}");
				code.endline();
				code.end();
			}
		}
	}
}
static char* gen_maths_lib(size_t* len) {

	MathType float_32_types[] = {
	{ "float", "f", 0 },
	{  "float1", "f", 1 },
	{  "float2", "f", 2 },
	{  "float3", "f", 3 },
	{  "float4", "f", 4 },
	};

	MathType float_64_types[] = {
	{ "double", "", 0 },
	{  "double1", "", 1 },
	{  "double2", "", 2 },
	{  "double3", "", 3 },
	{  "double4", "", 4 },
	};

	MathType int_32_types[] = {
	{  "int",  "", 0 },
	{ "int1", "", 1 },
	{ "int2", "", 2 },
	{ "int3", "", 3 },
	{ "int4", "", 4 },
	};

	MathType uint_32_types[] = {
	{  "uint", "", 0 },
	{ "uint1", "", 1 },
	{ "uint2", "", 2 },
	{ "uint3", "", 3 },
	{ "uint4", "", 4 },
	};

	MathType bool_types[] = {
	{  "bool", "", 0 },
	{ "bool1", "", 1 },
	{ "bool2", "", 2 },
	{ "bool3", "", 3 },
	{ "bool4", "", 4 },
	};

	SimpleMathOp real_ops[] = {
	{ "max",     STD"max",       FUNC,    REDUCTION,   UNARY },
	{ "min",     STD"min",       FUNC,    REDUCTION,   UNARY },
	{ "sum",        "+",         SYMBOL,  REDUCTION,   UNARY },
	{ "product",    "*",         SYMBOL,  REDUCTION,   UNARY },
	{ "operator-",  "-",         SYMBOL,  ELEMENTWISE, UNARY },
	{ "sq",         "sq",        FUNC,    ELEMENTWISE, UNARY },
	{ "saturate",   "saturate",  FUNC,    ELEMENTWISE, UNARY },
	{ "abs",        STD"abs",       FUNC,    ELEMENTWISE, UNARY },
	{ "floor",      STD"floor",     FUNC,    ELEMENTWISE, UNARY },
	{ "ceil",       STD"ceil",      FUNC,    ELEMENTWISE, UNARY },
	{ "fract",      "fract",     FUNC,    ELEMENTWISE, UNARY },
	{ "sign",       "sign",      FUNC,    ELEMENTWISE, UNARY },
	{ "step",       "step",      FUNC,    ELEMENTWISE, UNARY },
	{ "smoothstep", "smoothstep",FUNC,    ELEMENTWISE, UNARY },
	{ "smootherstep", "smootherstep",FUNC,ELEMENTWISE, UNARY },
	{ "exp",        STD"exp",       FUNC,    ELEMENTWISE, UNARY },
	{ "log",        STD"log",       FUNC,    ELEMENTWISE, UNARY },
	{ "sqrt",       STD"sqrt",      FUNC,    ELEMENTWISE, UNARY },
	{ "cos",        STD"cos",       FUNC,    ELEMENTWISE, UNARY },
	{ "sin",        STD"sin",       FUNC,    ELEMENTWISE, UNARY },
	{ "tan",        STD"tan",       FUNC,    ELEMENTWISE, UNARY },
	{ "acos",       STD"acos",      FUNC,    ELEMENTWISE, UNARY },
	{ "asin",       STD"asin",      FUNC,    ELEMENTWISE, UNARY },
	{ "atan",       STD"atan",      FUNC,    ELEMENTWISE, UNARY },
	{ "operator+",  "+",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator-",  "-",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator*",  "*",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator/",  "/",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "mod",        "mod",       FUNC,    ELEMENTWISE, BINARY },
	{ "max",        STD"max",       FUNC,    ELEMENTWISE, BINARY },
	{ "min",        STD"min",       FUNC,    ELEMENTWISE, BINARY },
	{ "pow",        STD"pow",       FUNC,    ELEMENTWISE, BINARY },
	{ "step",       "step",      FUNC,    ELEMENTWISE, BINARY },
	{ "lerp",       "lerp",      FUNC,    ELEMENTWISE, TERNARY },
	{ "ramp",       "ramp",      FUNC,    ELEMENTWISE, TERNARY },
	{ "smoothstep", "smoothstep",FUNC,    ELEMENTWISE, TERNARY },
	{ "smootherstep", "smootherstep",FUNC,    ELEMENTWISE, TERNARY },
	{ "clamp",      "clamp",     FUNC,    ELEMENTWISE, TERNARY },
	{ "operator+=",  "+=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator-=",  "-=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator*=",  "*=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator/=",  "/=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "isnan",       STD"isnan",    FUNC,    LOGICAL,     UNARY  },
	{ "operator==",  "==",       SYMBOL,  LOGICAL,     BINARY },
	{ "operator!=",  "!=",       SYMBOL,  LOGICAL,     BINARY },
	{ "operator>",   ">",        SYMBOL,  LOGICAL,     BINARY },
	{ "operator<",   "<",        SYMBOL,  LOGICAL,     BINARY },
	{ "operator>=",  ">=",       SYMBOL,  LOGICAL,     BINARY },
	{ "operator<=",  "<=",       SYMBOL,  LOGICAL,     BINARY },
	};

	SimpleMathOp basic_integer_ops[] = {
	{ "max",     STD"max",       FUNC,    REDUCTION,   UNARY },
	{ "min",     STD"min",       FUNC,    REDUCTION,   UNARY },
	{ "sum",        "+",         SYMBOL,  REDUCTION,   UNARY },
	{ "product",    "*",         SYMBOL,  REDUCTION,   UNARY },
	{ "operator-",  "-",         SYMBOL,  ELEMENTWISE, UNARY },
	{ "operator~",  "~",         SYMBOL,  ELEMENTWISE, UNARY },
	{ "sq",         "sq",        FUNC,    ELEMENTWISE, UNARY },
	{ "operator+",  "+",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator-",  "-",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator*",  "*",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator/",  "/",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator|",  "|",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator&",  "&",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator^",  "^",         SYMBOL,  ELEMENTWISE, BINARY },
	{ "max",        STD"max",       FUNC,    ELEMENTWISE, BINARY },
	{ "min",        STD"min",       FUNC,    ELEMENTWISE, BINARY },
	{ "clamp",      "clamp",     FUNC,    ELEMENTWISE, TERNARY },
	{ "operator+=",  "+=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator-=",  "-=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator*=",  "*=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator/=",  "/=",       SYMBOL,  ELEMENTWISE, COMPOUND },
	{ "operator==",  "==",       SYMBOL,  LOGICAL,     BINARY },
	{ "operator!=",  "!=",       SYMBOL,  LOGICAL,     BINARY },
	{ "operator>",   ">",        SYMBOL,  LOGICAL,     BINARY },
	{ "operator<",   "<",        SYMBOL,  LOGICAL,     BINARY },
	{ "operator>=",  ">=",       SYMBOL,  LOGICAL,     BINARY },
	{ "operator<=",  "<=",       SYMBOL,  LOGICAL,     BINARY },
	};

	SimpleMathOp signed_integer_ops[] = {
	{ "abs",        STD"abs",       FUNC,    ELEMENTWISE, UNARY },
	};

	SimpleMathOp bool_ops[] = {
	{ "operator==",  "==",       SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator!=",  "!=",       SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator||",  "||",       SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator&&",  "&&",       SYMBOL,  ELEMENTWISE, BINARY },
	{ "operator!",   "!",        SYMBOL,  ELEMENTWISE, UNARY },
	{ "all",         "&&",       SYMBOL,  REDUCTION,   UNARY },
	{ "any",         "||",       SYMBOL,  REDUCTION,   UNARY },
	};

	MathType* constructor_relatives[] = { float_32_types, int_32_types, uint_32_types };

	Code code;
	code.text = (char*)malloc(1024 * 1024);
	char* code_start = code.text;

#if 0
	gen_forward_decl_for_type(code, float_32_types);
	gen_forward_decl_for_type(code, int_32_types);
	gen_forward_decl_for_type(code, bool_types);
	
	code.newline();

	gen_structs_for_type(code, float_32_types, constructor_relatives, ARRSIZE(constructor_relatives));
	gen_structs_for_type(code, int_32_types, constructor_relatives, ARRSIZE(constructor_relatives));
	
	code.newline();

	gen_relative_constructor_definitions_for_type(code, float_32_types, constructor_relatives, ARRSIZE(constructor_relatives));
	gen_relative_constructor_definitions_for_type(code, int_32_types, constructor_relatives, ARRSIZE(constructor_relatives));

	code.newline();
#endif

	gen_structs_for_type(code, bool_types);

	gen_make_for_type(code, float_32_types, constructor_relatives, ARRSIZE(constructor_relatives));
	gen_make_for_type(code, int_32_types, constructor_relatives, ARRSIZE(constructor_relatives));
	gen_make_for_type(code, uint_32_types, constructor_relatives, ARRSIZE(constructor_relatives));
	gen_make_for_type(code, bool_types);

	gen_basic_funcs_for_reals(code, float_32_types[0]);
	gen_simple_funcs_for_type(code, bool_types, float_32_types, real_ops, ARRSIZE(real_ops));
	gen_vector_ops_for_reals(code, float_32_types);
	
	code.newline();

	gen_basic_funcs_for_integers(code, int_32_types[0]);
	gen_simple_funcs_for_type(code, bool_types, int_32_types, basic_integer_ops, ARRSIZE(basic_integer_ops));
	gen_simple_funcs_for_type(code, bool_types, int_32_types, signed_integer_ops, ARRSIZE(signed_integer_ops));

	code.newline();

	gen_basic_funcs_for_integers(code, uint_32_types[0]);
	gen_simple_funcs_for_type(code, bool_types, uint_32_types, basic_integer_ops, ARRSIZE(basic_integer_ops));

	code.newline();

	gen_simple_funcs_for_type(code, bool_types, bool_types, bool_ops, ARRSIZE(bool_ops));

	if (gui::Begin("math lib gen stats")) {
		gui::Text("function count: %d", code.func_count);
	} gui::End();

	*len = code.text - code_start;
	return code_start;
}

struct SimpleMathOpTest {
	char* func_name;
	union Element {
		double r64;
		float r32;
		int i32;
		bool b;
	} elements[4];
};
void gen_maths_lib_tests() {
	
	float test_vec_a[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
	float test_vec_b[4] = { 3.0f, 2.0f, 1.0f, 0.0f };
	SimpleMathOpTest real_ops[] = {
	{ "sum", 10.0f},
	{   "+", 4.0f, 4.0f, 4.0f, 4.0f},
	};
	
}
#ifdef LIB_ENABLE
static void test_lib() {
	
	float4 A = make_float4(5.0f);
	int4 B = make_int4(1) + make_int4(A);

	//log("hah! %d\n", B.x);
	//log(strnl(A));
	
}
#endif
ADD_DEMO(maths_lib_generator) {
	if (args.start) {
		return;
	}
	if (args.stop) {
		return;
	}

#ifdef LIB_ENABLE
	test_lib();
#endif

	size_t code_len = 0;
	char* code = gen_maths_lib(&code_len);

	if (gui::Begin("maths lib code gen")) {
		gui::TextUnformatted(code, code+code_len);
	} gui::End();

	if (gui::Begin("maths lib gen ctrl")) {
		if (gui::Button("write")) {
			FILE* f = fopen("src/mathslib.h", "wb");
			if (f) {
				fwrite(code, code_len, 1, f);
				fclose(f);
			}
		}
	} gui::End();

	free(code);
	
	mainRenderUI(args.app);
	mainSwap();

	renderViewport(0, 0, args.app.res_x, args.app.res_y);
	renderClearColor(0.9f, 0.5f, 0.1f);
}

/*

family is what defines which intrinsics to use for operations on elements. so u8,u16,u32 are same fam, but float and double are diff, even though it's just f32 and f64 conceptually
can call it "intrinsic family" to be more clear. perhaps then interestingly can even generate vectorized families? long shot.

num from num_n:               sum, length, max(unary), min(unary), normalize, normalize_or_1, normalize_or_0, any?, all?
num from num_n num_n:         dot, det
num_n from num_n:             max, min, saturate, exp, cos, sin, compare?
num_n from num_n num_n:       add, sub, mul, pow
num_n from num_n num_n num_n: lerp, clamp

in other words:

ternary elementwise - lerp, clamp
binary elementwise - max, min, add, mul, div, sub, compares
unary elementwise - square, saturate, trigs, pow, exp
binary reductions - dot, det
unary reductions - sum, length, max, min, normalize, normalize_or_

some subset of that works for integers
ternary elementwise - clamp
unary elementwise - square
binary reductions - ?
unary reductions - sum, max, min

separate subset for bools
unary elementwise - and, or, not
unary reductions - all, none, any

odd cases:
2D cross takes two num_2, but returns num_1. 3D cross takes 2 num_3 and returns a num_3?
-> "2D cross" is really a determinant, not a cross. so both should be called det, and return a num

2D rotation creation from a single float

rotations are a totally different beast. need basically a special case generator for quaternions

some cases are odd for single values:
length of num_1 generates sqrt(x*x) which is silly. make it not generate at all. same for max,min. same for sin/cos/exp. so the cases are:
- no num_1 because it's builtin (sin/cos/exp)
- no num_1 because it's inefficient (length, normalize, normalize_or_1)
- no num_1 because it's nonsense (max, min, any, all)
this is true regardless of family (double, int, etc). so function stores the valid arities (2-4, 1-4, etc). and really it's just "1 arity enable", or "min arity"
now the question is, where to store the map of which functions are active for which family? i think the family does this.

compares should generate bools? or ints? common pattern:
if (p >= vec2(0) && p < vec2(W,H) becomes:
if (all(p >= vec2(0)) && all(p < vec2(W,H)) or even
if (all(p >= vec2(0) && p < vec2(W,H)))
order of ops says that >= and < eval before &&, so that's a valid piece of code.
i reckon any/all/compares should return bools. let the language deal with it. besides, glsl does this.
if i ever need to do something clever like "sum across a bool4 to compute how many values are true", likely you're doing some serious memory intensive hacking, and are better off with bits, or by hand rolling.
worst case, just write a sum function that takes bool4 and returns an int, by hand.

most the elementwise basic ops are intrinsic (+-/* cos sin etc) but some have to be written (max, min, lerp, clamp)

tech notes:
- conversions from single elements up to N elements are fine, so inference from float to float2,3,4 is ok. this is because there are no inferences from float2 to float3,4, for example, it's just 1 to N
- conversions from wider precision single elements happen via standard method. so no need for constructors for float2 from double0, for example. the compiler warns if there is a truncation for these, just like a regular float can be inited from a double without err
- conversions from different precision equal element counts are problematic: the compiler considers a construction of int2 from float2 a blanked "initialization conversion from float to int, possible loss of data", whether there would be a truncation or not.
- if i manually cast, the warning goes away, but this makes overloading behaviour extremely fishy. for example functions like add(float2, float2) may exist for int2's and float2's and double2's. a call like add(float2(..), 3.0) is going to produce "random" looking behavior. c++ intuition says "convert up to double" since that's what +3.0 would do to a float. but instead a float2 is constructed from the 3.0 double, and the float2,float2 version of the func is called.
- therefore, i'm making these conversions explicit
- so in summary:
- prefer using the prefix like .f to initialize all mathN's with a single value, so that types are always "correct" passed in. otherwise it will behave just like c
- when initializing one width with another width, with more than one element, use an explicit constructor

*/

#if 0 // some testing code to see how 'explicit' behaves

struct float2;
struct double2;
struct int2;

struct float2 {
	float x, y;
	float2(float s) : x(s), y(s) { }
	float2(float _x, float _y) : x(_x), y(_y) { }
	//float2(double s) : x(s), y(s) {}
	//float2(double _x, double _y) : x(_x), y(_y) { }
	explicit float2(const double2& d);
	explicit float2(const int2& d);
};
float blah(float2 a, float2 b) {
	return max(a.x, b.x);
}
float blah(float a, float b) {
	return max(a, b);
}

struct double2 {
	double x, y;
	double2(double s) : x(s), y(s) {}
	double2(double _x, double _y) : x(_x), y(_y) { }
	//double2(float s) : x(s), y(s) { }
	//double2(float _x, float _y) : x(_x), y(_y) { }
	explicit double2(const float2& f);
};
double blah(double2 a, double2 b) {
	return max(a.x, b.x);
}
double blah(double a, double b) {
	return max(a, b);
}

struct int2 {
	int x, y;
	int2(int s) : x(s), y(s) { }
	int2(int _x, int _y) : x(_x), y(_y) { }
	//int2(float s) : x(s), y(s) {}
	//int2(float _x, float _y) : x(_x), y(_y) { }
	//int2(double s) : x(s), y(s) {}
	//int2(double _x, double _y) : x(_x), y(_y) { }
	//explicit int2(const float2& f);
	explicit int2(const float2& f);
	explicit int2(const double2& d);
};
int blah(int2 a, int2 b) {
	return max(a.x, b.x);
}
int blah(int a, int b) {
	return max(a, b);
}

float2::float2(const double2& d) : x(float(d.x)), y(float(d.y)) { }
float2::float2(const int2& i) : x(float(i.x)), y(float(i.y)) { }
double2::double2(const float2& f) : x(f.x), y(f.y) { }
int2::int2(const float2& f) : x((f.x)), y((f.y)) { }

#endif