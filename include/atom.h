#ifndef ATOM_INCLUDED
#define ATOM_INCLUDED

extern int         Atom_length(const char *str);
// exercise 3.7: add Atom_init
extern void        Atom_init(int hint);
extern const char *Atom_new(const char *str, int len);
extern const char *Atom_string(const char *str);
extern const char *Atom_int(long n);
// exercise 3.8: add Atom_free and Atom_reset
extern void Atom_free(const char *str);
extern void Atom_reset();
// exercise 3.9: add Atom_vload / aload
extern void Atom_aload(const char *strs[]);

#endif
