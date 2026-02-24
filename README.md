# Custom Memory Allocators

This repo contains a set of custom written memory allocators written in C as a
study project.

# Note

This set of functions are written with GCC in mind. As you can see in the
definition of `void jfree(void *__attribute__((nullable)) p)`, I used GCC's
attributes, so it's probably not possible to compile with clang, and clangd will
give a warning.

# License

This repo is licensed under [GNU General Public License v3.0](https://www.gnu.org/licenses/).
Please read the [LICENSE](./LICENSE) file for more information.
