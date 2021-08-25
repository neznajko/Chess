# Chess
\[ No \] heuristics h_re!? As well% For **Alpha Zero** thats *good*,^ here'z
*bad*`(:) The program izn wery preliminary version, namely 0.01 I guess?

Yezs I've been wondering HOEarth to write a chess program, and thanks to a
***IOInformatics*** problem about a *HEX* game have learned about the
existence of the ***Minimax Algorithm*** discovered by *John von Neumann*
before around *100* years, which is considered the beginning of *game theory*
[Minimax Theorem](https://en.wikipedia.org/wiki/Minimax_theorem).

At the beginning of the project was thinking to make it ***Public*** when,
"I can't beat the program", which turns out to be quite optimistic as
Chess Players say. I think I have to read a book or something about
Chess Programming.

At the present moment the evaluation of a position is not random, thats
the best I can say about it. The program goes 4 plies depth and pick up
the highest score, basically thats it, it can solve mates in 2

```Python
neznajko@BlackRook:Chess$ ./chess -h
chess -n (-m PATH) (-s START) (-l LEN) -h FEN
        -n              no engine
        -m PATH         puzzle solvings
        -s START        starting puzzle
        -l LEN          number of puzzles
        -h              print this

        FEN             initial position

 All fields are optional. For example after running
 "./chess" you will be able to play a game against
 the machine mua-ha-ha-ha-ha..
```

Thats how it looks like, definitely have to work on that:
