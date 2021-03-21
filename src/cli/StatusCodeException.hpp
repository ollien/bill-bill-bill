#ifndef STATUS_CODE_EXCEPTION_HPP
#define STATUS_CODE_EXCEPTION_H

#include <exception>
#include <string>

/**
 * StatusCodeException is an exception that should be thrown when the program should exit with the given status code.
 *
 * This is only intended to be thrown at the top level of the CLI; while it could be used elsewhere, only the main
 * entrpoint will handle the exiting with the right status code.
 */
class StatusCodeException : public std::exception {
 public:
	StatusCodeException(int status_code, std::string message) : status_code(status_code), message(std::move(message)) {
	}

	StatusCodeException(int status_code, const std::exception &cause) : StatusCodeException(status_code, cause.what()) {
	}

	const char *what() const noexcept override;
	int get_status_code() const noexcept;

 private:
	int status_code;
	std::string message;
};

#endif
