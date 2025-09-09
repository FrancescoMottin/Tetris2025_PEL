#include "tetris.hpp"
#include <sstream>
#include <fstream>
#include <vector>

void divider(int x){
    std::cout << std::endl;
    for(; x>0; x--){
        std::cout << "-";
    }
    std::cout << std::endl;
}

void skipM(std::istream& is) {
    char c = 0;
    is >> c;
    is.putback(c);
}

// funzione di supporto per stampare ogni mossa
void insert_and_print(tetris& t, piece& p, int x) {
    std::cout << "\nInserting piece at x = " << x << ":\n";
    p.print_ascii_art(std::cout);
    t.insert(p, x);
    std::cout << "State of tetris after insertion:\n";
    t.print_ascii_art(std::cout);
    std::cout << t << "\n";
}

void testH() {
    try {
        std::cout << "TestH Morris test\n" << std::endl;
        //std::ifstream file("input2.txt");
        std::ifstream file("Test/input1.txt");
        //tetris t1(3, 6, 0); (input 2)
        tetris t1(8, 7, 0); //input 1

        piece p;
        int x;

        // ciclo che legge coppie (pezzo, colonna) finché ci sono
        skipM(file);
        while (file) {
            file >> p >> x;
            insert_and_print(t1, p, x); 
            skipM(file);
        }

    } catch (const tetris_exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

/*
void testH() {
    try {
        std::cout << "TestH Morris test\n" << std::endl;
        std::ifstream file("input2.txt");
        tetris t1(3, 6, 0);

        piece p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11;
        int x;    

        file >> p1;
        file >> x;
        t1.insert(p1, x);

        file >> p2;
        file >> x;
        t1.insert(p2, x);

        file >> p3;
        file >> x;
        t1.insert(p3, x);

        file >> p4;
        file >> x;
        t1.insert(p4, x);

        file >> p5;
        file >> x;
        t1.insert(p5, x);

        file >> p6;
        file >> x;
        t1.insert(p6, x);

        file >> p7;
        file >> x;
        t1.insert(p7, x);

        file >> p8;
        file >> x;
        t1.insert(p8, x);

        file >> p9;
        file >> x;
        t1.insert(p9, x);

        file >> p10;
        file >> x;
        t1.insert(p10, x);

        file >> p11;
        file >> x;
        t1.insert(p11, x);

        std::cout << t1;
        t1.print_ascii_art(std::cout);
    } catch (const tetris_exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}
*/

void testG() {
    try{
        std::cout << "Test G - TEST Tetris" << std::endl;
        piece p;
        piece p2;
        piece p3;

        divider(100);
        //PARSER PIECE
        {
            std::ifstream file("Test/piece4.txt");
            std::ifstream file2("Test/piece5.txt");
            std::ifstream file3("Test/piece6.txt");

            if (!file || !file2 || !file3) {
                std::cerr << "Errore nell'aprire il file!" << std::endl;
                file.close();
                file2.close();
                file3.close();
            }
            else{

                file >> p;
                file2 >> p2;
                file3 >> p3;

                std::cout << "PARSER PIECE p" << std::endl;
                p.print_ascii_art(std::cout);
                //std::cout << p << std::endl;

                std::cout << "PARSER PIECE p2" << std::endl;
                p2.print_ascii_art(std::cout);
                //std::cout << p2 << std::endl;

                std::cout << "PARSER PIECE p3" << std::endl;
                p3.print_ascii_art(std::cout);
                //std::cout << p3 << std::endl;

                file.close();
                file2.close();
                file3.close();
            }
        }
        divider(100);

        //CONTRUCTORS
        {
            std::cout << "DEFAULT CONSTRUCTOR -> t1()" << std::endl;
            tetris t1;

            std::cout << "FANCY CONSTRUCTOR -> t2(5, 4, 0)" << std::endl;
            tetris t2(5, 4, 0);
            t2.print_ascii_art(std::cout);
            std::cout << "added element" << std::endl;
            t2.add(p2, 3,3);
            t2.print_ascii_art(std::cout);

            std::cout << "COPY CONSTRUCTOR -> t3(t2)" << std::endl;
            tetris t3(t2);
            std::cout << "T2" << std::endl;
            t2.print_ascii_art(std::cout);
            std::cout << "T3" << std::endl;
            t3.print_ascii_art(std::cout);

            std::cout << "MOVE CONSTRUCTOR -> tetris t4 = std::move(t3)" << std::endl;
            tetris t4 = std::move(t3);
            std::cout << "T3" << std::endl;
            t3.print_ascii_art(std::cout);
            std::cout << "T4" << std::endl;
            t4.print_ascii_art(std::cout);

        }
        divider(100);

        //ASSIGNMENT OPERATORS
        {
            std::cout << "ASSIGNMENT OPERATORS" << std::endl;
            std::cout << "T1" << std::endl;
            tetris t1(5, 4, 0);
            t1.add(p, 0,1);
            t1.print_ascii_art(std::cout);

            std::cout << "T2" << std::endl;
            tetris t2(5, 4, 0);
            t2.add(p2, 3,3);
            t2.print_ascii_art(std::cout);

            std::cout << "ASSIGNMENT OPERATOR COPY -> t1 = t2" << std::endl;
            t1 = t2;
            std::cout << "T1" << std::endl;
            t1.print_ascii_art(std::cout);
            std::cout << "T2" << std::endl;
            t2.print_ascii_art(std::cout);

            std::cout << "ASSIGNMENT OPERATOR MOVE -> t2 = std::move(t1)" << std::endl;
            t2 = std::move(t1);
            std::cout << "T1" << std::endl;
            t1.print_ascii_art(std::cout);
            std::cout << "T2" << std::endl;
            t2.print_ascii_art(std::cout);
        }
        divider(100);


        //COMPARISON OPERATORS
        {
            std::cout << "COMPARISON OPERATORS" << std::endl;
            std::cout << "T1" << std::endl;
            tetris t1(5, 4, 0);
            t1.add(p, 0,1);
            t1.print_ascii_art(std::cout);

            std::cout << "T2" << std::endl;
            tetris t2(5, 4, 0);
            t2.add(p2, 3,3);
            t2.print_ascii_art(std::cout);

            std::cout << "t1 == t1?: " << (t1==t1) << std::endl;
            std::cout << "t1 != t1?: " << (t1!=t1) << std::endl;
            std::cout << "t1 == t2?: " << (t1==t2) << std::endl;
            std::cout << "t1 != t2?: " << (t1!=t2) << std::endl;
        }
        divider(100);


        //CONTAINMENT
        {
            std::cout << "CONTAIMENT" << std::endl;
            std::cout << "T1" << std::endl;
            tetris t1;
            t1.print_ascii_art(std::cout);

            std::cout << "T2" << std::endl;
            tetris t2(5, 4, 0);
            t2.print_ascii_art(std::cout);

            std::cout << "P1" << std::endl;
            p.print_ascii_art(std::cout);

            std::cout << "P2" << std::endl;
            p2.print_ascii_art(std::cout);

            std::cout << "T2 contiene P1(x=1, y=2)? " << t2.containment(p, 1, 2) << " (dovrebbe essere 1)" << std::endl;
            t2.add(p, 1,2);
            t2.print_ascii_art(std::cout);
            std::cout << t2 << std::endl;

            std::cout << "T2 contiene P2(x=4, y=3)? " << t2.containment(p2, 4, 3) << " (dovrebbe essere 0)" << std::endl;
            std::cout << "T2 contiene P2(x=3, y=3)? " << t2.containment(p2, 3, 3) << " (dovrebbe essere 1)" << std::endl;
            t2.add(p2, 3,3);
            t2.print_ascii_art(std::cout);
            std::cout << t2 << std::endl;
        }
        divider(100);


        //INSERT
        {
            std::cout << "INSERT" << std::endl;
            std::cout << "T1 BEFORE" << std::endl;
            tetris t1(8, 8, 0);
            t1.print_ascii_art(std::cout);

            std::cout << "T1 AFTER INSERTING P3" << std::endl;
            t1.insert(p3, 0);
            t1.print_ascii_art(std::cout);
        }
        divider(100);

        //PARSER TETRIS (usa l'add)
        {
            tetris t;
            std::ifstream file("Test/tetris3.txt");

            if (!file) {
                std::cerr << "Errore nell'aprire il file!" << std::endl;
                file.close();
            }
            else{

                file >> t;
                t.print_ascii_art(std::cout);
                std::cout << t << std::endl;

                file.close();
            }
        }
        divider(100);

        //ITERATORS
        {
            tetris t;
            std::ifstream file("Test/tetris3.txt");

            if (!file) {
                std::cerr << "Errore nell'aprire il file!" << std::endl;
                file.close();
            }
            else{

                file >> t;
                t.print_ascii_art(std::cout);
                std::cout << t << std::endl;

                file.close();
            }

            t.insert(p2, 0);
            t.print_ascii_art(std::cout);

            std::cout << "ITERATORS" << std::endl;
            for(tetris::iterator index = t.begin(); index != t.end(); ++index){
                std::cout << index->p << "(x = " << index->x << ", y = " << index->y << ")" << std::endl;
            }

            std::cout << "CONST ITERATORS" << std::endl;
            const tetris t2 = t;
            for(tetris::const_iterator index = t2.begin(); index != t2.end(); ++index){
                std::cout << index->p << "(x = " << index->x << ", y = " << index->y << ")" << std::endl;
            }
        }
        divider(100);
    }
    catch(const tetris_exception& e){
        std::cerr << e.what() << std::endl;
    }    
}

void run_test(int test_number, const std::string& input_data, bool should_fail) {
    std::cout << "\033[36m=== TEST " << test_number << " ===\033[0m" << std::endl;
    std::cout << "Input:\n\"" << input_data << "\"\n" << std::endl;

    std::istringstream iss(input_data);
    piece p;
    try {
        iss >> p;
        std::cout << "piece pieno: " << std::boolalpha << p.full() << std::endl;
        std::cout << "piece vuoto: " << std::boolalpha << p.empty() << std::endl;
        p.print_ascii_art(std::cout);
        if (should_fail) {
            std::cout << "Test " << test_number << " FALLITO (doveva lanciare eccezione)\n" << std::endl;
            assert(false);
        } else {
            std::cout << "Test " << test_number << " passato\n" << std::endl;
        }
    }
    catch (const tetris_exception& e) {
        if (should_fail) {
            std::cout << "Eccezione prevista: " << e.what() << "\n" << std::endl;
            std::cout << "Test " << test_number << " passato\n" << std::endl;
        } else {
            std::cout << "Test " << test_number << " FALLITO (eccezione inaspettata): " << e.what() << "\n" << std::endl;
            assert(false);
        }
    }
    if(!should_fail) {
        std::cout << "Chiamo operator<< sul pezzo per vedere se coindice" << std::endl;
        std::cout << p << "\n" << std::endl;
    }
}
void testF() {
    std::cout << "Test F - TEST Parser Piece" << std::endl;
    run_test(1, "4 75 (([]()[]())(()[]()[])([]()()())(()[]()()))[]()(((mipiaceilcazzo)))", false);
    run_test(2, "  4 \t75  \n  ( (\n [ ]()[\n ]()) (( )[\n ]()[])([](\n )()(  )) ( ()[](\n  )() ) )", false);
    run_test(3, "8 90 (   []   (([]()[]()) () ([]()[]()) ())   ([][] ([]()[]())  ()) ()   )", false);
    run_test(9, "2 115 ()", false);
    run_test(10, "2 115 []", false);

    // Test invalidi
    run_test(4, "-4 75 (([]()[]())(()[]()[])([]()()())(()[]()()))", true);     // side negativo
    run_test(5, "4 75.5 (([]()[]())(()[]()[])([]()()())(()[]()()))", true);   // color float
    run_test(6, "4 0 (([]()[]())(()[]()[])([]()()())(()[]()()))", true);     // color = 0, fuori range
    run_test(7, "5 0 (([]()[]())(()[]()[])([]()()())(()[]()()))", true);    //side non potenza di 2
    run_test(8, "2 90 (   []   (([]()[]()) () ([]()[]()) ())   ([][] ([]()[]())  ()) ()   )", true); ////side piccola

    std::cout << "\033[36m=== Riepilogo Finale ===\033[0m" << std::endl;
    std::cout << "Tutti i test eseguiti correttamente" << std::endl;
}

void testE() {
    std::cout << "Test E - TEST Piece" << std::endl;
    try {
        // =============================
        // TEST Costruttore di default + move assignment
        // =============================
        piece p1;
        {
            piece temp(4, 1);
            p1 = std::move(temp);
            std::cout << "TEST Costruttore di default e move assignment: Superato\n";
            std::cout << "Creato p1: empty=" << std::boolalpha << p1.empty()
                      << " full=" << p1.full() << "\n";
        }

        // =============================
        // TEST Operatore () per impostare celle
        // =============================
        p1(0,1) = true;
        p1(0,2) = p1(1,1) = p1(1,2) = p1(2,1) = p1(2,2) = true;
        p1(3,0) = p1(3,1) = p1(3,2) = p1(3,3) = true;
        std::cout << "\nTEST Operatore() per impostare celle: Superato\n";
        p1.print_ascii_art(std::cout);

        // =============================
        // TEST Copy constructor
        // =============================
        piece p3(p1);
        std::cout << "\nTEST Copy constructor:\n";
        std::cout << "p3 == p1: " << std::boolalpha << (p3 == p1) << "\n";
        if (!(p3 == p1)) throw std::runtime_error("Copy constructor fallito");
        std::cout << "Superato\n";

        // =============================
        // TEST Copy assignment
        // =============================
        piece p4(4, 2);
        p4 = p1;
        std::cout << "\nTEST Copy assignment:\n";
        std::cout << "p4 == p1: " << std::boolalpha << (p4 == p1) << "\n";
        if (!(p4 == p1)) throw std::runtime_error("Copy assignment fallito");
        std::cout << "Superato\n";

        // =============================
        // TEST Self-assignment
        // =============================
        p4 = p4;
        std::cout << "\nTEST Self-assignment: Superato\n";

        // =============================
        // TEST p2 diverso
        // =============================
        piece p2(4, 3);
        for (uint32_t r = 1; r < p2.side(); r++)
            for (uint32_t c = 0; c < p2.side(); c++)
                p2(r,c) = true;

        std::cout << "\nTEST p2 diverso:\n";
        p2.print_ascii_art(std::cout);
        std::cout << "empty(0,2,2): " << p2.empty(0,2,2)
                  << " full(0,1,3): " << p2.full(0,1,3) << "\n";
        std::cout << "p2 == p1: " << (p2 == p1) << "\n";
        std::cout << "p2 != p1: " << (p2 != p1) << "\n";

        // =============================
        // TEST assegnamento p2 = p1
        // =============================
        p2 = p1;
        std::cout << "\nDopo p2 = p1:\n";
        std::cout << "p2 == p1: " << (p2 == p1) << "\n";

        // =============================
        // TEST rotazione
        // =============================
        std::cout << "\nTEST rotazione:\n";
        p2.rotate();
        p2.print_ascii_art(std::cout);
        std::cout << "p2 == p1: " << (p2 == p1) << "\n";

        // =============================
        // TEST 4 rotazioni
        // =============================
        piece p5(p1);
        for (int i = 0; i < 4; ++i) p5.rotate();
        if (!(p5 == p1)) throw std::runtime_error("4 rotazioni non tornano allo stato iniziale");
        std::cout << "TEST 4 rotazioni: Superato\n";

        // =============================
        // TEST cut_row
        // =============================
        for (uint32_t i = 0; i < p2.side(); i++) {
            std::cout << "\nCutting row: " << i << "\n";
            p2.cut_row(i);
            p2.print_ascii_art(std::cout);
        }
        std::cout << "empty dopo cut_row: " << p2.empty() << "\n";
        std::cout << "TEST cut_row: Superato\n";

        // =============================
        // TEST operator() out of bounds
        // =============================
        std::cout << "\nTEST operator() out of bounds:\n";
        try {
            p1(99, 0) = true;
            throw std::runtime_error("Eccezione NON lanciata (riga)");
        } catch (tetris_exception& e) {
            std::cout << "Eccezione correttamente catturata (riga): " << e.what() << "\n";
        }

        try {
            p1(0, 99) = true;
            throw std::runtime_error("Eccezione NON lanciata (colonna)");
        } catch (tetris_exception& e) {
            std::cout << "Eccezione correttamente catturata (colonna): " << e.what() << "\n";
        }

        // =============================
        // TEST cut_row() out of bounds
        // =============================
        std::cout << "\nTEST cut_row() out of bounds:\n";
        try {
            p1.cut_row(99);
            throw std::runtime_error("Eccezione NON lanciata da cut_row()");
        } catch (tetris_exception& e) {
            std::cout << "Eccezione correttamente catturata da cut_row(): " << e.what() << "\n";
        }

        // =============================
        // TEST costruttori invalidi
        // =============================
        std::cout << "\nTEST costruttori con parametri invalidi:\n";
        try {
            piece bad1(5, 2);
            throw std::runtime_error("Eccezione NON lanciata per side non valido");
        } catch (tetris_exception& e) {
            std::cout << "Eccezione per side non valido: " << e.what() << "\n";
        }

        try {
            piece bad2(4, 0);
            throw std::runtime_error("Eccezione NON lanciata per color == 0");
        } catch (tetris_exception& e) {
            std::cout << "Eccezione per color == 0: " << e.what() << "\n";
        }

        std::cout << "\nTutti i test superati con successo!\n";
    }
    catch (std::exception& e) {
        std::cout << "\nProgramma terminato prematuramente: " << e.what() << "\n";
    }
}

void testD() {
    try{
        std::cout << "Test D - TEST MAGGGGICO 1+2" << std::endl;
        piece p;
        piece p2;
        piece p3;

        divider(100);
        //PARSER PIECE
        {
            std::ifstream file("Test/piece4.txt");
            std::ifstream file2("Test/piece5.txt");
            std::ifstream file3("Test/piece6.txt");

            if (!file || !file2 || !file3) {
                std::cerr << "Errore nell'aprire il file!" << std::endl;
                file.close();
                file2.close();
                file3.close();
            }
            else{

                file >> p;
                file2 >> p2;
                file3 >> p3;

                std::cout << "PARSER PIECE p" << std::endl;
                p.print_ascii_art(std::cout);
                //std::cout << p << std::endl;

                std::cout << "PARSER PIECE p2" << std::endl;
                p2.print_ascii_art(std::cout);
                //std::cout << p2 << std::endl;

                std::cout << "PARSER PIECE p3" << std::endl;
                p3.print_ascii_art(std::cout);
                //std::cout << p3 << std::endl;

                file.close();
                file2.close();
                file3.close();
            }
        }
        divider(100);

        //TEST MAGGGGICO
        {
            std::cout << "TEST MAGGGGICO" << std::endl;

            std::cout << "PIECE 1" << std::endl;
            piece piece_bello;
            std::istringstream file("4 32 ((()()()[])(()()[]())[][])");
            file >> piece_bello;
            piece_bello.print_ascii_art(std::cout);

            std::cout << "TETRIS DEFAULT" << std::endl;
            tetris t(8, 8, 0);
            t.print_ascii_art(std::cout);

            std::cout << "INSERISCO PIECE 1" << std::endl;
            t.insert(piece_bello, 0);
            t.print_ascii_art(std::cout);

            std::cout << "PIECE 2" << std::endl;
            piece piece_bello2;
            std::istringstream file2("1 83 ()");
            file2 >> piece_bello2;
            piece_bello2.print_ascii_art(std::cout);

            std::cout << "INSERISCO PIECE 2" << std::endl;
            t.insert(piece_bello2, 1);
            t.print_ascii_art(std::cout);
        }
        divider(100);

        //TEST MAGGGGICO 2
        {
            std::cout << "TEST MAGGGGICO 2" << std::endl;

            //PIECE
            std::cout << "PIECE 1" << std::endl;
            piece piece_bello_1;
            std::istringstream file1("4 1 ([][](()[]()())([][]()[]))");
            file1 >> piece_bello_1;
            piece_bello_1.print_ascii_art(std::cout);

            std::cout << "PIECE 2" << std::endl;
            piece piece_bello_2;
            std::istringstream file2("2 220 ([][]()())");
            file2 >> piece_bello_2;
            piece_bello_2.print_ascii_art(std::cout);

            std::cout << "PIECE 3" << std::endl;
            piece piece_bello_3;
            std::istringstream file3("4 21 (([][]()[])[](()()()[])[])");
            file3 >> piece_bello_3;
            piece_bello_3.print_ascii_art(std::cout);

            std::cout << "PIECE 4" << std::endl;
            piece piece_bello_4;
            std::istringstream file4("4 128 ([]([][][]())[](()()[]()))");
            file4 >> piece_bello_4;
            piece_bello_4.print_ascii_art(std::cout);

            std::cout << "PIECE 5" << std::endl;
            piece piece_bello_5;
            std::istringstream file5("4 10 ([]([]()[]())[]([]()[]()))");
            file5 >> piece_bello_5;
            piece_bello_5.print_ascii_art(std::cout);

            std::cout << "PIECE 6" << std::endl;
            piece piece_bello_6;
            std::istringstream file6("4 14 ([][](()()()[])(()[]()[]))");
            file6 >> piece_bello_6;
            piece_bello_6.print_ascii_art(std::cout);

            std::cout << "PIECE 7" << std::endl;
            piece piece_bello_7;
            std::istringstream file7("1 13 ()");
            file7 >> piece_bello_7;
            piece_bello_7.print_ascii_art(std::cout);

            //serve per insert
            std::cout << "PIECE 8" << std::endl;
            piece piece_bello_8;
            std::istringstream file8("1 88 ()");
            file8 >> piece_bello_8;
            piece_bello_8.print_ascii_art(std::cout);

            //TETRIS
            std::cout << "TETRIS DEFAULT" << std::endl;
            tetris t1(9, 9, 0);
            tetris t2(9, 9, 0);
            t1.print_ascii_art(std::cout);

            std::cout << "TETRIS CRESCENTE" << std::endl;
            //AGGIUNTI IN ORDINE CRESCENTE
            t1.add(piece_bello_1, 0, 5);
            t1.add(piece_bello_2, 4, 5);
            //t1.add(piece_bello_8, 6, 5);
            t1.add(piece_bello_3, 3, 5);
            t1.add(piece_bello_4, 4, 5);
            t1.add(piece_bello_5, 5, 5);
            t1.add(piece_bello_6, 4, 3);
            t1.add(piece_bello_7, 5, 1);

            t1.print_ascii_art(std::cout);

            std::cout << "TETRIS DECRESCENTE" << std::endl;
            //AGGIUNTI IN ORDINE DECRESCENTE
            t2.add(piece_bello_7, 5, 1);
            t2.add(piece_bello_6, 4, 3);
            t2.add(piece_bello_5, 5, 5);
            t2.add(piece_bello_4, 4, 5);
            t2.add(piece_bello_3, 3, 5);
            t2.add(piece_bello_2, 4, 5);
            t2.add(piece_bello_1, 0, 5);

            t2.print_ascii_art(std::cout);

            std::cout << "TETRIS CRESCENTE" << std::endl;
            t1.insert(piece_bello_8, 5);
            t1.print_ascii_art(std::cout);

            std::cout << "TETRIS DECRESCENTE" << std::endl;
            t2.insert(piece_bello_8, 5);
            t2.print_ascii_art(std::cout);
        }
        divider(100);
    }
    catch(const tetris_exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void testC() {
    try {
        std::cout << "Test C - Play game tetris 2 GAME OVER" << std::endl;
        tetris t(6, 8, 0);

        std::istringstream is("2 202 ()");
        piece p;
        is >> p;

        std::istringstream is1("2 170 ()");
        piece p1;
        is1 >> p1;

        std::istringstream is2("2 120 ()");
        piece p2;
        is2 >> p2;

        std::istringstream is3("4 195 ()");
        piece p3;
        is3 >> p3;

        // PASSO 1
        std::cout << "\nPASSO 1\n";

        p.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p1.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 2\n";
        t.insert(p1, 2);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p3.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 2\n";
        t.insert(p3, 2);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p2.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 4\n";
        t.insert(p2, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::cout << "\nPASSO 2 GAME OVER\n";
        p.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 4\n";
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;
    }
    catch (tetris_exception e) {
        std::cout << e.what();
    }
}

void testB() {
    try {
        std::cout << "Test B - Play game tetris 2" << std::endl;
        tetris t(6, 8, 0);

        std::istringstream is("2 202 ()");
        piece p;
        is >> p;

        std::istringstream is1("2 170 ()");
        piece p1;
        is1 >> p1;

        std::istringstream is2("2 120 ()");
        piece p2;
        is2 >> p2;

        std::istringstream is3("4 195 ()");
        piece p3;
        is3 >> p3;

        // PASSO 1
        std::cout << "\nPASSO 1\n";

        p.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p1.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 2\n";
        t.insert(p1, 2);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p3.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 2\n";
        t.insert(p3, 2);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p2.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 4\n";
        t.insert(p2, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;

        // PASSO 2
        std::cout << "\nPASSO 2\n";

        p.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        /*
        std::cout << "\nPASSO 2 GAME OVER\n";
        p.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 4\n";
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;
        */

        // PASSO 3
        std::cout << "\nPASSO 3\n";

        p1.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 0\n";
        t.insert(p1, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        p.print_ascii_art(std::cout);
        std::cout << "inserting piece at x = 4\n";
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;
    }
    catch (tetris_exception e) {
        std::cout << e.what();
    }
}

void testA() {
    try{
        std::cout << "Test A - Play game tetris 1" << std::endl;
        std::ifstream file("Test/input_tetris.txt");
        tetris t;
        file >> t;
        t.print_ascii_art(std::cout);
        file.close();

        std::istringstream is("2 202 () 2 202 ()");
        piece p;
        is >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is1("4 100 ((()()[][])(()[]()[])[][])");
        is1 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is2("4 100 ([][](()()[][])(()[]()[]))");
        is2 >> p;
        p.rotate();
        p.rotate();
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 4\n";
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;

        is >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 6\n";
        t.insert(p, 6);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is3("4 32 (([][]()())[](()[]()[])[])");
        is3 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 4\n";
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is5("2 11 ()");
        is5 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
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
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is7("8 123 ([][](([][]()())([][]()())()())[])");
        is7 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 3\n";
        t.insert(p, 3);
        t.print_ascii_art(std::cout);
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
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is9("8 29 ([][][]([][][]([]()[][])))");
        is9 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is10("8 30 ([][][]([][][]([]()[][])))");
        is10 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is11("8 31 ([][][]([][][]([]()[][])))");
        is11 >> p;
        std::cout << "\n";
        p.print_ascii_art(std::cout);
        std::cout << "insertint piece at x = 0\n";
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
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
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "        PEL PROJECT 2024/2025 - TETRIS   \n";
    std::cout << "=========================================\n\n";

    while (true) {
        std::cout << "Seleziona il test da eseguire:\n";
        std::cout << "  1 -> Test A - Play game tetris 1\n";
        std::cout << "  2 -> Test B - Play game tetris 2\n";
        std::cout << "  3 -> Test C - Play game tetris 2 GAME OVER\n";
        std::cout << "  4 -> Test D - TEST MAGGGGICO 1+2\n";
        std::cout << "  5 -> Test E - TEST Piece\n";
        std::cout << "  6 -> Test F - TEST Parser Piece\n";
        std::cout << "  7 -> Test G - Test G - TEST Tetris\n";
        std::cout << "  8 -> Test H - Test H - TEST Morris\n";
        std::cout << "  0 -> Esci\n";
        std::cout << "\nScelta: ";

        int test = -1;
        std::cin >> test;

        if (std::cin.fail()) {
            std::cout << "\nInput non valido! Inserisci un numero tra 0 e 3.\n";
            std::cout << "Uscita dal programma.\n";
            break;
        }

        if (test == 0) {
            std::cout << "\nUscita dal programma.\n";
            break;
        }

        if (test < 0 || test > 8) {
            std::cout << "\nScelta non valida! Inserisci un numero tra 0 e 8.\n\n";
            continue;
        }

        divider(100);
        std::cout << "Hai selezionato il test: " << test << "\n\n";

        switch(test) {
            case 1: testA(); break;
            case 2: testB(); break;
            case 3: testC(); break;
            case 4: testD(); break;
            case 5: testE(); break;
            case 6: testF(); break;
            case 7: testG(); break;
            case 8: testH(); break;
        }

        divider(100);
        std::cout << "\nTest terminato.\n\n";
    }

    return 0;
}