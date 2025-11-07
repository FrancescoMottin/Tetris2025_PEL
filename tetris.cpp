#include "tetris.hpp"
#include <fstream>

#define CHECK_ERR(cond, msg) if((cond)) { is.setstate(std::ios_base::failbit);  throw tetris_exception(msg); }
using namespace std;

piece::piece() 
{
	m_side = 0;
    m_color = 0;
    m_grid = nullptr;
};

bool power2(uint32_t n) 
{
    if(n == 1) return true;
    if(n < 1 || n % 2 != 0) return false;
    return power2(n / 2);
};

piece::piece(uint32_t s, uint8_t c) 
{
	if(!power2(s)) throw tetris_exception("s = " + std::to_string(s) + " is not a power of 2");
	else if(c == 0) throw tetris_exception("c is 0");
	
	m_side = s;
    m_color = c;
    m_grid = new bool*[s];
    
    for(uint32_t it = 0; it < m_side; it++) 
		m_grid[it] = new bool[s]();             // The () value initializes the arrays to false
};

piece::piece(piece const& rhs) 
{
	m_side = rhs.m_side;
	m_color = rhs.m_color;
	
	m_grid = new bool*[m_side];
	
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) 
    {
		m_grid[it1] = new bool[m_side];
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) 
			m_grid[it1][it2] = rhs.m_grid[it1][it2];
	}
};

piece::piece(piece&& rhs) 
{
	m_side = rhs.m_side;
	m_color = rhs.m_color;
	m_grid = rhs.m_grid;
	
	rhs.m_side = 0;
	rhs.m_color = 0;
	rhs.m_grid = nullptr;
};

piece::~piece() 
{ 
	for(uint32_t it1 = 0; it1 < m_side; it1++)  // Delete on every array of the matrixS
		delete[] m_grid[it1];
	
	delete[] m_grid; // Deletes the array of pointers
	
	m_side = 0;
	m_color = 0;
};

piece& piece::operator=(piece const& rhs) 
{	
	if(this == &rhs) return *this;  // Control on the address of the objects to avoid auto assignment
	
	for(uint32_t it1 = 0; it1 < m_side; it1++) 
		delete[] m_grid[it1];                   // Delete on every array of the matrix
	
	delete[] m_grid;    // Deletes the array of pointers
	
	m_side = rhs.m_side;
	m_color = rhs.m_color;
	
	m_grid = new bool*[m_side];
	
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) 
    {
		m_grid[it1] = new bool[m_side];
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) 
			m_grid[it1][it2] = rhs.m_grid[it1][it2];
	}
	
	/* Dereferenziation of this
	 * this → pointer to the object
	 * *this → reference to the object
	 */
	return *this;	
};

piece& piece::operator=(piece&& rhs) 
{
	// Control on the address of the objects to avoid auto assignment
	if(this == &rhs) return *this;
	
	for(uint32_t it1 = 0; it1 < m_side; it1++) 
		delete[] m_grid[it1];                   // Delete on every array of the matrix
	
	delete[] m_grid; // Deletes the array of pointers
	
	m_side = rhs.m_side;
	m_color = rhs.m_color;
	m_grid = rhs.m_grid;
	
	rhs.m_side = 0;
	rhs.m_color = 0;
	rhs.m_grid = nullptr;
	
	/* Dereferenziation of this
	 * this -> pointer to the object
	 * *this -> reference to the object
	 */
	return *this;
};

bool piece::operator==(piece const& rhs) const 
{
	if(m_side != rhs.m_side || m_color != rhs.m_color) return false;
		
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++)
			if(m_grid[it1][it2] != rhs.m_grid[it1][it2]) return false;
	}
	
	return true;
};

bool piece::operator!=(piece const& rhs) const 
{ return !(*this == rhs); };

bool& piece::operator()(uint32_t i, uint32_t j) 
{
	if(i >= m_side || j >= m_side) throw tetris_exception("Given coordinates are incorrect. Out of bounds: y = " + std::to_string(i) + " x = " + std::to_string(j) + ". side = " + std::to_string(side()));
	return m_grid[i][j];
};

