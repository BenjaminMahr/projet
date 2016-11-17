sdlanim:	sdlanim.c
	g++  sdlanim.c `sdl-config --cflags --libs` -o sdlanim

clean:
	rm sdlanim
