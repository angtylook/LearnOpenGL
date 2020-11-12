#ifndef COMMON_MICROS_H_
#define COMMON_MICROS_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    \
TypeName(const TypeName&) = delete;        \
    \
TypeName&                                  \
    operator=(const TypeName&) = delete;

#endif  // COMMON_MICROS_H_
