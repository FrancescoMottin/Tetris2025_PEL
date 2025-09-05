#include <iostream> // Per input/output su console
#include <sstream>  // Per testare gli operatori di stream in memoria
#include <string>   // Per std::string e std::getline
#include <cassert>  // Per asserzioni semplici (anche se i try-catch sono preferibili per le eccezioni)
#include "tetris.cpp" // Include l'implementazione delle classi piece e tetris

// Funzione helper per stampare il risultato di un test
void print_test_result(const std::string& test_name, bool passed) {
    std::cout << "Test: " << test_name << " - " << (passed ? "PASSED" : "FAILED") << std::endl;
}

// --- Test per la classe Piece ---

// Test dei costruttori validi
bool test_piece_valid_constructors() {
    bool passed = true;
    try {
        piece p1(1, 1);
        if (p1.side() != 1 || p1.color() != 1 || !p1.empty()) passed = false; // Deve essere vuoto all'inizio

        piece p2(4, 254);
        if (p2.side() != 4 || p2.color() != 254 || !p2.empty()) passed = false;

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_valid_constructors: " << e.what() << std::endl;
        passed = false;
    } catch (...) {
        std::cerr << "Eccezione sconosciuta in test_piece_valid_constructors." << std::endl;
        passed = false;
    }
    return passed;
}

// Test dei costruttori non validi (dovrebbero lanciare eccezioni)
bool test_piece_invalid_constructors() {
    bool passed = true;

    // Test side = 0
    try {
        piece p(0, 1);
        passed = false; // Fallisce se non lancia eccezione
    } catch (const tetris_exception& e) {
        // Eccezione attesa
    } catch (...) {
        passed = false; // Fallisce per eccezione sbagliata
    }

    // Test side non potenza di 2
    try {
        piece p(3, 100);
        passed = false; // Fallisce se non lancia eccezione
    } catch (const tetris_exception& e) {
        // Eccezione attesa
    } catch (...) {
        passed = false;
    }

    // Test color = 0
    try {
        piece p(2, 0);
        passed = false; // Fallisce se non lancia eccezione
    } catch (const tetris_exception& e) {
        // Eccezione attesa
    } catch (...) {
        passed = false;
    }

    return passed;
}

