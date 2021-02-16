#include <exception>
#include <stdexcept>

// Domain error is for mathematical functions
// Argument is not part of domain
// We want the same properties as the domain_error but
// to give it a different name for the purposes of distinction
// e.g. sqrt(-1)
using domain_error = std::domain_error;

class inverse_domain_error : public domain_error {};