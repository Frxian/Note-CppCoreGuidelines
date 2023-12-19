# Note for CppCoreGuidelines
This is my note for CppCoreGuidelines.
Where: F

## Type Safety

### Problem areas
* **narrowing conversions**

  Minimize their use and use `narrow` or `narrow_cast` (from the GSL) where they are necessary.

* **range errors**

  Use `span`

* **array decay**

  Use `span` (from the GSL)

* **casts**

  Minimize their use. `Templates` can help.

* **unions**

  Use `variant` (in C++17)

### gsl::index / std::ptrdiff_t

Index for container and array. 

### Report the use of void* as a parameter or return type

### Report the use of more than one bool parameters

Because these bool parameters could be enum type.

### If an interface is a template, document its parameters using concepts

Warn if any non-variadic template parameter is not constrained by a concept (in its declaration or mentioned in a requires clause).

### Declare a pointer that must not be null as gsl::not_null



### Use gsl::zstring to describe a pointer to a zero terminated C-style string

`zstring` is a zero terminated `char` string.
`czstring` is a const zero terminated `char` string.
`wzstring` is a zero terminated `wchar_t` string.
`cwzstring` is a const zero terminated `wchar_t` string.
`u16zstring` is a zero terminated `char16_t` string.
`cu16zstring` is a const zero terminated `char16_t` string.
`u32zstring` is a zero terminated `char32_t` string.
`cu32zstring` is a const zero terminated `char32_t` string.

### Do not pass an array as a single pointer

use `std::string_view` or `span<char>` from the GSL to prevent range errors.



## Resource safety

### gsl::owner / std::unique_ptr / std::shared_ptr

Use them to mark the ownership of a pointer. 

A example using gsl::owner

```c++
gsl::owner<X*> compute(args)    // It is now clear that ownership is transferred
{
    gsl::owner<X*> res = new X{};
    // ...
    return res;
}
```





## Shared resource

### Avoid non-const global variables

* A function should not make control-flow decisions based on the values of variables declared at namespace scope.
* A function should not write to variables declared at namespace scope.

### Avoid singletons

Singletons are basically complicated global objects in disguise.



## Exceptions

### Use exceptions to signal a failure to perform a required task

A good rule for performance critical code is to move checking outside the critical part of the code.





## Assert

### Gsl::Expects() / gsl::Ensures() / static_assert() / assert() / std::terminate()

These are as pre/postcondition.



## API

### API definition rules

#### Pimple idiom

**Reason**:

* For `stable library ABI`, consider the Pimpl idiom. 
* `Private data members` participate in class layout and private member functions participate in overload resolution, changes to those implementation details require `recompilation` of all users of a class that uses them. 
* A `non-polymorphic interface class` holding a pointer to `implementation` (Pimpl) can isolate the users of a class from changes in its implementation at the cost of an indirection.

#### Prefer empty abstract classes as interfaces to class hierarchies

**Reason**:

* Abstract classes that are `empty` (have no non-static member data) are more likely to be `stable` than base classes with state.

#### Keep the number of function arguments low

**Reasons**:

* `Missing an abstraction`. There is an abstraction missing, so that a compound value is being passed as individual elements instead of as a single object that enforces an invariant. This not only expands the parameter list, but it leads to errors because the component values are no longer protected by an enforced invariant.
* `“Violating "one function, one responsibility`. The function is trying to do more than one job and should probably be refactored.

**Method**:

* Grouping arguments into "`bundles`" is a general technique to reduce the number of arguments and to increase the opportunities for checking.

#### Avoid adjacent parameters that can be invoked by the same arguments in either order with different meaning

**Reason**:

* Avoid adjacent parameters that can be invoked by the same arguments in either order with different meaning.

**Method**:

* Define a `struct` as the parameter type and name the fields for those parameters accordingly.

#### A function should perform a single logical operation

Keep function `short` and `simple`.

#### If a function might have to be evaluated at compile time, declare it constexpr

`constexpr`does not guarantee `compile-time` evaluation; it just guarantees that the function can be evaluated at compile time for constant expression arguments if the programmer requires it or the compiler decides to do so to optimize.

#### If a function is very small and time-critical, declare it inline

* Specifying `inline` (explicitly, or implicitly when writing member functions inside a class definition) encourages the compiler to do a better job.
* `Constexpr` implies inline.

#### If your function must not throw, declare it noexcept

