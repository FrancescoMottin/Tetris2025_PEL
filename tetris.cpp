#include "tetris.hpp"
#include <fstream>


using namespace std;

piece::piece() {
	this->m_side = 0;
    this->m_color = 0;
    this->m_grid = nullptr;
};

bool power2(uint32_t n) {
    if(n == 1)
        return true;
    if(n < 1 || n % 2 != 0)
        return false;
    return power2(n / 2);
};

piece::piece(uint32_t s, uint8_t c) {
	if(!power2(s))
		throw tetris_exception("s = " + std::to_string(s) + " is not a power of 2");
	else if(c == 0)
		throw tetris_exception("c is 0");
	
	this->m_side = s;
    this->m_color = c;
    this->m_grid = new bool*[s];
    
    for(uint32_t it = 0; it < this->m_side; it++) {
		// The () value initializes the arrays to false
		this->m_grid[it] = new bool[s]();
	}
};

piece::piece(piece const& rhs) {
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	
	this->m_grid = new bool*[this->m_side];
    
    for(uint32_t it = 0; it < this->m_side; it++) {
		this->m_grid[it] = new bool[this->m_side]();
	}
	
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) {
		this->m_grid[it1] = new bool[this->m_side];
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) {
			this->m_grid[it1][it2] = rhs.m_grid[it1][it2];
		}
	}
};

piece::piece(piece&& rhs) {
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	this->m_grid = rhs.m_grid;
	
	rhs.m_side = 0;
	rhs.m_color = 0;
	rhs.m_grid = nullptr;
};

piece::~piece() { 
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
		// Delete on every array of the matrix
		delete[] this->m_grid[it1];
	}
	
	// Deletes the array of pointers
	delete[] m_grid;
	
	this->m_side = 0;
	this->m_color = 0;
};

piece& piece::operator=(piece const& rhs) {
	// Control on the address of the objects to avoid auto assignment
	if(this == &rhs)
		return *this;
	
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
		// Delete on every array of the matrix
		delete[] this->m_grid[it1];
	}
	
	// Deletes the array of pointers
	delete[] this->m_grid;
	
	this->m_side = rhs.m_side;
	this->m_color = rhs.m_color;
	
	this->m_grid = new bool*[this->m_side];
	
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) {
		this->m_grid[it1] = new bool[this->m_side];
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) {
			this->m_grid[it1][it2] = rhs.m_grid[it1][it2];
		}
	}
	
	/* Dereferenziation of this
	 * this → pointer to the object
	 * *this → reference to the object
	 */
	return *this;	
};

piece& piece::operator=(piece&& rhs) {
	// Control on the address of the objects to avoid auto assignment
	if(this == &rhs)
		return *this;
	
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
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

bool piece::operator==(piece const& rhs) const {
	if(this->m_side != rhs.m_side || this->m_color != rhs.m_color)
		return false;
		
	for(uint32_t it1 = 0; it1 < rhs.m_side; it1++) {
		for(uint32_t it2 = 0; it2 < rhs.m_side; it2++) {
			if(this->m_grid[it1][it2] != rhs.m_grid[it1][it2])
				return false;
		}
	}
	
	return true;
};

bool piece::operator!=(piece const& rhs) const {
	return !(*this == rhs);
};

bool& piece::operator()(uint32_t i, uint32_t j) {
	if((i >= this->m_side) || (j >= this->m_side))
		throw tetris_exception("Given coordinates are incorrect. Out of bounds");
	return this->m_grid[i][j];
};

bool piece::operator()(uint32_t i, uint32_t j) const {
	if((i >= this->m_side) || (j >= this->m_side))
		throw tetris_exception("Given coordinates are incorrect. Out of bounds");
	return this->m_grid[i][j];
};

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const {
	if(i >= this->m_side || j >= this->m_side || i + s > this->m_side || j + s > this->m_side)
		throw tetris_exception("Given coordinates are incorrect. Out of bounds");
	
	for(uint32_t it1 = i; it1 < i + s; it1++) {
		for(uint32_t it2 = j; it2 < j + s; it2++) {
			if(this->m_grid[it1][it2])
				return false;
		}
	}
	
	return true;
};

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const {
	if(i >= this->m_side || j >= this->m_side || i + s > this->m_side || j + s > this->m_side)
		throw tetris_exception("Given coordinates are incorrect. Out of bounds");
		
	for(uint32_t it1 = i; it1 < i + s; it1++) {
		for(uint32_t it2 = j; it2 < j + s; it2++) {
			if(!this->m_grid[it1][it2])
				return false;
		}
	}
	
	return true;
};

bool piece::empty() const {
	if(this->m_side == 0)
		return true;
	
	return empty(0,0,this->m_side);
};

bool piece::full() const {
	if(this->m_side > 0)
		return true;
	
	return full(0,0,this->m_side);
};

void piece::rotate() {
	bool** tmp = new bool*[this->m_side];
    
    for(uint32_t it = 0; it < this->m_side; it++) {
		tmp[it] = new bool[this->m_side];
	}
	
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
		for(uint32_t it2 = 0; it2 < this->m_side; it2++) {
			tmp[it1][it2] = this->m_grid[it2][this->m_side - it1 - 1];
		}
	}
		
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
		for(uint32_t it2 = 0; it2 < this->m_side; it2++) {
			this->m_grid[it1][it2] = tmp[it1][it2];
		}
	}
	
	// Deletion of tmp. Otherwise there would be a memory leak
    for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
        delete[] tmp[it1];
    }
    delete[] tmp;
};

