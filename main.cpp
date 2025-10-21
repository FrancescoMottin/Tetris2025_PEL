#include "tetris.cpp"

#include <sstream>
#include <fstream>
#include <vector>


void stampa(tetris& t) {
    std::cout << "Hello";
	for(auto it = t.begin(); it != t.end(); it++) {
		it->p.print_ascii_art(std::cout);
	}
	std::cout << "\n";
	
	field f(t);
	
	for(uint32_t i = 0; i < f.t.height(); i++) {
		for(uint32_t j = 0; j < f.t.width(); j++) {
			std::cout << f.f[i][j] << " ";
		}
		std::cout << "\n";
	}
};

int main(/*int argc, char **argv*/) {
	 try{
        std::cout << "Test A - Play game tetris 1" << std::endl;
        std::ifstream file("Test/input_tetris.txt");
        tetris t;
        file >> t;
        //t.print_ascii_art(std::cout);
        stampa(t);
        file.close();
		
        std::istringstream is("2 202 ()");
        piece p;
        is >> p;
        std::cout << "\n";
        std::cout << "insertint piece at x = 0\n";
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is1("4 100 ((()()[][])(()[]()[])[][])");
        is1 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is2("4 100 ([][](()()[][])(()[]()[]))");
        is2 >> p;
        p.rotate();
        p.rotate();
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 4\n";
        t.insert(p, 4);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        is >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 6\n";
        t.insert(p, 6);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is3("4 32 (([][]()())[](()[]()[])[])");
        is3 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 4\n";
        t.insert(p, 4);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is5("2 11 ()");
        is5 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        /*
        std::cout << "/n test fall pieces";
        std::istringstream is55("1 11 ()");
        std::istringstream is555("1 30 ()");
        is55 >> p;
        t.insert(p, 3);
        is555 >> p;
        t.insert(p, 3);
        t.print_ascii_art(std::cout);
        */

        std::istringstream is6("1 11 ()");
        is6 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 2\n";
        t.insert(p, 2);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is7("8 123 ([][](([][]()())([][]()())()())[])");
        is7 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 3\n";
        t.insert(p, 3);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        /*
        std::istringstream is8("8 123 ([][](([][]()())([][]()())()())[])");
        is8 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 3\n";
        t.insert(p, 3);
        t.print_ascii_art(std::cout);
        std::cout << t;
        */

        std::istringstream is8("8 28 ([][][]([][][]([]()[][])))");
        is8 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is9("8 29 ([][][]([][][]([]()[][])))");
        is9 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is10("8 30 ([][][]([][][]([]()[][])))");
        is10 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        std::istringstream is11("8 31 ([][][]([][][]([]()[][])))");
        is11 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        //t.print_ascii_art(std::cout);
        stampa(t);
        std::cout << t;

        /*std::istringstream is11("4 15 (([][]()())([][]()[])[](()[]()[]))");
        is11 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;*/
    }
    catch(tetris_exception e){
        std::cout << e.what();
    }
	
	return 0;
}

