# Question 3

## 1. Background

The overall task is to write a single-table database which contains records and supports some simple querying over these records.

There are three main classes to implement, that being the `record`, the `database`, and the `query` classes. 

Just to be clear, the `database` in this assignment is to be regarded as a single collection of `record` objects, and in relational database terms, our `database` might be regarded as a single table.

For the sake of keeping the problem fairly simple, we will only store ASCII strings in these tables, so we will not think about integer values, real values, etc.

Note in the specification of how these classes should be implemented, it is up to you to make sensible decisions about where the keyword const might be put, where you might put an `&` or an `&&`,and where you think that moveable overloads of functions make sense.  

## 2. The Task

### 2.1. Part 1 - The Record Class

A `record` is a set of attribute-value pairs.  

The `record` class should be fairly trivial, the main work here is to correctly implement serialisation, as well as correctly define the member functions.

The data contents of a `record` could be, for example - 

```
{
first-name=joe
last-name=bloggs
zid=z123567
}
```

Some notes:

- You can assume that `operator>>` will only be invoked once per record empty. You do not need to consider the case of "over-writing" a previous read in record.

- You can assume no new line character `\n` will be part of the data provided for a record on input.

- The ordering of these attribute-pair values is not important semantically.

- We use the escape character '!'.  If the '=' or '!' character appears in the attribute or value, then it will preceded by the escape character.  For instance, the value `the = character is awesome!` will be encoded  `the != character is awesome!!`  Please note that you might want to leave this encoding task as something for last, for the majority of test cases this will not be required. 

Here is an outline of the members needed in your `record` class:

- constructor, destructor -
 The  `record` constructor should take no arguments and initialise a record to contain zero attributes. This should be a lightweight operation.  When the object is destroyed, any dynamically memory owned by the class is required to be freed.
- `get_value` -
  given a `std::string` attribute name, return a `std::string` value.  If the attribute is not present in the record, behaviour is undefined.
- `set_value` -
given a `std::string` attribute name, and a `std::string` value, add or update an attribute
- `has_attribute` -
return true or false if the record has the named attribute, takes an input parameter of `std::string`
- `delete_attribute` -
given a `std::string` attribute name, remove an attribute.  Returns true if the attribute was present, and false otherwise.
- `count` -
returns how many attribute-value pairs are stored inside this record. Returns a std::size_t.
- `operator<<` -
For streaming out the record to `ostream`. The format of a record begins with a opening brace `{` followed by a new line character, and is then followed by the attribute-value pairs, one per line, in the form:  `<attribute>=<value>` without leading whitespace and without trailing whitespace.
The end of the record is marked by a closing brace `}` on a line by itself followed by a new line character.  
- `operator>>` -
For reading records in from `istream`. It will expect the stream to be positioned on the start of the opening brace, which it will consume, and then read the records in the same format written by the `operator<<` above, then finally consume the closing `}` and trailing new line character.

### 2.2 Part 2 - The Database Class

The `database` is a collection of `record` objects.

Here is an outline of the behaviour needed from your `database` class:

- constructor, destructor - The default `database` constructor initialises a database to contain zero records. This should be a lightweight operation. The destructor should clean up any memory allocated on behalf of the database.
- `insert(record)` - Takes a record and adds it to the database
- `count()` returns `size_t` - return the total number of records, this should run in constant time.
- `operator<<` - for streaming out the database to `ostream`. The format of the database is simply a contatenation of the records that the database contains, serialised with Record `operator<<`
- `operator>>` - for streaming in the database from `istream`. 


Furthermore, you will need to implement two more methods, though you may need to complete Task 3 first before doing these:
- `delete_matching(query)` - takes a `query` and deletes all records matching the query, and then returns a `size_t` count of records deleted
- `select(query)` - takes a `query` and returns a new `database` object containing all records matching the query


## 2.3 Part 3 - The Query Class hierarchy

The `query` classes are a polymorphic collection of queries that can be composed upon the `database`.

We supply the incomplete `query` class here:

```
class query {
public:
    auto virtual matches(Record const& r) const -> bool { return true; }
}
```

`query` is the base class, and it provides a method `matches` which takes a `record`, and returns true if that `record` matches the the `query`.

From `query` are derived the following specialisations:

| Query | Initialisaton | `matches` behaviour |
|-|-|-|
| `query_equals` | constructor takes two string values `attr`, `value`  | returns `true` if there is an attribute named `attr` in the `record` where the value is equal to `value` |
| `query_less_than`  | constructor takes two string values `attr`, `value`  | returns `true` if there is an attribute named `attr` in the `record` where the value is less than `value` |
| `query_greater_than`  | constructor takes two string values `attr`, `value` | returns `true` if there is an attribute named `attr` in the `record` where the value is greater than `value` |
| `query_starts_with`  | constructor takes two string values `attr`, `value`  | returns `true` if there is an attribute named `attr` in the `record` where the value is that starts with  `value` |
| `query_and`  | constructor takes `query` objects | returns `true` if `matches` returns true on both contained `query` objects |
| `query_or` | constructor takes `query` objects | returns `true` if `matches` returns true on either contained `query` object |
| `query_not` | constructor takes one `query` object | returns `true` if `matches` returns false on contained `query` |

## 3. Usage

To test your code you can write your own tests in `test/q3` (like the one we've provided), or you can test your code manually by building and running `q3-example-usage` we've provided too.

Please note that we have only provided `students.db` as a starting point. Not all `.db` files have been provided.
