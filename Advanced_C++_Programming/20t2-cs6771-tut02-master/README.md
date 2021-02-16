# Tutorial - Week 2

## Questions 0–3

Questions 0–3 are theory questions, and don't require you to do any programming (you might like to
experiment with Question 3).

### Question 0

Due to popular request, we've added [HOME_SETUP.md](HOME_SETUP.md), which will take you through how
to install things locally. You don't need to go through this if you're comfortable using VLAB.

### Question 1

1. What is a `TEST_CASE`?

> A `TEST_CASE` is a uniquely-named scope that has the context for our test framework, and will keep
  track of all the `CHECK`s and `REQUIRE`s that pass/fail.

2. What is a `CHECK`? How is it different to an `assert`?

> `CHECK` will evaluate an expression and report if it is `false`, but the program will continue to
  run.

3. What is a `REQUIRE`? How is this different to an `assert`?

> `REQUIRE` is closer to an `assert` than a check, in that it terminates the test case, but unlike
  an `assert`, it will gracefully exit the test case, rather than terminate on the spot. Subsequent
  test cases are still execute, so unlike an assert, it only aborts a portion of the program, rather
  than the whole program.

4. What are `SECTION` blocks in Catch2?

> `SECTION` blocks allow us to write setup code for our checks. Every `SECTION` will run all the
  code in the `SECTION` block that it's enclosed in, all the way back up to the `TEST_CASE`. This
  means that we can modify what we're testing in one section, and still get the same state we
  started in all following states at the same level!

5. How do they make testing easier?

> All of your setup is done once, which reduces the amount of code duplication that you need to do,
  the amount of `REQUIRE`s you need to do, which optimises for both the reader and the writer.

### Question 2

1. What is the relationship between algorithms, iterators, and ranges?

> It's first important to note that all the containers we've talked about in this course are ranges
> (as `std::stack` and `std::queue` exempt; they aren't containers, but rather container adaptors).
> The relationship between algorithms and ranges is expressed through iterators. Instead of
> implementing each algorithm per range --- which would be an explosion of maintenance and QoI
> nightmares --- we write the algorithm once and prescribe a common interface between anything that
> models the concept of a range.
>
> This "common interface" is the iterator model; a way of abstracting a range's internal layout, so
> that the algorithm can work with elements without being concerned if your range is a `std::vector`,
> a `std::map`, an `absl::flat_hash_set`, or a `ranges::iota_view`*! The way this is achieved is by
> requiring iterators conform to a uniform interface that erases the internal structure: each
> iterator has internal knowledge of the data structure it's associated with, but the algorithm is
> only privy to the position an iterator is at in the range, and the value of the element, if the
> position doesn't indicate that we've exhausted all of the elements.
>
> A range is denoted by an iterator to some starting element, and a sentinel that describes a rule
> for how that range ends. A type that conforms to this concept has a `begin` member function and
> an `end` member function, where `begin` returns an iterator to the first element, and `end`
> returns a sentinel to say that we've exhausted all of the elements. Containers' `end` member
> functions (e.g. `std::vector<int>::end`) return an iterator of the same type as their `begin`. We
> call these a _common range_.
>
> *`ranges::iota_view` is the type of `views::iota`.

2. Why is this relationship so important, and how does it help us as programmers?

> The relationship is important because it allows us to abstract away the details of the data
> structure so that we only need to write each algorithm once, instead of a combinatorial explosion
> of times.
>
> It also allows experts to focus on what they're good at. Someone might be really good at writing
> optimised algorithms, but not necessarily in writing memory-efficient data structures. They can
> focus on the algorithm without needing to worry about the data structure at all.

3. How does this relate to the DRY (don't repeat yourself) principle?

> It relates to the DRY principle in that we need to write _x_ range types + _y_ algorithms, instead
> of _x_ range tpyes * _y_ algorithms.

### Question 3

Which algorithms can we replace these raw bits of code with?

```cpp
auto first(std::vector<int> const& v, int const needle) -> std::vector<int>::iterator {
  for (auto i = v.begin(); i != v.end(); ++i) {
    if (*i == needle) {
      return i;
    }
  }
  return v.end();
}

// Note: this one wasn't covered in the lectures. See C++ Reference to get an idea of what it might
// be: https://en.cppreference.com/w/cpp/algorithm.
auto second(std::vector<int>& v, std::vector<int>::iterator new_first) -> std::vector<int>::const_iterator {
  auto copy = std::vector<int>(v.begin(), new_first);
  v.erase(v.begin(), new_first);
  return v.insert(v.end(), copy.begin(), copy.end());
}
```

> Answers:
> `first` is `ranges::find`.
> `second` is `ranges::rotate`.

## Questions 4 -- 8

For each of the questions below, you'll be asked to write a function or a program (i.e. also write a
`main` function). You should also write a test case for each of them in the corresponding files in
the `test` directory.

### Question 4

Write a function that sorts a vector of strings in _descending_ order.

You should write your algorithm in `source/sort_descending.cpp` and your test in
`test/sort_descending.cpp`.

> See `solutions/sort_descending.cpp`.

### Question 5

> This question is from _Cracking the Coding Interview (6th Edition)_, by Gayle Laakmann McDowell.

Implement an algorithm to determine if a string has all unique characters. What if you can't use
multiple data structures?

You should write two algorithms in `source/all_unique.cpp` and your tests in
`test/all_unique_test.cpp`. The first algorithm should use an additional data structure; the second
one should not.

### Question 6

> This question is from _Cracking the Coding Interview (6th Edition)_, by Gayle Laakmann McDowell.

Given two strings, write a function to determine if one string is a permutation of the other.

**Note: In the real world, you would use `ranges::is_permutation`. This question is intended to get
you some experience with data structures that you'll need to use in your Word Ladder assignment.**

You should write your algorithm in `source/is_permutation.cpp` and your tests in
`test/is_permutation_test.cpp`.

> See `solutions/is_permutation.cpp`.

### Question 7

Write a program that reads in integers from the character input, converts them to a string, and
then writes them to the character output in a list style.

```
$ # Example
$ build/source/int_to_string
Please enter a list of space-separated numbers (press Ctrl+D to finish).
1 2 3 4 5 6 7
[1, 2, 3, 4, 5, 6, 7]
$ build/source/int_to_string
Please enter a list of space-separated numbers (press Ctrl+D to finish).
1 2 3 4 a
Something went wrong, program exiting...
$
```

You should write your program in `source/int_to_string.cpp`, and your tests in
`test/int_to_string_test.cpp`.

> See solutions/int_to_string.cpp
