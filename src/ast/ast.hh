#ifndef AST_HH
#define AST_HH

// ============================================================================
// ast.hh — the AST vocabulary shared by every stage of TestGen.
//
// Two distinct trees are described here, and it is worth keeping them apart:
//
//   Spec     the INPUT.  A formal description of an API: its global state and
//            one API block per endpoint-behaviour, each with a precondition
//            and a postcondition.  Built by hand in C++ (see src/specs/) —
//            there is no parser and no surface syntax.
//
//   Program  the OUTPUT. A flat list of Assign/Assume/Assert statements. Used
//            for both the Abstract Test Case (ATC) produced by genATC and the
//            Concrete Test Case (CTC) produced by Tester::generateCTC.
//
// The pipeline is:  Spec + test string -> ATC -> Test-API ATC -> CTC -> HTTP.
// See docs/ARCHITECTURE.md.
//
// OWNERSHIP: every node owns its children through unique_ptr, so no node is
// copyable. Whenever a subtree has to be reused (which genATC does constantly,
// since one API block may appear several times in a test string) it must be
// deep-copied with CloneVisitor. Do not try to copy these types directly.
//
// Forward declarations for all of these classes live in astvisitor.hh, which is
// included below. That is what lets Map name unique_ptr<Var> before Var itself
// is defined further down this file.
// ============================================================================

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ast/astvisitor.hh"

using namespace std;

// ============================================================================
// Tag enums
//
// Every node carries a tag naming its concrete type. Traversal code switches on
// the tag and then dynamic_casts, rather than relying on virtual dispatch on the
// nodes themselves — see CloneVisitor and Z3InputMaker for the pattern.
// ============================================================================

// Expected HTTP status of an API call. Declared for APIFuncDecl; the specs do
// not currently use it (see APIFuncDecl below).
enum class HTTPResponseCode
{
    OK_200,
    CREATED_201,
    BAD_REQUEST_400,
    // Add other response codes as needed
};

// Concrete type tag for Expr. Each value corresponds to one subclass.
enum class ExprType
{
    INPUT,           // Input        — see the note on class Input
    FUNCCALL,        // FuncCall     — the workhorse: calls, operators, conditions
    MAP,             // Map
    NUM,             // Num
    POLYFUNCCALL,    // PolymorphicFuncCall — forward-declared only, never defined
    SET,             // Set
    STRING,          // String
    SYMVAR,          // SymVar       — declared in symvar.hh, not here
    TUPLE,           // Tuple
    VAR,             // Var
    BOOL_CONST,      // BoolConst    } the three structured logical nodes below;
    BINARY_OP,       // BinaryOpExpr } supported throughout, but specs currently
    UNARY_OP,        // UnaryOpExpr  } encode conditions as FuncCall instead
};

// Binary operators for BinaryOpExpr.
enum class BinOp {
    // Comparisons
    EQ,      // =
    NEQ,     // !=
    LT,      // <
    LE,      // <=
    GT,      // >
    GE,      // >=

    // Logical
    AND,
    OR,
    IMPLIES,

    // Set/Map operations
    IN,
    NOT_IN
};

// Unary operators for UnaryOpExpr.
enum class UnOp {
    NOT
};

// Concrete type tag for TypeExpr.
// Note TYPE_VARIABLE has no corresponding class in this header.
enum class TypeExprType
{
    TYPE_CONST,
    TYPE_VARIABLE,
    FUNC_TYPE,
    MAP_TYPE,
    SET_TYPE,
    TUPLE_TYPE
};

// Concrete type tag for Stmt.
enum class StmtType
{
    ASSIGN,
    ASSUME,
    ASSERT,
    DECL
};

// ============================================================================
// Type expressions
//
// The small type language used to declare global state. In practice the specs
// only need TypeConst ("string", "int") and MapType — every global in all three
// subject applications is a string -> string map.
// ============================================================================

// Base of the type hierarchy. toString() is what PrintVisitor renders.
class TypeExpr
{
public:
    TypeExprType typeExprType;
public:
    virtual ~TypeExpr() = default;
    virtual string toString() = 0;

protected:
    TypeExpr(TypeExprType);
};

