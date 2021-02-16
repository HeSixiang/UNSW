#include "gdwg/graph.hpp"
#include <catch2/catch.hpp>
#include <cstring>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <range/v3/utility/common_tuple.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

static_assert(ranges::bidirectional_iterator<gdwg::graph<int, int>::iterator>);
static_assert(ranges::bidirectional_range<gdwg::graph<int, int>>);
static_assert(ranges::forward_iterator<gdwg::graph<int, int>::iterator>);

TEST_CASE("Comparisons") {
	using value_type = gdwg::graph<std::string, int>::value_type;
	auto const v = std::vector<value_type>{
	   {"A", "B", 1},
	   {"A", "C", 1},
	   {"B", "C", 3},
	   {"C", "A", 2},
	};
	auto const v2 = std::vector<value_type>{
	   {"A", "B", 4},
	   {"A", "C", 1},
	   {"B", "C", 3},
	   {"C", "A", 2},
	};
	auto g = gdwg::graph<std::string, int>{v.begin(), v.end()};
	auto g_empty = gdwg::graph<std::string, int>{};
	auto g_same = gdwg::graph<std::string, int>{v.begin(), v.end()};
	auto g_diff = gdwg::graph<std::string, int>{v2.begin(), v2.end()};
	CHECK(g == g_same);
	CHECK(!(g == g_empty));
	CHECK(!(g == g_diff));
}

TEST_CASE("Constructors and Extractor") {
	SECTION("Default Constructor") {
		auto g1 = gdwg::graph<std::string, int>();
		gdwg::graph<std::string, int> g2;
		auto* g3 = new gdwg::graph<std::string, int>();
		g1.insert_node("A");
		CHECK(g1.get_node_ref_count() == std::map<std::string, int>{{"A", 2}});
		CHECK(g1.get_weight_ref_count().empty());
		CHECK(fmt::format("{}", g1) == "A (\n)\n");
		g2.insert_node("A");
		g3->insert_node("B");
		CHECK(g1 == g2);
		CHECK(!(g2 == *g3));
		CHECK(!(g1 == *g3));
		delete g3;
	}

	SECTION("Initializer_list and Iterator") {
		auto v1 = std::vector<std::string>{"A", "B", "C"};
		auto g1 = gdwg::graph<std::string, int>{v1.begin(), v1.end()};
		auto g2 = gdwg::graph<std::string, int>{"A", "B", "C"};
		CHECK(g1.get_node_ref_count() == std::map<std::string, int>{{"A", 2}, {"B", 2}, {"C", 2}});
		CHECK(g1.get_weight_ref_count().empty());
		CHECK(fmt::format("{}", g1) == "A (\n)\nB (\n)\nC (\n)\n");
		CHECK(g1 == g2);
	}

	SECTION("Extractor and Iterator(value_type)") {
		using value_type = gdwg::graph<int, int>::value_type;
		auto const v = std::vector<value_type>{
		   {4, 1, -4},
		   {4, 1, -4},
		   {3, 2, 2},
		   {2, 4, 2},
		   {2, 1, 1},
		   {6, 2, 5},
		   {6, 3, 10},
		   {1, 5, -1},
		   {3, 6, -8},
		   {4, 5, 3},
		   {5, 2, 7},
		   {6, 3, 10},
		};
		auto g = gdwg::graph<int, int>(v.begin(), v.end());
		g.insert_node(64);
		auto out = std::ostringstream{};
		out << g;
		auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
		CHECK(out.str() == expected_output);
	}

	SECTION("Move") {
		using value_type = gdwg::graph<std::string, int>::value_type;
		auto const v = std::vector<value_type>{
		   {"A", "B", 1},
		   {"A", "C", 1},
		   {"B", "C", 3},
		   {"C", "A", 2},
		};
		auto other = gdwg::graph<std::string, int>(v.begin(), v.end());
		auto it_other_begin = other.begin();
		auto it_other_last = --other.end();
		CHECK(*(it_other_begin) == ranges::make_common_tuple("A", "B", 1));
		CHECK(*(it_other_last) == ranges::make_common_tuple("C", "A", 2));
		auto expect_result = gdwg::graph<std::string, int>(v.begin(), v.end());
		auto g_move = gdwg::graph<std::string, int>(std::move(other));
		// *this is equal to the value other had before this constructor's invocation
		CHECK(g_move == expect_result);
		// other.empty() is true.
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(other.empty());
		// All iterators pointing to elements owned by other prior to this operator's
		// invocation remain valid, but now point to the elements owned by *this.
		CHECK(*(it_other_begin) == ranges::make_common_tuple("A", "B", 1));
		CHECK(*(it_other_last) == ranges::make_common_tuple("C", "A", 2));

		auto g_assign_move = gdwg::graph<std::string, int>{"D", "E"};
		g_assign_move = std::move(g_move);

		// other.empty() is true.
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(g_move.empty());
		// All iterators pointing to elements owned by other prior to this operator's
		// invocation remain valid, but now point to the elements owned by *this.
		CHECK(*(it_other_begin) == ranges::make_common_tuple("A", "B", 1));
		CHECK(*(it_other_last) == ranges::make_common_tuple("C", "A", 2));

		// check the inside
		CHECK(g_assign_move.get_node_ref_count()
		      == std::map<std::string, int>{{"A", 3}, {"B", 3}, {"C", 4}});
		CHECK(g_assign_move.get_weight_ref_count() == std::map<int, int>{{1, 3}, {2, 2}, {3, 2}});
	}

	SECTION("Copy") {
		using value_type = gdwg::graph<std::string, int>::value_type;
		auto const v = std::vector<value_type>{
		   {"A", "B", 1},
		   {"A", "C", 1},
		   {"B", "C", 3},
		   {"C", "A", 2},
		};
		auto other = gdwg::graph<std::string, int>(v.begin(), v.end());
		auto g_copy = gdwg::graph<std::string, int>(other);
		auto g_assign_copy = gdwg::graph<std::string, int>{"D", "E"};
		g_assign_copy = g_copy;
		CHECK(other == g_copy);
		CHECK(g_copy == g_assign_copy);
	}
}

