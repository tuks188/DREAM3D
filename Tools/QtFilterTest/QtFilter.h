
#ifndef _QtFilter_H_
#define _QtFilter_H_





#define DREAM3D_FILTER_PROPERTY_DECL(Type, Name)\
void set##Name(Type value);\
Type get##Name() const;



#define DREAM3D_FILTER_PROPERTY_DEFN(Type, Name, Class)\
void Class::set##Name(Type value) { Class##Data* d = d_ptr.data(); d->m_##Name = value; }\
Type Class::get##Name() const { const Class##Data* d = d_ptr.constData(); return d->m_##Name; }


#include <QtCore/QString>
#include <QtCore/QExplicitlySharedDataPointer>


#include "DREAM3DLib/Common/AbstractFilter.h"

class SegmentGrainsData;

class SegmentGrains
{
  public:
    SegmentGrains();
    SegmentGrains(const SegmentGrains &other);
    virtual ~SegmentGrains();

    void deepCopy(SegmentGrains& s);

    DREAM3D_FILTER_PROPERTY_DECL(float, Misorientation)
    DREAM3D_FILTER_PROPERTY_DECL(QString, InputFile)

    SegmentGrains copy(const SegmentGrains& rhs);

  protected:
    QExplicitlySharedDataPointer<SegmentGrainsData> d_ptr;

//private:
//    void operator=(const SegmentGrains&); // Operator '=' Not Implemented

};

#endif /* _QtFilter_H_ */
