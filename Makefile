CFLAGS = -Wall -O2
CPPFLAGS = -Iblossom5
OBJDIR = output

OBJ_C = $(OBJDIR)/operacoesGrafo.o $(OBJDIR)/main.o $(OBJDIR)/christofides.o $(OBJDIR)/heldkarp.o
OBJ_CPP = $(OBJDIR)/wrapper_blossom.o \
           blossom5/misc.o blossom5/PMduals.o blossom5/PMexpand.o blossom5/PMinit.o \
           blossom5/PMinterface.o blossom5/PMmain.o blossom5/PMrepair.o blossom5/PMshrink.o \
           blossom5/MinCost/MinCost.o blossom5/GEOM/GPMinit.o blossom5/GEOM/GPMinterface.o \
           blossom5/GEOM/GPMkdtree.o blossom5/GEOM/GPMmain.o

all: tsp

$(OBJDIR):
	mkdir -p $(OBJDIR)

tsp: $(OBJ_C) $(OBJ_CPP)
	g++ $(OBJ_C) $(OBJ_CPP) -o tsp

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	gcc -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	g++ -c $< -o $@ $(CPPFLAGS)

clean:
	rm -rf $(OBJDIR)
	rm -f tsp tsp.exe
