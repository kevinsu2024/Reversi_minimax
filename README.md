# Reversi_minimax
Minimax implementation with alpha-beta pruning and transposition table for Othello(Reversi)

It's kinda smart so it can beat some AI's but not all.

Currently, it's still slow. (Depth 5 mid-game run time ~ 10s for a move, Depth 4 mid-game run time <0.5s for a move)
This is due to the bottleneck in the implementation, where I need to create a new model for each possible move instead of just updating a binary-form board.

Further improvements involves changing implementation of the game itself, which I may/may not get to in the future.

Games played against other AI's(on depth 5):

1. Eothello.com: 38 vs. 26 (win)
2. cardgames.io/reversi/: 35 vs. 29 (win)
3. hewgill.com/othello/:
  a. Easy: 9 vs. 55 (loss)
  b. Medium: 14 vs. 50 (loss)
  c. Hard: 17 vs. 47 (loss)