// A named function signature: name : params -> outp.
// Currently unused — defined and constructible, but nothing in the tree builds
// one. Not to be confused with APIFuncDecl below.
class FuncDecl
{
public:
    const string name;
    const unique_ptr<TypeExpr> params;
    const unique_ptr<TypeExpr> outp;

    FuncDecl(string, unique_ptr<TypeExpr>, unique_ptr<TypeExpr>);
};

// Type Expressions

// A primitive named type: "string", "int".
class TypeConst : public TypeExpr
{
public:
    const string name;
public:
    TypeConst(string name);
    virtual string toString();
};


// A function type: (params...) -> returnType.
class FuncType : public TypeExpr
{
public:
    const vector<unique_ptr<TypeExpr>> params;
    const unique_ptr<TypeExpr> returnType;
public:
    FuncType(vector<unique_ptr<TypeExpr>>, unique_ptr<TypeExpr>);
    virtual string toString();
};

// A map type: domain -> range. The type of every global in the three specs,
// e.g. B : (string -> string) is the library's book catalogue.
class MapType : public TypeExpr
{
public:
    const unique_ptr<TypeExpr> domain;
    const unique_ptr<TypeExpr> range;
public:
    MapType(unique_ptr<TypeExpr>, unique_ptr<TypeExpr>);
    virtual string toString();
};

// A product type: (T1, T2, ...).
class TupleType : public TypeExpr
{
public:
    const vector<unique_ptr<TypeExpr>> elements;
public:
    explicit TupleType(vector<unique_ptr<TypeExpr>>);
    virtual string toString();
};

// A set type: {T}.
class SetType : public TypeExpr
{
public:
    unique_ptr<TypeExpr> elementType;
public:
    explicit SetType(unique_ptr<TypeExpr>);
    virtual string toString();
};

// A typed name. Used for Spec::globals — one Decl per piece of abstract server
// state, e.g. Decl("B", MapType(string, string)).
class Decl
{
public:
    string name;
    unique_ptr<TypeExpr> type;
public:
    Decl(string, unique_ptr<TypeExpr>);
    virtual ~Decl() = default;
    // Copy constructor takes a non-const reference: it has to move the child
    // TypeExpr out of the source, so copying a Decl leaves the original empty.
    Decl( Decl &);
//    virtual unique_ptr<Decl> clone();
};

// ============================================================================
// Expressions
//
// Everything that can appear in a condition, a call argument, or the right-hand
// side of an assignment.
//
// CONVENTION — the one thing to know before reading a spec: preconditions and
// postconditions are not built from BinaryOpExpr/UnaryOpExpr. They are built as
// FuncCall trees over a small vocabulary:
//
//   Num(1)                       true (an unconditional / public endpoint)
//   in(k, dom(B))                key k is present in map B
//   not_in(k, dom(B))            key k is absent from B
//   '(B)  — written dom(B')      B in the POST-state
//   _result                      the value the API call returned
//
// So the library's saveBookOk block reads:
//   pre  : true
//   post : _result in dom(B')
// and getBookByCodeOk reads:
//   pre  : bookCode in dom(B)
// which is exactly why {saveBookOk, getBookByCodeOk} is a feasible sequence.
// ============================================================================

// Expressions

// Base of the expression hierarchy. exprType names the concrete subclass.
class Expr
{
public:
    ExprType exprType;

protected:
    Expr(ExprType);

public:
    virtual ~Expr() = default;
};

// A symbolic input hole.
// NOTE: ATCs do not actually use this node. genATC emits input holes as
// FuncCall("input", {}) — see makeInputStmt in src/atc/atc_generator.cc. This
// class survives because CloneVisitor still handles ExprType::INPUT.
class Input : public Expr {
public:
    Input();
};

// A function application: name(args...).
// Serves three roles at once, distinguished only by the name:
//   - a real API call            saveBook(title, author, desc)
//   - a condition                in(bookCode, dom(B))
//   - a built-in operator        dom(B), '(B), input()
// SEE::isAPI() is what decides which of these a given FuncCall is.
class FuncCall : public Expr
{
public:
    const string name;
    const vector<unique_ptr<Expr>> args;
public:
    FuncCall(string, vector<unique_ptr<Expr>>);
};

// A map literal: { k1: v1, k2: v2, ... }.
// Spec::init uses the empty case to start every global as {}.
class Map : public Expr
{
public:
    const vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> value;
public:
    explicit Map(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>);
};

