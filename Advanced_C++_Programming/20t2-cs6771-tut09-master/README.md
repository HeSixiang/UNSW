# Tutorial 9

## Question 1

When would we specialise classes? Why do we not specialise functions?

> * We do not specialise functions because: 1)
Specialising functions can have unintended behaviour - see lecture notes for more detail, 2) We already have this capability in the form of function overloading
> * We specialise classes when we need to either:
> * preserve a particular semantic for something that would not work otherwise
> * make an optimisation for a specific type (e.g. case of `std::vector<bool>` having a huge space optimisation.)

## Question 2

In `source/is_a_pointer.h`, define your own type trait, from scratch, for is_a_pointer, to be used in an application like this

```cpp
template <typename T>
printPointer(std::ostream& os, T t) {
  if constexpr (traits::is_a_pointer<T>::value) {
  	os << *t << "\n";
  } else {
  	os << t << "\n";
  }
}
```

Ensure that your type trait is wrapped in a trait namespace.

Write a test for your type trait in `test/is_a_pointer.cpp`

> See `solutions/is_a_pointer.h`
> See `solutions/is_a_pointer_test.cpp`

## Question 3

In `source/is_real_number.h`, use type traits in the std namespace to produce your own composition type trait that returns true if the type passed in is an integer or floating point. It should be used as follows

```cpp
template <typename T>
if (is_real_number<T>::value) {
  std::cout << "Is real number" << "\n";
}
```

> See `solutions/is_real_number.h`

## Question 4

Look at `include/streaming_median.h`.

This is a simple, inefficient class which calculates the current median (middle element) for a stream of data. It has the limitation of only working on odd sized streams.

See `source/use_streaming_median.cpp` for an example of usage.

### Part 1
`streaming_median` currently allows the user to use any (ordered) type. This means that the general implementation of `median` doesn't work on even sized streams, as for many types, such as `std::string`, there isn't an obvious way to calculate the midpoint between two values. For example:

```c++
["c++", "javascript", "kotlin", "R"] // What is the median of this data?
```

However, the median is well-defined for arithmetic types (`int`, `float`, `double`, etc):

```c++
[1.0, 2.0, 3.0, 4.0] // The median is (2.0 + 3.0) / 2.0 = 2.5.
```

Discuss some different approaches to allow `streaming_median` to work for even sized streams of arithmetic types.

> One approach is to provide a specialization of `streaming_median` for arithmetic types. This approach is implemented in `streaming_median_specialized.h`. However, this can result in some code duplication.

> Another approach is to modify the existing implementation of `median()` to add a compile time check for whether the template type parameter `T` is an arithmetic type, using the type trait `std::is_arithmetic<T>`. If it's the case, then we can return the average of the two middle values. This avoids code duplication, although it arguably makes the implementation of `median()` more complex.

> Another approach is to compile different implementations of `median()` depending on whether the template type parameter `T` satisfies `std::is_arithmetic<T>`. With C++20 Constraints and Concepts, this can be done using the `requires` keyword, as shown in `streaming_median_concepts.h`. In older versions of C++, a similar effect can be achieved using `std::enable_if`, as shown in `streaming_median_enable_if.h`.

### Part 2
Modify the existing `streaming_median` class to add a compile time check for whether the parameterized type is arithmetic, to provide an implementation of `median()` which works for evenly sized streams of arithmetic type.

> For this part and part 3, see `streaming_median.h`.

### Part 3
The current solution allows a user to instantiate a `streaming_median<T>` where `T` is a type which doesn't have a valid `operator<`, and thus cannot be ordered properly.

Only when calling `.add()` will the program fail to compile. For example:

```c++
class my_class {}; // my_class doesn't have operator< or operator<=> defined.

auto invalid_type_sm = streaming_median<my_class>{}; // Compiles.
invalid_type_sm.add(my_class{}); // Doesn't compile. Can also produce a somewhat complex compiler diagnostic.
```

Using a `requires` statement with the concept `concepts::totally_ordered`, prevent a user from mistakely instantiating a `streaming_median` with a type which is invalid.

> See `streaming_median.h`.

> Note: Using `concepts::totally_ordered` prevents the arguably valid use case of a class with `operator<` defined, but not the other relational operators.

## Question 5

Look at `source/decltype.cpp`.

What are the inferred types for each of the following?

> See `solutions/decltype.cpp`

## Question 6

What does the binding table for lvalues/rvalues look like?

> |type|lvalue|const lvalue|rvalue|const rvalue|
> |----|------|------------|------|------------|
> |template T&&|True|True|True|True|
> |T&|True|False|False|False|
> |const T&|True|True|True|True|
> |T&&|False|False|True|False|

## Question 7

Look at `source/forward.cpp`

This code currently doesn't work as the implementation for my_make_unique is incomplete. Complete it through the addition of using std::forward as well as variadic types. To compile with this code, you will need to use types.h which can be found <a href="https://github.com/cs6771/comp6771/blob/master/lectures/week8/forwarding/types.h">HERE</a> in the github.

> See `solutions/forward.cpp`