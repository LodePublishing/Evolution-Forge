#ifndef __DEBUG_H
#define __DEBUG_H

static class DEBUG
{
	private:
		int errorCode;				
		char errorMessage[200];
	public:
// needs to be implemented yet, in debug.getLastErrorCode() will then stand the errorcode from the last error
		int getLastErrorCode();
// the last error message
		const char* getLastErrorMessage();
// writes a message (like printf) to the 'error.log' with a time stamp, code is (yet) ignored
		void toLog(int code, const char* msg, ...);
	DEBUG();
	~DEBUG();
} debug;

#endif // __DEBUG_H