// Test dell'operatore di copia e assegnazione
bool test_piece_copy_assignment() {
    bool passed = true;
    try {
        piece p_original(2, 10);
        p_original(0,0) = true; // Rendi non vuota una cella

        piece p_copy = p_original; // Costruttore di copia
        if (!(p_copy == p_original)) passed = false;

        piece p_assign(4, 50); // Un altro pezzo
        p_assign = p_original; // Operatore di assegnazione
        if (!(p_assign == p_original)) passed = false;

        // Verifica deep copy: modifica la copia e controlla l'originale
        p_copy(0,0) = false;
        if (p_copy(0,0) == p_original(0,0)) passed = false; // Dovrebbero essere diversi

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_copy_assignment: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test dell'operatore di move e assegnazione
bool test_piece_move_assignment() {
    bool passed = true;
    try {
        piece p_original(2, 10);
        p_original(0,0) = true;
        piece p_original_copy = p_original; // Copia per confronto
        //std::cout << "Copia per confronto fatta" << std::endl;

        piece p_move = std::move(p_original); // Costruttore di move
        //std::cout << "Failstate 1" << std::endl;
        if (!(p_move == p_original_copy)) passed = false; // p_move deve essere uguale alla copia dell'originale
        //std::cout << "Failstate 2" << std::endl;
        if (!p_original.empty() || p_original.side() != 0) passed = false; // p_original deve essere in stato valido ma vuoto
        //std::cout << "Move constructor funzionante" << std::endl;

        piece p_assign_target(4, 50);
        p_assign_target = std::move(p_move); // Operatore di move assignment
        if (!(p_assign_target == p_original_copy)) passed = false; // p_assign_target deve essere uguale alla copia dell'originale
        if (!p_move.empty() || p_move.side() != 0) passed = false; // p_move deve essere in stato valido ma vuoto
        //std::cout << "Move assignment utilizzato" << std::endl;
    } 
    catch (const tetris_exception& e) 
    {
        std::cerr << "Eccezione in test_piece_move_assignment: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test operator() per accesso alle celle
bool test_piece_cell_access() {
    bool passed = true;
    try {
        piece p(2, 100);
        p(0,0) = true;
        p(1,1) = true;
        if (!p(0,0) || !p(1,1) || p(0,1) || p(1,0)) passed = false;

        // Test accesso fuori limiti (dovrebbe lanciare eccezione)
        try {
            p(2,0) = true; // Fuori limite
            passed = false; // Fallisce se non lancia eccezione
        } catch (const tetris_exception& e) {
            // Eccezione attesa
        } catch (...) {
            passed = false;
        }

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_cell_access: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test empty() e full()
bool test_piece_empty_full() {
    bool passed = true;
    try {
        piece p_empty(2, 100);
        if (!p_empty.empty()) passed = false;
        if (p_empty.full()) passed = false;

        piece p_full(1, 100);
        p_full(0,0) = true;
        if (p_full.empty()) passed = false;
        if (!p_full.full()) passed = false;

        // Test empty(i,j,s) e full(i,j,s)
        piece p_mixed(4, 100);
        p_mixed(0,0) = true; // Top-Left sub-quadrant has one full cell
        p_mixed(3,3) = true; // Bottom-Right sub-quadrant has one full cell

        // Top-Left (0,0,2) should be mixed
        if (p_mixed.empty(0,0,2) || p_mixed.full(0,0,2)) passed = false;
        // Top-Right (0,2,2) should be empty
        if (!p_mixed.empty(0,2,2)) passed = false;
        // Bottom-Left (2,0,2) should be empty
        if (!p_mixed.empty(2,0,2)) passed = false;
        // Bottom-Right (2,2,2) should be mixed
        if (p_mixed.empty(2,2,2) || p_mixed.full(2,2,2)) passed = false;

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_empty_full: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test rotate()
bool test_piece_rotate() {
    bool passed = true;
    try {
        piece p(2, 100); // Crea un pezzo 2x2
        p(0,0) = true;
        p(0,1) = true; // Forma a 'L' rovesciata nella parte superiore sinistra
        p(1,0) = true;

        //std::cout << "Creazione pezzo fatta" << std::endl;

        // 0 gradi:
        // # #
        // # .
        if (!p(0,0) || !p(0,1) || !p(1,0) || p(1,1)) passed = false;
        //std::cout << "0 gradi" << std::endl;

        p.rotate(); // 90 gradi orario
        // # #
        // . #
        if (!p(0,0) || !p(0,1) || p(1,0) || !p(1,1)) passed = false;    //Test modificato
        if (!passed) { std::cout << "Rotate 90 failed." << std::endl; return false; }
        //std::cout << "90 gradi" << std::endl;

        p.rotate(); // 180 gradi
        // . #
        // # #
        if (!p(0,1) || !p(1,0) || !p(1,1) || p(0,0)) passed = false; // La forma è la stessa per questo esempio specifico di rotazione
        if (!passed) { std::cout << "Rotate 180 failed." << std::endl; return false; }
        //std::cout << "180 gradi" << std::endl;

        p.rotate(); // 270 gradi
        // # .
        // # #
        //if (!p(0,0) || !p(0,1) || !p(1,0) || p(1,1)) passed = false;
        if (!p(0,0) || p(0,1) || !p(1,0) || !p(1,1)) passed = false;
        if (!passed) { std::cout << "Rotate 270 failed." << std::endl; return false; }
        //std::cout << "270 gradi" << std::endl;

        p.rotate(); // 360 gradi (torna all'originale)
        // # #
        // # .
        if (!p(0,0) || !p(0,1) || !p(1,0) || p(1,1)) passed = false;
        if (!passed) { std::cout << "Rotate 360 failed." << std::endl; return false; }
        //std::cout << "360 gradi" << std::endl;


    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_rotate: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}


// Test cut_row()
bool test_piece_cut_row() {
    bool passed = true;
    try {
        piece p(4, 100); // Crea un pezzo 4x4
        // Riempi le prime due righe
        for(uint32_t j=0; j<4; ++j) { p(0,j)=true; p(1,j)=true; }
        p(2,0)=true; // Una cella nella terza riga

        // Stato iniziale del pezzo (4x4):
        // # # # #  (Riga 0)
        // # # # #  (Riga 1)
        // # . . .  (Riga 2)
        // . . . .  (Riga 3)

        p.cut_row(1); // Taglia la riga 1

        // Stato atteso dopo cut_row(1):
        // . . . .  (Nuova riga 0 vuota)
        // # # # #  (Ex riga 0 scesa)
        // # . . .  (Riga 2 invariata)
        // . . . .  (Riga 3 invariata)

        // Verifica nuova riga 0 vuota
        for(uint32_t j=0; j<4; ++j) {
            if(p(0,j)) {
                passed = false;
                std::cerr << "Cut row failed: Riga 0 non vuota a colonna " << j << std::endl;
            }
        }

        // Verifica nuova riga 1 = ex riga 0 (tutta true)
        for(uint32_t j=0; j<4; ++j) {
            if(!p(1,j)) {
                passed = false;
                std::cerr << "Cut row failed: Riga 1 non corretta a colonna " << j << std::endl;
            }
        }

        // Verifica riga 2 invariata (solo colonna 0 true)
        if(!p(2,0) || p(2,1) || p(2,2) || p(2,3)) {
            passed = false;
            std::cerr << "Cut row failed: Riga 2 non invariata." << std::endl;
        }

        // Verifica riga 3 invariata (tutta false)
        for(uint32_t j=0; j<4; ++j) {
            if(p(3,j)) {
                passed = false;
                std::cerr << "Cut row failed: Riga 3 non corretta a colonna " << j << std::endl;
            }
        }

        // Test cut_row su riga invalida
        try {
            p.cut_row(p.side()); // Fuori limite (p.side() è ancora 4)
            passed = false;
        } catch (const tetris_exception& e) {
            // Eccezione attesa
        } catch (...) {
            passed = false;
        }

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_cut_row: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}


// Test operator<< e operator>> per piece (round-trip)
bool test_piece_stream_operators() {
    bool passed = true;
    try {
        piece p_original(4, 123);
        p_original(0,0) = true;
        p_original(1,1) = true;
        p_original(2,2) = true;
        p_original(3,3) = true;
        p_original(0,3) = true; // Rende un sotto-quadrante misto

        std::stringstream ss;
        ss << p_original; // Scrivi il pezzo nello stringstream

        //std::cout << "DEBUG: Contenuto di stringstream ss: '" << ss.str() << "'" << std::endl; // <--- AGGIUNGI QUESTO

        piece p_read;
        ss >> p_read; // Leggi il pezzo dallo stringstream      --> Il problema è in operator>>

        //std::cout << "DEBUG: Contenuto di stringstream ss: '" << ss.str() << "'" << std::endl;

        if (ss.fail()) {
            std::cerr << "Stream failed during piece read." << std::endl;
            passed = false;
        }
        if (!(p_original == p_read)) {
            std::cerr << "Original and read piece are not equal." << std::endl;
            passed = false;
        }

        // Test input malformato
        std::stringstream ss_bad;
        ss_bad << "2 100 ([])"; // Formato valido, ma poi un carattere extra
        piece p_bad;
        ss_bad >> p_bad; // Dovrebbe leggere correttamente
        ss_bad >> p_bad; // Prova a leggere di nuovo, dovrebbe fallire
        if (!ss_bad.fail()) {
            std::cerr << "Stream did not fail for malformed input." << std::endl;
            passed = false;
        }

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_piece_stream_operators: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// --- Test per la classe Tetris ---

// Test costruttori Tetris
bool test_tetris_constructors() {
    bool passed = true;
    try {
        tetris t1(10, 20, 0); // width, height, score
        if (t1.width() != 10 || t1.height() != 20 || t1.score() != 0) passed = false;

        // Test costruttori non validi (dovrebbero lanciare eccezioni)
        try {
            tetris t_invalid(0, 10, 0); // width = 0
            passed = false;
        } catch (const tetris_exception& e) {} // Eccezione attesa
        catch (...) { passed = false; }

        try {
            tetris t_invalid(10, 0, 0); // height = 0
            passed = false;
        } catch (const tetris_exception& e) {} // Eccezione attesa
        catch (...) { passed = false; }

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_tetris_constructors: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test add() e insert()
bool test_tetris_add_insert() {
    bool passed = true;
    try {
        tetris t(4, 4, 0); // Piccolo tabellone per test semplici
        piece p1(2, 100); // Pezzo 2x2
        p1(0,0) = true; p1(0,1) = true; p1(1,0) = true; p1(1,1) = true; // Quadrato pieno

        // Test add()
        t.add(p1, 0, 0); // Aggiungi in (0,0)
        // Verifica che il pezzo sia stato aggiunto (es. iterando o controllando print_ascii_art)
        // Per un test unitario, potresti voler controllare le celle specifiche
        // t.print_ascii_art(std::cout); // Per debug visivo

        // Test insert() - dovrebbe cadere in y=2
        piece p2(2, 101); // Un altro pezzo 2x2
        t.insert(p2, 0); // Inserisci nella colonna 0
        // Ora (0,0) e (0,1) sono occupati da p1, quindi p2 dovrebbe cadere in (0,2)
        // t.print_ascii_art(std::cout); // Per debug visivo

        // Test collisione con add()
        try {
            t.add(p1, 0, 0); // Collide con p1
            passed = false; // Fallisce se non previene la collisione
        } catch (const tetris_exception& e) {
            // Eccezione attesa (o add dovrebbe restituire false se gestisce così)
        } catch (...) { passed = false; }

        // Test Game Over (inserimento impossibile)
        tetris t_game_over(2, 2, 0);
        piece p_small(1,1); p_small(0,0)=true; // Pezzo 1x1
        t_game_over.add(p_small, 0,0);
        t_game_over.add(p_small, 1,0);
        t_game_over.add(p_small, 0,1);
        t_game_over.add(p_small, 1,1); // Riempie tutto il 2x2
        // t_game_over.print_ascii_art(std::cout);
        try {
            t_game_over.insert(p_small, 0); // Non c'è spazio
            passed = false; // Fallisce se non lancia eccezione
        } catch (const tetris_exception& e) {
            // Eccezione attesa (Game Over)
        } catch (...) { passed = false; }


    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_tetris_add_insert: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test iteratori
bool test_tetris_iterators() {
    bool passed = true;
    try {
        tetris t(4, 4, 0);
        piece p1(1, 1); p1(0,0)=true;
        piece p2(1, 2); p2(0,0)=true;
        t.add(p1, 0, 0);
        t.add(p2, 1, 0);

        int count = 0;
        for (auto it = t.begin(); it != t.end(); ++it) {
            count++;
            // Puoi anche verificare i valori di it->x, it->y, it->p
        }
        if (count != 2) passed = false;

        // Test su tabellone vuoto
        tetris t_empty(2,2,0);
        count = 0;
        for (auto it = t_empty.begin(); it != t_empty.end(); ++it) {
            count++;
        }
        if (count != 0) passed = false;

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_tetris_iterators: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

// Test operator<< e operator>> per tetris (round-trip)
bool test_tetris_stream_operators() {
    bool passed = true;
    try {
        tetris t_original(8, 16, 1000);
        piece p1(2, 10); p1(0,0)=true; p1(1,1)=true;
        piece p2(4, 20); p2(0,0)=true; p2(0,1)=true; p2(0,2)=true; p2(0,3)=true;
        t_original.add(p1, 0, 0);
        t_original.add(p2, 4, 0);
        t_original.insert(p1, 0); // Inserisci un altro pezzo

        std::stringstream ss;
        ss << t_original; // Scrivi il tabellone nello stringstream

        //std::cout << "Output serializzato:\n" << ss.str() << std::endl; // Per debug: stampa l'output serializzato

        tetris t_read; // Crea un nuovo oggetto vuoto
        ss >> t_read; // Leggi il tabellone dallo stringstream

        if (ss.fail()) {
            std::cerr << "Stream failed during tetris read.";
            passed = false;
        }

        // Verifica che t_read sia uguale a t_original
        // Questo richiede che operator== per tetris sia robusto
        if (!(t_original == t_read)) {
            std::cerr << "Original and read tetris objects are not equal." << std::endl;
            // Per debug, puoi stampare entrambi per vedere le differenze
            std::cout << "Original:\n" << t_original << std::endl;
            std::cout << "Read:\n" << t_read << std::endl;
            passed = false;
        }

    } catch (const tetris_exception& e) {
        std::cerr << "Eccezione in test_tetris_stream_operators: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}


int main() {
    std::cout << "--- Avvio Test per le Classi Piece e Tetris ---" << std::endl << std::endl;

    // --- Test per la Classe Piece ---
    std::cout << "--- Test Classe Piece ---" << std::endl;
    print_test_result("Piece - Costruttori Validi", test_piece_valid_constructors());
    print_test_result("Piece - Costruttori Non Validi (Eccezioni)", test_piece_invalid_constructors());
    print_test_result("Piece - Copia e Assegnazione", test_piece_copy_assignment());
    print_test_result("Piece - Move e Assegnazione", test_piece_move_assignment());
    print_test_result("Piece - Accesso Celle (operator())", test_piece_cell_access());
    print_test_result("Piece - empty() e full()", test_piece_empty_full());
    print_test_result("Piece - rotate()", test_piece_rotate());
    print_test_result("Piece - cut_row()", test_piece_cut_row());
    print_test_result("Piece - Operatori di Stream (<< e >>)", test_piece_stream_operators());
    std::cout << "--- Fine Test Classe Piece ---" << std::endl << std::endl;

    // --- Test per la Classe Tetris ---
    std::cout << "--- Test Classe Tetris ---" << std::endl;
    print_test_result("Tetris - Costruttori", test_tetris_constructors());
    print_test_result("Tetris - add() e insert()", test_tetris_add_insert());
    print_test_result("Tetris - Iteratori", test_tetris_iterators());
    print_test_result("Tetris - Operatori di Stream (<< e >>)", test_tetris_stream_operators());
    std::cout << "--- Fine Test Classe Tetris ---" << std::endl << std::endl;

    std::cout << "--- Tutti i Test Completati ---" << std::endl;

    return 0;
}