#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

//#define TESTING
#ifndef TESTING

#include <iostream>
#include <fstream>

class FileError : public std::runtime_error {
public:

    FileError( const std::string &fileNameIn ) : std::runtime_error( "" ), m_fileName( fileNameIn ) {
    }

    virtual const char* what( ) const throw () {
        return m_msg.c_str( );
    }

    std::string getFileName( ) const {
        return m_fileName;
    }

    virtual ~FileError( ) throw () {

    }

protected:
    std::string m_fileName;
    std::string m_msg;
};

class FileOpenError : public FileError {
public:

    FileOpenError( const std::string &fileNameIn ) : FileError( fileNameIn ) {
        m_msg = "Unable to open " + fileNameIn;
    }
};

class FileReadError : public FileError {
public:

    FileReadError( const std::string &fileNameIn ) : FileError( fileNameIn ) {
        std::ostringstream ostr;
        ostr << "Error reading " << fileNameIn;
        m_msg = ostr.str( );
    }
};

class FileWriteError : public FileError {
public:

    FileWriteError( const std::string &fileNameIn ) : FileError( fileNameIn ) {
        m_msg = "Unable to write " + fileNameIn;
    }
};

#endif // TESTING

class LogicError : public std::logic_error {
public:

    LogicError( ) : std::logic_error( "" ) {

    }

    virtual const char *what( ) const throw () {
        return m_message.c_str( );
    }

    virtual ~LogicError( ) throw () {

    }

protected:
    std::string m_message;
};

class OutOfRange : public LogicError {
public:

    OutOfRange( int argument, int beginOfRange, int endOfRange ) {
        std::string str_argument, str_beginOfRange, str_endOfRange;

        str_argument = intToString( argument );
        str_beginOfRange = intToString( beginOfRange );
        str_endOfRange = intToString( endOfRange );

        m_message = "Argument " + str_argument + " doesn't hit in the range [" +
                str_beginOfRange + ", " + str_endOfRange + "]";
    }

private:

    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

class EmptyArgument : public LogicError {
public:

    EmptyArgument( ) {
        m_message = "Error: empty argument.";
    }
};

#ifndef TESTING
void readData( const std::string &fileName, std::vector<int> &arr )
throw (FileOpenError, FileReadError);

void writeResult( const std::string &fileName, const std::vector<int> &oddArray, const std::vector<int> &evenArray, bool isYes )
throw (FileOpenError, FileWriteError);

bool statistics( const std::vector<int> &inputArray, std::vector<int> &oddArray, std::vector<int> &evenArray ) throw (EmptyArgument, OutOfRange);

int main( ) {
    std::string fileNameIn = "input.txt";
    std::vector<int> inputArray;

    try {
        readData( fileNameIn, inputArray );
    } catch ( const FileError &e ) {
        std::cerr << e.what( ) << std::endl;
        return 1;
    }

    bool isYes = false;
    std::vector<int> oddArray;
    std::vector<int> evenArray;

    try {
        isYes = statistics( inputArray, oddArray, evenArray );
    } catch ( const LogicError &e ) {
        std::cerr << e.what( ) << std::endl;
        return 1;
    } catch ( ... ) {
        std::cerr << "Uncaught exception." << std::endl;
        return 1;
    }

    std::string fileNameOut = "output.txt";
    try {
        writeResult( fileNameOut, oddArray, evenArray, isYes );
    } catch ( const FileError &e ) {
        std::cerr << e.what( ) << std::endl;
        return 1;
    }

    return 0;
}

void readData( const std::string &fileName, std::vector<int> &arr )
throw ( FileOpenError, FileReadError) {
    std::ifstream file;
    file.open( fileName.c_str( ) );
    if ( file.fail( ) ) {
        throw FileOpenError( fileName );
    }

    unsigned number = 0;
    if ( !(file >> number) ) {
        throw FileReadError( fileName );
    }

    int value = 0;
    for ( std::size_t i = 0; i < number; ++i ) {
        if ( !(file >> value) ) {
            throw FileReadError( fileName );
        } else {
            arr.push_back( value );
        }
    }
}

void writeResult( const std::string &fileName, const std::vector<int> &oddArray, const std::vector<int> &evenArray, bool isYes )
throw (FileOpenError, FileWriteError) {
    std::ofstream file;
    file.open( fileName.c_str( ) );

    if ( file.fail( ) ) {
        throw FileOpenError( fileName );
    }

    // Write Odd Array
    for ( std::size_t i = 0; i < oddArray.size( ); ++i ) {
        if ( !(file << oddArray[i]) ) {
            throw FileWriteError( fileName );
        }
        if ( i != (oddArray.size( ) - 1) ) {
            if ( !(file << " ") ) {
                throw FileWriteError( fileName );
            }
        }
    }
    if ( !(file << std::endl) ) {
        throw FileWriteError( fileName );
    }

    // Write Even Array
    for ( std::size_t i = 0; i < evenArray.size( ); ++i ) {
        if ( !(file << evenArray[i]) ) {
            throw FileWriteError( fileName );
        }
        if ( i != (evenArray.size( ) - 1) ) {
            if ( !(file << " ") ) {
                throw FileWriteError( fileName );
            }
        }
    }
    if ( !(file << std::endl) ) {
        throw FileWriteError( fileName );
    }

    // Write YES or NO
    if ( isYes ) {
        if ( !(file << "YES") ) {
            throw FileWriteError( fileName );
        }
    } else {
        if ( !(file << "NO") ) {
            throw FileWriteError( fileName );
        }
    }

    if ( !(file << std::endl) ) {
        throw FileWriteError( fileName );
    }
}
#endif // TESTING

bool statistics( const std::vector<int> &inputArray, std::vector<int> &outOddArray, std::vector<int> &outEvenArray ) throw (EmptyArgument, OutOfRange) {
    if ( inputArray.empty( ) ) {
        throw ( EmptyArgument( ));
    }

    int const beginOfRange = 1;
    int const endOfRange = 31;

    bool answer = false;

    for ( std::size_t i = 0; i < inputArray.size( ); ++i ) {
        if ( (inputArray[i] < beginOfRange) || (endOfRange < inputArray[i]) ) {
            throw ( OutOfRange( inputArray[i], beginOfRange, endOfRange ));
        }

        if ( inputArray[i] % 2 == 0 ) {
            outEvenArray.push_back( inputArray[i] );
        } else {
            outOddArray.push_back( inputArray[i] );
        }
    }

    // Answer
    if ( outEvenArray.size( ) >= outOddArray.size( ) ) {
        answer = true;
    } else {
        answer = false;
    }

    return answer;
}
