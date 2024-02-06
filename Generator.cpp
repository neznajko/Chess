////////////////////////////////////////////////////////
# include "Node.hpp"
# include "Move.hpp"
////////////////////////////////////////////////////////
/////////////////////////////////////////////[Generator]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
# include <set>
////////////////////////////////////////////////////////
Generator::Generator( Unit* const unit )
: unit( unit ),
  node( unit->node ),
  board( node->GetBoard())
{
# ifdef DEBUG
    std::cout << "::>> " << this->unit << endl;
# endif
}
////////////////////////////////////////////////////////
Generator * Generator::Factory( Unit * const unit ){
    switch( unit->GetFig()){
    case KING: {
        return new KingGen( unit );
    }
    case KNIGHT: {
        return new ShortRangeGen<KNIGHT>( unit );
    }
    case QUEEN: {
        return new LongRangeGen<QUEEN>( unit );
    }
    case ROOK: {
        return new LongRangeGen<ROOK>( unit );
    }
    case BISHOP: {
        return new LongRangeGen<BISHOP>( unit );
    }
    case PAWN: {
        if( unit->GetColor()){
            return new PawnGen<WHITE>( unit );
        } else {
            return new PawnGen<BLACK>( unit );
        }
    }
    default: break;
    }
    return {};
}
////////////////////////////////////////////////////////
void Generator::Unsubscribe() {
    // Calling here Unregister will cause problems
    for( const offset_t offset: offsets ){
        board.InLoopUnregister( this, offset );
    }
    offsets.clear();
}
////////////////////////////////////////////////////////
void Generator::GetMoves
( std::vector<Move>& movs ) const {
    const offset_t src{ unit->GetOffset()};
    const color_t srcColor{ unit->GetColor()};
    const offset_t npas{ node->enPassant };
    const rytes_t rytes{ node->rytes };
    for( const offset_t dst: offsets ){
        const color_t dstColor{
            board.GetUnitColor( dst )
        };
        if( dstColor == srcColor ){
            continue;
        }
        move_t type{ dstColor == RED ? MOVE : CRON };
        movs.push_back({ type, src, dst, npas, rytes });
    }
}
////////////////////////////////////////////////////////
std::string Generator::GetStr() const {
    if( offsets.empty()){
        return {};
    }
    // make it sorted
    std::set<offset_t> copY( offsets.begin(),
                             offsets.end());
    std::stringstream ss;
    ss << unit << ":: ";
    for( const offset_t offset: copY ){
        ss << Board::GetCoord( offset ) << " ";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os,
            Generator const * const gen ){
    const auto& s{ gen->GetStr()};
    return s.empty() ? os : os << s << endl;
}
////////////////////////////////////////////////////////
/////////////////////////////////////////[ShortRangeGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <fig_t FIG>
void ShortRangeGen<FIG>::Subscribe(){
    for( int j{}; j < NF_DR; ++j ){
        const offset_t k = unit->GetOffset() + DR[ j ];
        if( board.GetUnitColor( k ) == BLUE ){
            continue;
        }
        board.Register( this, k );
    }
}
////////////////////////////////////////////////////////
//////////////////////////////////////////[LongRangeGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <fig_t FIG>
void LongRangeGen<FIG>::Scan
( RegisterPtr f, int b, int dr ){
    while( true ){
        b += dr;
        const color_t color{
            board.GetUnitColor( b )
        };
        if( color == BLUE ){
            break;
        }
        // Register v Unregister
        ( board.*f )( this, b );
        if( color != RED ){
            break;
        }
    }
}
template <fig_t FIG>
void LongRangeGen<FIG>::Subscribe(){
    const int a{ unit->GetOffset()};
    RegisterPtr f{ &Board::Register };
    for( int j{ START }; j < NF_DR; j += STEP ){
        int b{ a };
        const int dr{ DR[ j ]}; 
        Scan( f, b, dr );
    }
}
template <fig_t FIG>
void LongRangeGen<FIG>::Update( const offset_t offset ){
    int j{ GetDirection( unit->GetOffset(), offset )};
    const int dr{ DR[ j ]};
    const color_t color{ board.GetUnitColor( offset )};
    int b{ offset };
    RegisterPtr f{
        color == RED ?
        &Board::Register :
        &Board::Unregister
    };
    Scan( f, b, dr );
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////[CaslGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void CaslGen::FlipFlop(){
    node->rytes &= ~mask;
}
////////////////////////////////////////////////////////
void CaslGen::Subscribe(){
    board.Register( this, kingSrc );
    board.Register( this, rookSrc );
}
////////////////////////////////////////////////////////
void CaslGen::Update( const offset_t offset ){
    FlipFlop();
}
////////////////////////////////////////////////////////
std::string CaslGen::GetStr() const {
    // discards the leading "Ke1:: " stuff
    static const int prefixLen{ 6 };
    const std::string s{ Generator::GetStr()};
    const int n = s.length();
    std::stringstream ss;
    if( n ){
        // discard the trailing space as well
        ss << s.substr( prefixLen, n - prefixLen - 1 ) 
           << " ";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
bool CaslGen::Go() const {
    if(!( mask & node->rytes )){
        return false;
    }
    if( board.GetUnitColor( rookCheckSq ) < RED ){
        return false;
    }
    for( auto k: checkSqs ){
        if( board.GetUnitColor( k ) < RED or
            board.Check( k, !color )){
            return false;
        }
    }
    return true;
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////[KingGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void KingGen::GetMoves
( std::vector<Move>& movs ) const {
    Generator::GetMoves( movs );
    //
    if( unit->InCheck()){
        return;
    }
    if( casl[ QSIDE ]->Go()){
        movs.push_back({
                LONGCASL,
                0, 0,
                node->enPassant,
                node->rytes });
    }
    if( casl[ KSIDE ]->Go()){
        movs.push_back({
                SHOTCASL,
                0, 0,
                node->enPassant,
                node->rytes });
    }
}
////////////////////////////////////////////////////////
std::string KingGen::GetStr() const {
    std::vector<std::string> caslStr{
        casl[ BLACK ]->GetStr(),
        casl[ WHITE ]->GetStr()
    };
    for( auto& s: caslStr ){
        if( !s.empty()){
            s = " " + s;
        }
    }
    return(
        Generator::GetStr() + " {" +
        caslStr[ BLACK ] + "} [" +
        caslStr[ WHITE ] + "]"
    );
}
///////////////////////////////////////////////[PawnGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <color_t C>
void PawnGen<C>::Subscribe(){
    const int front{ unit->GetOffset() + FORWARD };
    for( const int dr: { -1, 1 }){
        const int offset{ front + dr };
        const color_t color{
            board.GetUnitColor( offset )
        };
        if( color != BLUE ){
            board.Register( this, offset );
        }
    }
}
///////////////////////////////////////////////////////_
std::vector<Move> Boom( const std::vector<Move>& bufr ){
    std::vector<Move> blowback;
    for( const auto& mv: bufr ){
        for( move_t pmot{ QUEEN_PMOT };
             pmot <= NIGHT_PMOT;
             pmot <<= 1 ){
            Move moov{ mv };
            moov.type |= pmot;
            blowback.push_back( moov );
        }
    }
    return blowback;
}
///////////////////////////////////////////////////////_
template <color_t C> void PawnGen<C>::GetMoves
( std::vector<Move>& movs ) const {
    const offset_t src{ unit->GetOffset()};
    const offset_t npas{ node->enPassant };
    const rytes_t rytes{ node->rytes };
    std::vector<Move> bufr;//bcoz of the promotion blow
    // captures
    for( const auto dst: offsets ){
        const auto dstColor{ board.GetUnitColor( dst )};
        if( dstColor == RED ){
            if( dst == npas ){
                bufr.push_back({ NPAS|CRON, src, dst, npas, rytes });
            }
        } else {
            if( dstColor != unit->GetColor()){
                bufr.push_back({ CRON, src, dst, npas, rytes });
            }
        }
    }
    // movs
    static const int NFS[]{ 1, 2 }; // nf squares
    const int srcRow{ Board::GetRow( src )};
    const int nfs{ NFS[ srcRow == PAWN_ROW ]};
    offset_t dst{ src };
    for( int j{}; j < nfs; ++j ){
        dst += FORWARD;
        const auto dstColor{ board.GetUnitColor( dst )};
        if( dstColor != RED ){
            break;
        }
        // raise the NPAS flag in case of double move
        // so Node::MakeMove can check more easily.
        const move_t type = j ? NPAS|MOVE : MOVE;
        bufr.push_back({ type, src, dst, npas, rytes });
    }
    if( !bufr.empty()){
        // promotion
        const int dstRow{ Board::GetRow( dst )};
        if( dstRow == PMOT_ROW ){
            bufr = Boom( bufr ); 
        }
        movs.insert( movs.end(), bufr.begin(), bufr.end());
    }
}
////////////////////////////////////////////////////////
