####____################################################
TARGET   := chess
DEBUG    := -g3
CXXFLAGS := -Wall -DDEBUG -pedantic -std=c++17
OBJS     := chess.o io.o coor.o move.o figure.o node.o
SRCS     := $(OBJS:.o=.cpp)
DEPS     := deps.mk
#
${TARGET}: ${OBJS}
	g++ ${DEBUG} -o $@ $^

-include $(DEPS)

.PHONY: clean cleanall deps
clean:
	@$(RM) -v ${TARGET} ${OBJS}

cleanall:
	@$(RM) -v ${TARGET} ${OBJS} $(DEPS)

deps:
	g++ -MM $(SRCS) > $(DEPS)

########################################################
# log: make gitignore rule and start pushing
########################################################