bool piece::operator()(uint32_t i, uint32_t j) const 
{
	if(i >= m_side || j >= m_side) throw tetris_exception("Given coordinates are incorrect. Out of bounds: y = " + std::to_string(i) + " x = " + std::to_string(j) + ". side = " + std::to_string(side()));
	return m_grid[i][j];
};

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const 
{
	if(i >= m_side || j >= m_side || i + s > m_side || j + s > m_side) throw tetris_exception("Given coordinates are incorrect. Out of bounds");
		
	for(uint32_t it1 = i; it1 < i + s; it1++) 
    {
		for(uint32_t it2 = j; it2 < j + s; it2++)
			if(m_grid[it1][it2]) return false;
	}
	
	return true;
};

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const 
{
	if(i >= m_side || j >= m_side || i + s > m_side || j + s > m_side) throw tetris_exception("Given coordinates are incorrect. Out of bounds");
		
	for(uint32_t it1 = i; it1 < i + s; it1++) 
    {
		for(uint32_t it2 = j; it2 < j + s; it2++) 
			if(!m_grid[it1][it2]) return false;
	}
	
	return true;
};

bool piece::empty() const 
{
	if(m_side == 0) return true;	
	return empty(0,0,m_side);
};

bool piece::full() const { return full(0,0,m_side); };

void piece::rotate() 
{
	bool** tmp = new bool*[m_side];
    
    for(uint32_t it = 0; it < m_side; it++)
		tmp[it] = new bool[m_side];
	
	for(uint32_t it1 = 0; it1 < m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < m_side; it2++)
			tmp[it1][it2] = m_grid[it2][m_side - it1 - 1];
	}
		
	for(uint32_t it1 = 0; it1 < m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < m_side; it2++) 
			m_grid[it1][it2] = tmp[it1][it2];
	}
	
	// Deletion of tmp. Otherwise there would be a memory leak
    for(uint32_t it1 = 0; it1 < m_side; it1++) 
        delete[] tmp[it1];
    delete[] tmp;
};

void piece::cut_row(uint32_t i) 
{
	if(i >= m_side) throw tetris_exception("Given row is incorrect. Out of bounds");

	for(int row = i; row > 0; row--) 
    {
		for(uint32_t col = 0; col < m_side; col++)
			m_grid[row][col] = m_grid[row - 1][col];
	}

	for(uint32_t col = 0; col < m_side; col++)
		m_grid[0][col] = false;
};

//FUNZIONI DI DEBUG
void piece::print_ascii_art(std::ostream& os) const 
{
	for(uint32_t it1 = 0; it1 < m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < m_side; it2++) 
        {
			if(m_grid[it1][it2]) os << "\033[48;5;" << int(m_color) << "m" << ' ' << "\033[m";
			else os << ' ';
		}
		os << '\n';
	}
};

uint32_t piece::side() const { return m_side; };

int piece::color() const { return m_color; };

tetris::tetris() 
{
	m_score = 0;
    m_width = 0;
    m_height = 0;
    m_field = nullptr;
};

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) 
{
	if(w == 0 || h == 0) throw tetris_exception("exception in tetris constructor. width or height passed by argument equals to 0");
	
	m_score = s;
    m_width = w;
    m_height = h;
    m_field = nullptr;
};

tetris::tetris(tetris const& rhs) 
{
	m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_field = nullptr;
    
    node* tmp = rhs.m_field;
    
    while(tmp != nullptr) {
		if(m_field == nullptr) 
        {
			m_field = new node();
			m_field->tp = tmp->tp;
			m_field->next = nullptr;
		} 
        else 
        {
			node* it = m_field;
			
			while(it->next != nullptr) 
				it = it->next;
			
			node* newN = new node();
			newN->tp = tmp->tp;
			newN->next = nullptr;
			
			it->next = newN;
		}
		tmp = tmp->next;
	}  
};

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
};

tetris::~tetris() 
{
	m_score = 0;
    m_width = 0;
    m_height = 0;
    
    node* tmp = nullptr;
    
    while(m_field != nullptr) 
    {
		tmp = m_field;
		m_field = m_field->next;
		delete tmp;
	}
};

