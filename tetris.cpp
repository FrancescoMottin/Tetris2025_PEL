#include "tetris.hpp"
#include <fstream>

#define CHECK_ERR(cond, msg) if((cond)) { is.setstate(std::ios_base::failbit);  throw tetris_exception(msg); }
using namespace std;

piece::piece() 
{
	this->m_side = 0;
    this->m_color = 0;
    this->m_grid = nullptr;
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
	
	this->m_side = s;
    this->m_color = c;
    this->m_grid = new bool*[s];
    
    for(uint32_t it = 0; it < this->m_side; it++) 
    {
		// The () value initializes the arrays to false
		this->m_grid[it] = new bool[s]();
	}
};

piece::piece(piece const& rhs) 
{
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	
	this->m_grid = new bool*[this->m_side];
	
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) 
    {
		this->m_grid[it1] = new bool[this->m_side];
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) 
        {
			this->m_grid[it1][it2] = rhs.m_grid[it1][it2];
		}
	}
};

piece::piece(piece&& rhs) 
{
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	this->m_grid = rhs.m_grid;
	
	rhs.m_side = 0;
	rhs.m_color = 0;
	rhs.m_grid = nullptr;
};

piece::~piece() 
{ 
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
		// Delete on every array of the matrix
		delete[] this->m_grid[it1];
	}
	
	// Deletes the array of pointers
	delete[] m_grid;
	
	this->m_side = 0;
	this->m_color = 0;
};

piece& piece::operator=(piece const& rhs) 
{
	// Control on the address of the objects to avoid auto assignment
	if(this == &rhs) return *this;
	
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
		// Delete on every array of the matrix
		delete[] this->m_grid[it1];
	}
	
	// Deletes the array of pointers
	delete[] this->m_grid;
	
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	
	this->m_grid = new bool*[this->m_side];
	
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) 
    {
		this->m_grid[it1] = new bool[this->m_side];
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) 
        {
			this->m_grid[it1][it2] = rhs.m_grid[it1][it2];
		}
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
	
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
		// Delete on every array of the matrix
		delete[] this->m_grid[it1];
	}
	
	// Deletes the array of pointers
	delete[] this->m_grid;
	
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	this->m_grid = rhs.m_grid;
	
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
	if(this->m_side != rhs.m_side || this->m_color != rhs.m_color) return false;
		
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) 
        {
			if(this->m_grid[it1][it2] != rhs.m_grid[it1][it2]) return false;
		}
	}
	
	return true;
};

bool piece::operator!=(piece const& rhs) const { return !(*this == rhs); };

bool& piece::operator()(uint32_t i, uint32_t j) 
{
	if(i >= this->m_side || j >= this->m_side)
		throw tetris_exception("Given coordinates are incorrect. Out of bounds: y = " + std::to_string(i) + " x = " + std::to_string(j) + ". side = " + std::to_string(this->side()));
	return this->m_grid[i][j];
};

bool piece::operator()(uint32_t i, uint32_t j) const 
{
	if(i >= this->m_side || j >= this->m_side)
		throw tetris_exception("Given coordinates are incorrect. Out of bounds: y = " + std::to_string(i) + " x = " + std::to_string(j) + ". side = " + std::to_string(this->side()));
	return this->m_grid[i][j];
};

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const 
{
	if(i >= this->m_side || j >= this->m_side || i + s > this->m_side || j + s > this->m_side)
		throw tetris_exception("Given coordinates are incorrect. Out of bounds");
		
	for(uint32_t it1 = i; it1 < i + s; it1++) 
    {
		for(uint32_t it2 = j; it2 < j + s; it2++) 
        {
			if(this->m_grid[it1][it2]) return false;
		}
	}
	
	return true;
};

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const 
{
	if(i >= this->m_side || j >= this->m_side || i + s > this->m_side || j + s > this->m_side)
		throw tetris_exception("Given coordinates are incorrect. Out of bounds");
		
	for(uint32_t it1 = i; it1 < i + s; it1++) 
    {
		for(uint32_t it2 = j; it2 < j + s; it2++) 
        {
			if(!this->m_grid[it1][it2]) return false;
		}
	}
	
	return true;
};

