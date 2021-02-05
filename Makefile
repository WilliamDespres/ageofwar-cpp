# Compiler stuffs
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 #-Werror

# In and out folders
SRCDIR := src
OBJDIR := build
INCDIR := include

# Final desired executable
EXE := ageofwar

# Sources and objects
SRC := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/units/*.cpp) $(wildcard $(SRCDIR)/units/cave/*.cpp) $(wildcard $(SRCDIR)/units/modern/*.cpp) $(wildcard $(SRCDIR)/units/star/*.cpp) $(wildcard $(SRCDIR)/players/*.cpp) $(wildcard $(SRCDIR)/io/*.cpp)
OBJ := $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
INC := $(INCDIR)/*.hpp $(INCDIR)/units/*.hpp $(INCDIR)/units/cave/*.hpp $(INCDIR)/units/modern/*.hpp $(INCDIR)/units/star/*.hpp $(INCDIR)/players/*.hpp $(INCDIR)/io/*.hpp

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INC) | $(OBJDIR) $(OBJDIR)/units $(OBJDIR)/units/cave $(OBJDIR)/units/modern $(OBJDIR)/units/star $(OBJDIR)/players $(OBJDIR)/io
	$(CXX) -I $(INCDIR) $(CXXFLAGS) $< -c -o $@

ageofwar: $(OBJ) | $(OBJDIR)
	$(CXX) -I $(INCDIR) $(CXXFLAGS) $^ -o $@

$(OBJDIR) $(OBJDIR)/units $(OBJDIR)/units/cave $(OBJDIR)/units/modern $(OBJDIR)/units/star $(OBJDIR)/players $(OBJDIR)/io:
	mkdir $@

.PHONY:
	clean

clean:
	rm -r $(OBJDIR)
	rm -f ageofwar