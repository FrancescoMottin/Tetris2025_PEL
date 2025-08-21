#include <cstdint>
#include "tetris.hpp"

//forse non serve definire un impl* impl

/* Your implementation goes here! */
piece::piece()
{
    m_side = 0;
    m_color = 0;
    m_grid = nullptr; //E' un doppio puntatore, non può essere altro che nullptr
}

//Throws "tetris_exception" se side != 2^n OR color = 0
piece::piece(uint32_t s, uint8_t c)
{
    if((s & (s - 1)) != 0) throw tetris_exception("ERROR! - piece(uint32_t s, uint8_t c) -  La dimensione del pezzo (side) deve essere una potenza di 2.");
    if(s == 0)  throw tetris_exception("ERROR! - piece(uint32_t s, uint8_t c) -  La dimensione del pezzo (side) non puo' essere 0.");
    if(c == 0)  throw tetris_exception("ERROR! - piece(uint32_t s, uint8_t c) -  Il colore del pezzo (color) non puo' essere 0.");

    m_side = s;
    m_color = c;
    
    try
    {
        m_grid = new bool*[m_side]; //initialize a s*s matrix with all cells at false, must be an empty piece
        for(uint32_t i = 0; i < s; i++)
        {
            m_grid[i] = new bool[s];
            for(uint32_t j = 0; j < s; j++)
                m_grid[i][j] = false;
        }
    }
    catch(const std::bad_alloc& e)
    {
        for(uint32_t i = 0; i < m_side; i++)    //deallocazione colonne
            delete[] m_grid[i];                 
        delete[] m_grid;                        //deallocazione righe
        m_grid = nullptr;
    }
}

//Copy constructor
piece::piece(piece const& rhs)
{
    m_side = rhs.m_side;
    m_color = rhs.m_color;

    if(m_side > 0)
    {   
        try
        {
            m_grid = new bool*[m_side];
            for(uint32_t i = 0; i < m_side; i++)
            {
                m_grid[i] = new bool[m_side];
                for(uint32_t j = 0; j < m_side; j++)
                    m_grid[i][j] = rhs.m_grid[i][j];
            }
        }
        catch(const std::bad_alloc& e)
        {
            for(uint32_t i = 0; i < m_side; i++)    //deallocazione colonne
                delete[] m_grid[i];                 
            delete[] m_grid;                        //deallocazione righe
            m_grid = nullptr;
            
            throw tetris_exception("ERROR! - piece(piece const& rhs) - Errore di allocazione memoria.");
        }    
    }
    else m_grid = nullptr;
}

//Move constructor
piece::piece(piece&& rhs)
{
    m_side = rhs.m_side;
    m_color = rhs.m_color;
    m_grid = rhs.m_grid;

    rhs.m_side = 0;        
    rhs.m_color = 0;
    rhs.m_grid = nullptr;
}

piece::~piece()
{
    if(m_grid == nullptr) return ;

    for(uint32_t i = 0; i < m_side; i++)    //deallocazione colonne
        delete[] m_grid[i];                 
    delete[] m_grid;                        //deallocazione righe
    m_grid = nullptr;

    m_side = 0;
    m_color = 0;
}

//assignment operator
piece& piece::operator=(piece const& rhs)
{
    if(this == &rhs) return *this;

    //se la grandezza è differente tra this.m_grid e rhs.m_grid, avremmo errori!
    if(m_grid != nullptr)
    {
        for(uint32_t i = 0; i < m_side; i++)    //deallocazione colonne
            delete[] m_grid[i];                   
        delete[] m_grid;                        //deallocazione righe
        m_grid = nullptr;                       
    }
    
    m_side = rhs.m_side;
    m_color = rhs.m_color;
    if(m_side != 0)
    {
        m_grid = new bool*[m_side];
        for(uint32_t i = 0; i < m_side; i++)
        {
            m_grid[i] = new bool[m_side];
            for(uint32_t j = 0; j < m_side; j++)
                m_grid[i][j] = rhs.m_grid[i][j];
        }
    }
    else m_grid = nullptr;

    return *this;
}

