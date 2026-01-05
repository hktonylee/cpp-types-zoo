#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// Type name extraction using compiler intrinsics
template <typename T>
constexpr std::string_view type_name() {
    #if defined(__clang__)
        std::string_view p = __PRETTY_FUNCTION__;
        return std::string_view(p.data() + 34, p.size() - 34 - 1);
    #elif defined(__GNUC__)
        std::string_view p = __PRETTY_FUNCTION__;
        return std::string_view(p.data() + 49, p.find(';', 49) - 49);
    #else
        return "unknown";
    #endif
}

// Helper to print type info in markdown table format
#define PRINT_ROW(expr, type) \
    std::cout << "| `" << expr << "` | `" << type_name<type>() << "` |" << std::endl

// Function returning by value (prvalue)
int get_value() { return 42; }

// Function returning by lvalue reference
int& get_lref() { static int val = 42; return val; }

// Function returning by rvalue reference (xvalue)
int&& get_rref() { static int val = 42; return std::move(val); }

int main() {
    // Setup various input types
    int x = 42;                      // plain int
    const int cx = 42;               // const int
    int& rx = x;                     // lvalue reference
    const int& rcx = x;              // const lvalue reference
    int&& rrx = std::move(x);        // rvalue reference (but rrx itself is lvalue)
    const int&& rrcx = 42;           // const rvalue reference
    
    int* px = &x;                    // pointer
    const int* pcx = &x;             // pointer to const
    int* const cpx = &x;             // const pointer
    const int* const cpcx = &x;      // const pointer to const

    std::cout << "# C++ Types Zoo\n\n";
    std::cout << "A comprehensive exploration of `auto` and `decltype` type deduction.\n\n";
    std::cout << "**Compiler:** ";
    #if defined(__clang__)
        std::cout << "Clang " << __clang_major__ << "." << __clang_minor__;
    #elif defined(__GNUC__)
        std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__;
    #else
        std::cout << "Unknown";
    #endif
    std::cout << "\n\n";
    std::cout << "**C++ Standard:** " << __cplusplus << "\n\n";

    // ==========================================
    // Section 1: Base Variables
    // ==========================================
    std::cout << "## 1. Base Variable Types\n\n";
    std::cout << "| Variable | Declared Type |\n";
    std::cout << "|----------|---------------|\n";
    PRINT_ROW("x", decltype(x));
    PRINT_ROW("cx", decltype(cx));
    PRINT_ROW("rx", decltype(rx));
    PRINT_ROW("rcx", decltype(rcx));
    PRINT_ROW("rrx", decltype(rrx));
    PRINT_ROW("rrcx", decltype(rrcx));
    PRINT_ROW("px", decltype(px));
    PRINT_ROW("pcx", decltype(pcx));
    PRINT_ROW("cpx", decltype(cpx));
    PRINT_ROW("cpcx", decltype(cpcx));
    std::cout << "\n";

    // ==========================================
    // Section 2: decltype on Expressions
    // ==========================================
    std::cout << "## 2. decltype on Expressions\n\n";
    std::cout << "### 2.1 decltype on Variables (id-expressions)\n\n";
    std::cout << "When `decltype` is applied to an **unparenthesized** id-expression, it yields the **declared type**.\n\n";
    std::cout << "| Expression | Type |\n";
    std::cout << "|------------|------|\n";
    PRINT_ROW("decltype(x)", decltype(x));
    PRINT_ROW("decltype(cx)", decltype(cx));
    PRINT_ROW("decltype(rx)", decltype(rx));
    PRINT_ROW("decltype(rcx)", decltype(rcx));
    PRINT_ROW("decltype(rrx)", decltype(rrx));
    PRINT_ROW("decltype(rrcx)", decltype(rrcx));
    std::cout << "\n";

    std::cout << "### 2.2 decltype on Parenthesized Expressions\n\n";
    std::cout << "When `decltype` is applied to a **parenthesized** expression `(expr)`, it considers the **value category**:\n";
    std::cout << "- **lvalue** → `T&`\n";
    std::cout << "- **xvalue** → `T&&`\n";
    std::cout << "- **prvalue** → `T`\n\n";
    std::cout << "| Expression | Type | Why |\n";
    std::cout << "|------------|------|-----|\n";
    std::cout << "| `decltype((x))` | `" << type_name<decltype((x))>() << "` | x is lvalue |\n";
    std::cout << "| `decltype((cx))` | `" << type_name<decltype((cx))>() << "` | cx is lvalue |\n";
    std::cout << "| `decltype((rx))` | `" << type_name<decltype((rx))>() << "` | rx is lvalue |\n";
    std::cout << "| `decltype((rrx))` | `" << type_name<decltype((rrx))>() << "` | rrx is lvalue! |\n";
    std::cout << "| `decltype((std::move(x)))` | `" << type_name<decltype((std::move(x)))>() << "` | xvalue |\n";
    std::cout << "| `decltype((42))` | `" << type_name<decltype((42))>() << "` | prvalue |\n";
    std::cout << "| `decltype((get_value()))` | `" << type_name<decltype((get_value()))>() << "` | prvalue |\n";
    std::cout << "| `decltype((get_lref()))` | `" << type_name<decltype((get_lref()))>() << "` | lvalue |\n";
    std::cout << "| `decltype((get_rref()))` | `" << type_name<decltype((get_rref()))>() << "` | xvalue |\n";
    std::cout << "\n";

    // ==========================================
    // Section 3: auto Type Deduction
    // ==========================================
    std::cout << "## 3. auto Type Deduction\n\n";
    std::cout << "### 3.1 Plain `auto` (strips references and top-level cv-qualifiers)\n\n";
    std::cout << "| Declaration | Deduced Type |\n";
    std::cout << "|-------------|-------------|\n";
    { auto v = x;            PRINT_ROW("auto v = x", decltype(v)); }
    { auto v = cx;           PRINT_ROW("auto v = cx", decltype(v)); }
    { auto v = rx;           PRINT_ROW("auto v = rx", decltype(v)); }
    { auto v = rcx;          PRINT_ROW("auto v = rcx", decltype(v)); }
    { auto v = rrx;          PRINT_ROW("auto v = rrx", decltype(v)); }
    { auto v = std::move(x); PRINT_ROW("auto v = std::move(x)", decltype(v)); }
    { auto v = get_value();  PRINT_ROW("auto v = get_value()", decltype(v)); }
    { auto v = get_lref();   PRINT_ROW("auto v = get_lref()", decltype(v)); }
    { auto v = get_rref();   PRINT_ROW("auto v = get_rref()", decltype(v)); }
    std::cout << "\n";

    std::cout << "### 3.2 `auto&` (lvalue reference, preserves const)\n\n";
    std::cout << "| Declaration | Deduced Type |\n";
    std::cout << "|-------------|-------------|\n";
    { auto& v = x;           PRINT_ROW("auto& v = x", decltype(v)); }
    { auto& v = cx;          PRINT_ROW("auto& v = cx", decltype(v)); }
    { auto& v = rx;          PRINT_ROW("auto& v = rx", decltype(v)); }
    { auto& v = rcx;         PRINT_ROW("auto& v = rcx", decltype(v)); }
    { auto& v = rrx;         PRINT_ROW("auto& v = rrx", decltype(v)); }
    { auto& v = get_lref();  PRINT_ROW("auto& v = get_lref()", decltype(v)); }
    std::cout << "\n";
    std::cout << "**Note:** `auto& v = std::move(x)`, `auto& v = get_value()`, `auto& v = get_rref()` would be **errors** (can't bind lvalue ref to rvalue).\n\n";

    std::cout << "### 3.3 `const auto&` (extends lifetime of temporaries)\n\n";
    std::cout << "| Declaration | Deduced Type |\n";
    std::cout << "|-------------|-------------|\n";
    { const auto& v = x;            PRINT_ROW("const auto& v = x", decltype(v)); }
    { const auto& v = cx;           PRINT_ROW("const auto& v = cx", decltype(v)); }
    { const auto& v = rx;           PRINT_ROW("const auto& v = rx", decltype(v)); }
    { const auto& v = std::move(x); PRINT_ROW("const auto& v = std::move(x)", decltype(v)); }
    { const auto& v = get_value();  PRINT_ROW("const auto& v = get_value()", decltype(v)); }
    { const auto& v = 42;           PRINT_ROW("const auto& v = 42", decltype(v)); }
    std::cout << "\n";

    std::cout << "### 3.4 `auto&&` (forwarding/universal reference)\n\n";
    std::cout << "| Declaration | Deduced Type | Why |\n";
    std::cout << "|-------------|-------------|-----|\n";
    { auto&& v = x;            std::cout << "| `auto&& v = x` | `" << type_name<decltype(v)>() << "` | x is lvalue |\n"; }
    { auto&& v = cx;           std::cout << "| `auto&& v = cx` | `" << type_name<decltype(v)>() << "` | cx is lvalue |\n"; }
    { auto&& v = rx;           std::cout << "| `auto&& v = rx` | `" << type_name<decltype(v)>() << "` | rx is lvalue |\n"; }
    { auto&& v = rcx;          std::cout << "| `auto&& v = rcx` | `" << type_name<decltype(v)>() << "` | rcx is lvalue |\n"; }
    { auto&& v = rrx;          std::cout << "| `auto&& v = rrx` | `" << type_name<decltype(v)>() << "` | rrx is lvalue! |\n"; }
    { auto&& v = std::move(x); std::cout << "| `auto&& v = std::move(x)` | `" << type_name<decltype(v)>() << "` | xvalue |\n"; }
    { auto&& v = get_value();  std::cout << "| `auto&& v = get_value()` | `" << type_name<decltype(v)>() << "` | prvalue |\n"; }
    { auto&& v = get_lref();   std::cout << "| `auto&& v = get_lref()` | `" << type_name<decltype(v)>() << "` | lvalue |\n"; }
    { auto&& v = get_rref();   std::cout << "| `auto&& v = get_rref()` | `" << type_name<decltype(v)>() << "` | xvalue |\n"; }
    { auto&& v = 42;           std::cout << "| `auto&& v = 42` | `" << type_name<decltype(v)>() << "` | prvalue |\n"; }
    std::cout << "\n";

    // ==========================================
    // Section 4: decltype(auto)
    // ==========================================
    std::cout << "## 4. `decltype(auto)`\n\n";
    std::cout << "Preserves the **exact type** including references and cv-qualifiers.\n\n";
    std::cout << "| Declaration | Deduced Type | Why |\n";
    std::cout << "|-------------|-------------|-----|\n";
    
    // Without parentheses - uses id-expression rules
    { decltype(auto) v = x;   std::cout << "| `decltype(auto) v = x` | `" << type_name<decltype(v)>() << "` | id-expression → declared type |\n"; }
    { decltype(auto) v = cx;  std::cout << "| `decltype(auto) v = cx` | `" << type_name<decltype(v)>() << "` | id-expression → declared type |\n"; }
    // Note: decltype(auto) v = rx would be int& but that's valid
    // Note: decltype(auto) v = rrx would try to create int&& which can't bind to lvalue
    { decltype(auto) v = std::move(x); std::cout << "| `decltype(auto) v = std::move(x)` | `" << type_name<decltype(v)>() << "` | expression returns int&& |\n"; }
    
    // With parentheses - uses expression rules (lvalue → T&)
    { decltype(auto) v = (x);  std::cout << "| `decltype(auto) v = (x)` | `" << type_name<decltype(v)>() << "` | (x) is lvalue → adds & |\n"; }
    { decltype(auto) v = (cx); std::cout << "| `decltype(auto) v = (cx)` | `" << type_name<decltype(v)>() << "` | (cx) is lvalue → adds const& |\n"; }
    std::cout << "\n";
    std::cout << "**Warning:** `decltype(auto) v = rrx;` would be an **error** because `rrx` has declared type `int&&`, but `rrx` itself is an lvalue and can't bind to `int&&`.\n\n";

    // ==========================================
    // Section 5: Pointers
    // ==========================================
    std::cout << "## 5. Pointers with `auto`\n\n";
    std::cout << "| Declaration | Deduced Type | Note |\n";
    std::cout << "|-------------|-------------|------|\n";
    { auto v = px;   std::cout << "| `auto v = px` | `" << type_name<decltype(v)>() << "` | |\n"; }
    { auto v = pcx;  std::cout << "| `auto v = pcx` | `" << type_name<decltype(v)>() << "` | pointee const preserved |\n"; }
    { auto v = cpx;  std::cout << "| `auto v = cpx` | `" << type_name<decltype(v)>() << "` | top-level const stripped |\n"; }
    { auto v = cpcx; std::cout << "| `auto v = cpcx` | `" << type_name<decltype(v)>() << "` | top-level const stripped |\n"; }
    { auto* v = px;  std::cout << "| `auto* v = px` | `" << type_name<decltype(v)>() << "` | |\n"; }
    { auto* v = pcx; std::cout << "| `auto* v = pcx` | `" << type_name<decltype(v)>() << "` | pointee const preserved |\n"; }
    { const auto* v = px;  std::cout << "| `const auto* v = px` | `" << type_name<decltype(v)>() << "` | adds pointee const |\n"; }
    { auto* const v = px;  std::cout << "| `auto* const v = px` | `" << type_name<decltype(v)>() << "` | pointer is const |\n"; }
    std::cout << "\n";

    // ==========================================
    // Section 6: Value Categories Summary
    // ==========================================
    std::cout << "## 6. Value Categories Reference\n\n";
    std::cout << "```\n";
    std::cout << "          expression\n";
    std::cout << "          /        \\\n";
    std::cout << "      glvalue      rvalue\n";
    std::cout << "      /     \\     /     \\\n";
    std::cout << "   lvalue   xvalue   prvalue\n";
    std::cout << "```\n\n";
    std::cout << "| Category | Has Identity | Can Move From | Examples |\n";
    std::cout << "|----------|--------------|---------------|----------|\n";
    std::cout << "| **lvalue** | Yes | No | `x`, `*p`, `a[n]`, `++i` |\n";
    std::cout << "| **xvalue** | Yes | Yes | `std::move(x)`, `a[n]` where a is rvalue |\n";
    std::cout << "| **prvalue** | No | Yes | `42`, `x + y`, function returning by value |\n";
    std::cout << "\n";

    // ==========================================
    // Section 7: Key Insights
    // ==========================================
    std::cout << "## 7. Key Insights\n\n";
    std::cout << "1. **`auto` strips top-level const and references** — it always creates a new copy\n";
    std::cout << "2. **`auto&` preserves const** but requires an lvalue\n";
    std::cout << "3. **`auto&&` is a forwarding reference** — binds to anything, preserves value category\n";
    std::cout << "4. **`decltype(var)` returns declared type** of the variable\n";
    std::cout << "5. **`decltype((var))` adds reference** based on value category (lvalue → `T&`)\n";
    std::cout << "6. **`decltype(auto)` uses decltype rules** on the initializer expression\n";
    std::cout << "7. **Named rvalue references are lvalues!** — `rrx` in `int&& rrx = ...` is an lvalue\n";
    std::cout << "8. **`const auto&` extends lifetime** of temporaries\n";
    std::cout << "9. **`auto*` preserves pointee const** but `auto` strips top-level pointer const\n\n";

    // ==========================================
    // Section 8: Common Gotchas
    // ==========================================
    std::cout << "## 8. Common Gotchas\n\n";
    std::cout << "### Named rvalue references are lvalues\n\n";
    std::cout << "```cpp\n";
    std::cout << "int&& rrx = std::move(x);  // rrx has type int&&\n";
    std::cout << "auto&& v = rrx;            // but rrx is an lvalue! v is int&\n";
    std::cout << "```\n\n";
    std::cout << "### decltype with parentheses\n\n";
    std::cout << "```cpp\n";
    std::cout << "int x = 42;\n";
    std::cout << "decltype(x)   a;  // int\n";
    std::cout << "decltype((x)) b = x;  // int& (dangerous: returns reference to local!)\n";
    std::cout << "```\n\n";
    std::cout << "### auto strips const from the thing being copied\n\n";
    std::cout << "```cpp\n";
    std::cout << "const int cx = 42;\n";
    std::cout << "auto v = cx;  // v is int, not const int\n";
    std::cout << "```\n\n";

    std::cout << "---\n";
    std::cout << "*Generated by cpp-types-zoo*\n";

    return 0;
}
