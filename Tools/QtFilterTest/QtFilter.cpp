

#include "QtFilter.h"


class SegmentGrainsData : public QSharedData
{
  public:
    SegmentGrainsData()
      : QSharedData(), m_Misorientation(0.0f), m_InputFile("Foo")
    { }

    SegmentGrainsData(const SegmentGrainsData &other)
      : QSharedData(other), m_Misorientation(0.0f), m_InputFile("Foo")
    { }

    ~SegmentGrainsData() { }

    float m_Misorientation;
    QString m_InputFile;
};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SegmentGrains::SegmentGrains()
{
  d_ptr = new SegmentGrainsData;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SegmentGrains::SegmentGrains(const SegmentGrains &other)
  : d_ptr (other.d_ptr)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SegmentGrains::~SegmentGrains()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SegmentGrains::copyInto(SegmentGrains& s)
{
//  SegmentGrainsData* d = new SegmentGrainsData;
  s.d_ptr.detach();
//  d->m_Misorientation = d_ptr->m_Misorientation;
//  d->m_InputFile = d_ptr->m_InputFile;
//  s.d_ptr = d;
}


DREAM3D_FILTER_PROPERTY_DEFN(float, Misorientation, SegmentGrains)

DREAM3D_FILTER_PROPERTY_DEFN(QString, InputFile, SegmentGrains)

