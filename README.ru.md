# Шаблонный класс Optional

Реализация шаблонного класса Optional. Реализация упрощённого класса, с базовым функционалом класса optional стандартной библиотеке.
Сохраняет значение произвольного типа либо остаётся неинициализироанным.

Типичное применение класса - возврат значения из функции. Если функция не может вернуть корректное значение, возвращается Optional не содержащий значение

---

Операторы -> и *
- Возвращают сохраненное значение. Операторы не делают проверку на пустоту optional. Эти проверки на совести программиста, вызыающего операторы.

Метод T& Value()
- Возвращает значение, содержащееся в объекте класса. Метод Value() генерирует исключение BadOptionalAccess, если объект Optional пуст.

Метод bool HasValue()
- Проверка наличия значения. Возвращает true, если знчение есть, false - значения нет.

Метод void Emplace(Vs&&... values)
- Конструирует сохраняемое значене внутри объекта класса optional.

Метод void Reset()
- Удаляет сохранённое значение.