bool piece::empty() const 
{
	if(this->m_side == 0) return true;	
	return empty(0,0,this->m_side);
};

bool piece::full() const { return full(0,0,this->m_side); };

void piece::rotate() 
{
	bool** tmp = new bool*[this->m_side];
    
    for(uint32_t it = 0; it < this->m_side; it++) 
    {
		tmp[it] = new bool[this->m_side];
	}
	
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < this->m_side; it2++) 
        {
			//tmp[it1][it2] = m_grid[m_side - 1 - it1][it1];
            tmp[it1][it2] = this->m_grid[it2][this->m_side - it1 - 1]; //Rotate antioraria
		}
	}
		
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < this->m_side; it2++) 
        {
			this->m_grid[it1][it2] = tmp[it1][it2];
		}
	}
	
	// Deletion of tmp. Otherwise there would be a memory leak
    for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
        delete[] tmp[it1];
    }
    delete[] tmp;
};

void piece::cut_row(uint32_t i) 
{
	if(i >= this->m_side) throw tetris_exception("Given row is incorrect. Out of bounds");

	for(int row = i; row > 0; row--) 
    {
		for(uint32_t col = 0; col < this->m_side; col++) 
        {
			this->m_grid[row][col] = this->m_grid[row - 1][col];
		}
	}

	for(uint32_t col = 0; col < this->m_side; col++) 
    {
		this->m_grid[0][col] = false;
	}
};

//FUNZIONI DI DEBUG
void piece::print_ascii_art(std::ostream& os) const 
{
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) 
    {
		for(uint32_t it2 = 0; it2 < this->m_side; it2++) 
        {
			if(this->m_grid[it1][it2]) { os << "\033[48;5;" << int(this->m_color) << "m" << ' ' << "\033[m"; } 
            else { os << ' '; }
		}
		os << '\n';
	}
};

uint32_t piece::side() const { return this->m_side; };

int piece::color() const { return this->m_color; };

char nextNonSpaceChar(std::istream& is) 
{
    char c;
    while(is.get(c)) 
    {                   
        if(!std::isspace(static_cast<unsigned char>(c))) return c;
    }
    return '\0';
}

tetris::tetris() 
{
	this->m_score = 0;
    this->m_width = 0;
    this->m_height = 0;
    this->m_field = nullptr;
};

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) 
{
	if(w == 0 || h == 0) throw tetris_exception("exception in tetris constructor. width or height passed by argument equals to 0");
	
	this->m_score = s;
    this->m_width = w;
    this->m_height = h;
    this->m_field = nullptr;
};

