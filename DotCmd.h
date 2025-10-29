#ifndef DOTCMD
#define DOTCMD

#define S(x) #x

#define STRING(x) S(x)

#define NODE(name, data, next, prev) name [label = STRING(<h> name | <d> data |{ <n> next | <p> prev})]

#endif // DOTCMD
