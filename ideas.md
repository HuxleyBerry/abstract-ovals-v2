Algorithm improvement ideas:
 - In minimal image check
   - Use generators for stabilizer instead of whole stabilizer. Use schrier lemma to get generators
   - Do BFS to find minimum of orbit
   - Compare x with the result of conjugating g by h by only calculating the images within g^h as necessary
 - After switching from orderly algorithm to symmetry-ignoring algorithm, go back to algorithm X.
 - During increasing order solving stage, cover downwards only (see branch)
 - Use the the bounds on the number of involutions (both total and of specific type)
 - Read up on Knuth's ssmcc dancing links solver
 - Not enforcing canonicity at every stage, but some weaker form of pruning. Investigate semi-canonicity or strong semi-canonicity
 - Investigate if there's a way of using the pairwise ordering trick with secondary items to eliminate some symmetry

 Idea already implemented:
 - Dancing links improved variant which allocates options sequentially
 - Dancing links with non algorithm X algorithm which constructs canididates in increasing order. Should work nicely with minimal image checks.