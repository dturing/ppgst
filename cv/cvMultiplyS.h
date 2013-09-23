#ifndef W_cvMultiplyS_H
#define W_cvMultiplyS_H

#include <cvVideoOp.h>

class cvMultiplyS : public cvVideoOp {
	public:
		cvMultiplyS()
				: cvVideoOp()
				, scale( "scale","scale", 1, -100, 100 )
				, shift( "shift","shift", 0, -1000, 1000 )
				{
				AddProperty( scale );
				AddProperty( shift );
			}

		virtual bool Process( IplImage *in, IplImage *out ) {
				cvConvertScale( in, out, scale.Get(), shift.Get() );
				return TRUE;
			}
	
	protected:
		wFloatProperty scale, shift;
	
	private:
		typedef cvVideoOp inherited;
};

#endif