//move assignment operator
piece& piece::operator=(piece&& rhs)
{
    if(this == &rhs) return *this;

    if(m_grid != nullptr)
    {
        for(uint32_t i = 0; i < m_side; i++)    //deallocazione colonne
            delete[] m_grid[i];                 
        delete[] m_grid;
        m_grid = nullptr;
    }

    m_side = rhs.m_side;
    m_color = rhs.m_color;
    m_grid = rhs.m_grid;

    rhs.m_side = 0;        
    rhs.m_color = 0;
    rhs.m_grid = nullptr;

    return *this;
}

bool piece::operator==(piece const& rhs) const
{
    if(m_side != rhs.m_side || m_color != rhs.m_color) return false;
    if(m_side == 0) return true;

    for(uint32_t i = 0; i < m_side; i++)
        for(uint32_t j = 0; j < m_side; j++)
            if(m_grid[i][j] != rhs.m_grid[i][j]) return false;
    
    return true;
}

bool piece::operator!=(piece const& rhs) const { return !(operator==(rhs)); }

bool& piece::operator()(uint32_t i, uint32_t j)
{
    //tetris_exception if (i,j) is out of bounds.
    if(m_grid == nullptr || i >= m_side || j >= m_side) throw tetris_exception("ERROR! - operator()(uint32_t i, uint32_t j) - Indici (" + std::to_string(i) + ", " + std::to_string(j) + ") fuori dai limiti del pezzo (side=" + std::to_string(m_side) + ").");     
    return m_grid[i][j];
}  

