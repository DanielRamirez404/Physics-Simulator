#include "side.h"

Side getOppositeSide(Side side) {
  return (side == Side::left) ? Side::right : Side::left;
}
