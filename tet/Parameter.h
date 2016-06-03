#ifndef TTG_PARAMETER_H
#define TTG_PARAMETER_H

#include <string>
#include <vector>

/// forward declaration 
/// include openexr headers now will cause macros conflictions
class ExrImage;

namespace ttg {

class Parameter {
	
public:
	enum Operation {
		kHelp = 0,
		kDelaunay2D = 1
	};
	
	Parameter(int argc, char *argv[]);
	virtual ~Parameter();
	
	Operation operation() const;
	
	static void PrintHelp();
	
protected:

private:
	Operation m_operation;
};

}
#endif        //  #ifndef LFPARAMETER_H