bool piece::operator()(uint32_t i, uint32_t j) const
{
    //tetris_exception if (i,j) is out of bounds.
    if(m_grid == nullptr || i >= m_side || j >= m_side) throw tetris_exception("ERROR! - operator()(uint32_t i, uint32_t j) const - Indici (" + std::to_string(i) + ", " + std::to_string(j) + ") fuori dai limiti del pezzo (side=" + std::to_string(m_side) + ").");     
    return m_grid[i][j];
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const
{
    if(s == 0) return true;
    if(m_grid == nullptr) throw tetris_exception("ERROR! - empty(uint32_t i, uint32_t j, uint32_t s) - Accesso a griglia non inizializzata (nullptr).");    //tetris_exception if out of bounds
    if(i + s > m_side || j + s > m_side) throw tetris_exception("ERROR! - empty(uint32_t i, uint32_t j, uint32_t s) - Sotto-quadrante (" + std::to_string(i) + ", " + std::to_string(j) + ", size=" + std::to_string(s) + ") fuori dai limiti del pezzo (side=" + std::to_string(m_side) + ").");
    

    for(uint32_t i2 = i; i2 < i+s; i2++)
        for(uint32_t j2 = j; j2 < j+s; j2++)
            if(m_grid[i2][j2] == true) return false;

    return true;
}

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const
{
    if(s == 0) return true;
    if(m_grid == nullptr ) throw tetris_exception("ERROR! - full(uint32_t i, uint32_t j, uint32_t s) - Accesso a griglia non inizializzata (nullptr).");    //tetris_exception if out of bounds
    if(i + s > m_side || j + s > m_side) throw tetris_exception("ERROR! - full(uint32_t i, uint32_t j, uint32_t s) - Sotto-quadrante (" + std::to_string(i) + ", " + std::to_string(j) + ", size=" + std::to_string(s) + ") fuori dai limiti del pezzo (side=" + std::to_string(m_side) + ").");
    

    for(uint32_t i2 = i; i2 < i+s; i2++)
        for(uint32_t j2 = j; j2 < j+s; j2++)
            if(m_grid[i2][j2] == false) return false;

    return true;
}

bool piece::empty() const
{
    if(m_grid == nullptr && m_side == 0) return true;
    if(m_grid == nullptr) throw tetris_exception("ERROR! - empty() - Accesso a griglia non inizializzata (nullptr).");
    for(uint32_t i = 0; i < m_side; i++)
        for(uint32_t j = 0; j < m_side; j++)
            if(m_grid[i][j] == true) return false;

    return true;
}

bool piece::full() const
{
    if(m_grid == nullptr && m_side == 0) return false;
    if(m_grid == nullptr) throw tetris_exception("ERROR! - full() - Accesso a griglia non inizializzata (nullptr).");
    for(uint32_t i = 0; i < m_side; i++)
        for(uint32_t j = 0; j < m_side; j++)
            if(m_grid[i][j] == false) return false;

    return true;
}

void piece::rotate()
{
    if(m_grid == nullptr || m_side <= 1) return ;

    bool** tmp_grid = nullptr; 
    try
    {
        tmp_grid = new bool*[m_side];
        for(uint32_t i = 0; i < m_side; i++)    
        {         
            tmp_grid[i] = new bool[m_side];
            for(uint32_t j = 0; j < m_side; j++)
                tmp_grid[i][j] = false;
        }    
    }
    catch(const std::bad_alloc& e) 
    {
        if(tmp_grid)
        {
            for(uint32_t i = 0; i < m_side; i++)    
                delete[] tmp_grid[i];                   
            delete[] tmp_grid;
        }
        throw tetris_exception("ERROR! - rotate() - Errore di allocazione memoria durante la rotazione.");
    }

    for(uint32_t i = 0; i < m_side; i++)
    {
        for(uint32_t j = 0; j < m_side; j++)
        {
            uint32_t new_row = j;
            uint32_t new_col = m_side - i - 1;
            tmp_grid[new_row][new_col] = m_grid[i][j];  //Sarebbe preferibile prima allocare memoria e dopo copiare i valori
        } 
    }

    for(uint32_t i = 0; i < m_side; i++)    
        delete[] m_grid[i];                   
    delete[] m_grid;                        
    m_grid = nullptr;                       

    m_grid = tmp_grid;
}

//maybe implement try-catch for std::bad_alloc
//Probabilmente mal implementato
void piece::cut_row(uint32_t i)
{
    if(m_grid == nullptr || m_side == 0) throw tetris_exception("ERROR! - cut_row(uint32_t i) - Griglia non inizializzata (nullptr).");
    if(m_side == 0) throw tetris_exception("ERROR! - cut_row(uint32_t i) - Impossibile tagliare riga su un pezzo di dimensione 0.");
    if(i >= m_side) throw tetris_exception("ERROR! - cut_row(uint32_t i) - Indice di riga (" + std::to_string(i) + ") fuori dai limiti del pezzo (side=" + std::to_string(m_side) + ").");
    if(m_side == 1) 
    {
        delete[] m_grid[0];
        delete[] m_grid;
        m_side = 0;
        m_grid = nullptr;

        return ;
    }

    //Logica per lo scorrimento delle righe verso il basso (gravità interna al pezzo).
    // Y=0 è la riga superiore del pezzo, Y cresce verso il basso.
    for(uint32_t r = i; r < m_side - 1; r++)    //se i = 0, r-1 porta ad un errore di underflow
        for(uint32_t c = 0; c < m_side; c++)    
            m_grid[r][c] = m_grid[r+1][c];      //m_grid[r][c] = m_grid[r-1][c];

    //la riga 0 (la più in alto) conterrà una copia della sua versione originale. Deve essere svuotata
    for(uint32_t c = 0; c < m_side; c++)
        m_grid[m_side-1][c] = false;               //m_grid[m_side-1][c] = false; -> Cosi svuotiamo la riga più bassa
}

uint32_t piece::side() const { return m_side; }
int piece::color() const { return m_color; }

tetris::tetris()
{
    m_score = 0;
    m_width = 0;
    m_height = 0;
    m_field = nullptr;
}

//try-catch per l'assegnazione del field?
tetris::tetris(uint32_t w, uint32_t h, uint32_t s)  //: m_width(0), m_height(0), m_score(0), m_field(nullptr)
{
    if(w <= 0) throw tetris_exception("ERROR! - tetris(uint32_t w, uint32_t h, uint32_t s) - La larghezza del tabellone (w) non puo' essere 0.");
    if(h <= 0) throw tetris_exception("ERROR! - tetris(uint32_t w, uint32_t h, uint32_t s) - L'altezza del tabellone (h) non puo' essere 0.");
    m_score = s;
    m_width = w;
    m_height = h;   
    m_field = nullptr; //m_field rappresenta i diversi pezzi, non il tabellone stesso
}

//try_catch per ciclo (nel caso si fallisse l'allcoazione)
tetris::tetris(tetris const& rhs)
{
    m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_field = nullptr;          //Devo utilzizare tail_field per navigare la struttura, m_field rimane alla testa della lista

    node* tail_field = m_field;

    try
    {
        for(node* it = rhs.m_field; it != nullptr; it = it->next) //copia solo il primo nodo
        {
            node* new_field = new node{it->tp, nullptr};
            if(!tail_field) m_field = tail_field = new_field;
            else 
            {
                tail_field->next = new_field;
                tail_field = tail_field->next;
            }
        }
    }
    catch(const std::bad_alloc& e) 
    {
        node* curr_node = m_field;
        while(curr_node)
        {
            node* tmp_field = curr_node;
            curr_node = curr_node->next;
            delete tmp_field; 
        }
        m_field = nullptr;

        throw tetris_exception("ERROR! - tetris(tetris const& rhs) - Errore di allocazione memoria.");
    }
}

tetris::tetris(tetris&& rhs)
{
    m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_field = rhs.m_field;

    rhs.m_score = 0;
    rhs.m_width = 0;
    rhs.m_height = 0;
    rhs.m_field = nullptr;
}

tetris::~tetris()
{
    if(m_field == nullptr) return ;

    m_score = 0;
    m_width = 0;
    m_height = 0;

    node* tail_field = m_field;
    while(tail_field)
    {
        node* tmp_field = tail_field;
        tail_field = tail_field->next;
        delete tmp_field; 
    }
    m_field = nullptr;
}

//non safe, cosa succede se la copia fallisce? Usa try catch
tetris& tetris::operator=(tetris const& rhs)
{
    if(this == &rhs) return *this;
    
    node* tail_field = m_field;
    while(tail_field)
    {
        node* tmp_field = tail_field;
        tail_field = tail_field->next;
        delete tmp_field; 
    }
    m_field = nullptr;

    m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    try 
    {
        for(node* it = rhs.m_field; it != nullptr; it = it->next)      //Probabile errore ricostruzione lista
        {
            node* new_field = new node{it->tp, nullptr};
            if(!m_field) m_field = tail_field = new_field;
            else 
            {
                tail_field->next = new_field;
                tail_field = tail_field->next;
            }
        }
    }
    catch(const std::bad_alloc& e) 
    {
        node* curr_node = m_field;
        while(curr_node)
        {
            node* tmp_field = curr_node;
            curr_node = curr_node->next;
            delete tmp_field; 
        }
        m_field = nullptr;

        throw tetris_exception("ERROR! - operator=(tetris const& rhs) - Errore di allocazione memoria.");
    }
    
    return *this;
}

tetris& tetris::operator=(tetris&& rhs)
{
    if(this == &rhs) return *this;
    
    node* tail_field = m_field;
    while(tail_field)
    {
        node* tmp_field = tail_field;
        tail_field = tail_field->next;
        delete tmp_field; 
    }
    m_field = nullptr;

    m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_field = rhs.m_field;

    rhs.m_score = 0;
    rhs.m_width = 0;
    rhs.m_height = 0;
    rhs.m_field = nullptr;

    return *this;
}

bool tetris::operator==(tetris const& rhs) const
{
    if(m_score != rhs.m_score || m_width != rhs.m_width || m_height != rhs.m_height) return false;

    node* tail = m_field;
    node* it = rhs.m_field;
    while(tail && it)
    {
        if(tail->tp.p != it->tp.p || tail->tp.x != it->tp.x || tail->tp.y != it->tp.y ) return false;
        tail = tail->next;
        it = it->next;
    }

    return (!it && !tail);
}
bool tetris::operator!=(tetris const& rhs) const { return !operator==(rhs);}


//Il pezzo è già dentro alla lista
//where most of the game logic happens
//Arriva il pezzo contenuto nella lista tetris. Scorriamo tutta la lista e controlli se in qualche coordinata x abbiamo tutte le celle siano contenute dal pezzo (cut_row)
//Nota che il controllo se il row sia completamente usato tocca a questa funzione, cut_row() cancella solo la riga incriminata
void tetris::insert(piece const& p, int x)
{
    //that p is the piece whose bottom-left corner is at position (x,y) in the tetris field
    if(m_width == 0 || m_height == 0) throw tetris_exception("ERROR! - insert(piece const& p, int x) - Il tabellone non è stato inizializzato con dimensioni valide.");

    int y = 0;
    while(y + p.side() < m_height && containment(p, x, y+1)) {y++;} //eventualmente elimina p
    if(!containment(p,x,y)) throw tetris_exception("GAME OVER! - insert(piece const& p, int x) - Non possiamo inserire altri pezzi!");
    add(p, x, y); //aggiungere piece all'inizio della lista

    uint32_t clear_rows = 0;
    bool* row_full = new bool[m_height];
    try
    {
        for(uint32_t i = 0; i < m_height; i++)
        {
            row_full[i] = true;
            for(uint32_t j = 0; j < m_width; j++)
            {
                bool cell_occupied = false;
                node* curr = m_field;
                while (curr != nullptr)
                {
                    int rel_x = j - curr->tp.x;
                    int rel_y = i - curr->tp.y;

                    if(rel_x >= 0 && rel_y >= 0 && rel_x < curr->tp.p.side() && rel_y < curr->tp.p.side())
                    {
                        if(curr->tp.p.operator()(rel_y, rel_x))
                        {
                            cell_occupied = true;
                            break;
                        }
                    }
                    curr = curr->next;
                }
                

                if(!cell_occupied)
                {
                    row_full[i] = false;
                    break;
                }
            }
            if(row_full[i])  clear_rows++;
        }
    }
    catch(const std::bad_alloc& e)
    {
        delete[] row_full;
        throw tetris_exception("ERROR! - insert(piece const& p, int x) - Errore di allocazione memoria per row_full");
    }

    if(clear_rows > 0)
    {
        m_score += clear_rows * 100;
        
        node* tmp = m_field;
        while(tmp) 
        {
            piece& to_cut = tmp->tp.p;
            int pos_y = tmp->tp.y;
            
            for(uint32_t i = 0; i < m_height; i++)
            {
                if(row_full[i] && i >= pos_y && i < pos_y + to_cut.side()) 
                {
                    uint32_t rel_row = i - pos_y;
                    to_cut.cut_row(rel_row);
                }
            } 

            tmp = tmp->next;
        }

        //Aggiorna coordinate y dei pezzi dopo il taglio
        tmp = m_field;
        while(tmp)
        {
            int fall = 0;
            int pos_y = tmp->tp.y;

            for(uint32_t i = 0; i < m_height; i++)
                if(row_full[i] && i >= pos_y) fall++; 
        
            tmp->tp.y += fall;
            tmp = tmp->next;
        } 
    }
    delete[] row_full;
    
    //If, after cutting one or more rows, some piece becomes empty (i.e., piece::empty() returns true), then it must be removed from the list.
    node* new_head = nullptr;
    node* new_tail = nullptr;
    node* curr_node = m_field;
    while (curr_node)
    {
        node* tmp = curr_node->next;
        if((curr_node->tp.p).empty()) delete curr_node;
        else 
        {
            curr_node->next = nullptr;
            if(new_head == nullptr) 
            {
                new_head = curr_node;
                new_tail = curr_node;
            }
            else
            {
                new_tail->next = curr_node;     
                new_tail = curr_node;
            }
        }

        curr_node = tmp;
    }
    m_field = new_head;
}

void tetris::add(piece const& p, int x, int y)
{
    if(!containment(p,x,y)) throw tetris_exception("ERROR! - add(piece const& p, int x, int y) - Le coordinate non sono valide per il pezzo dato");

    tetris_piece new_tp;
    new_tp.p = p;
    new_tp.x = x;
    new_tp.y = y;

    node* new_node = nullptr;
    try{ new_node = new node{new_tp, nullptr}; }   //try-catch?
    catch(const std::bad_alloc& e)
    { 
        throw tetris_exception("ERROR! - add(piece const& p, int x, int y) - Errore di allocazione memoria durante l'aggiunta di un pezzo."); 
    }

    if(!m_field) m_field = new_node;
    else
    {
        node* current = m_field;
        while(current->next)
            current = current->next;
        current->next = new_node;
    }
}

bool tetris::containment(piece const& p, int x, int y) const
{
    ///*
    uint32_t p_side = p.side();
    for(uint32_t r = 0; r < p_side; r++)
    {
        for(uint32_t c = 0; c < p_side; c++)
        {
            if(p.operator()(r,c))    
            {
                uint32_t abs_x = x + c;
                uint32_t abs_y = y + r;
            
                if(abs_x >= m_width || abs_y >= m_height) return false;

                node* curr = m_field;
                while(curr)
                {
                    piece const& curr_piece = curr->tp.p;
                    uint32_t curr_x = curr->tp.x;
                    uint32_t curr_y = curr->tp.y;
                
                    uint32_t rel_x = abs_x - curr_x;
                    uint32_t rel_y = abs_y - curr_y;

                    if(rel_x < curr_piece.side() && rel_y < curr_piece.side())
                        if(curr_piece.operator()(rel_y,rel_x)) return false;

                    curr = curr->next;
                }
            }
        }
    }
    return true;
    //*/

    /*
    uint32_t p_side = p.side();
    for(uint32_t r = 0; r < p_side; r++)
    {
        for(uint32_t c = 0; c < p_side; c++)
        {
            if(p.operator()(r,c))    
            {
                int abs_x = x + c;
                int abs_y = y + r;
            
                if(abs_x < 0 || abs_y < 0 || abs_x >= m_width || abs_y >= m_height) return false;
            }
        }
    }
    
    node* curr = m_field;
    while(curr)
    {
        piece const& curr_piece = curr->tp.p;
        uint32_t curr_x = curr->tp.x;
        uint32_t curr_y = curr->tp.y;
    
        for(uint32_t r = 0; r < p.side(); r++)
        {
            for(uint32_t c = 0; c < p.side(); c++)
            {
                int abs_x = x + c;
                int abs_y = y + r;

                uint32_t rel_x = abs_x - curr_x;
                uint32_t rel_y = abs_y - curr_y;

                if(rel_x < curr_piece.side() && rel_y < curr_piece.side())
                    if(curr_piece.operator()(rel_y,rel_x)) return false;
            }
        }
        
        curr = curr->next;
    }
    return true;
    */
}


//NOT NECESSARY BUT USEFUL FOR DEBUGGING
void tetris::print_ascii_art(std::ostream& os) const
{
    char** tmp_mat = nullptr;
    try
    {
        tmp_mat = new char*[m_height];
        for(uint32_t i = 0; i < m_height; i++)
        {
            tmp_mat[i] = new char[m_width];
            for(uint32_t j = 0; j < m_width; j++)
                tmp_mat[i][j] = ' ';
        }
    }
    catch(const std::bad_alloc& e)
    {
        if(tmp_mat)
        {
            for(uint32_t i = 0; i < m_height; i++)
                delete tmp_mat[i];
            delete[] tmp_mat;
        }

        throw tetris_exception("ERROR! - print_ascii_art(std::ostream& os) const - Errore di allocazione memoria per la matrice ASCII.");
    }

    piece p;
    int x = 0;
    int y = 0;
    int abs_x = 0;
    int abs_y = 0;
    for(node* it = m_field; it; it = it->next)
    {
        p = it->tp.p;
        x = it->tp.x;
        y = it->tp.y;

        for(uint32_t i = 0; i < p.side(); i++)
        {
            for(uint32_t j = 0; j < p.side(); j++)
            {
                if(p(i,j) == true)
                {
                    abs_x = x + j;
                    abs_y = y + i;
                    tmp_mat[abs_y][abs_x] = '#';
                } 
            }
        }
    }

    os << '-';
    for(uint32_t i = 0; i < m_width; i++)
        os << '-';
    os << '-' << std::endl;

    
    for(uint32_t i = 0; i < m_height; i++)
    {
        os << '|';
        for(uint32_t j = 0; j < m_width; j++)
            os << tmp_mat[i][j];
        os << '|' << std::endl;
    }

    os << '-';
    for(uint32_t i = 0; i < m_width; i++)
        os << '-';
    os << '-' << std::endl;

    for(uint32_t i = 0; i < m_height; i++)
        delete[] tmp_mat[i];
    delete[] tmp_mat;
    tmp_mat = nullptr;
}

tetris::iterator::iterator(node* ptr) { m_ptr = ptr; }
tetris::iterator::reference tetris::iterator::operator*() { return m_ptr->tp;}                      //ritorno puntatore
tetris::iterator::pointer tetris::iterator::operator->() { return &(m_ptr->tp);}                    //ritorno reference
tetris::iterator& tetris::iterator::operator++()
{
    m_ptr = m_ptr->next;
    return *this;
}
tetris::iterator tetris::iterator::operator++(int /*dummy*/) 
{
    iterator tmp(m_ptr);
    m_ptr = m_ptr->next;
    return tmp;
}
bool tetris::iterator::operator==(iterator const& rhs) const { return m_ptr == rhs.m_ptr; }
bool tetris::iterator::operator!=(iterator const& rhs) const { return !operator==(rhs);}


tetris::const_iterator::const_iterator(node const* ptr) { m_ptr = ptr; }
tetris::const_iterator::reference tetris::const_iterator::operator*() const { return m_ptr->tp;}
tetris::const_iterator::pointer tetris::const_iterator::operator->() const { return &(m_ptr->tp);}
tetris::const_iterator& tetris::const_iterator::operator++()
{
    m_ptr = m_ptr->next;
    return *this;
}
tetris::const_iterator tetris::const_iterator::operator++(int /*dummy*/)
{
    const_iterator tmp(m_ptr);
    m_ptr = m_ptr->next;
    return tmp;
}
bool tetris::const_iterator::operator==(const_iterator const& rhs) const { return m_ptr == rhs.m_ptr; }
bool tetris::const_iterator::operator!=(const_iterator const& rhs) const { return !operator==(rhs); }


tetris::iterator tetris::begin() { return iterator(m_field);}
tetris::iterator tetris::end() { return iterator(nullptr);}
tetris::const_iterator tetris::begin() const { return const_iterator(m_field);}
tetris::const_iterator tetris::end() const {return const_iterator(nullptr);}

uint32_t tetris::score() const { return m_score; }
uint32_t tetris::width() const { return m_width; }
uint32_t tetris::height() const { return m_height; }

//crea handler stato p.empty() e p.full()
void input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset)
{
    if(is.fail()) return ;
    
    char c;
    is >> std::skipws >> c;
    if(is.fail()) 
    {
        is.setstate(std::ios_base::failbit);
        return ;
    }

    if(curr_side == 1) 
    {
        if(c == '[') 
        {
            is >> std::skipws >> c;
            if(is.fail() || c != ']')
            {
                is.setstate(std::ios_base::failbit);
                return ;
            }
            else p(row_offset, col_offset) = false;
        }
        else if(c == '(') 
        {
            is >> std::skipws >> c;
            if(is.fail() || c != ')')
            {
                is.setstate(std::ios_base::failbit);
                return ;
            }
            else p(row_offset, col_offset) = true;
        }
        else                    //failing state
        {
            is.setstate(std::ios_base::failbit);
            return ;
        }
        
        return ;
    }
    else if(curr_side > 1)
    {
        int half_side = curr_side / 2;

        if(c == '[')
        {
            is >> std::skipws >> c;
            if(is.fail() || c != ']')
            {
                is.setstate(std::ios_base::failbit);
                return ;
            }
    
            for(uint32_t i = row_offset; i < row_offset + curr_side; i++)
                for(uint32_t j =col_offset; j < col_offset + curr_side; j++)
                    p(i,j) = false;
        }
        else if (c == '(')
        {
            char next_c;
            is >> std::skipws >> next_c;
            if(is.fail()) { is.setstate(std::ios_base::failbit); return ; }

            if(next_c == ')')
            {
                for(uint32_t i = row_offset; i < row_offset + curr_side; i++)
                    for(uint32_t j =col_offset; j < col_offset + curr_side; j++)
                        p(i,j) = true;
            }
            else
            {
                if (!is.putback(next_c)) 
                {
                    is.setstate(std::ios_base::failbit);
                    return;
                }

                //Top-Lefts
                input_grid_rec(is, p, half_side, row_offset, col_offset);
                if(is.fail())
                {
                    is.setstate(std::ios_base::failbit);
                    return;
                }
            
                //Top-rigth
                input_grid_rec(is, p, half_side, row_offset, col_offset + half_side);
                if(is.fail()) 
                {
                    is.setstate(std::ios_base::failbit);
                    return;
                }
            
                //Bottom-left
                input_grid_rec(is, p, half_side, row_offset + half_side, col_offset);
                if(is.fail()) 
                {
                    is.setstate(std::ios_base::failbit);
                    return;
                }
            
                //Bottom-right
                input_grid_rec(is, p, half_side, row_offset + half_side, col_offset + half_side);
                if(is.fail()) 
                {
                    is.setstate(std::ios_base::failbit);
                    return;
                }

                is >> std::skipws >> c;
                if(is.fail() || c != ')')
                {
                    is.setstate(std::ios_base::failbit);
                    return ;
                }
            }
        }
    }
}


