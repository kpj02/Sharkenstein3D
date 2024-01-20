#
# Makefile
#

# Folders
SRC = src

# this is the directory I will compile from (i.e. the argument to -I)
IDIR := include

# all object files will be put here
OBJDIR := objdir

# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS += -MMD -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast \
		   -I$(SFML_ROOT)/include

LDFLAGS += -L$(SFML_ROOT)/lib -lsfml-graphics -lsfml-audio -lsfml-window \
		   -lsfml-system

# Object modules
GAME_OBJECTS = Game_Object.o Static.o Dynamic.o Star.o Projectile.o Mortal.o \
               Player.o Enemy.o Melee.o Weapon.o Rocket_Launcher.o Minigun.o \
			   Power_Up.o Chainpowerup.o Minipowerup.o Chainsaw.o Explosion.o\
			   Range.o Boss.o Bullet.o Shotgun.o Shotgunpowerup.o Gib.o

MAP = Map.o Map_Tile.o

STATES = Program.o State.o Game.o Hajscore.o Main_Menu.o Button.o \
         New_hajscore.o Game_Over.o Credits.o

MAIN = $(addprefix $(OBJDIR)/, main.o  $(GAME_OBJECTS) $(MAP) $(STATES))

TEST = $(addprefix $(OBJDIR)/, test.o test_main.o $(GAME_OBJECTS) $(MAP))

# Main objectives - created with 'make' or 'make main'.
play: $(MAIN)
	$(CCC) -I$(IDIR) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

test.out: $(TEST)
	$(CCC) -I$(IDIR) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

# Part objectives
$(OBJDIR)/%.o:  $(SRC)/%.cc $(IDIR)/%.h | dir
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRC)/main.cc | dir
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/test.o: $(SRC)/test.cc | dir
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/test_main.o: $(SRC)/test_main.cc | dir
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $< -o $@


dir:
	@mkdir -p $(OBJDIR)

# Include automatically generated dependency files
-include $(wildcard $(OBJDIR)/*.d)

# 'make clean' removes object files and memory dumps.
clean:
	@ \rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.d *.gch core

# 'make zap' also removes the executable and backup files.
zap: clean
	@ \rm -rf *.out *~