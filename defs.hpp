////////////////////////////////////////////////////////
# ifndef DEFS_HPP
# define DEFS_HPP
////////////////////////////////////////////////////////
enum { BLACK, WHITE, RED, BLUE };
enum { QSIDE, KSIDE, CENTER };
enum { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, JOKER };
////////////////////////////////////////////////////////
typedef char          offset_t;
typedef int           color_t;
typedef bool          flank_t;
typedef unsigned char fig_t;
typedef unsigned char rytes_t;
typedef unsigned char move_t;
////////////////////////////////////////////////////////
constexpr move_t MOVE{   0           };
constexpr move_t CRON{        1 << 0 };
constexpr move_t QUEEN_PMOT{  1 << 1 };
constexpr move_t ROOK__PMOT{  1 << 2 };
constexpr move_t BSHOP_PMOT{  1 << 3 };
constexpr move_t NIGHT_PMOT{  1 << 4 };
constexpr move_t SHOTCASL{    1 << 5 };
constexpr move_t LONGCASL{    1 << 6 };
constexpr move_t NPAS{        1 << 7 };
////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
