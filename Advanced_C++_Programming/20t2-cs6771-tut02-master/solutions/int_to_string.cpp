#include <iostream>
#include <absl/strings/str_cat.h>
#include <fmt/format.h>
#include <range/v3/action.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

#include "int_to_string.hpp"

// This is sort of a "grab bag" that exposes you to various content that you saw in Week 2 lectures.
auto int_to_string(std::vector<int> const& ints) -> std::string {
	if (ints.empty()) {
		return "[]";
	}

	// When working with range pipelines, it's often a good idea to name your lambda expressions, so
	// that your pipeline reads more naturally.
	auto append_comma = [](int const x) {
		// Putting lambda expressions on a single line is neat, but it also makes getting the debugger
		// to break inside the lambda difficult. If you need to set a breakpoint inside a lambda, you
		// should make the lambda's body span multiple lines, even if it's only a one liner.

		// `fmt::format` should feel a similar to `sprintf` from C, but it's actually a lot closer to
		// string formatting in Python. We use `{}` to act as a placeholder for what we're planning to
		// convert to a string. Unlike `sprintf`, this is type-checked at compile-time, meaning that
		// if your type can't be formatted (e.g. a random struct you made without providing the
		// necessary hooks), the compiler will let you know. Week 3 will show you one of the two
		// hooks, and either Week 7 or Week 10 will show you the other.
		//
		// It's also much simpler to use *by default*, since unless you need advanced formatting
		// techniques, there's exactly one format specifier you need to remember: `{}`. We won't be
		// covering any of the advanced formatting techniques in this class.
		//
		// Finally, unlike `sprintf`, which takes a pointer-to-`char` array as an input (and returns
		// an `int` to indicate how many bytes were written), `fmt::format` will return the string
		// that you're formatting, which makes it much more natural to use.
		//
		// If you're interested in learning more about {fmt}, the documentation can be found at
		// https://fmt.dev/latest/index.html.
		return fmt::format("{}, ", x);

		// If this were a more complex string (i.e. we had multiple things to format), then we would
		// use named arguments to minimise confusion. This will likely feel at-home to Pythoneers,
		// but somewhat foreign to C programmers. Note that `{x}` is used twice, but only specified
		// once.
		// ```
		// return fmt::format("{x}, {y}, {x}", fmt::arg("x", x), fmt::arg("y", y));
		// ```
	};

	// Here, we form a range pipeline. `intermediate` builds up a description of a computation we
	// intend to have happen, but haven't actioned yet. This is called lazy computation. If you're
	// strucggling to wrap your head around "lazy computation" and "description of what we intend to
	// have happen", you can think of it like this:
	//        Say we have a vector of ints, and our ultimate goal is to print them to screen. Before
	//        we call a function to action the printing, the vector is just sitting around, occupying
	//        memory, but it's not actually *doing* anything; it's just a description of how we
	//        intend to store our integers. Lazy computation takes this a bit further, and means that
	//        build up a description of some or all of the work we'd like to do, but don't do any of
	//        it until a specific command. It's just saying "we'd like to do this at some point in"
	//        the future.
	//
	// The "pipe" operator is used to pass information from one part of our description to the next,
	// and allows us to "naturally" read the operation. This is called "reactive programming". If we
	// were to adopt the more traditional functional approach, this is how we'd have to format the
	// initialisation of intermediate:
	//
	//        auto intermediate_part1 =
	//            views::common(views::transform(views::drop_last(ints, 1), append_comma));
	//        auto const intermediate = std::vector<int>(intermediate_part1.begin(),
	//                                                   intermediate_part1.end());
	//
	// While we can do that, it's both more difficult to read (as we need to read it inside out), and
	// it needs to be done in two steps (since we can't directly construct a vector from an arbitrary
	// range expression). The "pipe" operator allows us to say "and then":
	//
	//        Take our vector of ints,
	//        and then discard the last element,
	//        and then transform the int into a string with a comma appended,
	//        and then construct a vector out of the preceding range.
	//
	// Note that while views are lazy, `ranges::to<std::vector>` (the thing that "constructs a
	// vector out of the preceding range") is not. This is our _action_ "command".
	namespace views = ranges::views;
	auto const intermediate = ints
	                        | views::drop_last(1)
	                        | views::transform(append_comma)
	                        | ranges::to<std::vector>;

	// For very technical reasons, we can't use certain views like `views::join` on something that
	// returns a temporary object. A temporary object is one that hasn't been given a name (i.e.
	// isn't a variable or constant, but still exists in memory). We'll expand more on temporary
	// objects in Week 5.
	//
	// In the above declaration, there are two lines that return temporaries. The first is
	// `views::transform(append_comma)`, which returns a temporary `std::string` object. This means
	// that we can't put any views after it, and need to store it in an intermediate variable (hence
	// why we've called that constant `intermediate`).
	// The second one is `ranges::to<std::vector>`. That also can't be followed by a view, but it
	// *can* be placed after one that returns a temporary because of how it's designed. If you're
	// wondering how to solve this problem using a single pipeline, swing by in Week 10, where we'll
	// explore the solution in a *non-assessable* lecture.
	//
	// `intermediate` is a vector of strings. The question the interface we were given asks us to
	// return a string, so we need to somehow flatten that vector of strings into just a single
	// string. Since a vector of strings is a range-of-ranges, we can use views::join to take that
	// range-of-ranges and give us back a flattened range. Or, as a "poem":
	//
	//        Take our intermediate vector of strings,
	//        and join them into a single range,
	//        and then construct a string out of the flattened vector of strings.
	auto result = intermediate | views::join | ranges::to<std::string>;

	// Remember that we discarded the last int when we were building up `intermediate`. The reason
	// that we did this is so we don't have a trailing comma in our list (i.e. we don't want to
	// see "[1, 2, 3, 4,]"). It's now time that we add that final int into our string.
	//
	// We never modified the actual `ints` before, so it still has that last element. `drop_last`
	// just means "don't consdier this element during processing". Let's now grab it, and append
	// it to our string of ints by converting it to a string as well.
	return result.append(fmt::format("[{}]", ints.back()));
}
