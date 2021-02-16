# Tutorial 8

## Question 0

### VLAB/Remote SSH

Run the following command.

```bash
unzip ~cs6771/vcpkg-ass3/vcpkg-export-20200721-161758.zip -d `pwd`/../
```

### At home

**Note: you will need to update your vcpkg library to complete this tutorial. If you're cloning a
new instance of vcpkg for each tutorial, you can disregard Question 0.**

1. Navigate to your vcpkg directory.
2. Run `git pull`.
3. Run `./bootstrap-vcpkg -disableMetrics`.
4. Run `./vcpkg update`.
5. Run `./vcpkg upgrade`.
6. Run `./vcpkg upgrade --no-dry-run`.
7. The update process has completed, you can proceed to Question 1.

## Question 1

What is iterator invalidation? Why does it occur?

> Iterator invalidation occurs when a mutable operation changes the state of a container in such a
> way that any previous iterators for the container are no longer valid. This may occur simply
> because the object no longer exists (eg. `std::set::erase`), because the iterator was stored
> internally as a pointer and the object has been moved (eg. `std::vector`), or for some other
> reason.

## Question 2

Assignment 3 involves abstracting a container of containers as a single container. Consider a simple
example of `std::vector<std::vector<int>>`. Abstracting the container requires storing the outer
iterator and the inner iterator. Discuss potential issues with this idea, and once you think you've
discovered all of them, discuss how you might solve these issues.

> * Comparing end iterators (no inner iterator exists, and trying to access one is undefined)
> * ++ operator on an empty vector in the middle
> * begin() with an empty vector at the start
> * Solution: Store a sentinel value (you need to know when it's the end, so you can know when not
>            to check the inner iterator).

## Question 3

Look at `source/rope_user.cpp` and `source/rope.hpp`

We have defined a rope class as a bunch of strings tied together, and have defined a basic class and
some starter code using it.

1. Write an input iterator for this class so that you can uncomment lines 65-72. You can use the
   `static_assert`s that we provide to check you're on the right path.

> See `rope_q31.hpp` and `rope_user_q31.cpp`.

2. Discuss two ways we might modify our code to allow both a const and a non-const iterator. What
   are the advantages and disadvantages for each?

> The first, and most naïve way is to create an `iterator` type and a `const_iterator` type. This
> will yield simple and easy to read code, but at the cost of near-100% duplication. Any changes to
> `iterator` will yield a change to `const_iterator`, and the testing will also need to be
> completely duplicated.
>
> The second, and more robust way, is to parameterise the constness of the iterator. This minimises
> the amount of code duplication, but at a minor cognitive burden (we need to remember to do this
> for every iterator that we write).
>
> The third, and most robust way, is to implement an iterator adaptor similar to
> `std::reverse_iterator`. Then, we can write a single `iterator` implementation and declare our
> `const_iterator` as an alias for `const_iterator<iterator>`. The major advantage of this is that
> the cognitive burden imposed from the second solution is lifted with almost no associated cost for
> any individual iterator: the cost is largely incurred by the iterator adaptor author, who needs to
> design, implement, and test the iterator. Unfortunately, there are no major libraries known to
> directly support this, so you'd need to write the adaptor yourself (or [propose][1] it to a major
> library).

3. Modify your iterator so that it models both `std::bidirectional_iterator` and
   `std::output_iterator<rope::iterator, char>`.

> See `rope_q33.hpp` and `rope_user_q33.cpp`.

## Question 4

Look at `source/silly_set.cpp`

1. Complete this `silly_set` wrapper class (yes, it's a bit redundant).

* Implement the member functions
* Template the container type and set set to be the default container

> See `solutions/silly_set_q41.cpp`

Use template template parameters to avoid the case where we can accidentally define
`silly_set<int, std::vector<float>>`.

> See `solutions/silly_set_q42.cpp`

## Question 5 (Question 3 extension)

Prioritise Question 4 over the Question 5, but if you have time, try this out for a little fun.

1. **(Difficulty: medium-hard)** Can this iterator be designed to model
   `std::random_access_iterator`? Justify your answer. (Note: if it can, you _do not_ need to
   implement it for the tutorial.)

> While this type could permit a random-access iterator, the current interface and internal
> representation don't provide sufficient guarantees to for an iterator to model
> `std::random_access_iterator`.
>
> A good way to summarise a random-access iterator is that it is a bidirectional iterator that can
> move arbitrary distances in _constant_ time, with respect to the total number of elements in the
> range. Since our interface passes an unadultered vector of strings to the internal representation,
> it's entirely possible that a `rope` could be constructed using a vector where each of its
> elements is exactly one character in length. This would mean that the number of elements is
> proportional to the number of strings, and any random-access operations would happen in linear
> time, which breaks the concept's axioms (semantic, plaintext requirements).
>
> There are a few ways in which we could change our `rope` type to make modelling the concept of a
> random-access iterator possible. One way would be for our internal representation to also keep
> track of the size of each string. Then, any calculations that we need to make when defining our
> operations would use this second vector as a lookup table.
>
> Another way is to curate the input data so that it fits in a different internal representation.
> We could maintain a sequence of strings that have a predefined and fixed size; any strings that
> are longer than that will be split up and put into multiple strings. This would involve a lot of
> work in the constructor, but it would simplify the internal representation than the first solution
> (note: a simplified representation doesn't mean that it's faster or smaller). This second solution
> approaches the implementation of `std::deque<char>`, so it'd be better again to use that, given
> it's maintained by industry experts.

2. **(Difficulty: medium-hard)** _Assuming_ we can make a random-access iterator, can this iterator
   also be designed to model `std::contiguous_iterator`? Justify your answer. (Note: if it can, you
   _do not_ need to implement it for the tutorial.)

> Regardless of how you interpret 5.1, it isn't possible for `rope::iterator` to model
> `std::contiguous_iterator`.
>
> A contiguous iterator is a random-access iterator that syntactically resembles a pointer, and
> representationally resembles an array: that is, each element is packed in memory inbetween its
> predecessor and successor. Because we maintain a vector of heap-allocated objects, this
> container's representation isn't contiguous, and thus its iterator cannot model
> `std::contiguous_iterator`.

3. **(Difficulty: hard)** Question 3.4 asked you to change `rope::iterator` from being a read-only
   iterator into a mutable iterator. Introduce a `const_iterator` type that is read-only.

> See `solutions/rope_q5.hpp`, lines 9-88.

4. **(Difficulty: easy)** What changes need to be made to `rope` so that calling `rope::begin` and
   `rope::end` on a constant `rope` gives us back a `const_iterator`?

> See `solutions/rope_q5.hpp`, lines 91-128.
>
> Your tutor might mention that it's a good idea to add `rope::cbegin` and `rope::cend`. While these
> are not strictly required, they can be considered handy by many programmers. `ranges::cbegin` and
> `ranges::cend` will construct a `const_iterator` for you, so we already have a way to get these
> iterators, and we don't need to add additional boilerplate to our types. The same is true for
> `rope::rbegin`, `rope::rend`, `rope::crbegin`, and `rope::crend`. This decision is ultimately up
> to you, but the free functions reduce the number of member functions you need to maintain from
> twelve to four.
