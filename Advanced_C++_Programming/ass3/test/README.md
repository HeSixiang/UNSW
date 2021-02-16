## Design:
Three containers inside my graph: weight_set, node_set and node_map. </br>
* node_set: to maintain the set of share pointers for node N. Make sure only storing one instance of each node N. </br>
* weight_set: to maintain the set of share pointers for weight E. Make sure only storing one instance of each weight E. </br>
* node_map: to maintain the edge information of graph with shared pointers of N and E. </br>

In my design, time complexity of erase_edge(iterator) cannot be constant time because I have to maintain the weight_set (when the reference count of E in weight_set equals to 1, I need to delete it because no node is using it). </br>

The reason why I decided to use weight_set is because I did not see this sentence or might be it was updated later: </br>

	"[Note: You may store a unique weight multiple times, but no more than once for each distinct edge with that weight. end note]"

When I realized a weight can be stored multiple, it was too late, I have already finished most of part. </br>
Without maintaining the weight_set, of course, I can make erase_edge(iterator) amortised constant time but might need more space(space and time trade off). </br>

## Testing:

To make sure the underlying code works correctly, I have two more extra accessors to get information of node_set and weight_set. </br>

1.	get_node_ref_count(): Get the map <key: copy value of node N, value: reference count of node N>
2.	get_weight_ref_count(): Get the map <key: copy value of weight E, value: reference count of weight E>

With two more helper methods, I can not only check the result is correct but also check each graph only store one instance of each node N and weight E during creating, deleting, and inserting. </br>

I wrote my test code based on the detailed requirements of assignment document. </br>
Basically, the steps are: </br>
* Step1: Check effects are correct. </br>
* Step2: Return value is correct. </br>
* Step3: Exception works correctly. </br>
* Step4: Check postconditions. </br>

Let me give detailed example of move construction for how I tested on step 4: </br>
```
auto operator=(graph&& other) noexcept -> graph&;
```
Postconditions: </br>

1.	*this is equal to the value other had before this operator's invocation.
```
auto expect_result = gdwg::graph<std::string, int>(v.begin(), v.end());
auto other = gdwg::graph<std::string, int>(v.begin(), v.end());
auto g_move = gdwg::graph<std::string, int>(std::move(other));
CHECK(g_move == expect_result);
```
2.	other.empty() is true.
```
// NOLINTNEXTLINE(bugprone-use-after-move)
CHECK(other.empty());
```

3.	All iterators pointing to elements owned by other prior to this operator's invocation remain valid, but now point to the elements owned by *this.
```
auto other = gdwg::graph<std::string, int>(v.begin(), v.end());
auto it_other_begin = other.begin();
auto it_other_last = --other.end();
CHECK(*(it_other_begin) == ranges::make_common_tuple("A", "B", 1));
CHECK(*(it_other_last) == ranges::make_common_tuple("C", "A", 2));
auto g_move = gdwg::graph<std::string, int>(std::move(other));
CHECK(*(it_other_begin) == ranges::make_common_tuple("A", "B", 1));
CHECK(*(it_other_last) == ranges::make_common_tuple("C", "A", 2));
```

Also, I putted some static_assert (compile-time checking) in front of the test file to have basic test for iterator. </br>
```
static_assert(ranges::bidirectional_iterator<gdwg::graph<int, int>::iterator>);
static_assert(ranges::bidirectional_range<gdwg::graph<int, int>>);
static_assert(ranges::forward_iterator<gdwg::graph<int, int>::iterator>);
```
