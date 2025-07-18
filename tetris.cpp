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
    if(((s & (s - 1)) != 0 || s == 0) || c == 0) throw tetris_exception("");
    m_side = s;
    m_color = c;
    
    m_grid = new bool*[m_side]; //initialize a s*s matrix with all cells at false, must be an empty piece
    for(uint32_t i = 0; i < s; i++)
    {
        m_grid[i] = new bool[s];
        for(uint32_t j = 0; j < s; j++)
            m_grid[i][j] = false;
    }
    //assert(empty() == true);
}

//Copy constructor
piece::piece(piece const& rhs)
{
    m_side = rhs.m_side;
    m_color = rhs.m_color;

    if(m_side > 0)
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
    //assert(empty() == true);
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
    if(m_grid == nullptr) return ; //forse throw tetris_exception?
    
    m_side = 0;
    m_color = 0;

    for(uint32_t i = 0; i < m_side; i++)    //deallocazione colonne
        delete[] m_grid[i];                 
    delete[] m_grid;                        //deallocazione righe

    m_grid = nullptr;

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

    for(uint32_t i = 0; i < m_side; i++)
        for(uint32_t j = 0; j < m_side; j++)
            if(m_grid[i][j] != rhs.m_grid[i][j]) return false;
    
    return true;
}

bool piece::operator!=(piece const& rhs) const
{
    return !(operator==(rhs));
}

bool& piece::operator()(uint32_t i, uint32_t j)
{
    if(m_grid == nullptr || i >= m_side || j >= m_side) throw tetris_exception("");     //tetris_exception if (i,j) is out of bounds.
    return m_grid[i][j];
}  

