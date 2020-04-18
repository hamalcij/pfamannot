[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/parserWrapper.md)
[//]: # (Jan Hamalcik)
[//]: # ()
[//]: # (Programmer's documentation of ParserWrapper class)
[//]: # ()

# `class ParserWrapper`

`class ParserWrapper` is a singleton wrapper wrapping `class Parser`.
It allows the static method
`static void Parser::signalHandler(int signum)` to access private
variables of a specific instance of `Parser` needed to handle a
`SIGINT` interrupt signal raised by the user during download or
inflating.
The class is defined in [`source/parser.hpp`](../../source/parser.hpp).

## Private variables

```cpp
Parser* parser_;
```

Variable `parser_` points to current instance of `Parser`, thus
allowing only one instance at one time.
Through this pointer, static methods defined in `Parser` can access
private data of the current instance.

---

## Public functions

```cpp
static ParserWrapper& ParserWrapper::getInstance();
```

Returns static reference to an instance of `ParserWrapper`, wrapping
around an instance of `Parser`.

---

```cpp
void ParserWrapper::set(Parser* parser);
```

Procedure `set()` sets the pointer `parser_` to point to an instance
of `Parser`. Following parameters have to be specified:

* `Parser* parser` is the pointer to a specific `Parser`
instance.
This shall be set from the instance itself using the keyword `this`.

---

```cpp
void Parser* ParserWrapper::get();
```

Returns a pointer to private variable `parser_`.

---

[Next](flowController.md)

[Previous](parser.md)
