# Woody-Woodpacker

## A Troll of the Elves

```
#!/usr/bin/lilypond
\version "2.24.1"
\relative c'' {
	\time 2/4
	\partial 8 (\tuplet 3/2 {c16 e g} | c8) g8 r8
	\tuplet 3/2 {(c,16 e g} | \partial 4 c8) g8
}
```
