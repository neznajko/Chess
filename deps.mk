chess.o: chess.cpp coor.hpp io.hpp
io.o: io.cpp io.hpp
coor.o: coor.cpp coor.hpp io.hpp
move.o: move.cpp figure.hpp move.hpp coor.hpp io.hpp
figure.o: figure.cpp node.hpp figure.hpp move.hpp coor.hpp io.hpp
node.o: node.cpp node.hpp figure.hpp move.hpp coor.hpp io.hpp