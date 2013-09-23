#ifndef W_cvErode_H
#define W_cvErode_H

#include <cvVideoOp.h>

class wcvErode : public cvVideoOp {
	public:
		wcvErode()
				: cvVideoOp()
				, iterations( "iterations","number of times dilation is applied", 3, 1, 100 )
				{
				AddProperty( iterations );
			}

		virtual bool Process( IplImage *in, IplImage *out ) {
				cvErode( in, out, 0, iterations.Get() );
				return TRUE;
			}
	
	protected:
		wIntegerProperty iterations;
	
	private:
		typedef cvVideoOp inherited;
};

#endif
