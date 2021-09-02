TARGET   = chess
DEBUG    = -g3
CXXFLAGS = -Wall
OBJS     = chess.o
#
${TARGET}: ${OBJS}
	g++ ${DEBUG} -o $@ $^
#
chess.o: chess.cpp

.PHONY: clean
clean:
	@$(RM) -v ${TARGET} ${OBJS}

# log:
#
# coor.hpp:   <=====: coor.cpp
# - <cstdint>
# ------------
# - <string> 
# - <ostream>
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
