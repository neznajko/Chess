####____################################################
TARGET   := chess
DEBUG    := -g3
CXX      := g++
CXXFLAGS := -pedantic -std=c++17
OBJS     := chess.o io.o coor.o move.o figure.o node.o \
            cast.o
SRCS     := $(OBJS:.o=.cpp)
DEPS     := deps.mk
#
${TARGET}: ${OBJS}
	$(CXX) ${DEBUG} -o $@ $^

-include $(DEPS)

.PHONY: clean cleanall deps
clean:
	@$(RM) -v ${TARGET} ${OBJS}

cleanall:
	@$(RM) -v ${TARGET} ${OBJS} $(DEPS)

deps:
	$(CXX) -MM $(SRCS) > $(DEPS)

########################################################
# log:
########################################################
