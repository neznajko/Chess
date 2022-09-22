####____################################################
TARGET   := chess
DEBUG    := g3
CXX      := g++
OBJDIR   := obj
SRCDIR   := src
INCDIR   := inc
DEPDIR   := dep
CXXFLAGS := -$(DEBUG) -pedantic -std=c++17 -I$(INCDIR)
OBJS     := $(addprefix $(OBJDIR)/, chess.o)
DEPS     := $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $(OBJS))
################################################################
$(TARGET): $(OBJS)
	$(CXX) -$(DEBUG) -o $@ $^
################################################################
-include $(DEPS)
################################################################
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@$(CXX) -MM $(CXXFLAGS) $< > $*.d # -I
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$@:|' < $*.d.tmp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
	@mv -f $*.d $(DEPDIR)
################################################################
.PHONY: clean
clean:
	@$(RM) -v $(TARGET) $(OBJS) $(DEPS)
########################################################
# log: http://scottmcpeak.com/autodepend/autodepend.html
########################################################
