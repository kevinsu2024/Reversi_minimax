#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)

{ }

Model::Model(int width, int height)
        : board_({width, height}),
        table(),
        hash_table(),
          board_copy(board_)
{
    for(int i = 0; i < 8; i++){
        std::vector<std::vector<int>> table_row;
        for(int k = 0; k < 8; k++){
            std::vector<int> table_row_row;
            for(int j = 0; j < 2; j++){
                table_row_row.push_back(rand() % 5000);
            }
            table_row.push_back(table_row_row);
        }
        table.push_back(table_row);
    }
    compute_next_moves_();
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

const Move*
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        // Nothing was found, so return NULL (nullptr in C++)
        return nullptr;
    } else {
        // Dereferences the iterator to get the value then returns a pointer
        // to that value. This is safe as the pointer is to a value living
        // within the `next_moves_` structure.
        return &(*i);
    }
}

void
Model::play_move(Position pos)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    const Move* movep = find_move(pos);
    if (!movep) {
        throw Client_logic_error("Model::play_move: no such move");
    }
    really_play_move_(*movep);
}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set
Model::find_flips_(Position current, Dimensions dir) const
{
    Position_set output = Position_set();
    current += dir;
    while(((0 <= current.x && current.x < board_.dimensions().width) &&
             (0 <= current.y && current.y < board_.dimensions().height))
          && board_[current] == other_player(turn_)){
        output[current] = true;
        current += dir;
    }

    if (((current.x < 0)|| (current.y < 0) || (current.x >= board_
    .dimensions().width) || (current.y >= board_
            .dimensions().height)) || board_[current] == Player::neither){
        output.clear();

    }
    return output;
}

Position_set
Model::evaluate_position_(Position pos) const
{
    Position_set output = Position_set();
    if(board_[pos] == Player::neither){
        for(auto dim : Board::all_directions()){
            output |= find_flips_(pos, dim);
        }
    }
    if(!output.empty()){
        output[pos] = true;
    }
    return output;
}

void
Model::compute_next_moves_()
{
    next_moves_.clear();
    for(auto pos : board_.center_positions()){
        if(board_[pos] == Player::neither){
            next_moves_[pos] = {pos};
        }
    }
    if(next_moves_.empty()){
        for(auto pos : board_.all_positions()){
            Position_set pset = evaluate_position_(pos);
            if(!pset.empty()){
                next_moves_[pos] = pset;
            }
        }
    }
}

bool
Model::advance_turn_()
{
    turn_ = other_player(turn_);
    compute_next_moves_();
    return !(next_moves_.empty());


    // ^^^ this is wrong
}

void
Model::set_game_over_()
{
     turn_ = Player::neither;
     if (board_.count_player(Player::dark) > board_.count_player
     (Player::light)){
         winner_ = Player::dark;
     }
     else if (board_.count_player(Player::dark) < board_.count_player
     (Player::light)){
         winner_ = Player::light;
     }
     else{
         winner_ = Player::neither;
     }

}

void
Model::really_play_move_(Move move)
{
    board_.set_all(move.second, turn_);
    if(!advance_turn_()){
        if(!advance_turn_()){
            set_game_over_();
        }
    }
}
int
Model::find_hash(Board board)
{
    int hash = 0;
    for (ge211::Posn<int> pos: board.all_positions()){
        if (board[pos] == Player::light || board[pos] == Player::dark){
            int piece = 0;
            if (board[pos] == Player::light){
                piece = 0;
            }
            else{
                piece = 1;
            }
            hash ^= table[pos.x][pos.y][piece];
        }
    }
    return hash;
}
int
Model::minimax_(Move move, int depth, int alpha, int beta, bool
maximizingPlayer, Player initialPlayer)
{

    if (depth == 0 || is_game_over() || next_moves_.size() == 0){
        if (winner_ == other_player(initialPlayer)){
            return -64;
        }
        else if (winner_ == initialPlayer){
            return 64;
        }
        else{
            return board_.count_player(initialPlayer) - board_.count_player
            (other_player(initialPlayer));
        }

    }

    ge211::Posn<int> best_val_pos = {100,100};
    if (maximizingPlayer){
        int maxEval = -100;
        for (Move val : next_moves_){
            ge211::Posn<int> val_pos = val.first;
            int eval = minimax_(val, depth - 1, alpha, beta, false,
                                initialPlayer);
            if (eval > maxEval){
                best_val_pos = val_pos;
            }
            maxEval = std::max(eval, maxEval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
            {
                break;
            }

        }
        return maxEval;
    }
    else{
        int minEval = 100;
        for (Move val : next_moves_){
            ge211::Posn<int> val_pos = val.first;
            int eval = minimax_(val, depth - 1, alpha, beta, true,
                                initialPlayer);
            if(eval < minEval){
                best_val_pos = val_pos;
            }
            minEval = std::min(minEval,eval);
            beta = std::min(beta, eval);
            if (beta <= alpha){
                break;
            }

        }
        return minEval;
    }

}
