# Tutorial 3

## Question 1

How might we use a lambda function in the following example to sort vec by the number of vowels in each string? If the number of vowels is equal, then sort by the number of consonants.

```cpp
std::vector<std::string> vec{"We", "love", "lambda", "functions"};
```

Implement it in `tests/lambda_string.cpp`

> See `solutions/lambda_string.cpp`

## Question 1

When writing a lambda function, when would you capture by value, and when would you capture by reference?

> * Capture by value when the variable may change, and you want the variable at the current time
> * Capture by value when the lifetime of the lambda may exceed the lifetime of the variable
> * Capture by reference when the variable may change, and you want to stay updated
> * Capture by reference when the variable is unable to be copied, or expensive to copy

## Question 3	

This task focuses on using standard algorithms to read a list of newline-seperated words from a file (try /usr/share/dict/words or /usr/dict/words) into a vector (hint: see std::istream_iterator).

> See `solutions/dict.cpp` and `solutions/dict_test.cpp`

### Question 3.1

In `source/dict.cpp`, write a function that takes in the word list as a stream, and outputs a vector of strings that are the words.

### Question 3.2

In `source/dict.cpp`, write a function that uses standard algorithms to split the string into words, filtered to only words that are in the word dict, and reconstruct this into a string (hint: see std::istringstream, std::istream_iterator, std::copy_if, std::ostringstream, and std::ostream_iterator)

### Question 3.3

Add your own tests to `test/dict.cpp`. This file has not been created, so you will have to create it yourself and add the appropriate line to `CMakeLists.txt`.

### Question 3.4

Discuss why separating your functions you want to test is a good idea.

> You can't test any file with a main function in it. Additionally, testing is much easier when you have several small parts to test.

### Question 3.5

Assume now that the word list and strings are both very large. Discuss how we could make this code run much faster (hint: a different data structure may be required. Tutors, students should know the data type, but not what it is called in C++)
    
> A std::set would be faster. std::binary_search on a std::vector which was in sorted order would be faster still. std::unordered_set would be fastest.

### Question 3.6

Discuss the effect the use of automatic type deduction (through the use of auto keyword, and by not having to declare types at all when calling functions) on the quantity of code you had to change, and the depth of the testing required.

> When you change the type of the function, only the things that specifically declared their type had to be changed. Anything that declared itself as auto was able to use the same constructors.

## Question 4

Open `source/car.h` and `source/car.cpp`

### Question 4.1

Create a constructor for the car that takes in the manufacturer name (e.g. Toyota) and the number of seats. Ensure that your constructor uses a member initializer list and uniform initialisation. Why is it important to use a member initializer list? Why is uniform initialisation preferred since C++11?

> Initialiser lists and uniform initialisation avoid having to construct an object once, and then reassign it to a different value after construction. It is more efficient, and for some types, you will not be able to compile without it.

### Question 4.2

Create a default constructor that delegates to the previous constructor using the values of "unknown" and 4

Create const member functions to get the manufacturer and number of seats. What does it mean for a class or function to be const correct?

> A const-correct class provides const member functions when no state is changed inside a function, and provides both const and non-const overloads for when a function returns a reference to a data member that the user may be able to modify.

### Question 4.3

Create a static data member to keep count of the number of car objects created. Modify your constructors to ensure that the count increases when a new object is created. Do you need to increase the object count in your delegating constructor?

### Question 4.4

Ensure that your static object count is initialised to 0, where should you do this, in the header file or the cpp file?

### Question 4.5

Create a static function to return the object count. What does it mean for an function or data member to be static? Is the static data member part of the object or the class?

### Question 4.6

Create a destructor that decreases the object count when an object is destroyed

### Question 4.7

Test your code extensively by adding tests to the test directory, and adjusting the `CMakeLists.txt` file accordingly.

Make sure you keep your code - this example will continue next week.

> See `solutions/car.cpp` and `solutions/car.h` and `solutions/car_test.cpp`