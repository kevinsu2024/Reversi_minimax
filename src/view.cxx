#include "view.hxx"
#include "controller.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 50;
static int const margins = 5;
static Color const square_color {2,96,30};
static Color const light_color {255, 255, 255};
static Color const dark_color {0,0,0};
static Color const hover_square_color {150,2,4};
static Color const possible_square_color {101,86,102};
static Color const gray_color {160,160,160};

View::View(Model const& model)
        : model_(model),
          squares({grid_size, grid_size},square_color),
          hover_sprite({grid_size,grid_size}, hover_square_color),
          light_sprite(0.4*grid_size,light_color),
          hover_light_sprite(0.2*grid_size,light_color),
          dark_sprite(0.4*grid_size,dark_color),
          hover_dark_sprite(0.2*grid_size,dark_color),
          gray_sprite(0.4*grid_size,gray_color),
          possible_move_sprite(0.2*grid_size, possible_square_color)

{ }

void View::draw(Sprite_set& set, Position hover_mouse_posn)
{
    for (Position pos: model_.board()){
        auto move = model_.find_move(screen_to_board(hover_mouse_posn));
        Player plyr = model_[pos];
        // if(model_.is_game_over()){
        //     if(plyr == model_.winner()){
        //         set.add_sprite(squares, board_to_screen(pos));
        //     }
        //     else{
        //         set.add_sprite(, board_to_screen(pos));
        //     }
        //
        // }
        if(move && move->second[pos]){
            set.add_sprite(hover_sprite, board_to_screen(pos));
        }
        else{
            set.add_sprite(squares, board_to_screen(pos));
        }
        if(model_.find_move(pos)){
            set.add_sprite(possible_move_sprite, board_to_screen(pos)
            +Dimensions(0.3*grid_size,0.3*grid_size),1);
        }

        if(plyr != Player::neither){
            add_player_sprite_(set, plyr, board_to_screen(pos) +
            Dimensions(0.1*grid_size,0.1*grid_size), 2);
        }
    }

    //add_player_sprite_(set, model_.turn(), hover_mouse_posn, 2);
    if(model_.turn() == Player::light){
        set.add_sprite(hover_light_sprite,hover_mouse_posn-Dimensions
        (0.2*grid_size,0.2*grid_size), 3);
    }
    else if (model_.turn() == Player::dark){
        set.add_sprite(hover_dark_sprite, hover_mouse_posn-Dimensions
                (0.2*grid_size,0.2*grid_size), 3);
    }





}

View::Position
View::board_to_screen(Model::Position logical) const{
    return View::Position{logical.x * (grid_size + margins),logical.y * (grid_size
    + margins)};
}

Model::Position
View::screen_to_board(Model::Position physical) const{
    return View::Position{(int)physical.x/(grid_size + margins), (int)
    physical.y/(grid_size + margins)};
}

void View::add_player_sprite_ (Sprite_set& sprites, Player which,
                               Model::Position physical, int z_layer) const{
    if(which == Player::light){
        if(!model_.is_game_over() || Player::light == model_.winner()){
            sprites.add_sprite(light_sprite, physical, z_layer);
        }
        else{
            sprites.add_sprite(gray_sprite, physical, z_layer);
        }
    } else if (which == Player::dark){
        if(!model_.is_game_over() || Player::dark == model_.winner()){
            sprites.add_sprite(dark_sprite, physical, z_layer);
        }
        else{
            sprites.add_sprite(gray_sprite, physical, z_layer);
        }
    } else {}
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return (grid_size + margins) * model_.board().dimensions() - Dimensions
    {margins, margins};
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}


