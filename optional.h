#include <stdexcept>
#include <utility>

// Статья про перегрузку операторов
// https://akrzemi1.wordpress.com/2014/06/02/ref-qualifiers/

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    ~Optional();

    bool HasValue() const;

    template <typename... Vs>
    void Emplace(Vs&&... values);

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() &;
    const T& operator*() const&;
    T operator*() &&;

    T* operator->();
    const T* operator->() const;

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() &;
    const T& Value() const&;
    T Value() &&;

    void Reset();

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* p_value_ = nullptr;
};

template <typename T>
inline bool Optional<T>::HasValue() const {
    return is_initialized_;
}

template <typename T>
inline T& Optional<T>::operator*() & {
    return *p_value_;
}

template <typename T>
inline T Optional<T>::operator*() && {
    return std::move(*p_value_);
}

template <typename T>
inline const T& Optional<T>::operator*() const& {
    return *p_value_;
}

template <typename T>
inline T* Optional<T>::operator->() {
    return p_value_;
}

template <typename T>
inline const T* Optional<T>::operator->() const {
    return p_value_;
}

template <typename T>
inline T& Optional<T>::Value() & {
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return *p_value_;
}

template <typename T>
inline T Optional<T>::Value() && {
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return std::move(*p_value_);
}

template <typename T>
inline const T& Optional<T>::Value() const& {
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return *p_value_;
}

template <typename T>
inline void Optional<T>::Reset() {
    if (is_initialized_) {
        p_value_->~T();
        p_value_ = nullptr;
        is_initialized_ = false;
    }
}

template <typename T>
inline Optional<T>::Optional(const T& value) {
    p_value_ = new (data_) T{value};
    is_initialized_ = true;
}

template <typename T>
inline Optional<T>::Optional(T&& value) {
    p_value_ = new (data_) T{std::move(value)};
    is_initialized_ = true;
}

template <typename T>
inline Optional<T>::Optional(const Optional& other) {
    if (other.is_initialized_) {
        p_value_ = new (data_) T{other.Value()};
        is_initialized_ = true;
    }
}

template <typename T>
inline Optional<T>::Optional(Optional&& other) {
    if (other.is_initialized_) {
        p_value_ = new (data_) T{std::move(other).Value()};
        is_initialized_ = true;
    }
}

template <typename T>
inline Optional<T>& Optional<T>::operator=(const T& value) {
    if (is_initialized_) {
        *p_value_ = value;
    } else {
        p_value_ = new (data_) T{value};
        is_initialized_ = true;
    }
    return *this;
}

template <typename T>
inline Optional<T>& Optional<T>::operator=(T&& rhs) {
    if (is_initialized_) {
        *p_value_ = std::move(rhs);
    } else {
        p_value_ = new (data_) T{std::move(rhs)};
        is_initialized_ = true;
    }
    return *this;
}

template <typename T>
inline Optional<T>& Optional<T>::operator=(const Optional& rhs) {
    if (this != &rhs) {
        if (is_initialized_) {
            if (rhs.is_initialized_) {
                *p_value_ = *(rhs.p_value_);
            } else {
                Reset();
            }
        } else {
            if (rhs.is_initialized_) {
                p_value_ = new (data_) T{*(rhs.p_value_)};
                is_initialized_ = true;
            }
        }
    }
    return *this;
}

template <typename T>
inline Optional<T>& Optional<T>::operator=(Optional&& rhs) {
    if (this != &rhs) {
        if (is_initialized_) {
            if (rhs.is_initialized_) {
                *p_value_ = std::move(*(rhs.p_value_));
            } else {
                Reset();
            }
        } else {
            if (rhs.is_initialized_) {
                p_value_ = new (data_) T{std::move(*(rhs.p_value_))};
                is_initialized_ = true;
            }
        }
    }
    return *this;
}

template <typename T>
inline Optional<T>::~Optional() {
    if (p_value_) {
        p_value_->~T();
    }
}

template <typename T>
template <typename... Vs>
inline void Optional<T>::Emplace(Vs&&... values) {
    if (p_value_) {
        p_value_->~T();
    }
    p_value_ = new (data_) T{std::forward<Vs>(values)...};
    is_initialized_ = true;
}