bool piece::operator()(uint32_t i, uint32_t j) const
{
    if(m_grid == nullptr || i >= m_side || j >= m_side) throw tetris_exception("");     //tetris_exception if (i,j) is out of bounds.
    return m_grid[i][j];
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const
{
    if(s == 0) return true;
    if(m_grid == nullptr ) throw tetris_exception("");    //tetris_exception if out of bounds
    if(i >= m_side || j >= m_side) throw tetris_exception("");
    if(i + s > m_side || j + s > m_side) throw tetris_exception("");
    

    for(int i2 = i; i2 < i+s; i2++)
        for(int j2 = j; j2 < j+s; j2++)
            if(m_grid[i2][j2] == true) return false;

    return true;
}

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const
{
    if(s == 0) return true;
    if(m_grid == nullptr ) throw tetris_exception("");    //tetris_exception if out of bounds
    if(i >= m_side || j >= m_side) throw tetris_exception("");
    if(i + s > m_side || j + s > m_side) throw tetris_exception("");
    

    for(int i2 = i; i2 < i+s; i2++)
        for(int j2 = j; j2 < j+s; j2++)
            if(m_grid[i2][j2] == false) return false;

    return true;
}

bool piece::empty() const
{
    if(m_grid == nullptr) throw tetris_exception("");
    for(int i = 0; i < m_side; i++)
        for(int j = 0; j < m_side; j++)
            if(m_grid[i][j] == true) return false;

    return true;
}

bool piece::full() const
{
    if(m_grid == nullptr) throw tetris_exception("");
    for(int i = 0; i < m_side; i++)
        for(int j = 0; j < m_side; j++)
            if(m_grid[i][j] == false) return false;

    return true;
}

void piece::rotate()
{
    if(m_grid == nullptr) return ;

    bool** tmp_grid = new bool*[m_side];
    if(m_side > 1)
    {
        for(uint32_t i = 0; i < m_side; i++)
        {         
            tmp_grid[i] = new bool[m_side];
            for(uint32_t j = 0; j < m_side; j++)
            {
                int new_row = j;
                int new_col = m_side - i - 1;
                tmp_grid[new_row][new_col] = m_grid[i][j];
            }    
        }
    }
    else tmp_grid = nullptr;

    if(m_grid != nullptr && m_side > 0)
    {
        for(uint32_t i = 0; i < m_side; i++)    
            delete[] m_grid[i];                   
        delete[] m_grid;                        
        m_grid = nullptr;                       
    }

    m_grid = tmp_grid;
}

//maybe implement try-catch for std::bad_alloc
void piece::cut_row(uint32_t i)
{
    if(m_grid == nullptr || m_side == 0) throw tetris_exception("");
    if(i >= m_side) throw tetris_exception("");
    if(m_side == 1) 
    {
        delete[] m_grid[0];
        delete[] m_grid;
        m_side = 0;
        m_grid = nullptr;

        return ;
    }

    bool** tmp_grid = new bool*[m_side-1]; // (m_side-1) x (m_side-1) oppure (m_side-1) x m_side
    for (uint32_t r = 0; r < m_side; ++r)
    {
        if (r < i)
        {
            tmp_grid[r] = new bool[m_side-1];
            for(uint32_t c = 0; c < m_side-1; ++c)
                tmp_grid[r][c] = m_grid[r][c];
        }
        else if(r > i)
        {
            tmp_grid[r-1] = new bool[m_side-1];
            for(uint32_t c = 0; c < m_side-1; ++c)
                tmp_grid[r-1][c] = m_grid[r][c];
        }
        //No r == i because we want to skip it
    }
    
    if(m_grid != nullptr && m_side > 0)
    {
        for(uint32_t i = 0; i < m_side; i++)    
            delete[] m_grid[i];                   
        delete[] m_grid;                        
        m_grid = nullptr;                       
    }

    m_grid = tmp_grid;
}

//NOT NECESSARY BUT USEFUL FOR DEBUGGING
void tetris::print_ascii_art(std::ostream& os) const
{
}

uint32_t piece::side() const { return m_side; }
int piece::color() const { return m_color; }

/*
class tetris
{
    struct node {
        tetris_piece tp;
        node* next;
    };
    private:
        uint32_t m_score;
        uint32_t m_width;
        uint32_t m_height;
        node* m_field;
};
*/

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
    if(w <= 0 || h <= 0) throw tetris_exception("");
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
    for(node* it = rhs.m_field; it != nullptr; it = it->next)
    {
        node* new_field = new node{it->tp, nullptr};
        if(!m_field) m_field = tail_field = new_field;
        else tail_field->next = new_field;
        tail_field = tail_field->next;
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
    if(*this ==  rhs) return *this;

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
    for(node* it = rhs.m_field; it != nullptr; it = it->next)
    {
        node* new_field = new node{it->tp, nullptr};
        if(!m_field) m_field = tail_field = new_field;
        else tail_field->next = new_field;
        tail_field = tail_field->next;
    }

    return *this;
}

tetris& tetris::operator=(tetris&& rhs)
{
    
}

bool tetris::operator==(tetris const& rhs) const
{

}
bool tetris::operator!=(tetris const& rhs) const
{

}

void tetris::insert(piece const& p, int x)
{

}
void tetris::add(piece const& p, int x, int y)
{

}

bool tetris::containment(piece const& p, int x, int y) const
{

}
void tetris::print_ascii_art(std::ostream& os) const
{

}

/*
struct iterator {
using iterator_category = std::forward_iterator_tag;
using value_type = tetris_piece;
using reference = tetris_piece&;
using pointer = tetris_piece*;

private:
node* m_ptr;
};
*/
tetris::iterator::iterator(node* ptr)
{}
tetris::iterator::reference tetris::iterator::operator*()
{}
tetris::iterator::pointer tetris::iterator::operator->()
{}
tetris::iterator& tetris::iterator::operator++()
{}
tetris::iterator tetris::iterator::operator++(int /*dummy*/)
{}
bool tetris::iterator::operator==(iterator const& rhs) const
{}
bool tetris::iterator::operator!=(iterator const& rhs) const
{}


tetris::const_iterator::const_iterator(node const* ptr)
{}
tetris::const_iterator::reference tetris::const_iterator::operator*() const
{}
tetris::const_iterator::pointer tetris::const_iterator::operator->() const
{}
tetris::const_iterator& tetris::const_iterator::operator++()
{}
tetris::const_iterator tetris::const_iterator::operator++(int /*dummy*/)
{}
bool tetris::const_iterator::operator==(const_iterator const& rhs) const
{}
bool tetris::const_iterator::operator!=(const_iterator const& rhs) const
{}


tetris::iterator tetris::begin()
{}
tetris::iterator tetris::end()
{}
tetris::const_iterator tetris::begin() const
{}
tetris::const_iterator tetris::end() const
{}

uint32_t tetris::score() const { return m_score; }
uint32_t tetris::width() const { return m_width; }
uint32_t tetris::height() const { return m_height; }

/*
private:
uint32_t m_score;
uint32_t m_width;
uint32_t m_height;
node* m_field;
};*/
std::istream& operator>>(std::istream& is, piece& p)
{

}
std::ostream& operator<<(std::ostream& os, piece const& p)
{}

std::ostream& operator<<(std::ostream& os, tetris const& t)
{}
std::istream& operator>>(std::istream& is, tetris& t)
{}
