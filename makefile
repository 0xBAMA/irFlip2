all: exe

exe:
	g++ -o irFlip2 main.cc lodepng.cc

test: exe
	./irFlip2 example/in.png RRRA
	./irFlip2 example/in.png RGAA
	./irFlip2 example/in.png RbaA
	./irFlip2 example/in.png aRRA
