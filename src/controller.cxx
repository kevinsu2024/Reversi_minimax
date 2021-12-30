#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          hover_mouse_posn(ge211::Posn<int>{0,0}),
          moves_count(0)
{ }

void
Controller::on_mouse_down(ge211::Mouse_button button, ge211::Posn<int> position)
{
    moves_count++;
    ge211::Posn<int> logical = view_.screen_to_board(position);
    ge211::Posn<int> best_move = {100000,100000};
    //std::pair<const ge211::Posn<int>,Position_set>{logical, model_.next_moves_[logical]};
    int best_move_score = -1000;
    if(model_.turn() == Player::light){
        if(model_.find_move(logical)){
            model_.play_move(logical);
        }
    }
    else if (model_.turn() == Player::dark){
        if ( moves_count < 4){
            if(model_.find_move(logical)){
                model_.play_move(logical);
            }
        }
        for (Move move: model_.next_moves_){
            ge211::Posn<int> move_pos = move.first;
            int eval_score = model_.minimax_(move, 10, -1000, 1000,
                                             true,
                                             Player::dark);
            std::cout << "move score: " + std::to_string(eval_score) + "|\n";
            if (eval_score > best_move_score){
            best_move = move_pos;
            best_move_score = eval_score;
            }
        }
        if (!(best_move == ge211::Posn<int>{100000,100000})){
            model_.play_move(best_move);
        }
    }


}

void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    hover_mouse_posn = position;
}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, hover_mouse_posn);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}



