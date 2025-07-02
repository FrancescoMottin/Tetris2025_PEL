#include "tetris.hpp"

/* Your implementation goes here! */
piece::piece()
{
    m_side = 0;
    m_color = 0;
    m_grid = nullptr; //E' un doppio puntatore, non può essere 
}

piece::piece(uint32_t s, uint8_t c)
{
    m_side = s;
    m_color = c;
    m_grid; //initialize a s*s matrix with all cells at false
}

piece::piece(piece const& rhs)
{
}

piece::piece(piece&& rhs)
{
}

piece::~piece()
{
}

piece& piece::operator=(piece const& rhs)
{
}

piece& piece::operator=(piece&& rhs)
{
}

bool piece::operator==(piece const& rhs) const
{
}

bool piece::operator!=(piece const& rhs) const
{
}

bool& piece::operator()(uint32_t i, uint32_t j)
{
}

bool piece::operator()(uint32_t i, uint32_t j) const
{
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const
{
}

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const
{
}

bool piece::empty() const
{
}

bool piece::full() const
{
}

void piece::rotate()
{
}

void piece::cut_row(uint32_t i)
{
}

void tetris::print_ascii_art(std::ostream& os) const
{
}

uint32_t piece::side() const
{
}

int piece::color() const
{
    
}

tetris::tetris()
{
    m_score = 0;
    m_width = 0;
    m_height = 0;
    m_field = nullptr;
}

tetris::tetris(uint32_t w, uint32_t h, uint32_t s = 0)
{

}

tetris::tetris(tetris const& rhs)
{

}
tetris::tetris(tetris&& rhs)
{

}
tetris::~tetris()
{

}

tetris& tetris::operator=(tetris const& rhs)
{

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

uint32_t tetris::score() const
{}
uint32_t tetris::width() const
{}
uint32_t tetris::height() const
{}

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
