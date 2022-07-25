#top_level makefile to build all.

DEPAND1 = lib
DEPAND2 = prog
DEPAND3 = include

all: $(DEPAND1) $(DEPAND2) $(DEPAND3)
	(cd $(DEPAND1); make)
	(cd $(DEPAND2); make)

clean:
	(cd $(DEPAND1); make clean)
	(cd $(DEPAND2); make clean)
	
