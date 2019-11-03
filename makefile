FLAGS=-pedantic -pedantic-errors -Wextra -Wall -Waggregate-return -Wno-attributes -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wconversion -Wno-deprecated-declarations -Wdisabled-optimization -Wno-div-by-zero -Wno-endif-labels -Werror -Werror-implicit-function-declaration -Wfatal-errors -Wfloat-equal -Wformat -Wformat=2 -Wno-format-extra-args -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit -Wimplicit-function-declaration -Wimplicit-int -Wimport -Wno-import -Winit-self -Winline -Wno-int-to-pointer-cast -Winvalid-pch -Wunsafe-loop-optimizations -Wmain -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wno-multichar -Wnonnull -Wpacked -Wparentheses -Wpointer-arith -Wno-pointer-to-int-cast -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wsystem-headers -Wtrigraphs -Wundef -Wuninitialized -Wunknown-pragmas -Wno-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings

tempsmoy : tempsmoy.c
	gcc tempsmoy.c -o tempsmoy $(FLAGS)
clean :
	rm tempsmoy
	
