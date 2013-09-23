#ifndef W_wBaseTransform_H
#define W_wBaseTransform_H

#include <gst/base/gstbasetransform.h>
#include <wElement.h>

class wBaseTransform : public wElement<GstBaseTransform,GstBaseTransformClass> {
	public:
		wBaseTransform() {
			
			}

	protected:
	
	public:
		static GType get_parent_type() {
				return GST_TYPE_BASE_TRANSFORM;
			}
};

#endif
