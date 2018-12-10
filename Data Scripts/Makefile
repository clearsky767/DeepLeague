cc = g++
prom = testMove2

$(prom): testMove2.cpp
	$(cc) testMove2.cpp -g -o $(prom) `pkg-config --cflags --libs opencv`

clean:
	rm -rf $(obj) $(prom)
