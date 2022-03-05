all: exe

exe:
	g++ -o irFlip2 main.cc lodepng.cc

test: exe
	./irFlip2 in.png RRRA
	./irFlip2 in.png RGAA
	./irFlip2 in.png RbaA
	./irFlip2 in.png aRRA
