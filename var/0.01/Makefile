DEBUG  = -g3
CFLAGS = -Wall
OBJS   = chess.o coor.o figure.o move.o cast.o \
         position.o command.o
#
chess: ${OBJS}
	g++ ${DEBUG} -o $@ $^

command.o: command.cpp command.hpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

position.o: position.cpp position.hpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

cast.o: cast.cpp position.hpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

move.o: move.cpp move.hpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

figure.o: figure.cpp figure.hpp stuff.hpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

coor.o: coor.cpp coor.hpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

chess.o: chess.cpp
	g++ ${DEBUG} ${CFLAGS} -c -o $@ $<

.PHONY: clean
clean:
	@$(RM) -v chess ${OBJS}

# log:
#   Figure <- Coor
#     Move <- Coor
# Position <- Castles, Coor, Board, Figure
