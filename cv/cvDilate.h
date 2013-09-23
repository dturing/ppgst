#ifndef W_cvDilate_H
#define W_cvDilate_H

#include <cvVideoOp.h>

class wcvDilate : public cvVideoOp {
	public:
		wcvDilate()
				: cvVideoOp()
				, iterations( "iterations","number of times dilation is applied", 3, 1, 100 )
				{
				AddProperty( iterations );
			}

		virtual bool Process( IplImage *in, IplImage *out ) {
				cvDilate( in, out, 0, iterations.Get() );
				return TRUE;
			}
	
	protected:
		wIntegerProperty iterations;
	
	private:
		typedef cvVideoOp inherited;
};

#endif
