#include <iostream>
#include <vector>

// http://jessicarbrown.com/images/ft-cpp-loop.gif
int main()
{
    
    // non-c++11 syntax:
    
    // 1. create a for loop that prints numbers 1 to 5
	std::cout << "Ejercicio 1: " << std::endl;

	for(int i = 1; i <= 5; i++) {
		std::cout << i << std::endl;
	}

	std::cout << "-------------" << std::endl;
	std::cout << "Ejercicio 2: " << std::endl;

    // 2. create a for loop that iterates through a vector of strings and print
    // them
    
    std::vector<std::string> vec;
    vec.push_back("uno");
    vec.push_back("dos");
    vec.push_back("tres");
    vec.push_back("cuatro");
    vec.push_back("cinco");

	for(int i = 0; i < (int)vec.size(); i++) {
        std::cout << vec[i] << std::endl;
    }

	std::cout << "-------------" << std::endl;
	std::cout << "Ejercicio 3: " << std::endl;

    // 3. create a for loop that gets a map of <string, float> and returns a
    // new map of of (char*, int)
    
	std::map<std::string, float> the_map = {{"zero", 0}, {"uno", 1}, {"dos", 2}};
	for(auto kv : the_map) {
        std::cout << kv.first << ", " << kv.second << std::endl;
		//TODO: Nuevo formato.
    }
	std::cout << "-------------" << std::endl;
	std::cout << "Ejercicio 4: " << std::endl;

    // 4. create a for loop that gets a map of <string, float> and updates the
    // same map so that it adds "yeah!" to each string and rounds the float
    // number. transform it in place.

	for(auto kv : the_map) {
        std::cout << kv.first << ", " << kv.second << std::endl;
		//TODO: Nuevo formato.
    }

	std::cout << "-------------" << std::endl;
	std::cout << "Ejercicio 5: " << std::endl;

    //----------------------------------------------------------------------
        
    // 5. transform every previous exercise to ranged for loops. use as much
    // c++11 syntax as you can
    
	std::cout << "-------------" << std::endl;
	std::cout << "Ejercicio 6: " << std::endl;

    // 6. write a for loop that prints a pyramid of height N, like this:    
    //	
    //	               **
    //	              ****
    //	             ******
    //	            ********
    //	           **********
    //	          ************
    
}

//Recursive function that create a piramid with height N.
std::string crearPiramide (int size, int totalSize) {
	return crearPiramide(size - 1,totalSize);
}