* If an `exception` is not supposed to be thrown, the program cannot be assumed to cope with the error and should be `terminated` as soon as possible. Declaring a function noexcept helps optimizers by reducing the number of `alternative execution paths`. It also speeds up the exit after failure.
* If you know that your application code cannot respond to an `allocation failure`, it could be appropriate to add `noexcept` even on functions that allocate.
* **Destructors, swap functions, move operations, and default constructors should never throw.**

#### For general use, take T* or T& arguments rather than smart pointers

**Reasons**:

* Passing a `smart pointer transfers` or `shares ownership` and should only be used when `ownership semantics` are intended. A function that does `not manipulate lifetime` should take `raw pointers` or `references` instead.
* Passing a `shared smart pointer` (e.g., std::shared_ptr) implies a `run-time cost`.

**Enforcement**:

* Warn if a function takes a parameter of a smart pointer type (that overloads operator-> or operator\*) that is copyable but the function only calls any of: `operator\*, operator-> or get()`. **Suggest using a T* or T& instead**.
* Flag a parameter of a smart pointer type (a type that overloads operator-> or operator\*) that is copyable/movable but `never copied/moved` from in the function body, and that is never modified, and that is `not passed along to another function` that could do so. That means the ownership semantics are not used. **Suggest using a T* or T& instead**.

#### Prefer pure functions

easy to reason about, to optimize and can be memoized. 

ref: https://poe.com/s/BdF0lOMejSzPbOeL66oT

#### Unused parameters should be unnamed

If parameters are `conditionally unused`, declare them with the `[[maybe_unused]]` attribute.

#### If an operation can be reused, give it a name



### Parameter passing expression rules

**Take parameters properly**:

![take-parameters-properly](/Users/frxianz/Coding/Resource/C++/take-parameters-properly.JPG)

#### Prefer simple and conventional ways of passing information

* **Normal parameter passing**

  ![image-20231206102439531](/Users/frxianz/Library/Application Support/typora-user-images/image-20231206102439531.png)

* **Advanced parameter passing**

  ![image-20231206102526584](/Users/frxianz/Library/Application Support/typora-user-images/image-20231206102526584.png)

#### For "in" parameters, pass cheaply-copied types by value and others by reference to const

* What is "`cheap to copy`" depends on the machine architecture, but two or three words (`doubles, pointers, references`) are usually best `passed by value`. 
* Avoid "esoteric techniques" such as passing arguments as `T&&` "for efficiency". Most rumors about performance advantages from passing by && are **false** or **brittle**.
* If you need the notion of an `optional value`, use a `pointer`, `std::optional`.
* Some `user-defined` and `standard library types`, such as `span<T>` or the` iterators` are cheap to copy and may be passed by value.

#### For "forward" parameters, pass by TP&& and only std::forward the parameter



#### For "out" output values, prefer return values to output parameters

* If a type is `expensive to move` (e.g., array<BigTrivial>), consider allocating it on the`free store` and return a `handle (e.g., unique_ptr)`, or passing it in` a reference to non-const` target object to fill (to be used as an out-parameter).
* To `reuse an object` that carries capacity (e.g., std::string, std::vector) across `multiple calls` to the function in an `inner loop`: treat it as an `in/out parameter` and pass by `reference`.

#### To return multiple "out" values, prefer returning a struct or tuple

* by convention of using a `tuple` (including `pair`), possibly with the extra convenience of tie or structured `bindings` (C++17)
* When the `tuple` to be returned is initialized from `local variables` that are `expensive to copy`, explicit `move` may be helpful to avoid copying.

#### Prefer T* over T& when "no argument" is a valid option

* **Reason**

  A pointer (T*) can be a `nullptr` and a reference (T&) cannot, there is no valid "null reference.

* If you prefer the `pointer notation` (-> and/or * vs. .), `not_null<T*>` provides the same guarantee as T&.”.

### Parameter passing semantic rules

#### Use T* or owner<T*> to designate a single object

* plain `T*` is used for many `weakly-related` purposes, such as:

  * Identify `a (single) object` (not to be deleted by this function).
  * Point to an object allocated on the `free store` (and delete it later).
  * Hold the `nullptr`.
  * Identify a `C-style string` (zero-terminated array of characters).
  * Identify `an array` with a length specified separately.
  * Identify `a location` in an array.

* ```c++
  void use2(span<int> p, zstring s, owner<int*> q)
  {
      p[p.size() - 1] = 666; // OK, a range error can be caught
      cout << s; // OK
      delete q;  // OK
  }
  ```

#### Use a `not_null` to indicate that "null" is not a valid value

* `not_null<T*>` makes it obvious to a reader (human or machine) that a test for `nullptr` is not necessary before dereference.
* `not_null` is not just for built-in pointers. It works for `unique_ptr`, `shared_ptr`, and other pointer-like types.

