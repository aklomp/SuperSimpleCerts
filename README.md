# SuperSimpleCerts

A GTK utility for the Linux desktop to create, store and update SSL certificates.
As the name implies, intended to be super easy to use.
Basically a user-friendly GUI on top of PolarSSL's certificate library.

This hobby project is currently nowhere near complete.
The primary goal was to practice the process of creating this kind of application, and to play around with some technologies.

## Technical features

- Uses PolarSSL as the crypto backend;
- Stores certificates in an SQLite database (todo);
- Interface created and instrumented in Glade;
- Compiles the interface file into the binary (for a standalone binary);
- Modularized as much as possible at the translation unit level.