void piece::cut_row(uint32_t i) {
	if(i >= this->m_side) throw tetris_exception("Given row is incorrect. Out of bounds");

	for(int row = i; row > 0; row--) {
		for(uint32_t col = 0; col < this->m_side; col++) {
			this->m_grid[row][col] = this->m_grid[row - 1][col];
		}
	}

	for(uint32_t col = 0; col < this->m_side; col++) {
		this->m_grid[0][col] = false;
	}
};

//FUNZIONI DI DEBUG
void piece::print_ascii_art(std::ostream& os) const {
	for(uint32_t it1 = 0; it1 < this->m_side; it1++) {
		for(uint32_t it2 = 0; it2 < this->m_side; it2++) {
			if(this->m_grid[it1][it2]) {
				 os << "\033[48;5;" << int(this->m_color) << "m" << ' ' << "\033[m";
			 } else {
				 os << ' ';
			 }
		}
		os << '\n';
	}
};

uint32_t piece::side() const {
	return this->m_side;
};

int piece::color() const {
	return this->m_color;
};

char nextNonSpaceChar(std::istream& is) {
    char c;
    while(is.get(c)) {                   
        if(!std::isspace(static_cast<unsigned char>(c))) {
            return c;                     
        }
    }
    return '\0';
}

void new_quadrant(std::istream& is, piece& p, uint32_t it1, uint32_t it2, uint32_t side) {
    if (side == 0) return;

    char c = nextNonSpaceChar(is);

    if (c == '[') {
        // Empty quadrant, the next char must be ']'
        if (nextNonSpaceChar(is) == ']') {
			for (uint32_t i = it1; i < it1 + side; i++) {
				for (uint32_t j = it2; j < it2 + side; j++) {
					p(i, j) = false;
				}
			}
		}
    }
    else if (c == '(') {
        // It may be () or a recursive structure
        char next = nextNonSpaceChar(is);
        if (next == ')') {
            // Full quadrant
            for (uint32_t i = it1; i < it1 + side; i++) {
                for (uint32_t j = it2; j < it2 + side; j++) {
                    p(i, j) = true;
                }
            }
        } else {
            // Recursive structure
            is.putback(next);

            new_quadrant(is, p, it1, it2, side / 2);                     
            new_quadrant(is, p, it1, it2 + (side / 2), side / 2);              
            new_quadrant(is, p, it1 + (side / 2), it2, side / 2);              
            new_quadrant(is, p, it1 + (side / 2), it2 + (side / 2), side / 2);       

            nextNonSpaceChar(is);
        }
    }
};

std::istream& operator>>(std::istream& is, piece& p) {
	uint32_t side, color;
    
    if (!(is >> side >> color)) {
		is.setstate(std::ios::failbit);
		return is;
	}

	p = piece(side, color);
	
	new_quadrant(is, p, 0, 0, side);
		
	return is;		
};

