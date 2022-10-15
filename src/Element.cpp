#include "Element.hpp"

Element::Element() : hidden(true),
                     bombs(0),
                     contains(false),
                     defused(true) {

}

bool Element::isHidden() {
    return hidden;
}

bool Element::containsBomb() {
    return contains;
}

uint8_t Element::numAdjacentBombs() {
    return bombs;
}

void Element::reveal() {
    hidden = false;
}

void Element::setBomb() {
    contains = true;
    defused = false;
}

void Element::setNumAdjacentBombs(const uint8_t numAdjacentBombs) {
    this->bombs = numAdjacentBombs;
}

void Element::defuse() {
    defused = true;
}

bool Element::isLive() {
    return !defused;
}
