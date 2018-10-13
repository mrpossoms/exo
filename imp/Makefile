CXX=g++
FLAGS=-std=c++11
INC=-Iinc/

obj:
	mkdir obj

obj/%.o: src/%.cpp obj
	$(CXX) $(INC) $(FLAGS) -c $< -o $@

clean:
	rm -rf obj