// An integer literal. Num(1) is also the specs' idiom for the condition "true".
class Num : public Expr
{
public:
    const int value;
public:
    explicit Num(int);
};

// A set literal: { e1, e2, ... }.
class Set : public Expr
{
public:
    const vector<unique_ptr<Expr>> elements;
public:
    explicit Set(vector<unique_ptr<Expr>>);
};



// A string literal.
class String : public Expr
{
public:
    const string value;
public:
    explicit String(string);
};

// A boolean literal. Not used when authoring specs (they write Num(1) for
// true), but SEE seeds the path constraint with BoolConst(true).
class BoolConst : public Expr {
public:
    bool value;

    BoolConst(bool v) : Expr(ExprType::BOOL_CONST), value(v) {}
};

// A binary operation: left op right.
// Handled everywhere (CloneVisitor, the rewriter, Z3InputMaker) but never
// constructed when authoring a spec — conditions go through FuncCall instead.
// Available if you want structured conditions rather than named calls.
class BinaryOpExpr : public Expr {
public:
    BinOp op;
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;

    BinaryOpExpr(BinOp o, unique_ptr<Expr> l, unique_ptr<Expr> r)
        : Expr(ExprType::BINARY_OP), op(o), left(std::move(l)), right(std::move(r)) {}
};

// A unary operation: op operand. Same status as BinaryOpExpr.
class UnaryOpExpr : public Expr {
public:
    UnOp op;
    unique_ptr<Expr> operand;

    UnaryOpExpr(UnOp o, unique_ptr<Expr> e)
        : Expr(ExprType::UNARY_OP), op(o), operand(std::move(e)) {}
};

// A tuple: (e1, e2, ...). Lets an Assign bind several results at once, which is
// why Assign::left is an Expr rather than a Var.
class Tuple : public Expr
{
public:
    const vector<unique_ptr<Expr>> exprs;
public:
    explicit Tuple(vector<unique_ptr<Expr>> exprs);
};

// A variable reference — a global (B, S, Loans), a call argument (bookCode), or
// a generated name (_result0, tmp_B_1, bookCode0_old).
//
// Names are NOT stable across the pipeline: genATC suffixes every variable in
// block i with i (convert1), so bookCode in the first block becomes bookCode0.
// That suffixing is what keeps repeated calls to the same endpoint independent.
//
// operator< exists so Var can be a key in the std::set/std::map used to track
// primed variables.
class Var : public Expr
{
public:
    string name;
public:
    explicit Var(string);
    bool operator<(const Var &v) const;
};

// ============================================================================
// Specification nodes — the INPUT tree
// ============================================================================

// Function Declaration

// The signature of one API endpoint, including its expected HTTP status.
// Populates Spec::functions, which all three specs leave EMPTY: endpoint
// signatures are carried implicitly by each API block's call instead.
class APIFuncDecl
{
public:
    const string name;
    const vector<unique_ptr<TypeExpr>> params;
    const pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>> returnType;
public:
    APIFuncDecl(string,
             vector<unique_ptr<TypeExpr>>,
             pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>>);
};

// Initialization

// The starting value of one global: varName := expr.
// Every spec initialises every global to the empty map, so a run always starts
// from a clean server state.
class Init
{
public:
    const string varName;
    const unique_ptr<Expr> expr;
public:
    Init(string, unique_ptr<Expr>);
};

// A wrapper around a condition on a call's outcome.
// ResponseExpr may be null, meaning "no condition".
class Response
{
public:
    unique_ptr<Expr> ResponseExpr;
public:
    Response( unique_ptr<Expr>);
};

// The call part of an API block: the invocation plus a response condition.
//
// CAUTION: this response field is NOT where the postcondition lives. Every spec
// passes Response(nullptr) here and puts the postcondition on the enclosing
// API::response. Read API below before using this field.
class APIcall
{
public:
    const unique_ptr<FuncCall> call;
    const Response response;

public:
    APIcall(unique_ptr<FuncCall>, Response);
};

// API

