# libprofilerino
<b>libprofilerino</b> is a small library that enables profiling of PSVita homebrew with Razor CPU tracer for executables built with vitasdk. It also features an optional symbol table support so that markers will use proper function names instead of function addresses.

## Instructions

- `make install` the library.
- Call `profilerino_init` at the start of your main function in your homebrew.
- Compile your homebrew with `-finstrument-functions`.
- Once compiled, Run `nm --without-symbol-versions --demangle FILE.ELF > sym.txt`. (Optional)
- Place `sym.txt` in the same folder of your executable. (Optional)

## Credits

- Nevak for coming up with the neat idea of using CPU markers to replace natural markers inserted by Razor.
- fgsfds for the initial idea of using functions instrumentation with gcc to theorycraft an homemade profiler.