TEST_CASE("Accessors") {
	using value_type = gdwg::graph<std::string, int>::value_type;
	auto const v = std::vector<value_type>{
	   {"A", "B", 1},
	   {"A", "B", 2},
	   {"A", "C", 2},
	   {"B", "C", 2},
	   {"B", "C", 3},
	   {"C", "A", 2},
	};

	auto const expect_node = std::vector<std::string>{"A", "B", "C"};
	auto const expect_weight = std::vector<int>{1, 2};
	auto const expect_connections = std::vector<std::string>{"B", "C"};

	auto g2 = gdwg::graph<std::string, int>(v.begin(), v.end());
	g2.clear();
	CHECK(g2.empty());

	auto const g = gdwg::graph<std::string, int>(v.begin(), v.end());
	CHECK(g.is_node("A"));
	CHECK(!g.is_node("E"));

	CHECK_THROWS_MATCHES(g.is_connected("E", "A"),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if "
	                                              "src or dst node don't exist in the graph"));
	CHECK(!g.is_connected("C", "B"));
	CHECK(g.is_connected("A", "B"));

	CHECK(g.nodes() == expect_node);

	CHECK_THROWS_MATCHES(g.weights("E", "A"),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src or "
	                                              "dst node don't exist in the graph"));
	CHECK(g.weights("A", "B") == expect_weight);

	CHECK(g.find("C", "A", 2) == --g.end());
	CHECK(g.find("E", "A", 2) == g.end());
	CHECK(g.find("C", "A", 3) == g.end());

	CHECK_THROWS_MATCHES(g.connections("E"),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if "
	                                              "src doesn't exist in the graph"));
	CHECK(g.connections("A") == expect_connections);
}

