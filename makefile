debug:
	gcc batty.c -o batty -lpthread
	./batty -d
run:
	gcc batty.c -o batty -lpthread
	./batty
