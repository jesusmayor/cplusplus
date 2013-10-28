#include <iostream>
#include <vector>
#include <map>

int round( float);
std::string crearPiramide (int);
std::string crearPiramide (int, int);

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

	// 2. create a for loop that iterates through a vector of strings and print
    // them
	std::cout << "Ejercicio 2: " << std::endl;


	{
		std::vector<std::string> vec;
		vec.push_back("uno");
		vec.push_back("dos");
		vec.push_back("tres");
		vec.push_back("cuatro");
		vec.push_back("cinco");

		for(int i = 0; i < (int)vec.size(); i++) {
			std::cout << vec[i] << std::endl;
		}
	}
	std::cout << "-------------" << std::endl;

	// 3. create a for loop that gets a map of <string, float> and returns a
    // new map of of (char*, int)
	std::cout << "Ejercicio 3: " << std::endl;
	{
		std::map<std::string, float> theMap;
		theMap.insert (std::make_pair("zero", 0));
		theMap.insert (std::make_pair("uno", 1));
		theMap.insert (std::make_pair("dos", 2));

		{
			std::map<char*, int> newMap;
			std::map<std::string, float>::iterator it = theMap.begin();
			for(; it!=theMap.end(); ++it) {
				char* pChar = new char[it->first.size() + 1];
				std::copy(it->first.begin(),it->first.end(), pChar);
				pChar[it->first.size()] = '\0'; // Don't forget the terminating 0
				newMap.insert (std::make_pair( pChar, (int)it->second));
				std::cout << it->first << ", " << it->second << std::endl;
			}
		}
		std::cout << "-------------" << std::endl;

		// 4. create a for loop that gets a map of <string, float> and updates the
		// same map so that it adds "yeah!" to each string and rounds the float
		// number. transform it in place.
		std::cout << "Ejercicio 4: " << std::endl;

		{
			std::map<std::string, float> newMap;
			{
    			std::map<std::string, float>::iterator it = theMap.begin();
    			for(; it!=theMap.end(); ++it) {
    				newMap.insert(std::make_pair( it->first + "yeah!",
					round(it->second)));
    			}
			}
			std::map<std::string, float>::iterator it = newMap.begin();
			for(; it!=newMap.end(); ++it) {
				std::cout << it->first << ", " << it->second << std::endl;
			}
		}
	}
	std::cout << "-------------" << std::endl;
	//----------------------------------------------------------------------
        
    // 5. transform every previous exercise to ranged for loops. use as much
    // c++11 syntax as you can
	std::cout << "Ejercicio 5: " << std::endl;

    //5.1
	for(int i = 1; i <= 5; i++) {
		std::cout << i << std::endl;
	}
	//5.2
	{
		std::vector<std::string> vec = {"uno","dos","tres","cuatro","cinco"};

		for(auto value : vec) {
			std::cout << value << std::endl;
		}
	}
	//5.3
	std::map<std::string, float> theMap = {{"zero", 0},{"uno", 1},{"dos", 2}};
	{
		std::map<char*, int> newMap;
		for(auto kp : theMap) {
			char* pChar = new char[kp.first.size() + 1];
			std::copy(kp.first.begin(),kp.first.end(), pChar);
			pChar[kp.first.size()] = '\0'; // Don't forget the terminating 0
			newMap.insert (std::make_pair( pChar, (int)kp.second));
			std::cout << kp.first << ", " << kp.second << std::endl;
		}
	}
	//5.4
	{
		std::map<std::string, float> newMap;
		{
    		for(auto kp : theMap) {
    			newMap.insert(std::make_pair( kp.first + "yeah!",
				round(kp.second)));
    			std::cout << kp.first << ", " << kp.second << std::endl;
    		}
		}
		//Se imprime para comprobar resultados, pero no hace falta.
		for(auto kp : newMap) {
			std::cout << kp.first << ", " << kp.second << std::endl;
		}
	}

	std::cout << "-------------" << std::endl;

	// 6. write a for loop that prints a pyramid of height N, like this:    
    //	
    //	               **
    //	              ****
    //	             ******
    //	            ********
    //	           **********
    //	          ************
	std::cout << "Ejercicio 6: " << std::endl;

	std::cout << crearPiramide (10) << std::endl;
}


std::string crearPiramide (int size) {
    return crearPiramide(size ,size);
}

//Funcion recursiva para construir la pirámide.
//La he hecho recursiva por rizar el rizo.
std::string crearPiramide (int size, int totalSize) {
    std::string str;
    if (size >= 0){
        for (int i = 0; i < totalSize; i++){
            if (i >= size) 
                str += "**";
            else 
                str += " ";
        }
    }else 
        return "";
    return  str + "\n" + crearPiramide(size - 1,totalSize) ;
}

int round( float r ) {
    return (r > 0.0) ? (r + 0.5) : (r - 0.5); 
}