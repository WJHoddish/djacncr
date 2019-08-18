#include "../include/int.h"

std::ostream& operator<<(std::ostream& os, const Int& src) {
	os << src.data_;
	return os;
}