tetris& tetris::operator=(tetris const& rhs) 
{
	if(*this == rhs) return *this;
	
	m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    
    node* tmp = nullptr;
    
    while(m_field != nullptr) 
    {
		tmp = m_field;
		m_field = m_field->next;
		delete tmp;
	}
    
    tmp = rhs.m_field;
    
    while(tmp != nullptr) 
    {
		if(m_field == nullptr) 
        {
			node* newN = new node();
			newN->tp = tmp->tp;
			newN->next = nullptr;
			m_field = newN;
		} 
        else 
        {
			node* it = m_field;
			
			while(it->next != nullptr)
				it = it->next;
			
			node* newN = new node();
			newN->tp = tmp->tp;
			newN->next = nullptr;
			
			it->next = newN;
		}
		tmp = tmp->next;
	}
	
	return *this;	
};

tetris& tetris::operator=(tetris&& rhs) 
{
    if (this != &rhs) 
    {
        node* tmp;
        while (m_field != nullptr) 
        {
            tmp = m_field;
            m_field = m_field->next;
            delete tmp;
        }

        m_score = rhs.m_score;
        m_width = rhs.m_width;
        m_height = rhs.m_height;
        m_field = rhs.m_field;

        rhs.m_score = 0;
        rhs.m_width = 0;
        rhs.m_height = 0;
        rhs.m_field = nullptr;
    }

    return *this;
};

bool tetris::operator==(tetris const& rhs) const 
{
	if(m_score != rhs.m_score || m_width != rhs.m_width || m_height != rhs.m_height) return false;
	
	node* tmp1 = m_field;
	node* tmp2 = rhs.m_field;
	
	while(tmp1 != nullptr && tmp2 != nullptr) 
    {
		if(tmp1->tp.p != tmp2->tp.p || tmp1->tp.x != tmp2->tp.x || tmp1->tp.y != tmp2->tp.y) return false;
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;		
	}
	
	if((tmp1 != nullptr && tmp2 == nullptr) || (tmp1 == nullptr && tmp2 != nullptr)) return false;
	
	return true;
};
bool tetris::operator!=(tetris const& rhs) const { return !(*this == rhs);};

struct field 
{
    bool** f;
    const tetris& t;

    field(const tetris& rhs) : f(nullptr), t(rhs) 
    {
        f = new bool*[t.height()];
        for(uint32_t i = 0; i < t.height(); ++i)
            f[i] = new bool[t.width()]();

        for(auto it = rhs.begin(); it != rhs.end(); ++it) 
            add(*it);
    };

    ~field() 
    {
        for(uint32_t i = 0; i < t.height(); i++) 
            delete[] f[i];
        delete[] f;
    };

    void add(const tetris_piece& tp) 
    {
        int piece_y = int(tp.p.side()) - 1;
        for(int i = tp.y; i > tp.y - int(tp.p.side()); --i) 
        {
            int piece_x = 0;
            for(int j = tp.x; j < tp.x + int(tp.p.side()); ++j) 
            {
                if(tp.p(piece_y, piece_x)) 
                    if (i >= 0 && i < int(t.height()) && j >= 0 && j < int(t.width())) f[i][j] = true;

                ++piece_x;
            }
            --piece_y;
        }
    };

    bool full_row() const 
    {
        for(int it1 = int(t.height()) - 1; it1 >= 0; --it1) 
        {
            uint32_t c = 0;
            for(uint32_t it2 = 0; it2 < t.width(); ++it2) 
                if(f[it1][it2]) ++c;

            if(c == t.width()) return true;
        }
        return false;
    };

    int first_full_row() const 
    {
        for(int it1 = int(t.height()) - 1; it1 >= 0; --it1) 
        {
            int c = 0;
            for(uint32_t it2 = 0; it2 < t.width(); ++it2)
                if(f[it1][it2]) ++c;

            if(uint32_t(c) == t.width()) return it1;
        }
        return int(t.height());
    };

    void clear_field() 
    {
        for(int i = 0; i < int(t.height()); ++i)
            for(int j = 0; j < int(t.width()); ++j)
                f[i][j] = false;
    };
};

