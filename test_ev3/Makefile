all:
	gcc -o ev3 main.c ev3.c api.c program.c flux.c -lusb-1.0 -lrt
warn:
	gcc -Wall -o ev3 main.c ev3.c api.c program.c flux.c -lusb-1.0 -lrt
stop:
	gcc -Wall -o stop stopAll.c ev3.c api.c -lusb-1.0 -lrt
flux:
	gcc -Wall -o flux flux.c
me:
	touch xkcd_me
a:
	touch xkcd_a
sandwich:
	/home/dave/cmd/test_sandwich
