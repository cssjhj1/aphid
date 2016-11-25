#include <QtGui>
#include "sampleKernelDialog.h"
#include "sampleKernelWidget.h"
#include "gsamp.h"

using namespace lfr;

namespace aphid {
namespace gpr {

SampleKernelDialog::SampleKernelDialog(const Covariance<float, RbfKernel<float> > & covar,
										QWidget *parent)
    : QDialog(parent)
{
	DenseMatrix<float> A(covar.K().numRows(), covar.K().numColumns() );
	A.copy(covar.K() );
	A.addDiagonal(.1f);

	DenseMatrix<float> invK(A.numRows(), A.numColumns() );
	invK.copy(A );
	if(!invK.inverseSymmetric() ) {
		std::cout<<"\n ERROR K cannot inverse!";
	}
	
	std::cout<<"\n K"<<A;

	std::cout<<"\n Kinv"<<invK;
	
	DenseMatrix<float> KKi(A.numRows(), A.numColumns() );
	A.mult(KKi,invK);
	 
	std::cout<<"\n KKi"<<KKi;
	 
    qDebug()<<"\n sample gaussian distribution";
    DenseMatrix<float> smps;
    SvdSolver<float> svder;
    gsamp(smps, covar.K(), 1, &svder);
	
	m_wig = new SampleKernelWidget(smps, this);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_wig);
	layout->setStretch(2, 1);
	layout->setSpacing(4);
	
	setLayout(layout);
    setWindowTitle(tr("Sample Gaussian distribution") );
    resize(480, 480);
}

}
}