// One API block — a single endpoint behaviour, and the unit a test string is
// written in terms of.
//
//   name      the string a test string refers to, e.g. "saveBookOk". Defaults
//             to "" but is supplied for every block in all three specs, since
//             genATC resolves test strings by matching against it.
//   pre       precondition. genATC lowers this to assume(pre) — this is the
//             condition whose satisfiability decides whether a sequence is
//             feasible.
//   call      the invocation.
//   response  THE POSTCONDITION. Named "response" for historical reasons; it
//             describes the state after the call and is lowered to assert(post).
//             Postconditions refer to the post-state by priming, dom(B').
//
// Blocks come in Ok and Err flavours — getBookByCodeOk has precondition
// bookCode in dom(B), getBookByCodeErr has bookCode not_in dom(B). Both are
// legitimate things to test.
class API
{
public:

    string name; // Optional name for the API block
    unique_ptr<Expr> pre;
    unique_ptr<APIcall> call;
    Response response;
public:
    API(unique_ptr<Expr>, unique_ptr<APIcall>, Response, string name="");
};

// Block class (placeholder as it wasn't fully specified in the grammar)
// Top-level Spec class

// The root of a specification: one subject application, described formally.
// Produced by makeLibrarySpec() / makeRestaurantSpec() / makeEcommerceSpec().
//
//   globals    abstract server state, e.g. the library's B, S, Req, Loans
//   init       starting value of each global (all empty maps)
//   functions  endpoint signatures — empty in all three specs
//   blocks     THE API BLOCKS. The alphabet a test string is drawn from, and
//              what genATC searches when resolving a name. A plain vector, so
//              it is never null and is iterated directly.
//
// All four fields are const: a Spec is immutable once constructed.
class Spec
{
public:
    const vector<unique_ptr<Decl>> globals;
    const vector<unique_ptr<Init>> init;
    const vector<unique_ptr<APIFuncDecl>> functions;
    const vector<unique_ptr<API>> blocks;
public:
    Spec(vector<unique_ptr<Decl>>,
         vector<unique_ptr<Init>>,
         vector<unique_ptr<APIFuncDecl>>,
         vector<unique_ptr<API>>);
};

// ============================================================================
// Statements and Program — the OUTPUT tree
//
// genATC lowers each API block of a test string into this fixed shape:
//
//     x_i    := input()      one per free argument variable
//     v_old  := v            snapshot of each primed variable used in post
//     assume(pre_i)          <-- the feasibility condition
//     _result{i} := call_i(...)
//     assert(post_i)         primes stripped, _result renamed to _result{i}
//
// The Assume statements across all blocks are the accumulated precondition of
// the whole test string, and are what a feasibility oracle should collect.
// ============================================================================

// Base of the statement hierarchy. statementType names the concrete subclass.
class Stmt
{
public:
    const StmtType statementType;
public:
    virtual ~Stmt() = default;
protected:
    Stmt(StmtType);
};

// Assignment statement: l = r

// left := right. Also the form an API call takes: _result0 := saveBook(...).
class Assign : public Stmt
{
public:
    const unique_ptr<Expr> left;  // Changed from Var to Expr to support tuples
    const unique_ptr<Expr> right;
public:
    Assign(unique_ptr<Expr>, unique_ptr<Expr>);
};

// Assume statement: assume(c)

// A condition taken as given. Carries an API block's precondition; the
// conjunction of these is what Z3 is asked about, and UNSAT here means the test
// string describes an impossible scenario.
class Assume : public Stmt
{
public:
    const unique_ptr<Expr> expr;
public:
    Assume(unique_ptr<Expr>);
};

// Assert statement: assert(c)

// A condition that must hold after a call — an API block's postcondition. This
// is the actual test oracle: what the generated test checks.
class Assert : public Stmt
{
public:
    const unique_ptr<Expr> expr;
public:
    Assert(unique_ptr<Expr>);
};

//    is the root of our AST

// The root of a test case: a flat, ordered statement list with no nesting or
// control flow. Represents both the ATC and the CTC — the difference is whether
// input() holes have been replaced by solved concrete values.
//
// GOTCHA: statements is const, so the passes that need to move statements out
// of a Program (genATC, Tester::generateATC) const_cast it. Keep that in mind
// before treating a Program as immutable.
class Program
{
public:
    const vector<unique_ptr<Stmt>> statements;
public:
    explicit Program(vector<unique_ptr<Stmt>>);
};
#endif
