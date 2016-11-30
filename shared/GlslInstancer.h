#ifndef APH_GLSL_INSTANCER_H
#define APH_GLSL_INSTANCER_H

#include "GlslBase.h"
#include <AllMath.h>

namespace aphid {
    
class GlslLegacyInstancer : public GLSLBase 
{
    Matrix44F m_worldMat;
    GLint m_worldMatLoc;
    Vector3F m_distantLightVec;
    GLint m_distantLightVecLoc;
    
public:
    GlslLegacyInstancer();
    virtual ~GlslLegacyInstancer();
    
    void setWorldTm(const Matrix44F & x);
    void setDistantLightVec(const Vector3F & x);
    
protected:
    virtual const char* vertexProgramSource() const;
	virtual const char* fragmentProgramSource() const;
	virtual void defaultShaderParameters();
	virtual void updateShaderParameters() const;
	
private:
    
};    

class GlslInstancer : public GLSLBase 
{
public:
    GlslInstancer();
    virtual ~GlslInstancer();
    
protected:
    virtual const char* vertexProgramSource() const;
	virtual const char* fragmentProgramSource() const;
	
private:
    
};

}
#endif