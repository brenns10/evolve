Evolutionary Algorithm Example
==============================

It's an evolutionary algorithm!  A silly one, but it gets the point across.  The
point of the search is to recover the input string.  It does this by creating a
"population" of random parent strings.  Then it "mutates" and "crosses" the
parents to create a new population, and then selects the most fit strings to use
as the parents.  (Fitness is defined as how many characters of the string they
have in common).  This process continues until the fittest string is the same as
the search string.

You can build it by running `make` (it should build on Linux with no fuss).  Run
it with `bin/release/evolve [string-to-search-for]`.  It'll output the fitness
of each generation's top individual as it goes.  It may take a long time for
long strings (or segfault if you make the string big enough).

The idea for this code isn't original -- I saw the idea mentioned on
StackOverflow, and apparently the SO user got the idea for it from something
called Rosetta Code.  It's a fun coding exercise for me.