void grid_construction(std::ostream& os, piece const& p, uint32_t it1, uint32_t it2, int s) {
	if(s == 1) {
		if(p(it1, it2) == true)
			os << "()";
		else
			os << "[]";
	} else {
		if(p.full(it1,it2,s))
			os << "()";
		else if(p.empty(it1,it2,s)) {
			os << "[]";
		} else {
			os << '(';
			grid_construction(os, p, it1, it2, s/2);
			grid_construction(os, p, it1, it2 + (s/2), s/2);
			grid_construction(os, p, it1 + (s/2), it2, s/2);
			grid_construction(os, p, it1 + (s/2), it2 + (s/2), s/2);
			os << ')';
		}
	}
};

std::ostream& operator<<(std::ostream& os, piece const& p) {
	os << p.side() << ' ' << p.color() << ' ';

	int s = p.side();		
	grid_construction(os, p, 0, 0, s);
	
	return os;
};

tetris::tetris() {
	this->m_score = 0;
    this->m_width = 0;
    this->m_height = 0;
    this->m_field = nullptr;
};

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) {
	if(w == 0 || h == 0) {
		throw tetris_exception("exception in tetris constructor. width or height passed by argument equals to 0");
	}
	
	this->m_score = s;
    this->m_width = w;
    this->m_height = h;
    this->m_field = nullptr;
};