void tetris::insert(piece const& p, int x) 
{
	if(p.empty()) return;
		
	int max_y = -1;
	bool obstacle = false;
	 
    for(int y = 0; y < int(m_height) + int(p.side()); y++) 
    {
        if(containment(p, x, y) && !obstacle) 
            max_y = y; 
        else 
        {
			field f(*this);
			
			uint32_t piece_x = 0;
			int piece_y = int(p.side()) - 1;
			for(int i = y; i > y - int(p.side()); --i) 
            {
				piece_x = 0;
				for(int j = x; j < x + int(p.side()); ++j) 
                {
					if(((i >= 0 && i < int(m_height)) && (j >= 0 && j < int(m_width))) && (p(piece_y, piece_x) && f.f[i][j])) obstacle = true;
			
					++piece_x;
				}
				--piece_y;
			}		
		}
    }

    if(max_y == -1) throw tetris_exception("GAME OVER!!! tetris piece p cannot be placed");

    add(p, x, max_y);
    
    field f(*this);
	
	// finds all the full row inside the field
	while(f.full_row()) 
    {
		int cutted_row = -1;
		// iterates all the pieces and cut the row
		for(auto it = begin(); it != end(); it++) 
        {
			int field_y = it->y;
			for(int y = int(it->p.side()) - 1; y >= 0; --y) 
            {
				if(f.first_full_row() == field_y) 
                {
					cutted_row = field_y;
					it->p.cut_row(y);
				}
					
				field_y--;
			} 
		}
		
		m_score = m_score + m_width;		
		
		// checks if the pieces can be shifted down
		for(auto it = begin(); it != end(); it++) 
        {
			int field_y = it->y;
			
			bool shift = true;
			for(int i = int(it->p.side()) - 1; i >= 0; --i) 
            {
				for(int j = 0; j < int(it->p.side()); ++j) 
					if(it->p(i, j) && field_y >= cutted_row) shift = false;
	
				field_y--;
			} 
			
			if(shift) it->y = it->y + 1;
		}
		
		// updates the field f
		f.clear_field();
		for(auto it = begin(); it != end(); it++)
			f.add(*it);
	}
	
	// all empty pieces are removed
	while(m_field != nullptr && m_field->tp.p.empty()) 
    {
		node* to_delete = m_field;
		m_field = m_field->next;
		delete to_delete;
	}

	node* tmp = m_field;
	while(tmp != nullptr) 
    {
		if(tmp->next != nullptr) 
        {
			if(tmp->next->tp.p.empty()) 
            {
				node* to_delete = tmp->next;
				tmp->next = tmp->next->next;
				delete to_delete;
			} 
            else tmp = tmp->next;
		} 
        else tmp = tmp->next;
	}
};

void tetris::add(piece const& p, int x, int y) 
{
	if(!containment(p, x, y))  throw tetris_exception("exception in tetris add function. tetris piece p cannot be contained at offset (x: " + std::to_string(x) + ", y: " + std::to_string(y) + ")");

	node* newN = new node();
	
	newN->tp.p = p;
	newN->tp.x = x;
	newN->tp.y = y;
	
	newN->next = m_field;
	m_field = newN;
};

bool tetris::containment(piece const& p, int x, int y) const 
{
    field f(*this);
    
    uint32_t piece_x = 0;
    int piece_y = int(p.side()) - 1;
    for(int i = y; i > y - int(p.side()); --i) 
    {
        piece_x = 0;
        for(int j = x; j < x + int(p.side()); ++j) 
        {
            if(p(piece_y, piece_x)) 
            {
                if((i < 0 || i >= int(m_height)) || (j < 0 || j >= int(m_width))) return false;
                if(f.f[i][j]) return false;
            }
            ++piece_x;
        }
        --piece_y;
    }
    return true;
};