void output_grid_rec(std::ostream& os, piece const& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset)
{
    if(curr_side == 1) 
    {
        //os <<  "(";   --> Formato non accettato
        if(p(row_offset, col_offset) == false) os << "[]";
        else os << "()";
        //os << ")";
        return ;
    }
    else if(curr_side > 1)
    {
        if(p.empty(row_offset, col_offset, curr_side))
        {
            os << "[]";
            return ;
        } 
        else if(p.full(row_offset, col_offset, curr_side)) 
        {
            os << "()";
            return ;
        }

        os << "(";

        uint32_t half_side = curr_side / 2;
        output_grid_rec(os, p, half_side, row_offset, col_offset);
        output_grid_rec(os, p, half_side, row_offset, col_offset + half_side);
        output_grid_rec(os, p, half_side, row_offset + half_side, col_offset);
        output_grid_rec(os, p, half_side, row_offset + half_side, col_offset + half_side);

        os << ")";
    }
}

std::istream& operator>>(std::istream& is, piece& p)
{
    uint32_t val_side;
    uint32_t val_color_32;
    is >> std::skipws >> val_side >> std::skipws >> val_color_32;
    if(is.fail()) 
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }
    if (val_color_32 > 255) 
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }
    uint8_t val_color = static_cast<uint8_t>(val_color_32); 
    
    //Controlliamo se val_side è 2^n
    if((val_side & (val_side - 1)) != 0 || val_side == 0 || val_color == 0)   
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }      
    piece temp_piece;                 
    try{ temp_piece = piece(val_side, val_color); }  //try catch in caso fallisca allocaziones
    catch(const tetris_exception& e)
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    //Commentata tutta la logica in operator>>: verrà tutta gestita da input_grid_rec
    input_grid_rec(is, temp_piece, val_side, 0, 0);
    if(is.fail())
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }


    p = std::move(temp_piece); //Usa il move constructor
    return is;
}