#### Use a `span` or a `span_p` to designate a half-open sequence

* A `span<T>` object does not own its elements and is so small that it can be passed by value.
* Passing a `span` object as an argument is exactly as efficient as passing a pair of pointer arguments or passing a pointer and an integer count.

#### Use a `zstring` or a `not_null<zstring>` to designate a C-style string

* We must distinguish `C-style strings` from `a pointer to a single character` or `an old-fashioned pointer to an array of characters`.

#### Use unique_ptr and shared_ptr to transfer or share ownership

* Prefer a `unique_ptr` over a `shared_ptr` if there is never more than one owner at a time. 
* That pervasive use of `shared_ptr` has a cost (`atomic operations` on the `shared_ptr`’s reference count have a measurable aggregate cost).

### Value return semantic rules

#### Never (directly or indirectly) return a pointer or a reference to a local object

* To avoid the `crashes and data corruption` that can result from the use of such `a dangling pointer`.
* This applies only to `non-static local variables`. All `static` variables are (as their name indicates) statically allocated, so that pointers to them cannot `dangle`.

#### Return a `T&` when copy is undesirable and “returning no object” isn’t needed

* `The return of a reference` must not imply `transfer of ownership`.

#### Don’t return a `T&&`

* Returning an rvalue reference is fine when the reference to the temporary is being passed `“downward” to a callee`.
* `it’s not fine` when passing such a reference `“upward” to a larger caller scope`.
* `std::move` and `std::forward` do return `&&`, but they are just casts – used by convention only in expression contexts where a reference to a temporary object is passed along within the same expression before the temporary is destroyed.

#### Don’t `return std::move(local)`

* With `guaranteed copy elision`, it is now almost always a `pessimization` to expressly use `std::move` in a return statement.

#### Don’t return `const T`

```c++
const vector<int> fct();    // bad: that "const" is more trouble than it is worth

void g(vector<int>& vx)
{
    // ...
    fct() = vx;   // prevented by the "const"
    // ...
    vx = fct(); // expensive copy: move semantics suppressed by the "const"
    // ...
}
```

### Other API rules

#### Where there is a choice, prefer default arguments over overloading

* There is `no guarantee` that `a set of overloaded functions` all implement the same semantics. The use of `default arguments` can avoid code replication.

* There is not a choice when a set of functions are used to do a semantically equivalent operation to a set of types. For example:

  ```c++
  void print(const char&);
  void print(int);
  void print(zstring);
  ```

#### Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms

* For `efficiency and correctness`, you nearly always want to capture by reference when using the lambda `locally`. 
* This includes when `writing or calling parallel algorithms` that are `local` because they join before returning.

#### Avoid capturing by reference in lambdas that will be used non-locally, including returned, stored on the heap, or passed to another thread

* `Pointers and references` to locals `shouldn’t outlive their scope`. Lambdas that capture by reference are just another place to store a reference to a local object, and shouldn’t do so if they (or a copy) outlive the scope.
* If a non-local pointer must be captured, consider using `unique_ptr`; this handles both lifetime and synchronization.
* If the `this` pointer must be captured, consider using `[*this]` capture, which creates a copy of the entire object.

#### When writing a lambda that captures `this` or any class data member, don’t use `[=]` default capture

* Writing `[=]` in a member function appears to capture by value, but actually captures `data members` by `reference` because it actually captures the invisible `this` pointer by value. If you meant to do that, write `this` explicitly.
* If you intend to capture a `copy of all class data members`, consider C++17 `[*this]`.

#### Avoid unnecessary condition nesting

* Shallow nesting of conditions makes the code easier to follow.

* ```c++
  // Bad: Deep nesting
  void foo() {
      ...
      if (x) {
          computeImportantThings(x);
      }
  }
  
  // Bad: Still a redundant else.
  void foo() {
      ...
      if (!x) {
          return;
      }
      else {
          computeImportantThings(x);
      }
  }
  
  // Good: Early return, no redundant else
  void foo() {
      ...
      if (!x)
          return;
  
      computeImportantThings(x);
  }
  ```

* ```c++
  // Bad: Unnecessary nesting of conditions
  void foo() {
      ...
      if (x) {
          if (y) {
              computeImportantThings(x);
          }
      }
  }
  
  // Good: Merge conditions + return early
  void foo() {
      ...
      if (!(x && y))
          return;
  
      computeImportantThings(x);
  }
  ```

## For

### Std::ranges::for_each
```c++
for_each(v, [](int x) { /* do something with the value of x */ });
```