// FUNZIONE DI DEBUG
void tetris::print_ascii_art(std::ostream& os) const 
{
    int** tmp_mat = nullptr;
    
    try 
    {
        tmp_mat = new int*[m_height];
        for(uint32_t i = 0; i < m_height; i++) 
        {
            tmp_mat[i] = new int[m_width];
            for(uint32_t j = 0; j < m_width; j++)
                tmp_mat[i][j] = -1;
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
                    //tmp_mat[abs_y][abs_x] = '#';  
                    
                    if(abs_x >= 0 && abs_y >= 0 && abs_x < (int) m_width && abs_y < (int) m_height) tmp_mat[abs_y][abs_x] = p.color();
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
        {
            //os << tmp_mat[i][j];
            if(tmp_mat[i][j] != -1) os << "\033[38;5;" << tmp_mat[i][j] << "m#\033[0m"; 
            else os << " ";
        }
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
};

// ITERATOR METHODS
tetris::iterator::iterator(node* ptr) : m_ptr(ptr) {};
tetris::iterator::reference tetris::iterator::operator*() { return m_ptr->tp; }; // returns the reference to the tetris_piece object 
tetris::iterator::pointer tetris::iterator::operator->() { return &(m_ptr->tp); }; // returns the pointer to the tetris_piece object 
tetris::iterator& tetris::iterator::operator++() // prefix increment: ++it 
{
	m_ptr = m_ptr->next;
	return *this;
};
tetris::iterator tetris::iterator::operator++(int /*dummy*/) // postfix increment: it++
{
	iterator it = m_ptr;	
	++(*this); // call to the prefix operator on this
	return it;
};
bool tetris::iterator::operator==(iterator const& rhs) const { return m_ptr == rhs.m_ptr; }; // checks if two iterators are pointing to the same node in the memory
bool tetris::iterator::operator!=(iterator const& rhs) const { return m_ptr != rhs.m_ptr; };

// CONST ITERATOR METHODS
tetris::const_iterator::const_iterator(node const* ptr) : m_ptr(ptr) {};
tetris::const_iterator::reference tetris::const_iterator::operator*() const { return m_ptr->tp; };
tetris::const_iterator::pointer tetris::const_iterator::operator->() const { return &(m_ptr->tp); };
tetris::const_iterator& tetris::const_iterator::operator++() 
{
	m_ptr = m_ptr->next;
	return *this;
};
tetris::const_iterator tetris::const_iterator::operator++(int) 
{
	const_iterator it = m_ptr;
	++(*this);
	return it;
};
bool tetris::const_iterator::operator==(const_iterator const& rhs) const { return m_ptr == rhs.m_ptr; };
bool tetris::const_iterator::operator!=(const_iterator const& rhs) const { return m_ptr != rhs.m_ptr; };

typename tetris::iterator
tetris::begin() { return m_field; };

typename tetris::iterator
tetris::end() { return nullptr; };

typename tetris::const_iterator
tetris::begin() const { return m_field; };

typename tetris::const_iterator
tetris::end() const { return nullptr; };

uint32_t tetris::score() const { return m_score; };
uint32_t tetris::width() const { return m_width; };
uint32_t tetris::height() const { return m_height; };

// S->()|[]|(SSSS)
void input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset)
{
    CHECK_ERR(is.fail(), "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Errore di lettura pre-lettura");
 
    char c;
    is >> std::ws >> c;
    CHECK_ERR(is.fail(), "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Errore di sintassi inziale");

    if(curr_side == 1) 
    {
        char next;
        if(c == '[') 
        {
            is >> std::ws >> next;
            CHECK_ERR(is.fail() || next != ']', "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Caso side=1 vuoto, sintassi non rispettata");
            p(row_offset, col_offset) = false;
        }
        else if(c == '(') 
        {
            is >> std::ws >> next;
            CHECK_ERR(is.fail() || next != ')', "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Caso side=1 pieno, sintassi non rispettata");
            p(row_offset, col_offset) = true;
        }
        else CHECK_ERR(true, "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Stato fallimentare (side == 1)");//failing state
        
        return ;
    }

    if(c == '[')
    {
        is >> std::ws >> c;
       
        CHECK_ERR(is.fail() || c != ']', "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Caso vuoto, sintassi non rispettata");//failing state
    
        for(uint32_t i = row_offset; i < row_offset + curr_side; i++)
            for(uint32_t j =col_offset; j < col_offset + curr_side; j++)
                p(i,j) = false;

        return; 
    }
    else if (c == '(')
    {
        int half_side = curr_side / 2;

        is >> std::ws; // >> next_c;
        char next_c = is.peek();
        
        CHECK_ERR(is.fail(), "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Sintassi non rispettata");

        if(next_c == ')')
        {
            is.get();
            for(uint32_t i = row_offset; i < row_offset + curr_side; i++)
                for(uint32_t j =col_offset; j < col_offset + curr_side; j++)
                    p(i,j) = true;
            
            return ;
        }
        
        //Top-Left
        try{ input_grid_rec(is, p, half_side, row_offset, col_offset); }
        catch(const tetris_exception& e) { CHECK_ERR(true, e.what() + " 1");}
            
        //Top-rigth
        try{ input_grid_rec(is, p, half_side, row_offset, col_offset + half_side); }
        catch(const tetris_exception& e) { CHECK_ERR(true, e.what() + " 2"); }
            
        //Bottom-left
        try{ input_grid_rec(is, p, half_side, row_offset + half_side, col_offset); }
        catch(const tetris_exception& e) { CHECK_ERR(true, e.what() + " 3"); }
            
        //Bottom-right
        try{ input_grid_rec(is, p, half_side, row_offset + half_side, col_offset + half_side); }
        catch(const tetris_exception& e) { CHECK_ERR(true, e.what() + " 4"); }

        is >> std::skipws >> c;
        CHECK_ERR(is.fail() || c != ')', "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Ultima parentesi di chiusura ')' non rispettata ");
                
        return ;
    }
    
    CHECK_ERR(true, "ERROR! - input_grid_rec(std::istream& is, piece& p, uint32_t curr_side, uint32_t row_offset, uint32_t col_offset) - Carattere iniziale non valido");
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
    CHECK_ERR(is.fail(), "ERROR! - operator>>(std::istream& is, piece& p) - Formato input invalido (inzio funzione).");
    CHECK_ERR(val_color_32 > 255, "ERROR! - operator>>(std::istream& is, piece& p) - Colore dato in input non valido");
    
    uint8_t val_color = static_cast<uint8_t>(val_color_32); 
    
    //Controlliamo se val_side è 2^n
    CHECK_ERR((val_side & (val_side - 1)) != 0 || val_side == 0 || val_color == 0 , "ERROR! - operator>>(std::istream& is, piece& p) - Side o color non validi.");
    
    piece temp_piece;                 
    try{ temp_piece = piece(val_side, val_color); }  //try catch in caso fallisca allocazione
    catch(const tetris_exception& e) { CHECK_ERR(true, e.what()); }

    //Commentata tutta la logica in operator>>: verrà tutta gestita da input_grid_rec
    try { input_grid_rec(is, temp_piece, val_side, 0, 0); }
    catch( const tetris_exception& e) { CHECK_ERR(true, e.what()); }

    p = std::move(temp_piece); //Usa il move constructor
    return is;
}


std::ostream& operator<<(std::ostream& os, piece const& p)  //empty(i,j,s) and full(i,j,s) are useful to write the piece is the recursive format to an output stream
{
    os << p.side() << " " << static_cast<uint32_t>(p.color()) << " ";
    output_grid_rec(os, p, p.side(), 0, 0);

    return os;
}

//Errori non arrivano?
std::istream& operator>>(std::istream& is, tetris& t)
{
    uint32_t width;
    uint32_t height;
    uint32_t score;
    is >> std::skipws >> width >> height >> score;
    CHECK_ERR(is.fail(), "ERROR! - operator>>(std::istream& is, tetris& t) - Formato input invalido (lettura fallita).");

    tetris temp_t(width, height, score);

    uint32_t num_pieces;
    is >> std::skipws >> num_pieces;
    CHECK_ERR(is.fail(), "ERROR! - operator>>(std::istream& is, tetris& t) - Formato input invalido (lettura fallita).");

    for(uint32_t i = 0; i < num_pieces; i++)
    {   
        piece p_data;
        int x = 0;
        int y = 0;
        is >> std::skipws >> x >> y;
        
        is >> std::skipws >> p_data;
        
        CHECK_ERR(is.fail(), "ERROR! - operator>>(std::istream& is, tetris& t) - Formato input invalido (lettura fallita).");

        try { temp_t.add(p_data, x, y); }  
        catch(const std::bad_alloc& e) { CHECK_ERR(true, "ERROR! - operator>>(std::istream& is, tetris& t) - Errore di allocazione nella lista (causa add)"); }
        catch (const tetris_exception& e) { CHECK_ERR(true, e.what()); }
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