tetris::tetris(tetris const& rhs) {
	this->m_score = rhs.m_score;
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    this->m_field = nullptr;
    
    node* tmp = rhs.m_field;
    
    while(tmp != nullptr) {
		if(this->m_field == nullptr) {
			this->m_field = new node();
			this->m_field->tp = tmp->tp;
			this->m_field->next = nullptr;
		} else {
			node* it = this->m_field;
			
			while(it->next != nullptr) {
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

tetris::tetris(tetris&& rhs) {
	this->m_score = rhs.m_score;
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    this->m_field = rhs.m_field;
    
    rhs.m_score = 0;
    rhs.m_width = 0;
    rhs.m_height = 0;
    rhs.m_field = nullptr;
};

tetris::~tetris() {
	this->m_score = 0;
    this->m_width = 0;
    this->m_height = 0;
    
    node* tmp = nullptr;
    
    while(this->m_field != nullptr) {
		tmp = this->m_field;
		this->m_field = this->m_field->next;
		delete tmp;
	}
};

tetris& tetris::operator=(tetris const& rhs) {
	if(*this == rhs)
		return *this;
	
	this->m_score = rhs.m_score;
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    
    node* tmp = nullptr;
    
    while(this->m_field != nullptr) {
		tmp = this->m_field;
		this->m_field = this->m_field->next;
		delete tmp;
	}
    
    tmp = rhs.m_field;
    
    while(tmp != nullptr) {
		if(this->m_field == nullptr) {
			node* newN = new node();
			newN->tp = tmp->tp;
			newN->next = nullptr;
			this->m_field = newN;
		} else {
			node* it = this->m_field;
			
			while(it->next != nullptr) {
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

tetris& tetris::operator=(tetris&& rhs) {
    if (this != &rhs) {
        node* tmp;
        while (this->m_field != nullptr) {
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

bool tetris::operator==(tetris const& rhs) const {
	if(this->m_score != rhs.m_score || this->m_width != rhs.m_width || this->m_height != rhs.m_height)
		return false;
	
	node* tmp1 = this->m_field;
	node* tmp2 = rhs.m_field;
	
	while(tmp1 != nullptr && tmp2 != nullptr) {
		if(tmp1->tp.p != tmp2->tp.p || tmp1->tp.x != tmp2->tp.x || tmp1->tp.y != tmp2->tp.y)
			return false;
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;		
	}
	
	if((tmp1 != nullptr && tmp2 == nullptr) || (tmp1 == nullptr && tmp2 != nullptr))
		return false;
	
	return true;
};

bool tetris::operator!=(tetris const& rhs) const {
	return !(*this == rhs);
};

struct field {
    bool** f;
    const tetris& t;

    field(const tetris& rhs) : f(nullptr), t(rhs) {
        this->f = new bool*[this->t.height()];
        for(uint32_t i = 0; i < this->t.height(); ++i) {
            this->f[i] = new bool[this->t.width()]();
        }

        for(auto it = rhs.begin(); it != rhs.end(); ++it) {
            this->add(*it);
        }
    };

    ~field() {
        for(uint32_t i = 0; i < this->t.height(); i++) {
            delete[] this->f[i];
        }
        delete[] this->f;
    };

    void add(const tetris_piece& tp) {
        int piece_y = int(tp.p.side()) - 1;
        for(int i = tp.y; i > tp.y - int(tp.p.side()); --i) {
            int piece_x = 0;
            for(int j = tp.x; j < tp.x + int(tp.p.side()); ++j) {
                if (tp.p(piece_y, piece_x)) {
                    if (i >= 0 && i < int(this->t.height()) && j >= 0 && j < int(this->t.width()))
                        this->f[i][j] = true;
                }
                ++piece_x;
            }
            --piece_y;
        }
    };

    bool full_row() const {
        for(int it1 = int(this->t.height()) - 1; it1 >= 0; --it1) {
            uint32_t c = 0;
            for(uint32_t it2 = 0; it2 < this->t.width(); ++it2) {
                if(this->f[it1][it2])
                    ++c;
            }
            if(c == this->t.width()) return true;
        }
        return false;
    };

    int first_full_row() const {
        for(int it1 = int(this->t.height()) - 1; it1 >= 0; --it1) {
            int c = 0;
            for(uint32_t it2 = 0; it2 < this->t.width(); ++it2) {
                if(this->f[it1][it2]) ++c;
            }
            if(uint32_t(c) == this->t.width()) return it1;
        }
        return int(this->t.height());
    };

    void clear_field() {
        for(int i = 0; i < int(this->t.height()); ++i)
            for(int j = 0; j < int(this->t.width()); ++j)
                f[i][j] = false;
    };
};

void tetris::insert(piece const& p, int x) {
	int max_y = -1;

    for(int y = 0; y < int(this->m_height); y++) {
        if(this->containment(p, x, y)) max_y = y;
    }

    if(max_y == -1) 
        throw tetris_exception("GAME OVER!!! tetris piece p cannot be placed");

    this->add(p, x, max_y);
    
    field f(*this);
    
    // finds all the full row inside the field
	while(f.full_row()) {
		node* tmp = this->m_field;
		while(tmp != nullptr) {
			for(int i = tmp->tp.p.side() - 1; i >= 0; i--) {
				if (tmp->tp.y + i == f.first_full_row()) {
					tmp->tp.p.cut_row(i);
				}
			}
			tmp = tmp->next;
		}
		
		// updates the field after the cut_row()
		f.clear_field();
		tmp = this->m_field;
		while(tmp != nullptr) {
			f.add(tmp->tp);
			tmp = tmp->next;
		}
		
		// checks  if the pieces can be shifted down
		tmp = this->m_field;
		this->m_field = nullptr;
		node* tmp1 = tmp;
		
		while(tmp1 != nullptr) {
			if(!tmp1->tp.p.empty()) {
				max_y = -1;
			
				for(int i = 0; i < int(this->m_height); ++i) {
					if(this->containment(tmp1->tp.p, tmp1->tp.x, i)) max_y = i;
				}
				this->add(tmp1->tp.p, tmp1->tp.x, max_y);              
			}
			tmp1 = tmp1->next;
		}

		
		while(tmp != nullptr) {
			node* to_delete = tmp;
			tmp = tmp->next;
			delete to_delete;
		}
		
	}	
};

void tetris::add(piece const& p, int x, int y) {
	if(!this->containment(p, x, y)) {
		throw tetris_exception("exception in tetris add function. tetris piece p cannot be contained at offset (x: " + std::to_string(x) + ", y: " + std::to_string(y) + ")");
	}
			
	node* newN = new node();
	
	newN->tp.p = p;
	newN->tp.x = x;
	newN->tp.y = y;
	
	newN->next = this->m_field;
	this->m_field = newN;
};

bool tetris::containment(piece const& p, int x, int y) const {
    field f(*this);
    
    uint32_t piece_x = 0;
    int piece_y = int(p.side()) - 1;
    for(int i = y; i > y - int(p.side()); --i) {
        piece_x = 0;
        for(int j = x; j < x + int(p.side()); ++j) {
            if(p(piece_y, piece_x)) {
                if((i < 0 || i >= int(this->m_height)) || (j < 0 || j >= int(this->m_width)))
                    return false;
                if(f.f[i][j]) return false;
            }
            ++piece_x;
        }
        --piece_y;
    }
    return true;
};


// FUNZIONE DI DEBUG
void tetris::print_ascii_art(std::ostream& os) const {
    int** tmp_mat = nullptr;
    
    try {
        tmp_mat = new int*[m_height];
        for(uint32_t i = 0; i < m_height; i++) {
            tmp_mat[i] = new int[m_width];
            for(uint32_t j = 0; j < m_width; j++)
                tmp_mat[i][j] = -1;
        }
    }
    catch(const std::bad_alloc& e) {
        if(tmp_mat) {
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
    
    for(node* it = m_field; it; it = it->next) {
        p = it->tp.p;
        x = it->tp.x;
        y = it->tp.y;

        for(uint32_t i = 0; i < p.side(); i++) {
            for(uint32_t j = 0; j < p.side(); j++) {
                if(p(i,j) == true) {
                    abs_x = x + j;
                    abs_y = y + i;
                    //tmp_mat[abs_y][abs_x] = '#';  
                    
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

    
    for(uint32_t i = 0; i < m_height; i++) {
        os << '|';
        for(uint32_t j = 0; j < m_width; j++) {
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

tetris::iterator::reference tetris::iterator::operator*() {
	// returns the reference to the tetris_piece object 
	return this->m_ptr->tp;
};

tetris::iterator::pointer tetris::iterator::operator->() {
	// returns the pointer to the tetris_piece object 
	return &(this->m_ptr->tp);
};

// prefix increment: ++it
tetris::iterator& tetris::iterator::operator++() {
	this->m_ptr = this->m_ptr->next;
	return *this;
};

// postfix increment: it++
tetris::iterator tetris::iterator::operator++(int /*dummy*/) {
	iterator it = this->m_ptr;
	// call to the prefix operator on this
	++(*this);
	return it;
};

// checks if two iterators are pointing to the same node in the memory
bool tetris::iterator::operator==(iterator const& rhs) const {
	return this->m_ptr == rhs.m_ptr;
};

bool tetris::iterator::operator!=(iterator const& rhs) const {
	return this->m_ptr != rhs.m_ptr;
};

// CONST ITERATOR METHODS
tetris::const_iterator::const_iterator(node const* ptr) : m_ptr(ptr) {};

tetris::const_iterator::reference tetris::const_iterator::operator*() const { 
	return this->m_ptr->tp;
};

tetris::const_iterator::pointer tetris::const_iterator::operator->() const {
	return &(this->m_ptr->tp);
};

tetris::const_iterator& tetris::const_iterator::operator++() {
	this->m_ptr = this->m_ptr->next;
	return *this;
};

tetris::const_iterator tetris::const_iterator::operator++(int) {
	const_iterator it = this->m_ptr;
	++(*this);
	return it;
};

bool tetris::const_iterator::operator==(const_iterator const& rhs) const {
	return this->m_ptr == rhs.m_ptr;
};

bool tetris::const_iterator::operator!=(const_iterator const& rhs) const {
	return this->m_ptr != rhs.m_ptr;
};

typename tetris::iterator
tetris::begin() {
	return this->m_field;
};

typename tetris::iterator
tetris::end() {
	return nullptr;
};

typename tetris::const_iterator
tetris::begin() const {
	return this->m_field;
};

typename tetris::const_iterator
tetris::end() const {
	return nullptr;
};


uint32_t tetris::score() const {
	return this->m_score;
};

uint32_t tetris::width() const {
	return this->m_width;
};

uint32_t tetris::height() const {
	return this->m_height;
};

std::ostream& operator<<(std::ostream& os, tetris const& t) {
	os << t.score() + ' ' + t.width() + ' ' + t.height() + '\n';
	
	for(tetris::const_iterator it = t.begin(); it != t.end(); ++it) {
		os << it->p;
		os << ' ';
		os << it->x; 
		os << ' '; 
		os << it->y;
		os << '\n';
	}
	
	return os;
};

std::istream& operator>>(std::istream& is, tetris& t) {
	uint32_t score, width, height, x, y;
	piece p;
	
    is >> score >> width >> height;
    t = tetris(width, height, score);
    
    string line;
	while(is >> p >> x >> y) {
		t.add(p, x, y);				
	}

	return is;
};