std::ostream& operator<<(std::ostream& os, piece const& p)  //empty(i,j,s) and full(i,j,s) are useful to write the piece is the recursive format to an output stream
{
    os << p.side() << " " << static_cast<uint32_t>(p.color()) << " ";
    output_grid_rec(os, p, p.side(), 0, 0);

    return os;
}

std::istream& operator>>(std::istream& is, tetris& t)
{
    uint32_t width;
    uint32_t height;
    uint32_t score;
    is >> std::skipws >> width >> height >> score;
    if(is.fail()) 
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    tetris temp_t(width, height, score);

    uint32_t num_pieces;
    is >> std::skipws >> num_pieces;
    if(is.fail())
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    for(uint32_t i = 0; i < num_pieces; i++)
    {
        
        piece p_data;
        int x = 0;
        int y = 0;
        is >> std::skipws >> x >> y;
        
        is >> std::skipws >> p_data;
        
        if(is.fail())
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        try { temp_t.add(p_data, x, y); }   //temp_t.insert(p_data, x);
        catch(const std::bad_alloc& e)
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }
        catch (const tetris_exception& e) 
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }
    }

    t = std::move(temp_t);
    return is;
}

std::ostream& operator<<(std::ostream& os, tetris const& t)
{
    os << t.width() << " " << t.height() << " " << t.score() << std::endl;  //Dimensioni e Punteggio

    uint32_t piece_count = 0;
    for(auto it = t.begin(); it != t.end(); ++it)  { piece_count++; }
    os << piece_count << std::endl;                     //Numero pezzi

    for(auto it = t.begin(); it != t.end(); ++it)
        os << it->x << " " << it->y << " " << it->p << std::endl;  //X, Y e Piece
    return os;
}