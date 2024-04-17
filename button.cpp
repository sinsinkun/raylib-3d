#include <string>
#include <raylib.h>
#include "button.hpp"

using namespace App;

void Button::updateState(Vector2 mousePos, bool mouseClicked, int& mouseHoverCount) {
  // determine hover state
  bool xin = (mousePos.x > pos.x) && (mousePos.x < pos.x + size.x);
  bool yin = (mousePos.y > pos.y) && (mousePos.y < pos.y + size.y);
  if (xin && yin) {
    _isHovered = true;
    mouseHoverCount++;
  }
  else _isHovered = false;

  // determine click state
  if (_isHovered && !_isClicked && mouseClicked) {
    state = Just_Clicked;
    _isClicked = true;
  }
  else if (_isHovered && _isClicked && mouseClicked) state = Clicking;
  else if (_isHovered && _isClicked && !mouseClicked) {
    state = Just_Released;
    _isClicked = false;
  }
  else if (_isHovered) state = Hover;
  else state = None;
}

void Button::updatePos(Vector2 newPos) {
  pos = newPos;
}

void Button::render() {
  Color bgColor;
  Vector2 dpos = pos;
  Vector2 dsize = size;
  Vector2 dtext = textOffset;
  // change drawn properties based on state
  switch (state) {
    case Hover:
    case Just_Released:
      bgColor = cbgHover;
      break;
    case Just_Clicked:
    case Clicking:
      bgColor = cbgClick;
      dpos.x -= 2.5f;
      dpos.y -= 2.5f;
      dsize.x += 5.0f;
      dsize.y += 5.0f;
      dtext.x += 1.0f;
      dtext.y += 1.0f;
      break;
    case None:
    default:
      bgColor = cbgNone;
      break;
  }
  
  // define button size/position
  Rectangle rect = {dpos.x, dpos.y, dsize.x, dsize.y};
  Rectangle rectShadow = {dpos.x + 2.0f, dpos.y, dsize.x + 2.0f, dsize.y + 5.0f};

  // render to screen
  DrawRectangleRounded(rectShadow, 0.5, 8, cShadow);
  DrawRectangleRounded(rect, 0.5, 8, bgColor);

  // draw text
  if (text.length() > 0) {
    DrawText(text.c_str(), pos.x + dtext.x, pos.y + dtext.y, 24, cText);
  }
}