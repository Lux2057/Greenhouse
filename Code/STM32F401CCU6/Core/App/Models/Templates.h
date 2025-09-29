#ifndef TEMPLATES_H
#define TEMPLATES_H

template <typename T>
class Validated
{
public:
    Validated() = delete;
    Validated(bool valid, T val) : Value(val), Valid(valid) {};

    T Value;
    bool Valid{false};
};

#endif // TEMPLATES_H