tetris::tetris(tetris const& rhs) 
{
	this->m_score = rhs.m_score;
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    this->m_field = nullptr;
    
    node* tmp = rhs.m_field;
    
    while(tmp != nullptr) 
    {
		if(this->m_field == nullptr) 
        {
			this->m_field = new node();
			this->m_field->tp = tmp->tp;
			this->m_field->next = nullptr;
		} 
        else 
        {
			node* it = this->m_field;
			
			while(it->next != nullptr) 
            {
				it = it->next;
			}
			
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
	this->m_score = rhs.m_score;
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    this->m_field = rhs.m_field;
    
    rhs.m_score = 0;
    rhs.m_width = 0;
    rhs.m_height = 0;
    rhs.m_field = nullptr;
};

tetris::~tetris() 
{
	this->m_score = 0;
    this->m_width = 0;
    this->m_height = 0;
    
    node* tmp = nullptr;
    
    while(this->m_field != nullptr) 
    {
		tmp = this->m_field;
		this->m_field = this->m_field->next;
		delete tmp;
	}
};

tetris& tetris::operator=(tetris const& rhs) 
{
	if(*this == rhs) return *this;
	
	this->m_score = rhs.m_score;
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    
    node* tmp = nullptr;
    
    while(this->m_field != nullptr) 
    {
		tmp = this->m_field;
		this->m_field = this->m_field->next;
		delete tmp;
	}
    
    tmp = rhs.m_field;
    
    while(tmp != nullptr) 
    {
		if(this->m_field == nullptr) 
        {
			node* newN = new node();
			newN->tp = tmp->tp;
			newN->next = nullptr;
			this->m_field = newN;
		} 
        else 
        {
			node* it = this->m_field;
			
			while(it->next != nullptr) 
            {
				it = it->next;
			}
			
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
        while (this->m_field != nullptr) 
        {
            tmp = this->m_field;
            this->m_field = this->m_field->next;
            delete tmp;
        }

        this->m_score = rhs.m_score;
        this->m_width = rhs.m_width;
        this->m_height = rhs.m_height;
        this->m_field = rhs.m_field;

        rhs.m_score = 0;
        rhs.m_width = 0;
        rhs.m_height = 0;
        rhs.m_field = nullptr;
    }

    return *this;
};

bool tetris::operator==(tetris const& rhs) const 
{
	if(this->m_score != rhs.m_score || this->m_width != rhs.m_width || this->m_height != rhs.m_height) return false;
	
	node* tmp1 = this->m_field;
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

bool tetris::operator!=(tetris const& rhs) const { return !(*this == rhs); };

struct field 
{
    bool** f;
    const tetris& t;

    field(const tetris& rhs) : f(nullptr), t(rhs) 
    {
        this->f = new bool*[this->t.height()];
        for(uint32_t i = 0; i < this->t.height(); ++i) 
        {
            this->f[i] = new bool[this->t.width()]();
        }

        for(auto it = rhs.begin(); it != rhs.end(); ++it) 
        {
            this->add(*it);
        }
    };

    ~field() 
    {
        for(uint32_t i = 0; i < this->t.height(); i++) 
        {
            delete[] this->f[i];
        }
        delete[] this->f;
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
                {
                    if (i >= 0 && i < int(this->t.height()) && j >= 0 && j < int(this->t.width())) this->f[i][j] = true;
                }
                ++piece_x;
            }
            --piece_y;
        }
    };

    bool full_row() const 
    {
        for(int it1 = int(this->t.height()) - 1; it1 >= 0; --it1) 
        {
            uint32_t c = 0;
            for(uint32_t it2 = 0; it2 < this->t.width(); ++it2) 
            {
                if(this->f[it1][it2]) ++c;
            }
            if(c == this->t.width()) return true;
        }
        return false;
    };

    int first_full_row() const 
    {
        for(int it1 = int(this->t.height()) - 1; it1 >= 0; --it1) 
        {
            int c = 0;
            for(uint32_t it2 = 0; it2 < this->t.width(); ++it2) 
            {
                if(this->f[it1][it2]) ++c;
            }
            if(uint32_t(c) == this->t.width()) return it1;
        }
        return int(this->t.height());
    };

    void clear_field() 
    {
        for(int i = 0; i < int(this->t.height()); ++i)
            for(int j = 0; j < int(this->t.width()); ++j)
                f[i][j] = false;
    };
};

/*
void tetris::insert(piece const& p, int x) 
{
	if(p.empty()) return;
		
	int max_y = -1;
	bool obstacle = false;
	 
    for(int y = 0; y < int(this->m_height) + int(p.side()); y++) 
    {
        if(this->containment(p, x, y) && !obstacle) { max_y = y; } 
        else break; //Se trovi ostacolo, fermati!		
    }

    if(max_y == -1) throw tetris_exception("GAME OVER!!! tetris piece p cannot be placed");
    this->add(p, x, max_y);
    
    field f(*this);
	
	// finds all the full row inside the field
	while(f.full_row()) 
    {
		int cutted_row = -1;
		// iterates all the pieces and cut the row
		for(auto it = this->begin(); it != this->end(); it++) 
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
		
		this->m_score = this->m_score + this->m_width;		
		
		// checks if the pieces can be shifted down
		for(auto it = this->begin(); it != this->end(); it++) 
        {
			int field_y = it->y;
			
			bool shift = true;
			for(int i = int(it->p.side()) - 1; i >= 0; --i) 
            {
				for(int j = 0; j < int(it->p.side()); ++j) 
                {
					if(it->p(i, j) && field_y >= cutted_row) 
                    {
						shift = false;
					}
				}	
				field_y--;
			} 
			
			if(shift) 
            {
				it->y = it->y + 1;
			}
		}
		
		// updates the field f
		f.clear_field();
		for(auto it = this->begin(); it != this->end(); it++) 
        {
			f.add(*it);
		}
	}
	
	// all empty pieces are removed
	while(this->m_field != nullptr && this->m_field->tp.p.empty()) 
    {
		node* to_delete = this->m_field;
		this->m_field = this->m_field->next;
		delete to_delete;
	}
	node* tmp = this->m_field;
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
            else 
            {
				tmp = tmp->next;
			}
		} 
        else 
        {
			tmp = tmp->next;
		}
	}
};
*/

void tetris::insert(piece const& p, int x) {
    if(p.empty()) return;
    
    // 1. Cerca la posizione di atterraggio senza aver ancora aggiunto il pezzo
    int max_y = -1;
    bool found = false;
    for(int y = -int(p.side()); y < (int)m_height; ++y) {
        if(this->containment(p, x, y)) {
            max_y = y;
            found = true;
        } else if(found) break;
    }

    if(!found) throw tetris_exception("GAME OVER!!!");

    // 2. Solo ora aggiungi
    this->add(p, x, max_y);

    // 3. Rimuovi righe e applica gravità
    for(int i = (int)m_height - 1; i >= 0; --i) {
        field f(*this);
        bool full = true;
        for(uint32_t c = 0; c < m_width; ++c) if(!f.f[i][c]) { full = false; break; }

        if(full) {
            m_score += m_width;
            for(auto it = begin(); it != end(); ++it) {
                if(i >= it->y && i < it->y + (int)it->p.side()) it->p.cut_row(i - it->y);
                if(it->y < i) it->y++;
            }
            i++; // Combo check
        }
    }
    
    while(m_field && m_field->tp.p.empty()) {
        node* del = m_field;
        m_field = m_field->next;
        delete del;
    }
    node* curr = m_field;
    while(curr && curr->next) {
        if(curr->next->tp.p.empty()) {
            node* del = curr->next;
            curr->next = curr->next->next;
            delete del;
        } else {
            curr = curr->next;
        }
    }
}

/*
void tetris::insert(piece const& p, int x) 
{
    if(p.empty()) return;
        
    int max_y = -1;
    bool found_at_least_one = false;

    // 1. TROVA IL PUNTO DI ATTERRAGGIO
    // Dobbiamo scansionare da quando il pezzo è tutto fuori (sopra) 
    // a quando tocca il fondo.
    for(int y = -int(p.side()); y < int(this->m_height); y++)
    {
        if(this->containment(p, x, y)) 
        { 
            max_y = y; 
            found_at_least_one = true;
        } 
        else 
        {
            // Se avevamo trovato una posizione valida e ora non più,
            // significa che abbiamo toccato un ostacolo. Fermiamoci.
            if(found_at_least_one) break; 
        }
    }

    // Se non è mai stato trovato un posto (nemmeno in cima), allora è Game Over
    if(max_y == -1) throw tetris_exception("GAME OVER!!! tetris piece p cannot be placed");
    
    this->add(p, x, max_y);

    // 2. RIMOZIONE RIGHE (Logica solida)
    for(int i = int(m_height) - 1; i >= 0; --i) 
    {
        field f(*this);
        bool row_is_full = true;
        for(uint32_t col = 0; col < m_width; ++col) {
            if(!f.f[i][col]) { row_is_full = false; break; }
        }

        if(row_is_full) { 
            m_score += m_width;
            for(auto it = this->begin(); it != this->end(); ++it) {
                if (i >= it->y && i < it->y + int(it->p.side())) {
                    it->p.cut_row(i - it->y);
                }
                // Ogni pezzo che "inizia" sopra la riga i deve scendere
                if (it->y < i) {
                    it->y++;
                }
            }
            i++; 
        }
    }

    // 3. PULIZIA LISTA (Versione anti-frammentazione)
    while(m_field && m_field->tp.p.empty()) {
        node* del = m_field;
        m_field = m_field->next;
        delete del;
    }
    node* curr = m_field;
    while(curr && curr->next) {
        if(curr->next->tp.p.empty()) {
            node* del = curr->next;
            curr->next = curr->next->next;
            delete del;
        } else {
            curr = curr->next;
        }
    }
}
*/

void tetris::add(piece const& p, int x, int y) 
{
	if(!this->containment(p, x, y)) 
    {
		throw tetris_exception("exception in tetris add function. tetris piece p cannot be contained at offset (x: " + std::to_string(x) + ", y: " + std::to_string(y) + ")");
	}
			
	node* newN = new node();
	
	newN->tp.p = p;
	newN->tp.x = x;
	newN->tp.y = y;
	
	newN->next = this->m_field;
	this->m_field = newN;
};

/*
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
                // Coordinate grid_x e grid_y già calcolate nel tuo ciclo
                if (j < 0 || j >= (int)m_width || i >= (int)m_height)   return false; // Blocca muri laterali e pavimento

                if (i >= 0) // Controlla i pezzi esistenti solo se siamo dentro la griglia
                    if (f.f[i][j]) return false; // Collisione con un altro mattoncino

                // Se grid_y < 0 (sopra il campo), non facciamo nulla: il pezzo può stare lì.
            }
            ++piece_x;
        }
        --piece_y;
    }
    return true;
};
*/
bool tetris::containment(piece const& p, int x, int y) const 
{
    for (uint32_t py = 0; py < p.side(); ++py) {
        for (uint32_t px = 0; px < p.side(); ++px) {
            
            // Se il pixel del pezzo che stiamo inserendo è pieno
            if (p(py, px)) {
                int field_x = x + (int)px;
                int field_y = y + (int)py;

                // 1. Controllo Bordi Laterali (Sinistro e Destro)
                if (field_x < 0 || field_x >= (int)m_width) return false;

                // 2. Controllo Bordo Inferiore (Pavimento)
                if (field_y >= (int)m_height) return false;

                // 3. Controllo Bordo Superiore
                // Se il pixel è sopra la griglia (y < 0), non può collidere con pezzi esistenti
                // ma è comunque in una posizione valida per ora.
                if (field_y < 0) 
                {
                    if (field_x < 0 || field_x >= (int)m_width) return false;
                    continue; // Non controllare collisioni con altri pezzi se siamo sopra il campo
                }
                // 4. Controllo Collisioni con pezzi già presenti (Logica "get_pixel" integrata)
                // Scorriamo la lista concatenata dei pezzi già nel campo
                for (const_iterator it = this->begin(); it != this->end(); ++it) {
                    // Verifichiamo se le coordinate (field_x, field_y) cadono dentro questo pezzo già posizionato
                    if (field_x >= it->x && field_x < it->x + (int)it->p.side() &&
                        field_y >= it->y && field_y < it->y + (int)it->p.side()) {
                        
                        // Se il pixel corrispondente nel pezzo esistente è true, c'è collisione
                        if (it->p(field_y - it->y, field_x - it->x)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    // Se nessun pixel ha violato i bordi o colliso con pezzi esistenti
    return true; 
}

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
                delete[] tmp_mat[i];
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
                    abs_y = y - (p.side() - 1) + i;  
                    
                    if(abs_x >= 0 && abs_y >= 0 && abs_x < (int) m_width && abs_y < (int) m_height)
                        tmp_mat[abs_y][abs_x] = p.color();
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

tetris::iterator::reference tetris::iterator::operator*() 
{
	// returns the reference to the tetris_piece object 
	return this->m_ptr->tp;
};

tetris::iterator::pointer tetris::iterator::operator->() 
{
	// returns the pointer to the tetris_piece object 
	return &(this->m_ptr->tp);
};

// prefix increment: ++it
tetris::iterator& tetris::iterator::operator++() 
{
	this->m_ptr = this->m_ptr->next;
	return *this;
};

// postfix increment: it++
tetris::iterator tetris::iterator::operator++(int /*dummy*/) 
{
	iterator it = this->m_ptr;
	// call to the prefix operator on this
	++(*this);
	return it;
};

// checks if two iterators are pointing to the same node in the memory
bool tetris::iterator::operator==(iterator const& rhs) const { return this->m_ptr == rhs.m_ptr; };

bool tetris::iterator::operator!=(iterator const& rhs) const { return this->m_ptr != rhs.m_ptr; };

// CONST ITERATOR METHODS
tetris::const_iterator::const_iterator(node const* ptr) : m_ptr(ptr) {};
tetris::const_iterator::reference tetris::const_iterator::operator*() const { return this->m_ptr->tp; };
tetris::const_iterator::pointer tetris::const_iterator::operator->() const { return &(this->m_ptr->tp); };
tetris::const_iterator& tetris::const_iterator::operator++() 
{
	this->m_ptr = this->m_ptr->next;
	return *this;
};
tetris::const_iterator tetris::const_iterator::operator++(int) 
{
	const_iterator it = this->m_ptr;
	++(*this);
	return it;
};

bool tetris::const_iterator::operator==(const_iterator const& rhs) const { return this->m_ptr == rhs.m_ptr; };
bool tetris::const_iterator::operator!=(const_iterator const& rhs) const { return this->m_ptr != rhs.m_ptr; };

typename tetris::iterator
tetris::begin() { return this->m_field; };
typename tetris::iterator
tetris::end() { return nullptr; };

typename tetris::const_iterator
tetris::begin() const { return this->m_field; };
typename tetris::const_iterator
tetris::end() const { return nullptr; };


uint32_t tetris::score() const { return this->m_score; };
uint32_t tetris::width() const { return this->m_width; };
uint32_t tetris::height() const { return this->m_height; };

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

//score width height \n p1 x1 y1 \n p2 x2 y2 \n
//Errori non arrivano?
std::istream& operator>>(std::istream& is, tetris& t)
{
    uint32_t width;
    uint32_t height;
    uint32_t score;
    is >> std::skipws >> score >> width >> height;
    CHECK_ERR(is.fail(), "ERROR! - operator>>(std::istream& is, tetris& t) - Formato input invalido (lettura fallita).");

    tetris temp_t(width, height, score);

    while(is >> std::ws)
    {
        if (is.peek() == EOF) break;

        piece p_data;
        int x = 0;
        int y = 0;

        if (is >> p_data >> x >> y) 
        {
            try { temp_t.add(p_data, x, y); } 
            catch (...) { CHECK_ERR(true, "ERROR! - operator>>(std::istream& is, tetris& t) - Errore di allocazione nella lista (causa add)"); }
        } 
        else 
        { 
            CHECK_ERR(is.fail(), "ERROR! - operator>>(std::istream& is, tetris& t) - Formato input invalido (lettura fallita).");
            break; 
        }
    }

    t = std::move(temp_t);
    return is;
}

std::ostream& operator<<(std::ostream& os, tetris const& t)
{
    os << t.score() << " " << t.width() << " " << t.height() << std::endl;  //Dimensioni e Punteggio

    for(auto it = t.begin(); it != t.end(); ++it)
        os << it->p << " " << it->x << " " << it->y << std::endl;  //Piece, X e Y

    return os;
}
