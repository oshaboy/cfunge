%fingerprint-spec 1.4
%fprint:CFFI
%url:https://github.com/oshaboy/rcfunge-with-ffi
%desc:C Foreign Function Interface
%safe: false
%begin-instrs
#I Name              Desc
B getbyte getbyte
C ccall ccall
D duplicate duplicate
F free free
G and and
I popp popp
L dlopen dlopen
M malloc malloc
O print_pstack print_pstack
P funge_to_pointer funge_to_pointer
S generate_string generate_string
T dlsym dlsym
U dlclose dlclose
W dereference dereference
Y swap swap
%end
