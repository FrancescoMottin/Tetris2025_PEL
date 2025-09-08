#include <iostream> // Per input/output su console
#include <sstream>  // Per testare gli operatori di stream in memoria
#include <string>   // Per std::string e std::getline
#include <cassert>  // Per asserzioni semplici (anche se i try-catch sono preferibili per le eccezioni)
#include "tetris.cpp" // Include l'implementazione delle classi piece e tetris

void print_test_result(const std::string& test_name, bool passed) 
{
    std::cout << "Test: " << test_name << " - " << (passed ? "PASSED" : "FAILED") << std::endl;
}

void piece::print_ascii_art(std::ostream& os) const
{
    os << "Piece (side=" << m_side << ", color=" << (int)m_color << ")\n";
    os << "\033[38;5;" << static_cast<int>(m_color) << "m";

    for (uint32_t i = 0; i < m_side; i++) {
        for (uint32_t j = 0; j < m_side; j++) {
            os << (m_grid[i][j] ? '#' : '.');
        }
        os << '\n';
    }

    os << "\033[0m";
}

bool test_piece_stream_operators() 
{
    bool passed = true;
    try 
    {
        piece p_original(4, 123);
        p_original(0,0) = true;
        p_original(1,1) = true;
        p_original(2,2) = true;
        p_original(3,3) = true;
        p_original(0,3) = true; // Rende un sotto-quadrante misto

        std::stringstream ss;
        ss << p_original; // Scrivi il pezzo nello stringstream

        std::cout << "DEBUG operator<< stringa pre-serializzazione: " << ss.str() << std::endl;
        std::cout << "Posizione iniziale cursore: " << ss.tellg() << std::endl;
        std::cout << "ASCII Art del pezzo originale:\n";
        p_original.print_ascii_art(std::cout);

        piece p_read;
        ss >> p_read; // Leggi il pezzo dallo stringstream      --> Il problema è in operator>>
        
        std::cout << "DEBUG operator<< stringa serializzata: " << ss.str() << std::endl;
        std::cout << "Posizione cursore dopo lettura: " << ss.tellg() << std::endl;
        std::cout << "ASCII Art del pezzo letto dallo stream:\n";
        p_read.print_ascii_art(std::cout);
        
        // Mostra cosa resta da leggere
        //std::string rest;
        //std::getline(ss, rest, '\0');
        //std::cout << "DEBUG residuo nel buffer dopo lettura: '" << rest << "'" << std::endl;
        
        if (ss.fail()) {
            std::cerr << "Stream failed during piece read." << std::endl;
            passed = false;
        }
        if (!(p_original == p_read)) {
            std::cerr << "Original and read piece are not equal." << std::endl;
            passed = false;
        }

        std::cout << "" << std::endl;
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

        // --- Caso 1: Pezzo vuoto ---
        piece p_empty(2, 42);
        std::stringstream ss;
        ss << p_empty;
        piece p_read;
        ss >> p_read;
        if (!(p_empty == p_read)) 
        {
            std::cerr << "Empty piece not preserved after stream." << std::endl;
            passed = false;
        }


        // --- Caso 2: Pezzo pieno ---
        piece p_full(3, 77);
        for (uint32_t r=0; r<p_full.side(); ++r)
            for (uint32_t c=0; c<p_full.side(); ++c)
                p_full(r,c) = true;
    
        std::stringstream ss;
        ss << p_full;
        piece p_read;
        ss >> p_read;
        if (!(p_full == p_read)) 
        {
            std::cerr << "Full piece not preserved after stream." << std::endl;
            passed = false;
        }


        // --- Caso 4: Input malformato ---
        std::stringstream ss_bad("3 50 (([])");
        piece p_bad;
        ss_bad >> p_bad;
        if (!ss_bad.fail()) 
        {
            std::cerr << "Malformed input not detected." << std::endl;
            passed = false;
        }


        // --- Caso 5: Pezzo side=1 vuoto ---
        std::stringstream ss("1 9 ([])");
        piece p1;
        ss >> p1;
        if (ss.fail() || p1.side() != 1) 
        {
            std::cerr << "Failed to parse side=1 piece." << std::endl;
            passed = false;
        }
    } 
    catch (const tetris_exception& e) 
    {
        std::cerr << "Eccezione in test_piece_stream_operators: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}

bool test_tetris_stream_operators() 
{
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
        std::cout << "ASCII Art del tetris originale:\n";
        t_original.print_ascii_art(std::cout);
        
        //std::cout << "Output serializzato:\n" << ss.str() << std::endl; // Per debug: stampa l'output serializzato

        tetris t_read; // Crea un nuovo oggetto vuoto
        ss >> t_read; // Leggi il tabellone dallo stringstream
        std::cout << "ASCII Art del tetris letto dallo stream:\n";
        t_read.print_ascii_art(std::cout);

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

        // --- Caso 1: Campo vuoto ---
        tetris t1(10, 20, 0);
        std::stringstream ss;
        ss << t1;
        tetris t_read;
        ss >> t_read;
        if (!(t1 == t_read)) 
        {
            std::cerr << "Empty tetris not preserved after stream." << std::endl;
            passed = false;
        }


        // --- Caso 2: Un pezzo solo ---
        tetris t2(6, 6, 0);
        piece p(2, 5); p(0,0) = true;
        t2.add(p, 1, 1);
        std::stringstream ss;
        ss << t2;
        tetris t_read;
        ss >> t_read;
        if (!(t2 == t_read)) 
        {
            std::cerr << "Single-piece tetris not preserved after stream." << std::endl;
            passed = false;
        }


        // --- Caso 3: Campo pieno ---
        tetris t3(4, 4, 0);
        piece p_full(4, 9);
        for (uint32_t r=0; r<p_full.side(); ++r)
            for (uint32_t c=0; c<p_full.side(); ++c)
                p_full(r,c) = true;
        t3.add(p_full, 0, 0);
        std::stringstream ss;
        ss << t3;
        tetris t_read;
        ss >> t_read;
        if (!(t3 == t_read)) 
        {
            std::cerr << "Full tetris not preserved after stream." << std::endl;
            passed = false;
        }


        // --- Caso 4: Input malformato ---
        std::stringstream ss_bad("8 16 1000 2 10 ([])"); 
        tetris t_bad;
        ss_bad >> t_bad;
        if (!ss_bad.fail()) 
        {
            std::cerr << "Malformed tetris input not detected." << std::endl;
            passed = false;
        }



    } 
    catch (const tetris_exception& e) 
    {
        std::cerr << "Eccezione in test_tetris_stream_operators: " << e.what() << std::endl;
        passed = false;
    }
    return passed;
}