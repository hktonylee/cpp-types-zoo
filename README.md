# C++ Types Zoo

A comprehensive exploration of `auto` and `decltype` type deduction.

**Compiler:** GCC 11.4

**C++ Standard:** 201703

## 1. Base Variable Types

| Variable | Declared Type |
|----------|---------------|
| `x` | `int` |
| `cx` | `const int` |
| `rx` | `int&` |
| `crx` | `const int&` |
| `rrx` | `int&&` |
| `crrx` | `const int&&` |
| `px` | `int*` |
| `pcx` | `const int*` |
| `cpx` | `int* const` |
| `cpcx` | `const int* const` |

## 2. decltype on Expressions

### 2.1 decltype on Variables (id-expressions)

When `decltype` is applied to an **unparenthesized** id-expression, it yields the **declared type**.

| Expression | Type |
|------------|------|
| `decltype(x)` | `int` |
| `decltype(cx)` | `const int` |
| `decltype(rx)` | `int&` |
| `decltype(crx)` | `const int&` |
| `decltype(rrx)` | `int&&` |
| `decltype(crrx)` | `const int&&` |

### 2.2 decltype on Parenthesized Expressions

When `decltype` is applied to a **parenthesized** expression `(expr)`, it considers the **value category**:
- **lvalue** → `T&`
- **xvalue** → `T&&`
- **prvalue** → `T`

| Expression | Type | Why |
|------------|------|-----|
| `decltype((x))` | `int&` | x is lvalue |
| `decltype((cx))` | `const int&` | cx is lvalue |
| `decltype((rx))` | `int&` | rx is lvalue |
| `decltype((rrx))` | `int&` | rrx is lvalue! |
| `decltype((std::move(x)))` | `int&&` | xvalue |
| `decltype((42))` | `int` | prvalue |
| `decltype((get_value()))` | `int` | prvalue |
| `decltype((get_lref()))` | `int&` | lvalue |
| `decltype((get_rref()))` | `int&&` | xvalue |

## 3. auto Type Deduction

### 3.1 Plain `auto` (strips references and top-level cv-qualifiers)

| Declaration | Deduced Type |
|-------------|-------------|
| `auto v = x` | `int` |
| `auto v = cx` | `int` |
| `auto v = rx` | `int` |
| `auto v = crx` | `int` |
| `auto v = rrx` | `int` |
| `auto v = std::move(x)` | `int` |
| `auto v = get_value()` | `int` |
| `auto v = get_lref()` | `int` |
| `auto v = get_rref()` | `int` |

### 3.2 `auto&` (lvalue reference, preserves const)

| Declaration | Deduced Type |
|-------------|-------------|
| `auto& v = x` | `int&` |
| `auto& v = cx` | `const int&` |
| `auto& v = rx` | `int&` |
| `auto& v = crx` | `const int&` |
| `auto& v = rrx` | `int&` |
| `auto& v = get_lref()` | `int&` |

**Note:** `auto& v = std::move(x)`, `auto& v = get_value()`, `auto& v = get_rref()` would be **errors** (can't bind lvalue ref to rvalue).

### 3.3 `const auto&` (extends lifetime of temporaries)

| Declaration | Deduced Type |
|-------------|-------------|
| `const auto& v = x` | `const int&` |
| `const auto& v = cx` | `const int&` |
| `const auto& v = rx` | `const int&` |
| `const auto& v = std::move(x)` | `const int&` |
| `const auto& v = get_value()` | `const int&` |
| `const auto& v = 42` | `const int&` |

### 3.4 `auto&&` (forwarding/universal reference)

| Declaration | Deduced Type | Why |
|-------------|-------------|-----|
| `auto&& v = x` | `int&` | x is lvalue |
| `auto&& v = cx` | `const int&` | cx is lvalue |
| `auto&& v = rx` | `int&` | rx is lvalue |
| `auto&& v = crx` | `const int&` | crx is lvalue |
| `auto&& v = rrx` | `int&` | rrx is lvalue! |
| `auto&& v = std::move(x)` | `int&&` | xvalue |
| `auto&& v = get_value()` | `int&&` | prvalue |
| `auto&& v = get_lref()` | `int&` | lvalue |
| `auto&& v = get_rref()` | `int&&` | xvalue |
| `auto&& v = 42` | `int&&` | prvalue |

## 4. `decltype(auto)`

Preserves the **exact type** including references and cv-qualifiers.

| Declaration | Deduced Type | Why |
|-------------|-------------|-----|
| `decltype(auto) v = x` | `int` | id-expression → declared type |
| `decltype(auto) v = cx` | `const int` | id-expression → declared type |
| `decltype(auto) v = std::move(x)` | `int&&` | expression returns int&& |
| `decltype(auto) v = (x)` | `int&` | (x) is lvalue → adds & |
| `decltype(auto) v = (cx)` | `const int&` | (cx) is lvalue → adds const& |

**Warning:** `decltype(auto) v = rrx;` would be an **error** because `rrx` has declared type `int&&`, but `rrx` itself is an lvalue and can't bind to `int&&`.

## 5. Pointers with `auto`

| Declaration | Deduced Type | Note |
|-------------|-------------|------|
| `auto v = px` | `int*` | |
| `auto v = pcx` | `const int*` | pointee const preserved |
| `auto v = cpx` | `int*` | top-level const stripped |
| `auto v = cpcx` | `const int*` | top-level const stripped |
| `auto* v = px` | `int*` | |
| `auto* v = pcx` | `const int*` | pointee const preserved |
| `const auto* v = px` | `const int*` | adds pointee const |
| `auto* const v = px` | `int* const` | pointer is const |

## 6. Value Categories Reference

```
          expression
          /        \
      glvalue      rvalue
      /     \     /     \
   lvalue   xvalue   prvalue
```

| Category | Has Identity | Can Move From | Examples |
|----------|--------------|---------------|----------|
| **lvalue** | Yes | No | `x`, `*p`, `a[n]`, `++i` |
| **xvalue** | Yes | Yes | `std::move(x)`, `a[n]` where a is rvalue |
| **prvalue** | No | Yes | `42`, `x + y`, function returning by value |

## 7. Key Insights

1. **`auto` strips top-level const and references** — it always creates a new copy
2. **`auto&` preserves const** but requires an lvalue
3. **`auto&&` is a forwarding reference** — binds to anything, preserves value category
4. **`decltype(var)` returns declared type** of the variable
5. **`decltype((var))` adds reference** based on value category (lvalue → `T&`)
6. **`decltype(auto)` uses decltype rules** on the initializer expression
7. **Named rvalue references are lvalues!** — `rrx` in `int&& rrx = ...` is an lvalue
8. **`const auto&` extends lifetime** of temporaries
9. **`auto*` preserves pointee const** but `auto` strips top-level pointer const

## 8. Common Gotchas

### Named rvalue references are lvalues

```cpp
int&& rrx = std::move(x);  // rrx has type int&&
auto&& v = rrx;            // but rrx is an lvalue! v is int&
```

### decltype with parentheses

```cpp
int x = 42;
decltype(x)   a;  // int
decltype((x)) b = x;  // int& (dangerous: returns reference to local!)
```

### auto strips const from the thing being copied

```cpp
const int cx = 42;
auto v = cx;  // v is int, not const int
```

---
*Generated by cpp-types-zoo*
