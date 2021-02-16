# Tutorial 10

This tutorial was written in 2019, before the use of the 2020 toolchain. Many of these files will not compile outright on the 2020 toolchain, so we recommend just compiling them directly with g++ and then running the binaries. This is OK, as this week's tutorial is mainly about understand conceptual ideas from week 9.

## Question 1

Look at `source/private.cpp`

Compile this with `g++ source/private.cpp -o private`

### Question 1.1

Why does this code not compile?

> You cannot access a private member from a derived class.
> See `solutions/private1.cpp`

### Question 1.2

The code will now compile, but what are the potential disadvantages of this solution? How would we fix it?

> It is generally considered bad practice to make data members anything other than private without good reason. Otherwise, any class inheriting off it would be relying off its implementation, rather than its interface. We should encapsulate members correctly.

> See `solutions/private2.cpp`
> This allows a subclass to only depend upon the interface of the base class, rather than the implementation. This means that if the base class chooses to refactor or modify the code, the subclass won't be affected.
> If we later want to add a setter, this gives us the power to do so, through either a non-const getter, or a manual setter.

## Question 2

Look at `source/type.cpp`

Compile this with `g++ source/type.cpp -o type`

### Question 2.1

For each of these variables, what is the static and dynamic type (for b, consider this both after and before assigning d to it)?

> The static type is the declared type of a variable. Hence, b has static type B, d has static type `door`, bref and dbref have static type `banana&`, and dref has static type `door&`.
> The dynamic type is the type a variable actually is at runtime.
> `b` and `d` are neither references or pointers. Hence, their dynamic type is **always** the same as their static type. Hence, `b` and `d` have dynamic types `banana` and `door` respectively.
> `bref`, `dref`, and `dbref` are references. Hence, their dynamic type is the dynamic type of the object they refer to. Hence, bref has dynamic type `banana&`, `dref` has dynamic type `door&`, and `dbref` has dynamic type `door&`.

### Question 2.2

The assignment *b = d;* is legal but is poor style as it causes the object slicing problem. What is the object slicing problem?

> When you try and assign a derived class into a variable of type base class, the base class only has sufficient space for the base class subobject. Hence, it copies over the base class subobject and completely ignores the derived class' data.

### Question 2.3

What is the output of this code?

> Since f is a virtual function, we use the dynamic type of objects to determine which function to call (if it was not, we would use the static types). Hence, we output "banana door banana door door".

### Question 2.4

How can we correct this code to prevent the object slicing problem?

> Instead of storing an object of fixed size, store an object that can have dynamic size (specifically, a pointer). Use a `std::unique_pointer<banana>` instead of a `banana` for the type of b.

## Question 3

Look at `source/vtable.cpp`.

Draw the vtables for classes A, B and C

> |A|B|C|
> |-|-|-|
> |A::f(int)|B::f(int)|B::f(int)|
> |A::g()|A::g()|A::g()|
> |~A()|~B()|~C()|
> |VTABLE END|B::h()|B::h()|
> ||VTABLE END|C::f(int, int)|
> |||C::x()|
> |||VTABLE END|

## Question 4

Look at `source/basic.cpp`

Compile this with `g++ source/basic.cpp -o basic`

* Work out on paper/whiteboard what the output of this program is.

> animal (fish member)
> animal (amphibian member)
> amphibian (fish member)
> fish

* Change the declaration of class fish to:
```cpp
class fish  : public amphibian {
```

Work out what the new output is.

> animal (amphibian member)
> amphibian (fish base class)
> animal (fish member)
> animal (amphibian member)
> amphibian (fish member)
> fish

Why are the outputs different?

> Each derived class has a base class subobject which must be initialised before initialising anything else in the class.

## Question 5

Look at `source/destructors.cpp`

Compile this with `g++ source/destructors.cpp -o destructors`

On paper/whiteboard work out the output of this program.

> ~bingo()
> ~yellow() (bingo member)
> ~apple() (base class)
> ~foxtrot() (A member)

> Note that this is just the reveres order that you would do a constructor in.


# Revision Questions

## Revision Question 1

Look at `source/constructors.cpp`

Compile this with `g++ source/constructors.cpp -o constructors`

On paper/whiteboard work out the output of this program.

> Pointer: ctor dtor
> Value: ctor copy-ctor dtor dtor

## Revision Question 2

A friend function of a class can access:
 * Only the public members of the clas
 * Only the public and protected members of the class
 * All members of the class
 * All members of the class and its base
 * Only the public and protected members of the class and its base

> All members of the class.

## Revision Question 3

Look at `source/revision.cpp`. Work out the output.

Compile this with `g++ source/revision.cpp -o revision`

> 1 2 1 1

## Revision Question 4

What are the differences (e.g., types, semantics, memory usage) between a T, a pointer to a T, and a reference to a T?

> |type|T|T*|T&|
> |----|-|--|--|
> |semantics|A value|An optional reference to a value|A reference to a value|
> |memory|sizeof(T)|sizeof(pointer)|sizeof(pointer)|

## Revision Question 5

Explain the differences in the meaning and effect of using *const* in the following member function prototypes:

```cpp
const T& get_value_at_index(int i);
T get_value_at_index(const int i);
T get_value_at_index(int i) const;
```

> The first returns a const reference. It returns a reference to the value stored in the container, but you can't modify the value through this reference.
> The second returns a copy of the value stored in the container. A const int parameter is no different to an int parameter.
> The third also returns a copy of the value stored in the container. The const here indicates that this is a const member function, and so does not modify the container.
