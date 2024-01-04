# Optional template class

[Русский язык](README.ru.md)

Simple realiztion of similar to standard library class optional.
Keeps value or non-initialised if empty.

The typical use of class optional used is as return value for function that could be able not return a value.

---

Operators -> and *
- Accesses the contained value. Operators don't check tha optional is empty. This check is programmer's resposibility.

Method T& Value()
- Returns the contained value. Method Value() generates exception BadOptionalAccess, if Optional empty.

Method bool HasValue()
- checks whether the object contains a value.

Method void Emplace(Vs&&... values)
- Constructs the contained value in-place.

Method void Reset()
- Destroys contained value.