TEST_CASE("Modifiers and Iterator") {
	using value_type = gdwg::graph<std::string, int>::value_type;
	SECTION("Insert") {
		auto const v = std::vector<value_type>{
		   {"A", "B", 1},
		   {"A", "C", 1},
		   {"B", "C", 3},
		   {"C", "A", 2},
		};
		auto expect_g = gdwg::graph<std::string, int>(v.begin(), v.end());

		auto g = gdwg::graph<std::string, int>();

		CHECK(g.insert_node("A") == true);
		CHECK(g.insert_node("A") == false);
		CHECK(g.insert_node("B") == true);
		CHECK(g.insert_edge("A", "B", 1) == true);
		CHECK(g.insert_edge("A", "B", 1) == false);
		CHECK_THROWS_MATCHES((g.insert_edge("A", "C", 1)),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge "
		                                              "when "
		                                              "either src or dst node does not exist"));
		CHECK(g.insert_node("C") == true);
		CHECK(g.insert_edge("A", "C", 1) == true);
		CHECK(g.insert_edge("C", "A", 2) == true);
		CHECK(g.insert_edge("B", "C", 3) == true);
		CHECK(g == expect_g);

		// check the inside
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"A", 3}, {"B", 3}, {"C", 4}});
		CHECK(g.get_weight_ref_count() == std::map<int, int>{{1, 3}, {2, 2}, {3, 2}});
	}

	SECTION("Iterator") {
		//  A   B   C   D   E
		//     (AB)     A
		auto const v = std::vector<value_type>{
		   {"B", "A", 1},
		   {"B", "B", 2},
		   {"D", "A", 2},
		};
		auto g = gdwg::graph<std::string, int>(v.begin(), v.end());
		auto g_copy = g;
		g.insert_node("C");
		g.insert_node("E");

		auto it = g.begin();
		auto it_next = ++it;
		--it;
		CHECK(*(it++) == ranges::make_common_tuple("B", "A", 1));
		CHECK(*(it) == ranges::make_common_tuple("B", "B", 2));
		CHECK(*(++it) == ranges::make_common_tuple("D", "A", 2));
		CHECK(++it == g.end());
		CHECK(*(--it) == ranges::make_common_tuple("D", "A", 2));
		CHECK(*(it--) == ranges::make_common_tuple("D", "A", 2));
		CHECK(*it == ranges::make_common_tuple("B", "B", 2));
		CHECK(*(--it) == ranges::make_common_tuple("B", "A", 1));
		CHECK(it == g.begin());
		CHECK(!(it == g_copy.begin()));
		CHECK(!(++it == ++g_copy.begin()));
		CHECK(it++ == it_next);
		CHECK(!(it == it_next));
		CHECK(g.get_node_ref_count()
		      == std::map<std::string, int>{{"A", 4}, {"B", 3}, {"C", 2}, {"D", 2}, {"E", 2}});
		CHECK(g.get_weight_ref_count() == std::map<int, int>{{1, 2}, {2, 3}});
	}

	SECTION("Erase and Clear") {
		auto const v = std::vector<value_type>{{"A", "B", 1},
		                                       {"A", "B", 2},
		                                       {"A", "C", 1},
		                                       {"A", "C", 5},
		                                       {"B", "C", 3},
		                                       {"B", "A", 3},
		                                       {"C", "A", 2},
		                                       {"C", "A", 1}};
		auto g = gdwg::graph<std::string, int>(v.begin(), v.end());
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"A", 5}, {"B", 4}, {"C", 5}});
		CHECK(g.get_weight_ref_count() == std::map<int, int>{{1, 4}, {2, 3}, {3, 3}, {5, 2}});
		CHECK(g.erase_node("A"));
		auto const v_expect1 = std::vector<value_type>{
		   {"B", "C", 3},
		};
		CHECK(g == gdwg::graph<std::string, int>(v_expect1.begin(), v_expect1.end()));
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"B", 2}, {"C", 3}});
		CHECK(g.get_weight_ref_count() == std::map<int, int>{{3, 2}});
		CHECK(g.erase_node("C"));
		CHECK(g == gdwg::graph<std::string, int>{"B"});
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"B", 2}});
		CHECK(g.get_weight_ref_count().empty());
		CHECK(!g.erase_node("C"));

		g = gdwg::graph<std::string, int>(v.begin(), v.end());
		auto const v_expect2 = std::vector<value_type>{{"A", "B", 1},
		                                               {"A", "C", 1},
		                                               {"B", "C", 3},
		                                               {"C", "A", 2},
		                                               {"C", "A", 1}};
		CHECK(g.erase_edge("A", "B", 2));
		CHECK(g.erase_edge("A", "C", 5));
		CHECK(g.erase_edge("B", "A", 3));
		CHECK(!g.erase_edge("B", "A", 3));
		CHECK_THROWS_MATCHES((g.erase_edge("A", "E", 1)),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on "
		                                              "src or dst if they don't exist in the graph"));
		CHECK(g == gdwg::graph<std::string, int>(v_expect2.begin(), v_expect2.end()));
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"A", 4}, {"B", 3}, {"C", 4}});
		CHECK(g.get_weight_ref_count() == std::map<int, int>{{1, 4}, {2, 2}, {3, 2}});

		g = gdwg::graph<std::string, int>(v.begin(), v.end());
		auto it = g.begin();
		while (it != g.end()) {
			it = g.erase_edge(it);
		}
		CHECK(g == gdwg::graph<std::string, int>{"A", "B", "C"});
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"A", 2}, {"B", 2}, {"C", 2}});
		CHECK(g.get_weight_ref_count().empty());

		g = gdwg::graph<std::string, int>(v.begin(), v.end());
		CHECK(*(--g.end()) == ranges::make_common_tuple("C", "A", 2));
		it = g.erase_edge(++g.begin(), --g.end());
		auto const v_expect3 = std::vector<value_type>{
		   {"A", "B", 1},
		   {"C", "A", 2},
		};
		CHECK(*(it) == ranges::make_common_tuple("C", "A", 2));
		CHECK(g == gdwg::graph<std::string, int>(v_expect3.begin(), v_expect3.end()));
		CHECK(g.get_node_ref_count() == std::map<std::string, int>{{"A", 3}, {"B", 3}, {"C", 2}});
		CHECK(g.get_weight_ref_count() == std::map<int, int>{{1, 2}, {2, 2}});

		g = gdwg::graph<std::string, int>(v.begin(), v.end());
		g.clear();
		CHECK(g.empty());
	}

	SECTION("Replace and Merge") {
		auto const v1 = std::vector<value_type>{
		   {"A", "B", 3},
		   {"C", "B", 2},
		   {"D", "B", 4},
		};

		auto const v2 = std::vector<value_type>{
		   {"B", "A", 3},
		   {"B", "C", 2},
		   {"B", "D", 4},
		};

		auto const v3 = std::vector<value_type>{
		   {"A", "B", 1},
		   {"A", "C", 2},
		   {"A", "D", 3},
		};

		auto const v4 = std::vector<value_type>{
		   {"A", "B", 1},
		   {"A", "C", 2},
		   {"A", "D", 3},
		   {"B", "B", 1},
		};

		auto const expect1 = std::vector<value_type>{
		   {"A", "E", 3},
		   {"C", "E", 2},
		   {"D", "E", 4},
		};

		auto const expect2 = std::vector<value_type>{
		   {"A", "A", 3},
		   {"C", "A", 2},
		   {"D", "A", 4},
		};

		auto const expect3 = std::vector<value_type>{
		   {"A", "A", 3},
		   {"A", "C", 2},
		   {"A", "D", 4},
		};

		auto const expect4 = std::vector<value_type>{
		   {"B", "B", 1},
		   {"B", "C", 2},
		   {"B", "D", 3},
		};

		auto g1 = gdwg::graph<std::string, int>(v1.begin(), v1.end());
		auto g2 = gdwg::graph<std::string, int>(v2.begin(), v2.end());
		auto g3 = gdwg::graph<std::string, int>(v3.begin(), v3.end());
		auto g4 = gdwg::graph<std::string, int>(v4.begin(), v4.end());
		CHECK_THROWS_MATCHES((g1.merge_replace_node("E", "A")),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::merge_replace_node on old or new data if "
		                                              "they don't exist in the graph"));
		CHECK_THROWS_MATCHES((g1.replace_node("E", "F")),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node "
		                                              "on a node that doesn't exist"));
		CHECK(!g1.replace_node("A", "C"));
		CHECK(g1.replace_node("B", "E"));
		CHECK(g1 == gdwg::graph<std::string, int>(expect1.begin(), expect1.end()));
		CHECK(g1.replace_node("E", "B"));
		CHECK(g1 == gdwg::graph<std::string, int>(v1.begin(), v1.end()));

		g1.merge_replace_node("B", "A");
		CHECK(g1 == gdwg::graph<std::string, int>(expect2.begin(), expect2.end()));

		g2.merge_replace_node("B", "A");
		CHECK(g2 == gdwg::graph<std::string, int>(expect3.begin(), expect3.end()));

		g3.merge_replace_node("A", "B");
		CHECK(g3 == gdwg::graph<std::string, int>(expect4.begin(), expect4.end()));

		g4.merge_replace_node("A", "B");
		CHECK(g4 == gdwg::graph<std::string, int>(expect4.begin(), expect4.end()));
	}
}
