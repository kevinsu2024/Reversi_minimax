#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);


    // TODO: your code here
    // You will probably want to add arguments here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):
    void draw(ge211::Sprite_set& set, ge211::Posn<int> hover_mouse_posn);



    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    // TODO: Add any public member functions you need.

    Position board_to_screen(Position logical) const;

    Position screen_to_board (Position physical )
    const;

private:
    Model const& model_;
    ge211::Rectangle_sprite const squares;
    ge211::Rectangle_sprite const hover_sprite;
    ge211::Circle_sprite const light_sprite;
    ge211::Circle_sprite const hover_light_sprite;
    ge211::Circle_sprite const dark_sprite;

    ge211::Circle_sprite const hover_dark_sprite;
    ge211::Circle_sprite const gray_sprite;
    ge211::Circle_sprite const possible_move_sprite;


    void add_player_sprite_ (
            ge211::Sprite_set & sprites ,
            Player which ,
            ge211::Posn <int > physical ,
            int z_layer)
    const;







};
