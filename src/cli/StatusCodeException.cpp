#include "StatusCodeException.hpp"

/**
 * Get the message of the exception
 * @return const char* The exception message
 */
const char *StatusCodeException::what() const noexcept {
	return this->message.c_str();
}

/**
 * Get the status code that the program should return after getting this exception
 * @return int The status code
 */
int StatusCodeException::get_status_code() const noexcept {
	return this->status_code;
}
