
.PHONY: all clean depend info

CXXFLAGS += -pedantic -Werror -Wall -Wextra -g
LDFLAGS +=

SPEC_SRC = $(wildcard spec/*.cpp)

SRCS = $(SPECIFIC_SRC) $(SPEC_SRC) 
OBJS = $(SRCS:.cpp=.o)


TARGET = specsuite


.cpp.o:
	@echo CXX $<
	@$(CXX) $(CXXFLAGS) $< -c -o $@

all: info $(TARGET) $(SPEC_TARGET)
	@echo SPEC
	@./$(TARGET) $(subset)
	
info:
	@echo "  Compile options"
	@echo "  CXXFLAGS: $(CXXFLAGS)"
	@echo "  LDFLAGS: $(LDFLAGS)"


$(TARGET): $(OBJS)
	@echo LINK $@
	@$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	@echo CLEAN
	@$(RM) $(OBJS) $(SPEC_OBJ) $(SPEC_TARGET) $(TARGET) $(OBJ_DIR)

depend:
	@echo DEP
	@makedepend -Y -- $(CXXFLAGS) -- $(SRCS) $(SPEC_SRC)  > /dev/null 2>&1 
	@$(RM) Makefile.bak

# DO NOT DELETE

spec/spec.o: spec/spec.h
spec/spec_main.o: spec/spec.h
spec/spec_spec.o: spec/spec.h
spec/spec.o: spec/spec.h
spec/spec_main.o: spec/spec.h
spec/spec_spec.o: spec/spec.h
