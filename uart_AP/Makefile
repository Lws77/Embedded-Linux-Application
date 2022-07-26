#top_level makefile to build all.

DEPEND1 = lib
DEPEND2 = prog
DEPEND3 = include

all: $(DEPEND1) $(DEPEND2) $(DEPEND3)
	(cd $(DEPEND1); make)
	(cd $(DEPEND2); make)

clean:
	(cd $(DEPEND1); make clean)
	(cd $(DEPEND2); make clean)
	
